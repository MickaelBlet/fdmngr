#include <gtest/gtest.h>
#include <stdlib.h>

#include "fdmngr.h"

static void event(fdmngr_t* manager, int fd, short revents, void* user_data) {
    (void)manager;
    (void)fd;
    (void)revents;
    (void)user_data;
}

GTEST_TEST(fdmngr_add, fail) {
    {
        fdmngr_t* manager;
        manager = fdmngr_create(1);
        ASSERT_NE(manager, nullptr);
        EXPECT_EQ(fdmngr_add(manager, 0, POLLIN, FDMNGR_NOT_AUTOMATIC_CLOSING,
                             event, NULL),
                  0);
        EXPECT_NE(fdmngr_add(manager, 1, POLLIN, FDMNGR_NOT_AUTOMATIC_CLOSING,
                             event, NULL),
                  0);
        fdmngr_destroy(manager);
    }
    {
        fdmngr_t* manager;
        manager = fdmngr_create(1);
        ASSERT_NE(manager, nullptr);
        EXPECT_NE(fdmngr_add(manager, -1, POLLIN, FDMNGR_NOT_AUTOMATIC_CLOSING,
                             event, NULL),
                  0);
        EXPECT_NE(fdmngr_add(manager, 0, 0, FDMNGR_NOT_AUTOMATIC_CLOSING, event,
                             NULL),
                  0);
        EXPECT_NE(fdmngr_add(manager, 0, POLLIN, FDMNGR_NOT_AUTOMATIC_CLOSING,
                             NULL, NULL),
                  0);
        fdmngr_destroy(manager);
    }
    {
        fdmngr_t* manager;
        manager = fdmngr_create(1);
        ASSERT_NE(manager, nullptr);
        EXPECT_EQ(fdmngr_add(manager, 0, POLLIN, FDMNGR_NOT_AUTOMATIC_CLOSING,
                             event, NULL),
                  0);
        EXPECT_NE(fdmngr_add(manager, 0, POLLIN, FDMNGR_NOT_AUTOMATIC_CLOSING,
                             event, NULL),
                  0);
        fdmngr_destroy(manager);
    }
    {
        fdmngr_t* manager;
        manager = fdmngr_create(2);
        ASSERT_NE(manager, nullptr);
        EXPECT_EQ(fdmngr_add(manager, 0, POLLIN, FDMNGR_NOT_AUTOMATIC_CLOSING,
                             event, NULL),
                  0);
        EXPECT_NE(fdmngr_add(manager, 0, POLLIN, FDMNGR_NOT_AUTOMATIC_CLOSING,
                             event, NULL),
                  0);
        fdmngr_destroy(manager);
    }
}

GTEST_TEST(fdmngr_add, success) {
    fdmngr_t* manager;
    manager = fdmngr_create(2);
    ASSERT_NE(manager, nullptr);
    EXPECT_EQ(fdmngr_add(manager, 0, POLLIN, FDMNGR_NOT_AUTOMATIC_CLOSING,
                         event, NULL),
              0);
    EXPECT_EQ(fdmngr_add(manager, 1, POLLIN, FDMNGR_NOT_AUTOMATIC_CLOSING,
                         event, NULL),
              0);
    fdmngr_destroy(manager);
}
