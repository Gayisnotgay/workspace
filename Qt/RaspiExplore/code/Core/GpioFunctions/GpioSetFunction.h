#pragma once
#include "GpioFunction.h"
#include "GpioConfig/GpioConfig.h"

class GpioSetFunction : public GpioFunction {
public:
    GpioSetFunction() = default;
    ~GpioSetFunction() override = default;
    int Run(const std::vector<std::string>& args, std::string& output, int* flag) override;
    int Run(int argc, char* argv[]) override;
    virtual int ParseParam(const std::vector<std::string>& args, GpioAccessParam& param);

private:
    void ShowFunctionUsage() override;
    std::vector<std::string> SplitParam(int argc, char* argv[]);

};