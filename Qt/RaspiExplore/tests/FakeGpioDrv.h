#pragma once
#include <string>
#include "gmock/gmock.h"
#include "GpioDrvWrapper/GpioDrvWrapper.h"

class FakeGpioDrv : public GpioDrvWrapper {
public:
    FakeGpioDrv(const std::string& devName = "gpiochip0") {}
    ~FakeGpioDrv() override = default;
    MOCK_METHOD(void, Initialize, (), (override));
    MOCK_METHOD(int, SetPinMode, (unsigned int pinBcmOffset, PinMode mode), (override));
    MOCK_METHOD(int, SetPinValue, (unsigned int pinBcmOffset, bool isActive), (override));
    MOCK_METHOD(bool, GetPinValue, (unsigned int pinBcmOffset), (override));
    MOCK_METHOD(int, SetPinsMode, (const std::vector<unsigned int> &pinsBcmOffset, PinMode mode), (override));
    MOCK_METHOD(int, SetPinsValues, (const std::vector<unsigned int> &pinsBcmOffset, const std::vector<bool>& isActive), (override));
    MOCK_METHOD(std::vector<bool>, GetPinsValues, (const std::vector<unsigned int> &pinsBcmOffset), (override));
};