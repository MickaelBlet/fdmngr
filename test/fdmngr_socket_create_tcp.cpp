#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <sys/socket.h>

#include "blet/mockf.h"
#include "fdmngr_socket.h"

MOCKF_FUNCTION3(int, socket, (int __domain, int __type, int __protocol));
MOCKF_FUNCTION3(int, inet_pton,
                (int __af, const char* __restrict__ __cp,
                 void* __restrict__ __buf));
MOCKF_FUNCTION5(int, setsockopt,
                (int __fd, int __level, int __optname, const void* __optval,
                 socklen_t __optlen));
MOCKF_FUNCTION3(int, bind,
                (int __fd, const struct sockaddr* __addr, socklen_t __len));

GTEST_TEST(fdmngr_socket_create_tcp, fail) {
    using ::testing::_;
    using ::testing::Return;

    { EXPECT_EQ(fdmngr_socket_create_tcp(NULL, 4242), -1); }
    { EXPECT_EQ(fdmngr_socket_create_tcp(NULL, -1), -1); }
    { EXPECT_EQ(fdmngr_socket_create_tcp(NULL, 65536), -1); }
    {
        MOCKF_INIT(socket);
        MOCKF_EXPECT_CALL(socket, (_, _, _)).WillOnce(Return(-1));
        MOCKF_GUARD(socket);
        EXPECT_EQ(fdmngr_socket_create_tcp("0.0.0.0", 65535), -1);
    }
    {
        MOCKF_INIT(inet_pton);
        MOCKF_EXPECT_CALL(inet_pton, (_, _, _)).WillOnce(Return(-1));
        MOCKF_GUARD(inet_pton);
        EXPECT_EQ(fdmngr_socket_create_tcp("0.0.0.0", 65535), -1);
    }
    {
        MOCKF_INIT(bind);
        MOCKF_EXPECT_CALL(bind, (_, _, _)).WillOnce(Return(-1));
        MOCKF_GUARD(bind);
        EXPECT_EQ(fdmngr_socket_create_tcp("0.0.0.0", 65535), -1);
    }
}

GTEST_TEST(fdmngr_socket_create_tcp_reuseaddr, fail) {
    using ::testing::_;
    using ::testing::Return;

    { EXPECT_EQ(fdmngr_socket_create_tcp_reuseaddr(NULL, 4242), -1); }
    { EXPECT_EQ(fdmngr_socket_create_tcp_reuseaddr(NULL, -1), -1); }
    { EXPECT_EQ(fdmngr_socket_create_tcp_reuseaddr(NULL, 65536), -1); }
    {
        MOCKF_INIT(socket);
        MOCKF_EXPECT_CALL(socket, (_, _, _)).WillOnce(Return(-1));
        MOCKF_GUARD(socket);
        EXPECT_EQ(fdmngr_socket_create_tcp_reuseaddr("0.0.0.0", 65535), -1);
    }
    {
        MOCKF_INIT(inet_pton);
        MOCKF_EXPECT_CALL(inet_pton, (_, _, _)).WillOnce(Return(-1));
        MOCKF_GUARD(inet_pton);
        EXPECT_EQ(fdmngr_socket_create_tcp_reuseaddr("0.0.0.0", 65535), -1);
    }
    {
        MOCKF_INIT(setsockopt);
        MOCKF_EXPECT_CALL(setsockopt, (_, _, _, _, _)).WillOnce(Return(-1));
        MOCKF_GUARD(setsockopt);
        EXPECT_EQ(fdmngr_socket_create_tcp_reuseaddr("0.0.0.0", 65535), -1);
    }
#ifdef SO_REUSEPORT
    {
        MOCKF_INIT(setsockopt);
        MOCKF_EXPECT_CALL(setsockopt, (_, _, _, _, _))
            .WillOnce(Return(1))
            .WillOnce(Return(-1));
        MOCKF_GUARD(setsockopt);
        EXPECT_EQ(fdmngr_socket_create_tcp_reuseaddr("0.0.0.0", 65535), -1);
    }
#endif
    {
        MOCKF_INIT(bind);
        MOCKF_EXPECT_CALL(bind, (_, _, _)).WillOnce(Return(-1));
        MOCKF_GUARD(bind);
        EXPECT_EQ(fdmngr_socket_create_tcp_reuseaddr("0.0.0.0", 65535), -1);
    }
}

GTEST_TEST(fdmngr_socket_create_tcp_ipv6, fail) {
    using ::testing::_;
    using ::testing::Return;

    { EXPECT_EQ(fdmngr_socket_create_tcp_ipv6(NULL, 4242), -1); }
    { EXPECT_EQ(fdmngr_socket_create_tcp_ipv6(NULL, -1), -1); }
    { EXPECT_EQ(fdmngr_socket_create_tcp_ipv6(NULL, 65536), -1); }
    {
        MOCKF_INIT(socket);
        MOCKF_EXPECT_CALL(socket, (_, _, _)).WillOnce(Return(-1));
        MOCKF_GUARD(socket);
        EXPECT_EQ(fdmngr_socket_create_tcp_ipv6("", 65535), -1);
    }
    {
        MOCKF_INIT(inet_pton);
        MOCKF_EXPECT_CALL(inet_pton, (_, _, _)).WillOnce(Return(-1));
        MOCKF_GUARD(inet_pton);
        EXPECT_EQ(fdmngr_socket_create_tcp_ipv6("", 65535), -1);
    }
    {
        MOCKF_INIT(bind);
        MOCKF_EXPECT_CALL(bind, (_, _, _)).WillOnce(Return(-1));
        MOCKF_GUARD(bind);
        EXPECT_EQ(fdmngr_socket_create_tcp_ipv6(
                      "2001:0db8:3333:4444:5555:6666:7777:8888", 65535),
                  -1);
    }
}

GTEST_TEST(fdmngr_socket_create_tcp_ipv6_reuseaddr, fail) {
    using ::testing::_;
    using ::testing::Return;

    { EXPECT_EQ(fdmngr_socket_create_tcp_ipv6_reuseaddr(NULL, 4242), -1); }
    { EXPECT_EQ(fdmngr_socket_create_tcp_ipv6_reuseaddr(NULL, -1), -1); }
    { EXPECT_EQ(fdmngr_socket_create_tcp_ipv6_reuseaddr(NULL, 65536), -1); }
    {
        MOCKF_INIT(socket);
        MOCKF_EXPECT_CALL(socket, (_, _, _)).WillOnce(Return(-1));
        MOCKF_GUARD(socket);
        EXPECT_EQ(fdmngr_socket_create_tcp_ipv6_reuseaddr("", 65535), -1);
    }
    {
        MOCKF_INIT(inet_pton);
        MOCKF_EXPECT_CALL(inet_pton, (_, _, _)).WillOnce(Return(-1));
        MOCKF_GUARD(inet_pton);
        EXPECT_EQ(fdmngr_socket_create_tcp_ipv6_reuseaddr("", 65535), -1);
    }
    {
        MOCKF_INIT(setsockopt);
        MOCKF_EXPECT_CALL(setsockopt, (_, _, _, _, _)).WillOnce(Return(-1));
        MOCKF_GUARD(setsockopt);
        EXPECT_EQ(fdmngr_socket_create_tcp_ipv6_reuseaddr(
                      "2001:0db8:3333:4444:5555:6666:7777:8888", 65535),
                  -1);
    }
#ifdef SO_REUSEPORT
    {
        MOCKF_INIT(setsockopt);
        MOCKF_EXPECT_CALL(setsockopt, (_, _, _, _, _))
            .WillOnce(Return(1))
            .WillOnce(Return(-1));
        MOCKF_GUARD(setsockopt);
        EXPECT_EQ(fdmngr_socket_create_tcp_ipv6_reuseaddr(
                      "2001:0db8:3333:4444:5555:6666:7777:8888", 65535),
                  -1);
    }
#endif
    {
        MOCKF_INIT(bind);
        MOCKF_EXPECT_CALL(bind, (_, _, _)).WillOnce(Return(-1));
        MOCKF_GUARD(bind);
        EXPECT_EQ(fdmngr_socket_create_tcp_ipv6_reuseaddr(
                      "2001:0db8:3333:4444:5555:6666:7777:8888", 65535),
                  -1);
    }
}

GTEST_TEST(fdmngr_socket_create_tcp, success) {
    using ::testing::_;
    using ::testing::Return;

    {
        int sock = fdmngr_socket_create_tcp("0.0.0.0", 4242);
        EXPECT_NE(sock, -1);
        close(sock);
    }
}

GTEST_TEST(fdmngr_socket_create_tcp_reuseaddr, success) {
    using ::testing::_;
    using ::testing::Return;

    {
        int sock = fdmngr_socket_create_tcp_reuseaddr("0.0.0.0", 4242);
        EXPECT_NE(sock, -1);
        close(sock);
    }
}

GTEST_TEST(fdmngr_socket_create_tcp_ipv6, success) {
    using ::testing::_;
    using ::testing::Return;

    {
        int sock = fdmngr_socket_create_tcp_ipv6("::", 4242);
        EXPECT_NE(sock, -1);
        close(sock);
    }
}

GTEST_TEST(fdmngr_socket_create_tcp_ipv6_reuseaddr, success) {
    using ::testing::_;
    using ::testing::Return;

    {
        int sock = fdmngr_socket_create_tcp_ipv6_reuseaddr("::", 4242);
        EXPECT_NE(sock, -1);
        close(sock);
    }
}
