#pragma once

#include "GpioDrvWrapper.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include "gpiod.hpp"

class GpioDrvWrapperGpiod : public GpioDrvWrapper {
public:
    GpioDrvWrapperGpiod(const std::string& devName = "gpiochip0");
    ~GpioDrvWrapperGpiod() override;
    void Initialize() override;
    int SetPinMode(unsigned int pinBcmOffset, PinMode mode) override;
    int SetPinValue(unsigned int pinBcmOffset, bool isActive) override;
    bool GetPinValue(unsigned int pinBcmOffset) override;

    int SetPinsMode(const std::vector<unsigned int> &pinsBcmOffset, PinMode mode) override;
    int SetPinsValues(const std::vector<unsigned int> &pinsBcmOffset, const std::vector<bool>& isActive) override;
    std::vector<bool> GetPinsValues(const std::vector<unsigned int> &pinsBcmOffset) override;

private:
    std::unique_ptr<gpiod::chip> m_chip;
    std::map<std::vector<unsigned int>, gpiod::line_bulk> m_linebulkCache;
    std::string m_devName;
    std::string m_requester;
    std::mutex m_lineBulkCacheMutex;
};