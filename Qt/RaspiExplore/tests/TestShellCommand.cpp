#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "SmallTools/ShellCommand.h"

using namespace testing;
using std::string;

TEST(ShellCommand, ShouldExecReturnSystemInfoWhenInput_uname)
{
    auto result = ShellCommand::Exec("uname -a");
    EXPECT_FALSE(result.empty()) << result;
}