#ifndef FDMNGR_H_
#define FDMNGR_H_

#include <fcntl.h>
#include <features.h>
#include <poll.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct fdmngr fdmngr_t;
typedef struct fdmngr_fd fdmngr_fd_t;
typedef void (*fdmngr_poll_callback_t)(fdmngr_t* manager, int fd, short revents,
                                       void* user_data);

enum fdmngr_closing_e {
    FDMNGR_NOT_AUTOMATIC_CLOSING = 0,
    FDMNGR_AUTOMATIC_CLOSING = 1
};

struct fdmngr_fd {
    fdmngr_poll_callback_t poll_callback;
    void* user_data;
    enum fdmngr_closing_e auto_closing;
};

struct fdmngr {
    nfds_t max_nfds;
    nfds_t poll_nfds;
    pthread_mutex_t mutex_fds;
    struct pollfd* poll_fds;
    fdmngr_fd_t* fds;
    int last_errno;
};

/**
 * @brief Create a new file descriptor manager
 *
 * @param max_size Maximum number of file descriptors to manage
 * @return fdmngr_t* Pointer to the created manager, or NULL on failure
 */
fdmngr_t* fdmngr_create(unsigned int max_size);

/**
 * @brief Destroy the file descriptor manager and free resources
 *
 * @param manager Pointer to the manager to destroy
 */
void fdmngr_destroy(fdmngr_t* manager);

/**
 * @brief Add a file descriptor to be managed
 *
 * @param manager Pointer to the manager
 * @param fd File descriptor to add
 * @param events Events to monitor (e.g., POLLIN, POLLOUT)
 * @param closing Closing behavior (automatic or not)
 * @param callback Callback function to invoke on events
 * @param user_data User data to pass to the callback
 * @return 0 on success, -1 on failure
 */
int fdmngr_add(fdmngr_t* manager, int fd, short events,
               enum fdmngr_closing_e closing, fdmngr_poll_callback_t callback,
               void* user_data);

/**
 * @brief Remove a file descriptor from management
 *
 * @param manager Pointer to the manager
 * @param fd File descriptor to remove
 * @return 0 on success, -1 on failure
 */
int fdmngr_remove(fdmngr_t* manager, int fd);

/**
 * @brief Poll the managed file descriptors for events
 *
 * @param manager Pointer to the manager
 * @param timeout_ms Timeout in milliseconds (-1 for infinite)
 * @return Number of file descriptors with events, 0 on timeout, -1 on error
 */
int fdmngr_poll(fdmngr_t* manager, int timeout_ms);

#ifdef __cplusplus
}
#endif

#endif // #ifndef FDMNGR_H_
