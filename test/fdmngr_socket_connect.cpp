#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <sys/socket.h>

#include "blet/mockf.h"
#include "fdmngr_socket.h"

MOCKF_FUNCTION3(int, inet_pton,
                (int __af, const char* __restrict__ __cp,
                 void* __restrict__ __buf));
MOCKF_FUNCTION3(int, connect,
                (int __fd, const struct sockaddr* __addr, socklen_t __len));

GTEST_TEST(fdmngr_socket_connect, fail) {
    using ::testing::_;
    using ::testing::Return;

    { EXPECT_EQ(fdmngr_socket_connect(-1, "127.0.0.1", 4242), -1); }
    { EXPECT_EQ(fdmngr_socket_connect(0, NULL, 4242), -1); }
    { EXPECT_EQ(fdmngr_socket_connect(0, "127.0.0.1", -1), -1); }
    {
        MOCKF_INIT(inet_pton);
        MOCKF_EXPECT_CALL(inet_pton, (_, _, _)).WillOnce(Return(-1));
        MOCKF_GUARD(inet_pton);
        EXPECT_EQ(fdmngr_socket_connect(42, "127.0.0.1", 4242), -1);
    }
    {
        MOCKF_INIT(inet_pton);
        MOCKF_EXPECT_CALL(inet_pton, (_, _, _)).WillOnce(Return(1));
        MOCKF_GUARD(inet_pton);
        MOCKF_INIT(connect);
        MOCKF_EXPECT_CALL(connect, (_, _, _)).WillOnce(Return(-1));
        MOCKF_GUARD(connect);
        EXPECT_EQ(fdmngr_socket_connect(42, "127.0.0.1", 4242), -1);
    }
}

GTEST_TEST(fdmngr_socket_connect, success) {
    using ::testing::_;
    using ::testing::Return;

    {
        MOCKF_INIT(inet_pton);
        MOCKF_EXPECT_CALL(inet_pton, (_, _, _)).WillOnce(Return(1));
        MOCKF_GUARD(inet_pton);
        MOCKF_INIT(connect);
        MOCKF_EXPECT_CALL(connect, (_, _, _)).WillOnce(Return(0));
        MOCKF_GUARD(connect);
        EXPECT_EQ(fdmngr_socket_connect(42, "127.0.0.1", 4242), 0);
    }
}

GTEST_TEST(fdmngr_socket_connect_ipv6, fail) {
    using ::testing::_;
    using ::testing::Return;

    {
        EXPECT_EQ(fdmngr_socket_connect_ipv6(
                      -1, "2001:0db8:3333:4444:5555:6666:7777:8888", 4242),
                  -1);
    }
    { EXPECT_EQ(fdmngr_socket_connect_ipv6(0, NULL, 4242), -1); }
    {
        EXPECT_EQ(fdmngr_socket_connect_ipv6(
                      0, "2001:0db8:3333:4444:5555:6666:7777:8888", -1),
                  -1);
    }
    {
        MOCKF_INIT(inet_pton);
        MOCKF_EXPECT_CALL(inet_pton, (_, _, _)).WillOnce(Return(-1));
        MOCKF_GUARD(inet_pton);
        EXPECT_EQ(fdmngr_socket_connect_ipv6(
                      42, "2001:0db8:3333:4444:5555:6666:7777:8888", 4242),
                  -1);
    }
    {
        MOCKF_INIT(inet_pton);
        MOCKF_EXPECT_CALL(inet_pton, (_, _, _)).WillOnce(Return(1));
        MOCKF_GUARD(inet_pton);
        MOCKF_INIT(connect);
        MOCKF_EXPECT_CALL(connect, (_, _, _)).WillOnce(Return(-1));
        MOCKF_GUARD(connect);
        EXPECT_EQ(fdmngr_socket_connect_ipv6(
                      42, "2001:0db8:3333:4444:5555:6666:7777:8888", 4242),
                  -1);
    }
}

GTEST_TEST(fdmngr_socket_connect_ipv6, success) {
    using ::testing::_;
    using ::testing::Return;

    {
        MOCKF_INIT(inet_pton);
        MOCKF_EXPECT_CALL(inet_pton, (_, _, _)).WillOnce(Return(1));
        MOCKF_GUARD(inet_pton);
        MOCKF_INIT(connect);
        MOCKF_EXPECT_CALL(connect, (_, _, _)).WillOnce(Return(0));
        MOCKF_GUARD(connect);
        EXPECT_EQ(fdmngr_socket_connect_ipv6(
                      42, "2001:0db8:3333:4444:5555:6666:7777:8888", 4242),
                  0);
    }
}
