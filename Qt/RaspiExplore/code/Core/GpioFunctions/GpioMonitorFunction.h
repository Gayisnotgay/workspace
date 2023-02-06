#pragma once
#include <bitset>
#include "GpioGetFunction.h"

class GpioMonitorFunction : public GpioGetFunction {
public:
    GpioMonitorFunction() = default;
    ~GpioMonitorFunction() override = default;
    int Run(int argc, char* argv[]) override;
    // flag:
    //      bit0: b1-exit monitor process b0-not exit
    //      bit1: b1-enable the synchronization which will check bit2 to determin whether to wait for the consumer to fetch the content of [output]
    //      bit2: b1-[output] has new content coming in, GpioMonitorFunction will stop refreshing [output] until caller reset this bit to b0
    //            b0-Consumers should reset this bit to 0 after fetching the new content
    enum MonitorFlagBit {
        EXIT_MONITOR_PROCESS = 0,
        MONITOR_SYNCHRONIZE_ENABLE,
        MONITOR_NEWCONTENT_COMEIN_AND_WAIT_FOR_CONSUMER,
    };
    int Run(const std::vector<std::string>& args, std::string& output, int* flag) override;
private:
    void MonitorProcess(GpioAccessParam param, std::string& output, int* flag);
};