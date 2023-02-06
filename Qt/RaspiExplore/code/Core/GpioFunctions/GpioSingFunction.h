#pragma once
#include <vector>
#include <string>
#include "GpioFunction.h"
#include "GpioConfig/GpioToneConfig.h"

class GpioSingFunction : public GpioFunction {
public:
    GpioSingFunction() = default;
    ~GpioSingFunction() override = default;
    int Run(int argc, char* argv[]) override;
    int Run(const std::vector<std::string>& args, std::string& output, int* flag) override;
    virtual int ParseParam(const std::vector<std::string>& args, SingleGpioToneConfig& param);
private:
    void ShowFunctionUsage() override;
    int ExtractSongData(const std::string& songFilename, std::vector<SingleGpioToneConfig::Pwm>& songData);
    int ExtractNumberedMusicNotation(const std::string& songName, std::vector<std::string>& mscNotation);
    int ParseNumberedMusicNotation(const std::vector<std::string>& mscNotation, std::vector<SingleGpioToneConfig::Pwm>& songData);
    void GpioSing(const SingleGpioToneConfig& toneCfg, int* ctrlFlag);
};