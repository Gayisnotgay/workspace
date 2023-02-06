#include "GpioToneConfig.h"
#include <iostream>
#include <bitset>
#include <memory>
#include <limits>
#include <unistd.h>
#include "GpioConfig.h"
#include "NumMscNotationParser/MscAlphabet.h"
#include "SmallTools/TimeTool.h"

using std::bitset;
using std::get;
using std::numeric_limits;

void GpioToneConfig::ToneRing(const SingleGpioToneConfig& toneCfg, ToneRingType ringType/* = ToneRingType::ONCE */, int* ctrlFlag/*  = nullptr */)
{
    int result = m_gpioDrv.SetPinMode(toneCfg.offset, GpioDrvWrapper::PinMode::MODE_OUTPUT);
    if (result != EXIT_SUCCESS) {
        return;
    }

    int loopMax = (ringType == ToneRingType::ONCE) ? 1 : numeric_limits<int>::max();
    for (int index = 0; index < loopMax; ++index) {
        for (size_t toneIdx = 0; toneIdx < toneCfg.tones.size(); ++toneIdx) {
            OutputPWM(toneCfg.offset, toneCfg.tones[toneIdx]);
            if (ctrlFlag == nullptr) {
                continue;
            }
            if (bitset<32>(*ctrlFlag)[0]) {
                break;
            }
            unsigned int progress = toneIdx * 100 / toneCfg.tones.size();
            *ctrlFlag = (progress << 24) + (static_cast<unsigned int>(*ctrlFlag) & 0xffffff);
        }
    }
}

void GpioToneConfig::OutputPWM(unsigned int pinOffset, const SingleGpioToneConfig::Pwm& wave)
{
    double periodInMilliSecond = (1 / wave.toneFreq) * 1000;
    if (periodInMilliSecond >= wave.toneDuration) {
        TimeTool::millisleep(wave.toneDuration);
        return;
    }
    
    bool curLevel = true;
    double totalTimeElapsed = 0;
    while (totalTimeElapsed < wave.toneDuration) {
        (void)m_gpioDrv.SetPinValue(pinOffset, curLevel);
        curLevel = !curLevel;
        TimeTool::millisleep(periodInMilliSecond / 2);
        totalTimeElapsed += (periodInMilliSecond / 2);
    }
}