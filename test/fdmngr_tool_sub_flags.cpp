#include <fcntl.h>
#include <gtest/gtest.h>

#include "blet/mockf.h"
#include "fdmngr_tool.h"

MOCKF_FUNCTION2(int, fdmngr_tool_set_flags, (int fd, int flags));

ACTION(fdmngr_tool_set_flags) {
    int fd = (int)arg0;
    int flags = (int)arg1;
    (void)fd;
    (void)flags;
    return -1;
}

GTEST_TEST(fdmngr_tool_sub_flags, fail) {
    using ::testing::_;
    using ::testing::Return;

    { EXPECT_EQ(fdmngr_tool_sub_flags(66666, 0), -1); }
    {
        MOCKF_INIT(fdmngr_tool_set_flags);
        MOCKF_EXPECT_CALL(fdmngr_tool_set_flags, (_, _)).WillOnce(Return(-1));
        MOCKF_GUARD(fdmngr_tool_set_flags);
        EXPECT_EQ(fdmngr_tool_sub_flags(0, 0), -1);
    }
}

GTEST_TEST(fdmngr_tool_sub_flags, success) {
    using ::testing::_;
    using ::testing::Return;

    int current_flags;
    current_flags = fcntl(0, F_GETFL, 0);
    MOCKF_INIT(fdmngr_tool_set_flags);
    MOCKF_EXPECT_CALL(fdmngr_tool_set_flags, (_, _))
        .WillOnce(Return(current_flags));
    MOCKF_GUARD(fdmngr_tool_set_flags);
    EXPECT_EQ(fdmngr_tool_sub_flags(0, 0), current_flags);
}
