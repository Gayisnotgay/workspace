#pragma once
#include "GpioFunction.h"
#include "GpioConfig/GpioConfig.h"

class GpioGetFunction : public GpioFunction {
public:
    GpioGetFunction() = default;
    ~GpioGetFunction() override = default;
    int Run(int argc, char* argv[]) override;
    int Run(const std::vector<std::string>& args, std::string& output, int* flag) override;
    virtual int ParseParam(const std::vector<std::string>& args, GpioAccessParam& param);

protected:
    void ShowFunctionUsage() override;
};