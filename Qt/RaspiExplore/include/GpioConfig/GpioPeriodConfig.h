#pragma once

#include <vector>
#include "GpioConfig.h"

struct SingleGpioPeriodConfig {
    unsigned int offset;
    bool initValue;
    std::vector<unsigned int> switchTimeInMilliSecond;

    bool operator==(const SingleGpioPeriodConfig& other) const
    {
        return (offset == other.offset) && (initValue == other.initValue)&& (switchTimeInMilliSecond == other.switchTimeInMilliSecond);
    }
};

class GpioPeriodConfig: public GpioConfig {
public:
    GpioPeriodConfig(GpioDrvWrapper& gpioDrv) : GpioConfig(gpioDrv) {}
    ~GpioPeriodConfig() override = default;
    void ConfigCandence(const std::vector<SingleGpioPeriodConfig>& gpioCandence);
    void Run(int* ctrlFlag = nullptr);

private:
    void DriveGpio(const SingleGpioPeriodConfig& lineCfg, int* ctrlFlag);

    std::vector<SingleGpioPeriodConfig> m_cadence;
};