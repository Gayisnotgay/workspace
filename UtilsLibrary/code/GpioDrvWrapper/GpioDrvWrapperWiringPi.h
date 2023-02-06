#pragma once

#include "GpioDrvWrapper.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include "gpiod.hpp"

class GpioDrvWrapperWiringPi : public GpioDrvWrapper {
public:
    GpioDrvWrapperWiringPi(const std::string& devName = "gpiochip0");
    ~GpioDrvWrapperWiringPi() override = default;
    void Initialize() override;
    int SetPinMode(unsigned int pinBcmOffset, PinMode mode) override;
    int SetPinValue(unsigned int pinBcmOffset, bool isActive) override;
    bool GetPinValue(unsigned int pinBcmOffset) override;

    int SetPinsMode(const std::vector<unsigned int> &pinsBcmOffset, PinMode mode) override;
    int SetPinsValues(const std::vector<unsigned int> &pinsBcmOffset, const std::vector<bool>& isActive) override;
    std::vector<bool> GetPinsValues(const std::vector<unsigned int> &pinsBcmOffset) override;

private:
    std::string m_devName;
};