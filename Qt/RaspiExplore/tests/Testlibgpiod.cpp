#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "GpioFunctions/GpioMonitorFunction.h"
#include "GpioDrvWrapper/GpioDrvWrapperGpiod.h"
#include "GpioConfig/GpioConfig.h"
#include "MessageCenter/MessageCenter.h"
#include "SmallTools/StringAlign.h"
#include "SmallTools/HelperDefine.h"
#include "TerminalCtrl/TerminalCtrl.h"

using namespace testing;
using std::string;
using std::vector;
using std::get;
using std::endl;

TEST(GpioDrvWrapperGpiod, ShouldReturnSuccessWhenGetPinsValues)
{
    GpioDrvWrapperGpiod drv("gpiochip0");
    vector<unsigned int> pinsOffset = {14,15,16,17};
    int result = drv.SetPinsMode(pinsOffset, GpioDrvWrapper::PinMode::MODE_INPUT);
    EXPECT_THAT(result, EXIT_SUCCESS);
    auto values = drv.GetPinsValues(pinsOffset);
    vector<bool> expValues = {true, true, false, false};
    EXPECT_THAT(values, expValues);

    result = drv.SetPinsMode({14,15,16,17}, GpioDrvWrapper::PinMode::MODE_INPUT);
    EXPECT_THAT(result, EXIT_SUCCESS);
    values = drv.GetPinsValues(pinsOffset);
    EXPECT_THAT(values, expValues);

    result = drv.SetPinsMode({14,15,16,17}, GpioDrvWrapper::PinMode::MODE_INPUT);
    EXPECT_THAT(result, EXIT_SUCCESS);
    values = drv.GetPinsValues(pinsOffset);
    EXPECT_THAT(values, expValues);

    result = drv.SetPinsMode({14,15,16,17}, GpioDrvWrapper::PinMode::MODE_INPUT);
    EXPECT_THAT(result, EXIT_SUCCESS);
    values = drv.GetPinsValues(pinsOffset);
    EXPECT_THAT(values, expValues);

    result = drv.SetPinsMode({14,15,16,17}, GpioDrvWrapper::PinMode::MODE_INPUT);
    EXPECT_THAT(result, EXIT_SUCCESS);
    values = drv.GetPinsValues(pinsOffset);
    EXPECT_THAT(values, expValues);
}

TEST(GpioDrvWrapperGpiod, ShouldReturnSuccessWhenContinuouslyCallGpioConfig)
{
    GpioDrvWrapperGpiod drv("gpiochip0");
	GpioAccessParam param;
    get<0>(param) = {14,15,16,17};
    vector<bool> expValues = {true, true, false, false};
    int index = 0;
    while (index < 10) {
        int result = GpioConfig(drv).Get(param);
        EXPECT_THAT(result, EXIT_SUCCESS) << index;
        EXPECT_THAT(get<1>(param), expValues) << index;
        ++index;
        sleep(2);
    }
}

TEST(GpioDrvWrapperGpiod, ShouldNotFailWhenForeverCallGpioConfig)
{
    GpioDrvWrapperGpiod drv("gpiochip0");
	GpioAccessParam param;
    get<0>(param) = {14,15,16,17};
	auto gpioMonitor = [this, &drv, &param]()->string {
		ostringstream gpioMonitorInfo;
		int result = GpioConfig(drv).Get(param);
		if (result != EXIT_SUCCESS) {
			MessageCenter::Instance() << "GpioConfig Get failed: " << endl;
			return gpioMonitorInfo.str();
		}

		vector<string> pinVals;
		for_each(get<1>(param).begin(), get<1>(param).end(), [&](bool active) { ostringstream os(std::ios_base::ate); os << (active ? TerminalCtrl::FontColor(TerminalCtrl::Color::GREEN)+"High" : TerminalCtrl::FontColor(TerminalCtrl::Color::RED)+"Low"); pinVals.emplace_back(os.str()); });
		return StringAlign::OutputTableRow(pinVals, 25);
	};

	MessageCenter::Instance().RefreshMessageUntill(gpioMonitor, []()->bool{ return false; });
}

TEST(GpioDrvWrapperGpiod, ShouldNotFailReturnSuccessWhenInputPins14_15_16_17)
{
    char* funcPara[] = {const_cast<char*>("GpioMonitorFunction"), const_cast<char*>("libgpiod"), const_cast<char*>("14,15,16,17")};
    GpioMonitorFunction().Run(COUNTOF(funcPara), funcPara);
}
