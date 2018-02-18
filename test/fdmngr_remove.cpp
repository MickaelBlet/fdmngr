#include <gtest/gtest.h>
#include <stdlib.h>

#include "fdmngr.h"

static void event(fdmngr_t* manager, int fd, short revents, void* user_data) {
    (void)manager;
    (void)fd;
    (void)revents;
    (void)user_data;
}

GTEST_TEST(fdmngr_remove, fail) {
    fdmngr_t* manager;
    manager = fdmngr_create(1);
    ASSERT_NE(manager, nullptr);
    EXPECT_EQ(fdmngr_add(manager, 0, POLLIN, FDMNGR_NOT_AUTOMATIC_CLOSING,
                         event, NULL),
              0);
    EXPECT_NE(fdmngr_remove(manager, 1), 0);
    fdmngr_destroy(manager);
}

GTEST_TEST(fdmngr_remove, success) {
    {
        fdmngr_t* manager;
        manager = fdmngr_create(1);
        ASSERT_NE(manager, nullptr);
        EXPECT_EQ(fdmngr_add(manager, 0, POLLIN, FDMNGR_NOT_AUTOMATIC_CLOSING,
                             event, NULL),
                  0);
        EXPECT_EQ(fdmngr_remove(manager, 0), 0);
        fdmngr_destroy(manager);
    }
    {
        fdmngr_t* manager;
        manager = fdmngr_create(1);
        ASSERT_NE(manager, nullptr);
        int dev_null = open("/dev/null", O_RDONLY);
        EXPECT_EQ(fdmngr_add(manager, dev_null, POLLIN,
                             FDMNGR_AUTOMATIC_CLOSING, event, NULL),
                  0);
        EXPECT_EQ(fdmngr_remove(manager, dev_null), 0);
        fdmngr_destroy(manager);
    }
    {
        fdmngr_t* manager;
        manager = fdmngr_create(2);
        ASSERT_NE(manager, nullptr);
        int dev_null = open("/dev/null", O_RDONLY);
        EXPECT_EQ(fdmngr_add(manager, dev_null, POLLIN,
                             FDMNGR_AUTOMATIC_CLOSING, event, NULL),
                  0);
        EXPECT_EQ(fdmngr_add(manager, 0, POLLIN, FDMNGR_NOT_AUTOMATIC_CLOSING,
                             event, NULL),
                  0);
        EXPECT_EQ(fdmngr_remove(manager, dev_null), 0);
        EXPECT_EQ(fdmngr_remove(manager, 0), 0);
        fdmngr_destroy(manager);
    }
}
