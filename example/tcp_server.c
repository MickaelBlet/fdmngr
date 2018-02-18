#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "fdmngr.h"

void client_callback(fdmngr_t* manager, int fd, short revents,
                     void* user_data) {
    static int counter = 0;
    (void)user_data;
    if (revents & POLLIN) {
        char buffer[255];
        int bytes = read(fd, buffer, sizeof(buffer));
        if (bytes > 0) {
            buffer[bytes] = '\0';
            // printf("Input: %s", buffer);
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
        fdmngr_remove(manager, fd);
        close(fd);
    }
}

void listen_server_callback(fdmngr_t* manager, int fd, short revents,
                            void* user_data) {
    if (revents & POLLIN) {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int client_fd = accept(fd, (struct sockaddr*)&client_addr, &len);
        printf("New client %d\n", client_fd);
        // fdmngr_add_options(client_fd, O_NONBLOCK);
        if (fdmngr_add(manager, client_fd, POLLIN, FDMNGR_AUTOMATIC_CLOSING,
                       &client_callback, user_data) == -1) {
            printf("New client on fd %d force closed\n", client_fd);
            close(client_fd);
        }
        else {
            uint8_t ip[4];
            uint16_t port;
            fdmngr_socket_get_ipv4_infos(client_addr, ip, &port);
            printf("New client on fd %d accepted (%d.%d.%d.%d:%d)\n", client_fd,
                   ip[0], ip[1], ip[2], ip[3], port);
            int ret_write = write(client_fd, "Wellcome\n", 9);
            (void)ret_write;
        }
    }
    else if (revents & (POLLHUP | POLLERR)) {
        printf("Error or hang up on fd %d\n", fd);
        fdmngr_remove(manager, fd);
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

    fdmngr_t* manager = fdmngr_create(3);
    if (manager == NULL) {
        return (-1);
    }

    // Setup a listening socket
    int listenFd = fdmngr_socket_create_tcp_reuseaddr("127.0.0.1", 4242);
    listen(listenFd, 10);

    fdmngr_add(manager, listenFd, POLLIN, FDMNGR_AUTOMATIC_CLOSING,
               &listen_server_callback, NULL); // Monitor server

    g_is_run = 1;
    ini_signal();

    while (g_is_run > 0 && fdmngr_poll(manager, 60000) >= 0) {
        // continue looping
    }

    fdmngr_destroy(manager);

    printf("END\n");
    return 0;
}
