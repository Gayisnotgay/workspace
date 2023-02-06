#pragma once
#include <string>
#include <vector>
#include <memory>
#include "AutoRegFactory/AutoRegFactory.h"
#include "GpioDrvWrapper/GpioDrvWrapper.h"

class GpioFunction {
public:
    GpioFunction() = default;
    GpioFunction(size_t minParaLen) : m_minParaLen(minParaLen) {}
    virtual ~GpioFunction() = default;
    virtual int Run(int argc, char* argv[]);
    // flag:
    //      bit0: b1-exit Run process b0-not exit
    //      bit1: b1-enable the synchronization which will check bit2 to determin whether to wait for the consumer to fetch the content of [output]
    //      bit2: b1-[output] has new content coming in, GpioFunction will stop refreshing [output] until caller reset this bit to b0
    //            b0-Consumers should reset this bit to 0 after fetching the new content
    enum FunctionFlagBit {
        EXIT_RUN_PROCESS = 0,
        RUN_SYNCHRONIZE_ENABLE,
        RUN_NEWCONTENT_COMEIN_AND_WAIT_FOR_CONSUMER,
    };
    virtual int Run(const std::vector<std::string>& args, std::string& output, int* flag = nullptr);
protected:
    int CreateGpioDrv(const std::string& drvType);
    GpioDrvWrapper& GetGpioDrv();
    virtual void ShowFunctionUsage();

    std::string m_program = "GpioFunction";
    size_t m_minParaLen = 2;
private:
    std::unique_ptr<GpioDrvWrapper> m_gpioDrv = nullptr;
};

#ifndef GPIOFUNC_AUTO_REG
#define GPIOFUNC_AUTO_REG(GpioFunc) \
    FUNC_AUTO_REG(GpioFunc, #GpioFunc, [](int argc, char* argv[]){ return GpioFunc().Run(argc, argv); }, int, int, char**); \
    FUNC_AUTO_REG(Create##GpioFunc, #GpioFunc, [](){ return std::unique_ptr<GpioFunction>(new GpioFunc()); }, std::unique_ptr<GpioFunction>)
#endif

#ifndef ADVANCED_GPIOFUNC_AUTO_REG
#define ADVANCED_GPIOFUNC_AUTO_REG(GpioFunc) \
    FUNC_AUTO_REG(Advanced##GpioFunc, #GpioFunc, [](const std::vector<std::string>& args, std::string& output, int* flag = nullptr){ return GpioFunc().Run(args, output, flag); }, int, const std::vector<std::string>&, std::string&, int*)
#endif