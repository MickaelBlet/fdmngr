#include "fdmngr.h"

#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

fdmngr_t* fdmngr_create(unsigned int max_size) {
    fdmngr_t* new_manager;

    if (max_size == 0) {
        return NULL;
    }

    new_manager = (fdmngr_t*)malloc(sizeof(fdmngr_t));
    if (new_manager == NULL) {
        return NULL;
    }
    new_manager->poll_fds =
        (struct pollfd*)malloc(sizeof(struct pollfd) * max_size);
    if (new_manager->poll_fds == NULL) {
        free(new_manager);
        return NULL;
    }
    new_manager->fds = (fdmngr_fd_t*)malloc(sizeof(fdmngr_fd_t) * max_size);
    if (new_manager->fds == NULL) {
        free(new_manager->poll_fds);
        free(new_manager);
        return NULL;
    }
    new_manager->max_nfds = max_size;
    new_manager->poll_nfds = 0;
    pthread_mutex_init(&(new_manager->mutex_fds), NULL);
    return new_manager;
}

void fdmngr_destroy(fdmngr_t* manager) {
    nfds_t i;

    for (i = 0; i < manager->poll_nfds; i++) {
        if (manager->fds[i].auto_closing == FDMNGR_AUTOMATIC_CLOSING) {
            close(manager->poll_fds[i].fd);
        }
    }
    if (manager->poll_fds != NULL) {
        free(manager->poll_fds);
    }
    if (manager->fds != NULL) {
        free(manager->fds);
    }
    pthread_mutex_destroy(&(manager->mutex_fds));
    free(manager);
}

int fdmngr_add(fdmngr_t* manager, int fd, short events,
               enum fdmngr_closing_e closing, fdmngr_poll_callback_t callback,
               void* user_data) {
    nfds_t i;

    if (fd < 0 || events == 0 || callback == NULL) {
        return -1;
    }

    pthread_mutex_lock(&(manager->mutex_fds));
    if (manager->poll_nfds == manager->max_nfds) {
        // max filedescriptor
        pthread_mutex_unlock(&(manager->mutex_fds));
        return -1;
    }

    for (i = 0; i < manager->poll_nfds; i++) {
        if (manager->poll_fds[i].fd == fd) {
            // filedescriptor already added
            pthread_mutex_unlock(&(manager->mutex_fds));
            return -1;
        }
    }

    // init pollfd
    manager->poll_fds[manager->poll_nfds].fd = fd;
    manager->poll_fds[manager->poll_nfds].events = events;
    manager->poll_fds[manager->poll_nfds].revents = 0;

    // init fdmngr_fd_t
    manager->fds[manager->poll_nfds].poll_callback = callback;
    manager->fds[manager->poll_nfds].user_data = user_data;
    manager->fds[manager->poll_nfds].auto_closing = closing;

    // increase number of fds
    manager->poll_nfds++;
    pthread_mutex_unlock(&(manager->mutex_fds));
    return 0;
}

int fdmngr_remove(fdmngr_t* manager, int fd) {
    nfds_t i;

    pthread_mutex_lock(&(manager->mutex_fds));
    for (i = 0; i < manager->poll_nfds; i++) {
        if (manager->poll_fds[i].fd == fd) {
            break;
        }
    }
    if (i == manager->poll_nfds) {
        // fd not found
        pthread_mutex_unlock(&(manager->mutex_fds));
        return -1;
    }
    // close fd
    if (manager->fds[i].auto_closing == FDMNGR_AUTOMATIC_CLOSING) {
        close(manager->poll_fds[i].fd);
    }
    // move all next elements
    for (; i < manager->poll_nfds - 1; i++) {
        manager->poll_fds[i].fd = manager->poll_fds[i + 1].fd;
        manager->poll_fds[i].events = manager->poll_fds[i + 1].events;
        manager->fds[i].poll_callback = manager->fds[i + 1].poll_callback;
        manager->fds[i].user_data = manager->fds[i + 1].user_data;
        manager->fds[i].auto_closing = manager->fds[i + 1].auto_closing;
    }

    // clear last element
    manager->poll_nfds--;

    pthread_mutex_unlock(&(manager->mutex_fds));
    return 0;
}

#ifndef FDMNGR_HAVE_POLL
static int s_select(struct pollfd* fds, nfds_t nfds, int timeout_ms) {
    struct timeval tv;
    fd_set readfds;
    fd_set writefds;
    fd_set exceptfds;
    nfds_t i;
    int ret_poll;
    int max_fd;

    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);

    max_fd = -1;
    for (i = 0; i < nfds; i++) {
        if (fds[i].fd > max_fd) {
            max_fd = fds[i].fd;
        }
        if (fds[i].events & POLLIN) {
            FD_SET(fds[i].fd, &readfds);
        }
        if (fds[i].events & POLLOUT) {
            FD_SET(fds[i].fd, &writefds);
        }
        if (fds[i].events & (POLLERR | POLLHUP | POLLNVAL)) {
            FD_SET(fds[i].fd, &exceptfds);
        }
        fds[i].revents = 0;
    }

    if (timeout_ms >= 0) {
        tv.tv_sec = timeout_ms / 1000;
        tv.tv_usec = (timeout_ms % 1000) * 1000;
    }

    ret_poll = select(max_fd + 1, &readfds, &writefds, &exceptfds,
                      (timeout_ms >= 0) ? &tv : NULL);
    if (ret_poll > 0) {
        for (i = 0; i < nfds; i++) {
            if (FD_ISSET(fds[i].fd, &readfds)) {
                fds[i].revents |= POLLIN;
            }
            if (FD_ISSET(fds[i].fd, &writefds)) {
                fds[i].revents |= POLLOUT;
            }
            if (FD_ISSET(fds[i].fd, &exceptfds)) {
                fds[i].revents |= (POLLERR | POLLHUP | POLLNVAL);
            }
        }
    }
    return ret_poll;
}
#endif

int fdmngr_poll(fdmngr_t* manager, int timeout_ms) {
    nfds_t i;
    int ret_poll;
    nfds_t last_poll_nfds;
    int revents;

    pthread_mutex_lock(&manager->mutex_fds);
    if (manager->poll_nfds == 0) {
        pthread_mutex_unlock(&manager->mutex_fds);
        return -1;
    }
#ifdef FDMNGR_HAVE_POLL
    ret_poll = poll(manager->poll_fds, manager->poll_nfds, timeout_ms);
#else
    ret_poll = s_select(manager->poll_fds, manager->poll_nfds, timeout_ms);
#endif
    manager->last_errno = errno;
    if (ret_poll > 0) {
        i = 0;
        last_poll_nfds = manager->poll_nfds;
        while (i < manager->poll_nfds) {
            if (manager->poll_fds[i].revents != 0) {
                revents = manager->poll_fds[i].revents;
                manager->poll_fds[i].revents = 0;
                if (manager->fds[i].poll_callback != NULL) {
                    pthread_mutex_unlock(&manager->mutex_fds);
                    manager->fds[i].poll_callback(
                        manager, manager->poll_fds[i].fd, revents,
                        manager->fds[i].user_data);
                    pthread_mutex_lock(&manager->mutex_fds);
                    if (last_poll_nfds != manager->poll_nfds) {
                        /* Restart the loop if the list has changed */
                        last_poll_nfds = manager->poll_nfds;
                        i = 0;
                        continue;
                    }
                }
            }
            ++i;
        }
    }
    pthread_mutex_unlock(&manager->mutex_fds);
    errno = manager->last_errno;
    return ret_poll;
}
