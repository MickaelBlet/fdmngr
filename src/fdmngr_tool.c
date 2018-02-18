#include <fcntl.h>

int fdmngr_tool_get_flags(int fd) {
    int sock_opt;

    /* get current flags of the fd */
    sock_opt = fcntl(fd, F_GETFL, 0);
    if (sock_opt < 0) {
        return -1;
    }
    return sock_opt;
}

__attribute__((weak)) int fdmngr_tool_set_flags(int fd, int flags) {
    int sock_opt;

    /* set new flags to the fd */
    sock_opt = fcntl(fd, F_SETFL, flags);
    if (sock_opt < 0) {
        return -1;
    }
    return sock_opt;
}

int fdmngr_tool_add_flags(int fd, int flags) {
    int sock_opt;

    /* get current flags of the fd */
    sock_opt = fdmngr_tool_get_flags(fd);
    if (sock_opt >= 0) {
        sock_opt |= flags;
        /* set new flags to the fd */
        if (fdmngr_tool_set_flags(fd, sock_opt) < 0) {
            return -1;
        }
    }
    else {
        return -1;
    }
    return sock_opt;
}

int fdmngr_tool_sub_flags(int fd, int flags) {
    int sock_opt;

    /* get current flags of the fd */
    sock_opt = fdmngr_tool_get_flags(fd);
    if (sock_opt >= 0) {
        sock_opt &= (~flags);
        /* set new flags to the fd */
        if (fdmngr_tool_set_flags(fd, sock_opt) < 0) {
            return -1;
        }
    }
    else {
        return -1;
    }
    return sock_opt;
}
