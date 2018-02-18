#include "fdmngr_socket.h"

#include <arpa/inet.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

static int s_fdmngr_socket_create_inet(int type, const char* ip, int port,
                                       int reuseaddr) {
    struct sockaddr_in addr;
    int sockfd;
    int yes;

    if (ip == NULL || port < 0 || port > 65535) {
        return -1;
    }

    sockfd = socket(AF_INET, type, 0);
    if (sockfd < 0) {
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0) {
        close(sockfd);
        return -1;
    }

    if (reuseaddr) {
        /* active re use addr in socket */
        yes = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) ==
            -1) {
            close(sockfd);
            return -1;
        }
#ifdef SO_REUSEPORT
        yes = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(int)) ==
            -1) {
            close(sockfd);
            return -1;
        }
#endif
    }

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int fdmngr_socket_create_tcp(const char* ip, int port) {
    return s_fdmngr_socket_create_inet(SOCK_STREAM, ip, port, 0);
}

int fdmngr_socket_create_udp(const char* ip, int port) {
    return s_fdmngr_socket_create_inet(SOCK_DGRAM, ip, port, 0);
}

int fdmngr_socket_create_tcp_reuseaddr(const char* ip, int port) {
    return s_fdmngr_socket_create_inet(SOCK_STREAM, ip, port, 1);
}

int fdmngr_socket_create_udp_reuseaddr(const char* ip, int port) {
    return s_fdmngr_socket_create_inet(SOCK_DGRAM, ip, port, 1);
}

static int s_fdmngr_socket_create_inet6(int type, const char* ip, int port,
                                        int reuseaddr) {
    struct sockaddr_in6 addr6;
    int sockfd;
    int yes;

    if (ip == NULL || port < 0 || port > 65535) {
        return -1;
    }

    sockfd = socket(AF_INET6, type, 0);
    if (sockfd < 0) {
        return -1;
    }

    memset(&addr6, 0, sizeof(addr6));
    addr6.sin6_family = AF_INET6;
    addr6.sin6_port = htons(port);
    if (inet_pton(AF_INET6, ip, &addr6.sin6_addr) <= 0) {
        close(sockfd);
        return -1;
    }

    if (reuseaddr) {
        /* active re use addr in socket */
        yes = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) ==
            -1) {
            close(sockfd);
            return -1;
        }
#ifdef SO_REUSEPORT
        yes = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(int)) ==
            -1) {
            close(sockfd);
            return -1;
        }
#endif
    }

    if (bind(sockfd, (struct sockaddr*)&addr6, sizeof(addr6)) < 0) {
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int fdmngr_socket_create_tcp_ipv6(const char* ip, int port) {
    return s_fdmngr_socket_create_inet6(SOCK_STREAM, ip, port, 0);
}

int fdmngr_socket_create_udp_ipv6(const char* ip, int port) {
    return s_fdmngr_socket_create_inet6(SOCK_DGRAM, ip, port, 0);
}

int fdmngr_socket_create_tcp_ipv6_reuseaddr(const char* ip, int port) {
    return s_fdmngr_socket_create_inet6(SOCK_STREAM, ip, port, 1);
}

int fdmngr_socket_create_udp_ipv6_reuseaddr(const char* ip, int port) {
    return s_fdmngr_socket_create_inet6(SOCK_DGRAM, ip, port, 1);
}

static int s_fdmngr_socket_create_unix(int type, const char* path) {
    struct sockaddr_un addr;
    int sockfd;

    if (path == NULL) {
        return -1;
    }

    sockfd = socket(AF_UNIX, type, 0);
    if (sockfd < 0) {
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int fdmngr_socket_create_unix_stream(const char* path) {
    return s_fdmngr_socket_create_unix(SOCK_STREAM, path);
}

int fdmngr_socket_create_unix_dgram(const char* path) {
    return s_fdmngr_socket_create_unix(SOCK_DGRAM, path);
}

int fdmngr_socket_create(enum fdmngr_socket_type_e type, ...) {
    va_list args;
    va_start(args, type);
    int sockfd;

    const char* ip;
    int port;

    const char* path;

    sockfd = -1;
    switch (type) {
        case FDMNGR_SOCKET_TYPE_TCP: {
            ip = va_arg(args, const char*);
            port = va_arg(args, int);
            sockfd = fdmngr_socket_create_tcp(ip, port);
            break;
        }
        case FDMNGR_SOCKET_TYPE_UDP: {
            ip = va_arg(args, const char*);
            port = va_arg(args, int);
            sockfd = fdmngr_socket_create_udp(ip, port);
            break;
        }
        case FDMNGR_SOCKET_TYPE_TCP_REUSEADDR: {
            ip = va_arg(args, const char*);
            port = va_arg(args, int);
            sockfd = fdmngr_socket_create_tcp_reuseaddr(ip, port);
            break;
        }
        case FDMNGR_SOCKET_TYPE_UDP_REUSEADDR: {
            ip = va_arg(args, const char*);
            port = va_arg(args, int);
            sockfd = fdmngr_socket_create_udp_reuseaddr(ip, port);
            break;
        }
        case FDMNGR_SOCKET_TYPE_TCP_IPV6: {
            ip = va_arg(args, const char*);
            port = va_arg(args, int);
            sockfd = fdmngr_socket_create_tcp_ipv6(ip, port);
            break;
        }
        case FDMNGR_SOCKET_TYPE_UDP_IPV6: {
            ip = va_arg(args, const char*);
            port = va_arg(args, int);
            sockfd = fdmngr_socket_create_udp_ipv6(ip, port);
            break;
        }
        case FDMNGR_SOCKET_TYPE_TCP_IPV6_REUSEADDR: {
            ip = va_arg(args, const char*);
            port = va_arg(args, int);
            sockfd = fdmngr_socket_create_tcp_ipv6_reuseaddr(ip, port);
            break;
        }
        case FDMNGR_SOCKET_TYPE_UDP_IPV6_REUSEADDR: {
            ip = va_arg(args, const char*);
            port = va_arg(args, int);
            sockfd = fdmngr_socket_create_udp_ipv6_reuseaddr(ip, port);
            break;
        }
        case FDMNGR_SOCKET_TYPE_UNIX_STREAM: {
            path = va_arg(args, const char*);
            sockfd = fdmngr_socket_create_unix_stream(path);
            break;
        }
        case FDMNGR_SOCKET_TYPE_UNIX_DGRAM: {
            path = va_arg(args, const char*);
            sockfd = fdmngr_socket_create_unix_dgram(path);
            break;
        }
    }
    va_end(args);
    return sockfd;
}

int fdmngr_socket_connect(int socket, const char* ip, int port) {
    struct sockaddr_in addr;

    if (socket < 0 || ip == NULL || port <= 0 || port > 65535) {
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0) {
        return -1;
    }

    if (connect(socket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        return -1;
    }

    return 0;
}

int fdmngr_socket_connect_ipv6(int socket, const char* ip, int port) {
    struct sockaddr_in6 addr6;

    if (socket < 0 || ip == NULL || port <= 0 || port > 65535) {
        return -1;
    }

    memset(&addr6, 0, sizeof(addr6));
    addr6.sin6_family = AF_INET6;
    addr6.sin6_port = htons(port);
    if (inet_pton(AF_INET6, ip, &addr6.sin6_addr) <= 0) {
        return -1;
    }

    if (connect(socket, (struct sockaddr*)&addr6, sizeof(addr6)) < 0) {
        return -1;
    }

    return 0;
}

void fdmngr_socket_get_ipv4_infos(struct sockaddr_in in, uint8_t out_ip[4],
                                  uint16_t* out_port) {
    uint32_t ip = ntohl(in.sin_addr.s_addr); // convert to host byte order
    out_ip[0] = (ip >> 24) & 0xFF;
    out_ip[1] = (ip >> 16) & 0xFF;
    out_ip[2] = (ip >> 8) & 0xFF;
    out_ip[3] = ip & 0xFF;
    *out_port = htons(in.sin_port);
}
