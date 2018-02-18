#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "fdmngr.h"

void server_callback(fdmngr_t* manager, int fd, short revents,
                     void* user_data) {
    static int counter = 0;
    (void)user_data;
    if (revents & POLLIN) {
        char buffer[255];
        int bytes = read(fd, buffer, sizeof(buffer));
        if (bytes > 0) {
            buffer[bytes] = '\0';
            printf("Input: %s", buffer);
            dprintf(fd, "toto %d\n", counter++);
        }
        else {
            printf("Disconnected or EOF\n");
            fdmngr_remove(manager, fd);
            close(fd);
        }
    }
    // else if (revents & POLLOUT) {
    //     const char msg[] = "Hello, server!";
    //     ssize_t sent = write(fd, msg, sizeof(msg) - 1);
    //     if (sent < 0) {
    //         perror("send");
    //     } else {
    //         printf("Sent message: %s\n", msg);
    //     }
    // }
    else if (revents & (POLLHUP | POLLERR)) {
        printf("Disconnected or EOF by error\n");
        fdmngr_remove(manager, fd);
        close(fd);
    }
}

static int g_is_run = 0;

static void signal_stop(int sig_number) {
    (void)sig_number;
    g_is_run = 0;
}

static void ini_signal(void) {
    struct sigaction sig_action;

    memset(&sig_action, 0, sizeof(sig_action));

    sig_action.sa_handler = &signal_stop;
    sigemptyset(&sig_action.sa_mask);
    sig_action.sa_flags = 0;

    /* catch signal term actions */
    sigaction(SIGABRT, &sig_action, NULL);
    sigaction(SIGINT, &sig_action, NULL);
    sigaction(SIGQUIT, &sig_action, NULL);
    sigaction(SIGTERM, &sig_action, NULL);
}

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    fdmngr_t* manager = fdmngr_create(1024);
    if (manager == NULL) {
        return (-1);
    }

    // Setup a listening socket
    int listenFd;

    g_is_run = 1;
    ini_signal();
    while (g_is_run > 0) {
        listenFd = fdmngr_socket_create_tcp_reuseaddr("127.0.0.1", 0);
        if (fdmngr_socket_connect(listenFd, "127.0.0.1", 4242) != 0) {
            close(listenFd);
            int ret_write = write(1, ".", 1);
            (void)ret_write;
            usleep(1000000);
        }
        else {
            fdmngr_add(manager, listenFd, POLLIN, FDMNGR_AUTOMATIC_CLOSING,
                       &server_callback, NULL); // Monitor server
            while (g_is_run > 0 && fdmngr_poll(manager, 60000) >= 0) {
                // continue looping
            }
            printf("%s\n", strerror(errno));
            fflush(stdout);
            fdmngr_remove(manager, listenFd);
        }
    }

    fdmngr_destroy(manager);

    printf("END\n");
    return 0;
}
