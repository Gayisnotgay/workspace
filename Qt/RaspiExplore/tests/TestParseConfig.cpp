#include <vector>
#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ParseConfig/ParseConfig.h"

using namespace testing;
using std::string;
using std::map;
using std::vector;

TEST(ParseAssignCfg, ShouldReturnEmptyMapWithNullInput)
{
    auto parsedCfg = ParseConfig::ParseAssignCfg("");
    EXPECT_TRUE(parsedCfg.empty());
}

TEST(ParseAssignCfg, ShouldReturnAand1WhenInputSingleAssignment)
{
    auto parsedCfg = ParseConfig::ParseAssignCfg("A=1");
    map<string, string> expParsedCfg = {{"A", "1"}};
    EXPECT_THAT(parsedCfg, expParsedCfg);
}

TEST(ParseAssignCfg, ShouldReturnAand1_Band2WhenInputMultiAssignment)
{
    auto parsedCfg = ParseConfig::ParseAssignCfg("A=1 B=100");
    map<string, string> expParsedCfg = {{"A", "1"}, {"B", "100"}};
    EXPECT_THAT(parsedCfg, expParsedCfg);
}

TEST(ParseAssignCfg, ShouldReturnAand1_Band2WhenInputMultiAssignmentWithSpace)
{
    auto parsedCfg = ParseConfig::ParseAssignCfg("A =1  B= 100 ");
    map<string, string> expParsedCfg = {{"A", "1"}, {"B", "100"}};
    EXPECT_THAT(parsedCfg, expParsedCfg);
}

TEST(SplitCfgStr, ShouldReturnEmptyVectorWithNullInput)
{
    auto parsedCfg = ParseConfig::SplitCfgStr("");
    EXPECT_TRUE(parsedCfg.empty());
}

TEST(SplitCfgStr, ShouldReturnSingleElementWithSingleInput)
{
    auto parsedCfg = ParseConfig::SplitCfgStr("123");
    vector<string> expParsedCfg = {"123"};
    EXPECT_THAT(parsedCfg, expParsedCfg);
}

TEST(SplitCfgStr, ShouldReturnTwoElementWithTwoInput)
{
    auto parsedCfg = ParseConfig::SplitCfgStr("24,w128,");
    vector<string> expParsedCfg = {"24", "w128"};
    EXPECT_THAT(parsedCfg, expParsedCfg);
}

TEST(SplitCfgStr, ShouldReturnMultiElementWithMultiInput)
{
    auto parsedCfg = ParseConfig::SplitCfgStr("24, w128, 200, 100th");
    vector<string> expParsedCfg = {"24", "w128", "200", "100th"};
    EXPECT_THAT(parsedCfg, expParsedCfg);
}

TEST(SplitCfgStr, ShouldReturnMultiElementWithMultiInputandSpaces)
{
    auto parsedCfg = ParseConfig::SplitCfgStr(" 24 ,   w128,200, 100th  ");
    vector<string> expParsedCfg = {"24", "w128", "200", "100th"};
    EXPECT_THAT(parsedCfg, expParsedCfg);
}

TEST(ParsePattern, ShouldReturnUnderlineWhenInputMultiUnderlineCfg)
{
    auto parsedCfg = ParseConfig::ParsePattern("5-_", "(_{1,3})");
    EXPECT_TRUE(parsedCfg.size() == 1);
    EXPECT_THAT(parsedCfg, vector<string>{"_"});

    parsedCfg = ParseConfig::ParsePattern("5-__", "(_{1,3})");
    EXPECT_TRUE(parsedCfg.size() == 1);
    EXPECT_THAT(parsedCfg, (vector<string>{"__"}));

    parsedCfg = ParseConfig::ParsePattern("5-__", "(_)");
    EXPECT_TRUE(parsedCfg.size() == 2);
    EXPECT_THAT(parsedCfg, (vector<string>{"_", "_"}));
}

TEST(ParsePattern, ShouldReturnNumberWhenInputNumberCfgAndPatternEscape_d)
{
    auto parsedCfg = ParseConfig::ParsePattern("123", "(\\d+)");
    EXPECT_FALSE(parsedCfg.empty());
    EXPECT_THAT(parsedCfg, vector<string>{"123"});
}

TEST(ParsePattern, ShouldReturn2NumberWhenInput2NumberCfgAndPatternEscape_d)
{
    auto parsedCfg = ParseConfig::ParsePattern("123wsad, ds345", "(\\d+)");
    EXPECT_FALSE(parsedCfg.empty());
    EXPECT_THAT(parsedCfg, (vector<string>{"123", "345"}));
}

TEST(ParsePattern, ShouldReturnMultiFunctionWhenInputFunctionListCfgAndPatternEscape)
{
    string allFunctionList = R"(
    0.GpioGetFunction
    1.GpioMonitorFunction
    2.GpioPeriodSetFunction
    3.GpioSetFunction
    4.GpioSingFunction
    5.TerminalCtrlFunction
)";
    std::cout << "allFunctionList: " << allFunctionList << std::endl;
    auto parsedCfg = ParseConfig::ParsePattern(allFunctionList, "^\\s*\\d+\\.(\\w+)");
    EXPECT_FALSE(parsedCfg.empty());
    EXPECT_THAT(parsedCfg, (vector<string>{"GpioGetFunction", "GpioMonitorFunction", "GpioPeriodSetFunction", "GpioSetFunction", "GpioSingFunction", "TerminalCtrlFunction"}));
}