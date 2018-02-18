#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <sys/socket.h>

#include "blet/mockf.h"
#include "fdmngr_socket.h"

GTEST_TEST(fdmngr_socket_create, fail) {
    using ::testing::_;
    using ::testing::Return;

    { EXPECT_EQ(fdmngr_socket_create(FDMNGR_SOCKET_TYPE_TCP, NULL, 4242), -1); }
    { EXPECT_EQ(fdmngr_socket_create(FDMNGR_SOCKET_TYPE_UDP, NULL, 4242), -1); }
    {
        EXPECT_EQ(
            fdmngr_socket_create(FDMNGR_SOCKET_TYPE_TCP_REUSEADDR, NULL, 4242),
            -1);
    }
    {
        EXPECT_EQ(
            fdmngr_socket_create(FDMNGR_SOCKET_TYPE_UDP_REUSEADDR, NULL, 4242),
            -1);
    }
    {
        EXPECT_EQ(fdmngr_socket_create(FDMNGR_SOCKET_TYPE_TCP_IPV6, NULL, 4242),
                  -1);
    }
    {
        EXPECT_EQ(fdmngr_socket_create(FDMNGR_SOCKET_TYPE_UDP_IPV6, NULL, 4242),
                  -1);
    }
    {
        EXPECT_EQ(fdmngr_socket_create(FDMNGR_SOCKET_TYPE_TCP_IPV6_REUSEADDR,
                                       NULL, 4242),
                  -1);
    }
    {
        EXPECT_EQ(fdmngr_socket_create(FDMNGR_SOCKET_TYPE_UDP_IPV6_REUSEADDR,
                                       NULL, 4242),
                  -1);
    }
    {
        EXPECT_EQ(fdmngr_socket_create(FDMNGR_SOCKET_TYPE_UNIX_STREAM, NULL),
                  -1);
    }
    {
        EXPECT_EQ(fdmngr_socket_create(FDMNGR_SOCKET_TYPE_UNIX_DGRAM, NULL),
                  -1);
    }
}
