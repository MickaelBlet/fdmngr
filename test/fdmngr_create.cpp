#include <gtest/gtest.h>
#include <stdlib.h>

#include "blet/mockf.h"
#include "fdmngr.h"

MOCKF_FUNCTION1(void*, malloc, (size_t __size));

ACTION_P(malloc_fdmngr, size) {
    EXPECT_EQ(arg0, size);
    return ::malloc(size);
}

GTEST_TEST(fdmngr_create, fail) {
    using ::testing::_;
    using ::testing::Return;

    {
        fdmngr_t* manager;
        manager = fdmngr_create(0);
        EXPECT_EQ(manager, nullptr);
    }
    {
        fdmngr_t* manager;
        {
            MOCKF_INIT(malloc);
            MOCKF_EXPECT_CALL(malloc, (_)).WillOnce(Return((void*)NULL));
            MOCKF_GUARD(malloc);
            manager = fdmngr_create(1);
        }
        EXPECT_EQ(manager, nullptr);
    }
    {
        fdmngr_t* manager;
        {
            MOCKF_INIT(malloc);
            MOCKF_EXPECT_CALL(malloc, (_))
                .WillOnce(malloc_fdmngr(sizeof(fdmngr_t)))
                .WillOnce(Return((void*)NULL));
            MOCKF_GUARD(malloc);
            manager = fdmngr_create(1);
        }
        EXPECT_EQ(manager, nullptr);
    }
    {
        fdmngr_t* manager;
        {
            MOCKF_INIT(malloc);
            MOCKF_EXPECT_CALL(malloc, (_))
                .WillOnce(malloc_fdmngr(sizeof(fdmngr_t)))
                .WillOnce(malloc_fdmngr(sizeof(struct pollfd) * 1))
                .WillOnce(Return((void*)NULL));
            MOCKF_GUARD(malloc);
            manager = fdmngr_create(1);
        }
        EXPECT_EQ(manager, nullptr);
    }
    {
        fdmngr_t* manager;
        {
            MOCKF_INIT(malloc);
            MOCKF_EXPECT_CALL(malloc, (_))
                .WillOnce(malloc_fdmngr(sizeof(fdmngr_t)))
                .WillOnce(malloc_fdmngr(sizeof(struct pollfd) * 42))
                .WillOnce(Return((void*)NULL));
            MOCKF_GUARD(malloc);
            manager = fdmngr_create(42);
        }
        EXPECT_EQ(manager, nullptr);
    }
}

GTEST_TEST(fdmngr_create, valid) {
    {
        fdmngr_t* manager;
        manager = fdmngr_create(1);
        ASSERT_NE(manager, nullptr);
        fdmngr_destroy(manager);
    }
    {
        fdmngr_t* manager;
        manager = fdmngr_create(42);
        ASSERT_NE(manager, nullptr);
        fdmngr_destroy(manager);
    }
}
