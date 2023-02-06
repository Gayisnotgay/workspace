#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "AutoRegFactory/AutoRegFactory.h"
#include "GpioDrvWrapper/GpioDrvWrapper.h"
#include "GpioDrvWrapper/GpioDrvWrapperGpiod.h"
#include "FakeGpioDrv.h"

using std::string;
using std::get;
using std::unique_ptr;

GPIODRV_AUTO_REG(FakeGpioDrv, "FakeGpioDrv");

TEST(GpioDrvWrapper, ShouldReturnAllDrvInfoWhenQuery)
{
    auto funcInfo = FuncFactory<unique_ptr<GpioDrvWrapper>, const string&>::Instance().GetAllFuncInfo();
    EXPECT_EQ(get<0>(funcInfo), 3);
    EXPECT_TRUE(get<1>(funcInfo).find("libgpiod") != string::npos);
    EXPECT_TRUE(get<1>(funcInfo).find("wiringPi") != string::npos);
    EXPECT_TRUE(get<1>(funcInfo).find("FakeGpioDrv") != string::npos);
}