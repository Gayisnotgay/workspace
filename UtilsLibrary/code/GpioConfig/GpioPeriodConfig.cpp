#include "GpioPeriodConfig.h"
#include <memory>
#include <algorithm>
#include <thread>
#include <limits>
#include <time.h>
#include <bitset>
#include "MessageCenter/MessageCenter.h"
#include "SmallTools/TimeTool.h"

using std::vector;
using std::for_each;
using std::thread;
using std::bitset;

void GpioPeriodConfig::ConfigCandence(const vector<SingleGpioPeriodConfig>& gpioCandence)
{
    m_cadence = gpioCandence;
}

void GpioPeriodConfig::Run(int* ctrlFlag)
{
    if (m_cadence.empty()) {
        return;
    }

    vector<thread> gpioCtrlThreads;
    for (size_t index = 0; index < m_cadence.size(); ++index) {
        gpioCtrlThreads.emplace_back(thread([this, index](int* runCtrl){ DriveGpio(m_cadence[index], runCtrl); }, ctrlFlag));
    }

    std::for_each(gpioCtrlThreads.begin(), gpioCtrlThreads.end(), [](thread& thr){ thr.join(); });
    if ((ctrlFlag != nullptr) && bitset<32>(*ctrlFlag)[0]) {
        MessageCenter::Instance() << "GpioPeriodConfig::Run exit successfully" << std::endl;
        *ctrlFlag = 0x7fffffff;
    }

}

void GpioPeriodConfig::DriveGpio(const SingleGpioPeriodConfig& lineCfg, int* ctrlFlag)
{
    int result = m_gpioDrv.SetPinMode(lineCfg.offset, GpioDrvWrapper::PinMode::MODE_OUTPUT);
    if (result != EXIT_SUCCESS) {
        return;
    }

    auto gpioCurState = lineCfg.initValue;
    auto gpioCurDuration = lineCfg.switchTimeInMilliSecond.begin();
    while (gpioCurDuration != lineCfg.switchTimeInMilliSecond.end()) {
        m_gpioDrv.SetPinValue(lineCfg.offset, gpioCurState);

        if ((*gpioCurDuration) == std::numeric_limits<unsigned int>::max()) {
            break;
        }
        TimeTool::millisleep(*gpioCurDuration);

        ++gpioCurDuration;
        if (gpioCurDuration == lineCfg.switchTimeInMilliSecond.end()) {
            gpioCurDuration = lineCfg.switchTimeInMilliSecond.begin();
        }
        gpioCurState = !gpioCurState;

        if ((ctrlFlag != nullptr) && bitset<32>(*ctrlFlag)[0]) {
            break;
        }
    }
}