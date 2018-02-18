#include <gtest/gtest.h>
#include <unistd.h>

#include "blet/mockf.h"
#include "fdmngr.h"

MOCKF_FUNCTION1(int, close, (int fd));

static void event(fdmngr_t* manager, int fd, short revents, void* user_data) {
    (void)manager;
    (void)fd;
    (void)revents;
    (void)user_data;
}

GTEST_TEST(fdmngr_destroy, without_closing_fd) {
    using ::testing::_;
    using ::testing::Return;

    {
        fdmngr_t* manager;
        manager = fdmngr_create(1);
        EXPECT_NE(manager, nullptr);
        EXPECT_EQ(fdmngr_add(manager, 0, POLLIN, FDMNGR_NOT_AUTOMATIC_CLOSING,
                             event, NULL),
                  0);
        MOCKF_INIT(close);
        MOCKF_EXPECT_CALL(close, (_)).Times(0);
        MOCKF_GUARD(close);
        fdmngr_destroy(manager);
    }
}

GTEST_TEST(fdmngr_destroy, with_closing_fd) {
    using ::testing::_;
    using ::testing::Return;

    {
        fdmngr_t* manager;
        manager = fdmngr_create(1);
        EXPECT_NE(manager, nullptr);
        EXPECT_EQ(fdmngr_add(manager, 0, POLLIN, FDMNGR_AUTOMATIC_CLOSING,
                             event, NULL),
                  0);
        MOCKF_INIT(close);
        MOCKF_EXPECT_CALL(close, (_)).Times(1);
        MOCKF_GUARD(close);
        fdmngr_destroy(manager);
    }
}
