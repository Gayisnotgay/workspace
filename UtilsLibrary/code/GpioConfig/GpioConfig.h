#pragma once

#include <tuple>
#include <vector>
#include "GpioDrvWrapper/GpioDrvWrapper.h"

// gpiolines, gpiovalues
using GpioAccessParam = std::tuple<std::vector<unsigned int>, std::vector<bool>>;

class GpioConfig {
public:
    GpioConfig(GpioDrvWrapper& gpioDrv) : m_gpioDrv(gpioDrv) {}
    virtual ~GpioConfig() = default;
    void Set(const GpioAccessParam& cfg);
    int Get(GpioAccessParam& cfg);

protected:
    GpioDrvWrapper& m_gpioDrv;
};