#pragma once
#include <bits/c++config.h>
#include <vector>
#include <memory>
#include "AutoRegFactory/AutoRegFactory.h"

class GpioDrvWrapper {
public:
    GpioDrvWrapper() = default;
    virtual ~GpioDrvWrapper() = default;
    virtual void Initialize() = 0;
    
    enum class PinMode : int{
        MODE_INPUT = 0,
        MODE_OUTPUT,
    };

    virtual int SetPinMode(unsigned int pinBcmOffset, PinMode mode) = 0;
    virtual int SetPinValue(unsigned int pinBcmOffset, bool isActive) = 0;
    virtual bool GetPinValue(unsigned int pinBcmOffset) = 0;

    virtual int SetPinsMode(const std::vector<unsigned int> &pinsBcmOffset, PinMode mode) = 0;
    virtual int SetPinsValues(const std::vector<unsigned int> &pinsBcmOffset, const std::vector<bool>& isActive) = 0;
    virtual std::vector<bool> GetPinsValues(const std::vector<unsigned int> &pinsBcmOffset) = 0;
};

#ifndef GPIODRV_AUTO_REG
#define GPIODRV_AUTO_REG(GpioDrv, GpioDrvInfo) \
    FUNC_AUTO_REG(GpioDrv, GpioDrvInfo, [](const std::string& devName){ return std::unique_ptr<GpioDrvWrapper>(new GpioDrv(devName)); }, std::unique_ptr<GpioDrvWrapper>, const std::string&)
#endif