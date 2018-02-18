#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <sys/socket.h>

#include "blet/mockf.h"
#include "fdmngr_socket.h"

MOCKF_FUNCTION3(int, socket, (int __domain, int __type, int __protocol));
MOCKF_FUNCTION3(int, bind,
                (int __fd, const struct sockaddr* __addr, socklen_t __len));

GTEST_TEST(fdmngr_socket_create_unix_stream, fail) {
    using ::testing::_;
    using ::testing::Return;

    { EXPECT_EQ(fdmngr_socket_create_unix_stream(NULL), -1); }
    {
        MOCKF_INIT(socket);
        MOCKF_EXPECT_CALL(socket, (_, _, _)).WillOnce(Return(-1));
        MOCKF_GUARD(socket);
        EXPECT_EQ(fdmngr_socket_create_unix_stream(""), -1);
    }
    {
        MOCKF_INIT(bind);
        MOCKF_EXPECT_CALL(bind, (_, _, _)).WillOnce(Return(-1));
        MOCKF_GUARD(bind);
        EXPECT_EQ(fdmngr_socket_create_unix_stream(""), -1);
    }
}

GTEST_TEST(fdmngr_socket_create_unix_dgram, fail) {
    using ::testing::_;
    using ::testing::Return;

    { EXPECT_EQ(fdmngr_socket_create_unix_dgram(NULL), -1); }
    {
        MOCKF_INIT(socket);
        MOCKF_EXPECT_CALL(socket, (_, _, _)).WillOnce(Return(-1));
        MOCKF_GUARD(socket);
        EXPECT_EQ(fdmngr_socket_create_unix_dgram(""), -1);
    }
    {
        MOCKF_INIT(bind);
        MOCKF_EXPECT_CALL(bind, (_, _, _)).WillOnce(Return(-1));
        MOCKF_GUARD(bind);
        EXPECT_EQ(fdmngr_socket_create_unix_dgram(""), -1);
    }
}

GTEST_TEST(fdmngr_socket_create_unix_stream, success) {
    using ::testing::_;
    using ::testing::Return;

    {
        MOCKF_INIT(bind);
        MOCKF_EXPECT_CALL(bind, (_, _, _)).WillOnce(Return(0));
        MOCKF_GUARD(bind);
        int sock = fdmngr_socket_create_unix_stream("");
        EXPECT_NE(sock, -1);
        close(sock);
    }
}

GTEST_TEST(fdmngr_socket_create_unix_dgram, success) {
    using ::testing::_;
    using ::testing::Return;

    {
        int sock = fdmngr_socket_create_unix_dgram("");
        EXPECT_NE(sock, -1);
        close(sock);
    }
}
