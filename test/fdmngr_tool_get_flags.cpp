#include <fcntl.h>
#include <gtest/gtest.h>

#include "fdmngr_tool.h"

GTEST_TEST(fdmngr_tool_get_flags, fail) {
    EXPECT_EQ(fdmngr_tool_get_flags(66666), -1);
}

GTEST_TEST(fdmngr_tool_get_flags, success) {
    int current_flags;
    current_flags = fcntl(0, F_GETFL, 0);
    EXPECT_EQ(fdmngr_tool_get_flags(0), current_flags);
}
