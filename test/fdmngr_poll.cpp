#include <gtest/gtest.h>
#include <poll.h>

#include "blet/mockf.h"
#include "fdmngr.h"

MOCKF_FUNCTION3(int, poll,
                (struct pollfd * __fds, nfds_t __nfds, int __timeout));

static void event(fdmngr_t* manager, int fd, short revents, void* user_data) {
    (void)manager;
    (void)user_data;
    EXPECT_EQ(fd, 0);
    EXPECT_EQ(revents, POLLIN);
    fdmngr_remove(manager, fd);
}

static void event0(fdmngr_t* manager, int fd, short revents, void* user_data) {
    (void)manager;
    (void)user_data;
    EXPECT_EQ(fd, 0);
    EXPECT_EQ(revents, POLLIN);
}

static void event1(fdmngr_t* manager, int fd, short revents, void* user_data) {
    (void)manager;
    (void)user_data;
    EXPECT_EQ(fd, 1);
    EXPECT_EQ(revents, POLLIN);
    fdmngr_remove(manager, fd);
}

ACTION(poll) {
    struct pollfd* poll_fds = (struct pollfd*)arg0;
    nfds_t poll_nfds = (nfds_t)arg1;
    (void)poll_nfds;
    poll_fds[0].revents = POLLIN;
    return 1;
}

ACTION(poll2) {
    struct pollfd* poll_fds = (struct pollfd*)arg0;
    nfds_t poll_nfds = (nfds_t)arg1;
    (void)poll_nfds;
    poll_fds[0].revents = POLLIN;
    poll_fds[1].revents = POLLIN;
    return 2;
}

GTEST_TEST(fdmngr_poll, fail) {
    using ::testing::_;
    using ::testing::Return;

    {
        fdmngr_t* manager;
        manager = fdmngr_create(1);
        ASSERT_NE(manager, nullptr);
        EXPECT_EQ(fdmngr_add(manager, 0, POLLIN, FDMNGR_NOT_AUTOMATIC_CLOSING,
                             event, NULL),
                  0);
        {
            MOCKF_INIT(poll);
            MOCKF_EXPECT_CALL(poll, (_, _, _)).WillOnce(Return(-1));
            MOCKF_GUARD(poll);
            EXPECT_NE(fdmngr_poll(manager, -1), 0);
        }
        fdmngr_destroy(manager);
    }
    {
        fdmngr_t* manager;
        manager = fdmngr_create(1);
        ASSERT_NE(manager, nullptr);
        EXPECT_EQ(fdmngr_add(manager, 0, POLLIN, FDMNGR_NOT_AUTOMATIC_CLOSING,
                             event, NULL),
                  0);
        {
            MOCKF_INIT(poll);
            MOCKF_EXPECT_CALL(poll, (_, _, _)).WillOnce(Return(0));
            MOCKF_GUARD(poll);
            EXPECT_EQ(fdmngr_poll(manager, 42), 0);
        }
        fdmngr_destroy(manager);
    }
    {
        fdmngr_t* manager;
        manager = fdmngr_create(1);
        ASSERT_NE(manager, nullptr);
        { EXPECT_EQ(fdmngr_poll(manager, 42), -1); }
        fdmngr_destroy(manager);
    }
}

GTEST_TEST(fdmngr_poll, success) {
    using ::testing::_;

    {
        fdmngr_t* manager;
        manager = fdmngr_create(1);
        ASSERT_NE(manager, nullptr);
        EXPECT_EQ(fdmngr_add(manager, 0, POLLIN, FDMNGR_NOT_AUTOMATIC_CLOSING,
                             event, NULL),
                  0);
        {
            MOCKF_INIT(poll);
            MOCKF_EXPECT_CALL(poll, (_, _, _)).WillOnce(poll());
            MOCKF_GUARD(poll);
            EXPECT_EQ(fdmngr_poll(manager, 42), 1);
        }
        fdmngr_destroy(manager);
    }
    {
        fdmngr_t* manager;
        manager = fdmngr_create(2);
        ASSERT_NE(manager, nullptr);
        EXPECT_EQ(fdmngr_add(manager, 0, POLLIN, FDMNGR_NOT_AUTOMATIC_CLOSING,
                             event0, NULL),
                  0);
        EXPECT_EQ(fdmngr_add(manager, 1, POLLIN, FDMNGR_NOT_AUTOMATIC_CLOSING,
                             event1, NULL),
                  0);
        {
            MOCKF_INIT(poll);
            MOCKF_EXPECT_CALL(poll, (_, _, _)).WillOnce(poll2());
            MOCKF_GUARD(poll);
            EXPECT_EQ(fdmngr_poll(manager, 42), 2);
        }
        fdmngr_destroy(manager);
    }
}
