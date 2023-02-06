#include "GpioConfig.h"
#include "MessageCenter/MessageCenter.h"

using std::get;

void GpioConfig::Set(const GpioAccessParam& cfg)
{
    int result = m_gpioDrv.SetPinsMode(get<0>(cfg), GpioDrvWrapper::PinMode::MODE_OUTPUT);
    if (result != EXIT_SUCCESS) {
        return;
    }

    (void)m_gpioDrv.SetPinsValues(get<0>(cfg), get<1>(cfg));
}

int GpioConfig::Get(GpioAccessParam& cfg)
{
    int result = m_gpioDrv.SetPinsMode(get<0>(cfg), GpioDrvWrapper::PinMode::MODE_INPUT);
    if (result != EXIT_SUCCESS) {
        return result;
    }

    get<1>(cfg) = m_gpioDrv.GetPinsValues(get<0>(cfg));

    return EXIT_SUCCESS; 
}