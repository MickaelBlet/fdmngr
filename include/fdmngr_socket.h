#ifndef FDMNGR_SOCKET_H_
#define FDMNGR_SOCKET_H_

#include <arpa/inet.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a TCP socket.
 * @param ip The IP address to bind to.
 * @param port The port number to bind to (0 for any available port).
 * @return The socket file descriptor, or -1 on error.
 */
int fdmngr_socket_create_tcp(const char* ip, int port);

/**
 * @brief Create a UDP socket.
 * @param ip The IP address to bind to.
 * @param port The port number to bind to (0 for any available port).
 * @return The socket file descriptor, or -1 on error.
 */
int fdmngr_socket_create_udp(const char* ip, int port);

/**
 * @brief Create a TCP socket with SO_REUSEADDR option.
 * @param ip The IP address to bind to.
 * @param port The port number to bind to (0 for any available port).
 * @return The socket file descriptor, or -1 on error.
 */
int fdmngr_socket_create_tcp_reuseaddr(const char* ip, int port);

/**
 * @brief Create a UDP socket with SO_REUSEADDR option.
 * @param ip The IP address to bind to.
 * @param port The port number to bind to (0 for any available port).
 * @return The socket file descriptor, or -1 on error.
 */
int fdmngr_socket_create_udp_reuseaddr(const char* ip, int port);

/**
 * @brief Create a TCP IPv6 socket.
 * @param ip The IPv6 address to bind to.
 * @param port The port number to bind to (0 for any available port).
 * @return The socket file descriptor, or -1 on error.
 */
int fdmngr_socket_create_tcp_ipv6(const char* ip, int port);

/**
 * @brief Create a UDP IPv6 socket.
 * @param ip The IPv6 address to bind to.
 * @param port The port number to bind to (0 for any available port).
 * @return The socket file descriptor, or -1 on error.
 */
int fdmngr_socket_create_udp_ipv6(const char* ip, int port);

/**
 * @brief Create a TCP IPv6 socket with SO_REUSEADDR option.
 * @param ip The IPv6 address to bind to.
 * @param port The port number to bind to (0 for any available port).
 * @return The socket file descriptor, or -1 on error.
 */
int fdmngr_socket_create_tcp_ipv6_reuseaddr(const char* ip, int port);

/**
 * @brief Create a UDP IPv6 socket with SO_REUSEADDR option.
 * @param ip The IPv6 address to bind to.
 * @param port The port number to bind to (0 for any available port).
 * @return The socket file descriptor, or -1 on error.
 */
int fdmngr_socket_create_udp_ipv6_reuseaddr(const char* ip, int port);

/**
 * @brief Create a Unix domain stream socket.
 * @param path The file system path for the socket.
 * @return The socket file descriptor, or -1 on error.
 */
int fdmngr_socket_create_unix_stream(const char* path);

/**
 * @brief Create a Unix domain datagram socket.
 * @param path The file system path for the socket.
 * @return The socket file descriptor, or -1 on error.
 */
int fdmngr_socket_create_unix_dgram(const char* path);

enum fdmngr_socket_type_e {
    FDMNGR_SOCKET_TYPE_TCP = 0,
    FDMNGR_SOCKET_TYPE_UDP = 1,
    FDMNGR_SOCKET_TYPE_TCP_REUSEADDR = 2,
    FDMNGR_SOCKET_TYPE_UDP_REUSEADDR = 3,
    FDMNGR_SOCKET_TYPE_TCP_IPV6 = 4,
    FDMNGR_SOCKET_TYPE_UDP_IPV6 = 5,
    FDMNGR_SOCKET_TYPE_TCP_IPV6_REUSEADDR = 6,
    FDMNGR_SOCKET_TYPE_UDP_IPV6_REUSEADDR = 7,
    FDMNGR_SOCKET_TYPE_UNIX_STREAM = 8,
    FDMNGR_SOCKET_TYPE_UNIX_DGRAM = 9
};

/**
 * @brief Create a socket of the specified type.
 *
 * For TCP and UDP types, the function expects two additional arguments:
 * - const char* ip: The IP address to bind to.
 * - int port: The port number to bind to.
 *
 * For Unix domain socket types, the function expects one additional argument:
 * - const char* path: The file system path for the socket.
 *
 * @param type The type of socket to create (see enum fdmngr_socket_type_e).
 * @return The socket file descriptor, or -1 on error.
 */
int fdmngr_socket_create(enum fdmngr_socket_type_e type, ...);

/**
 * @brief Connect a socket to a specified IP address and port.
 * @param socket The socket file descriptor to connect.
 * @param ip The IP address to connect to.
 * @param port The port number to connect to.
 * @return 0 on success, -1 on error.
 */
int fdmngr_socket_connect(int socket, const char* ip, int port);

/**
 * @brief Connect a socket to a specified IPv6 address and port.
 * @param socket The socket file descriptor to connect.
 * @param ip The IPv6 address to connect to.
 * @param port The port number to connect to.
 * @return 0 on success, -1 on error.
 */
int fdmngr_socket_connect_ipv6(int socket, const char* ip, int port);

/**
 * @brief Extract IPv4 address and port from a sockaddr_in structure.
 * @param in The sockaddr_in structure to extract information from.
 * @param out_ip The output buffer for the IPv4 address (4 bytes).
 * @param out_port The output pointer for the port number.
 */
void fdmngr_socket_get_ipv4_infos(struct sockaddr_in in, uint8_t out_ip[4],
                                  uint16_t* out_port);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef FDMNGR_SOCKET_H_ */
