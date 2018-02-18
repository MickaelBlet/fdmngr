#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "fdmngr.h"

void stdin_callback(fdmngr_t* pm, int fd, short revents, void* user_data) {
    (void)user_data;
    if (revents & POLLIN) {
        char buffer[10];
        int bytes = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            printf("Result of input: %s", buffer);
        }
        else {
            printf("Disconnected or EOF\n");
            fdmngr_remove(pm, fd);
        }
    }
    else if (revents & (POLLHUP | POLLERR)) {
        printf("Error or hang up on fd %d\n", fd);
        fdmngr_remove(pm, fd);
    }
}

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    fdmngr_t* manager = fdmngr_create(1);
    if (manager == NULL) {
        return -1;
    }

    fdmngr_add(manager, 0, POLLIN | POLLHUP | POLLERR,
               FDMNGR_NOT_AUTOMATIC_CLOSING, &stdin_callback,
               NULL); // Monitor stdin

    printf("Input: ");
    fflush(stdout);
    while (fdmngr_poll(manager, 300000) >= 0) {
        // continue looping
    }
    fdmngr_destroy(manager);
    return 0;
}
