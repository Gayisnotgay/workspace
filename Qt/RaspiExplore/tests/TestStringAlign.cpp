#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "SmallTools/StringAlign.h"

using namespace testing;
using std::string;
using std::vector;
using std::make_tuple;

TEST(StringAlign, ShouldFormatStringWithLeftAlignAndWidth10WhenInputWidth10)
{
    vector<string> tableRow = {"Active", "Inactive", "Die", "Unknown"};
    EXPECT_EQ(StringAlign::OutputTableRow(tableRow, 10), "Active    Inactive  Die       Unknown   ");
}