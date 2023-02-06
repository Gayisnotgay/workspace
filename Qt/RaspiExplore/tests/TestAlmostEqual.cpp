#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "SmallTools/AlmostEqual.h"

using namespace testing;

TEST(IsAlmostEqual, ShouldReturnFalseWhenCompare0dot0and0dot1)
{
    EXPECT_FALSE(IsAlmostEqual(0.0, 0.1));
}

TEST(IsAlmostEqual, ShouldReturnTrueWhenCompare0dot01and0dot01)
{
    EXPECT_TRUE(IsAlmostEqual(0.01, 0.01));
    EXPECT_TRUE(IsAlmostEqual(-0.009, -0.009));
    EXPECT_TRUE(IsAlmostEqual(0.0, 0.0));
}