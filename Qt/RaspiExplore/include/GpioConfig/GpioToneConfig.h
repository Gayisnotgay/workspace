#pragma once

#include "GpioConfig.h"
#include "NumMscNotationParser/MscAlphabet.h"

struct SingleGpioToneConfig {
    unsigned int offset;
    using Pwm = MscAlphabet::AlphabetProperty;
    std::vector<Pwm> tones;
};

class GpioToneConfig: public GpioConfig {
public:
    GpioToneConfig(GpioDrvWrapper& gpioDrv) : GpioConfig(gpioDrv) {}
    ~GpioToneConfig() override = default;
    enum class ToneRingType {
        REPEAT,
        ONCE,
    };
    void ToneRing(const SingleGpioToneConfig& toneCfg, ToneRingType ringType = ToneRingType::ONCE, int* ctrlFlag = nullptr);
protected:
    void OutputPWM(unsigned int pinOffset, const SingleGpioToneConfig::Pwm& wave);
private:
    SingleGpioToneConfig m_toneCfg;
};