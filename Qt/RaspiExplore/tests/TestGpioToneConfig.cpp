#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <ctime>
#include <unistd.h>
#include "GpioConfig/GpioToneConfig.h"
#include "SmallTools/TimeTool.h"
#include "FakeGpioDrv.h"

using namespace testing;
using std::string;
using std::map;

TEST(GpioToneConfig, ShouldReturn100msWhenSleep100ms)
{
    TimeTool tt;
    TimeTool::millisleep(100);
    EXPECT_GE(tt.GetElapsedMilliSecond(), 100);
    std::cout << tt.GetElapsedMilliSecond() << std::endl;
}

TEST(GpioToneConfig, ShouldRingFor1sWhenInputOnlyTenAlphabet)
{
    TimeTool tt;
    SingleGpioToneConfig toneCfg = { 8, {{265, 100}, {265, 100}, {265, 100}, {265, 100}, {265, 100}, {265, 100}, {265, 100}, {265, 100}, {265, 100}, {265, 100}} };
    FakeGpioDrv fakeDrv;
    EXPECT_CALL(fakeDrv, SetPinMode(8, GpioDrvWrapper::PinMode::MODE_OUTPUT)).WillOnce(Return(EXIT_SUCCESS));
    EXPECT_CALL(fakeDrv, SetPinValue(8, _)).WillRepeatedly(Return(EXIT_SUCCESS));
    GpioToneConfig(fakeDrv).ToneRing(toneCfg);
    EXPECT_GE(tt.GetElapsedMilliSecond(), 1000);
    std::cout << tt.GetElapsedMilliSecond() << std::endl;
}