#include <gtest/gtest.h>

#include "fdmngr_socket.h"

GTEST_TEST(fdmngr_socket_get_ipv4_info, success) {
    {
        struct sockaddr_in in;
        in.sin_addr.s_addr = htonl(0x7F000001);
        in.sin_port = htons(4242);

        uint8_t ip[4];
        uint16_t port;
        fdmngr_socket_get_ipv4_infos(in, ip, &port);

        EXPECT_EQ(ip[0], 127);
        EXPECT_EQ(ip[1], 0);
        EXPECT_EQ(ip[2], 0);
        EXPECT_EQ(ip[3], 1);
        EXPECT_EQ(port, 4242);
    }
}
