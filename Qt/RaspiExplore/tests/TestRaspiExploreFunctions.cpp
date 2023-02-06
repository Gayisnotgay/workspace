#include <vector>
#include <string>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "AutoRegFactory/AutoRegFactory.h"
#include "GpioDrvWrapper/GpioDrvWrapperGpiod.h"
#include "FakeGpioDrv.h"
#include "SmallTools/HelperDefine.h"

using std::vector;
using std::string;
using std::get;

TEST(RunFunctions, ShouldReturnAllFuncInfoWhenQuery)
{
    auto funcInfo = FuncFactory<int, int, char**>::Instance().GetAllFuncInfo();
    EXPECT_EQ(get<0>(funcInfo), 6);
    EXPECT_TRUE(get<1>(funcInfo).find("GpioGetFunction") != string::npos);
    EXPECT_TRUE(get<1>(funcInfo).find("GpioMonitorFunction") != string::npos);
    EXPECT_TRUE(get<1>(funcInfo).find("GpioSetFunction") != string::npos);
    EXPECT_TRUE(get<1>(funcInfo).find("GpioPeriodSetFunction") != string::npos);
    EXPECT_TRUE(get<1>(funcInfo).find("GpioSingFunction") != string::npos);
    EXPECT_TRUE(get<1>(funcInfo).find("TerminalCtrlFunction") != string::npos);
}

TEST(RunFunctions, ShouldGpioGetFunctionReturnSuccessWhenInputValidCmdPara)
{
    char* funcPara[] = {const_cast<char*>("GpioGetFunction"), const_cast<char*>("FakeGpioDrv"), const_cast<char*>("18,23,25")};
    int result = FuncFactory<int, int, char**>::Instance().RunFunc("GpioGetFunction", COUNTOF(funcPara), funcPara);
    EXPECT_EQ(result, EXIT_SUCCESS);
}

TEST(RunFunctions, ShouldGpioSetFunctionReturnFailureWhenInputInValidPara)
{
    char* funcPara[] = {const_cast<char*>("GpioSetFunction"), const_cast<char*>("FakeGpioDrv"), const_cast<char*>("18,23,25")};
    int result = FuncFactory<int, int, char**>::Instance().RunFunc("GpioSetFunction", COUNTOF(funcPara), funcPara);
    EXPECT_EQ(result, EXIT_FAILURE);
}

TEST(RunFunctions, ShouldGpioSetFunctionReturnSuccessWhenInputValidPara)
{
    char* funcPara[] = {const_cast<char*>("GpioSetFunction"), const_cast<char*>("FakeGpioDrv"), const_cast<char*>("18=1,23=0,25=1")};
    int result = FuncFactory<int, int, char**>::Instance().RunFunc("GpioSetFunction", COUNTOF(funcPara), funcPara);
    EXPECT_EQ(result, EXIT_SUCCESS);
}

TEST(RunFunctions, ShouldReturnAllAdvancedFuncInfoWhenQuery)
{
    auto funcInfo = FuncFactory<int, const vector<string>&, string&, int*>::Instance().GetAllFuncInfo();
    EXPECT_EQ(get<0>(funcInfo), 4);
    EXPECT_TRUE(get<1>(funcInfo).find("GpioGetFunction") != string::npos);
    EXPECT_TRUE(get<1>(funcInfo).find("GpioMonitorFunction") != string::npos);
    EXPECT_TRUE(get<1>(funcInfo).find("GpioSetFunction") != string::npos);
    EXPECT_TRUE(get<1>(funcInfo).find("GpioPeriodSetFunction") != string::npos);
}

TEST(RunFunctions, ShouldGpioGetFunctionReturnSuccessWhenInputValidAdvancedPara)
{
    vector<string> args = {"GpioGetFunction", "FakeGpioDrv", "18", "23", "25"};
    string output;
    int result = FuncFactory<int, const vector<string>&, string&, int*>::Instance().RunFunc("GpioGetFunction", args, output, nullptr);
    EXPECT_EQ(result, EXIT_SUCCESS);
}

TEST(RunFunctions, ShouldGpioSetFunctionReturnSuccessWhenInputValidAdvancedPara)
{
    vector<string> args = {"GpioSetFunction", "FakeGpioDrv", "18", "1", "23", "1", "25", "1"};
    string output;
    int result = FuncFactory<int, const vector<string>&, string&, int*>::Instance().RunFunc("GpioSetFunction", args, output, nullptr);
    EXPECT_EQ(result, EXIT_SUCCESS);
}

TEST(RunFunctions, ShouldGpioPeriodSetFunctionReturnSuccessWhenInputValidAdvancedPara)
{
    vector<string> args = {"GpioPeriodSetFunction", "FakeGpioDrv", "18", "1", "200", "200", "23", "0", "200", "200", "25", "1", "200", "200"};
    string output;
    int result = FuncFactory<int, const vector<string>&, string&, int*>::Instance().RunFunc("GpioSetFunction", args, output, nullptr);
    EXPECT_EQ(result, EXIT_SUCCESS);
}