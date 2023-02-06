#pragma once
#include "GpioFunction.h"
#include "GpioConfig/GpioPeriodConfig.h"

class GpioPeriodSetFunction : public GpioFunction {
public:
    GpioPeriodSetFunction() = default;
    ~GpioPeriodSetFunction() override = default;
    int Run(int argc, char* argv[]) override;
    int Run(const std::vector<std::string>& args, std::string& output, int* flag = nullptr) override;
    virtual int ParseParam(const std::vector<std::string>& args, std::vector<SingleGpioPeriodConfig>& param);
private:
    void ShowFunctionUsage() override;
    std::vector<std::string> SplitParam(int argc, char* argv[]);
};