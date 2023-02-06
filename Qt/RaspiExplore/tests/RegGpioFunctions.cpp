#include "AutoRegFactory/AutoRegFactory.h"
#include "MessageCenter/MessageCenter.h"
#include "GpioFunctions/GpioGetFunction.h"
#include "GpioFunctions/GpioMonitorFunction.h"
#include "GpioFunctions/GpioSetFunction.h"
#include "GpioFunctions/GpioPeriodSetFunction.h"
#include "GpioFunctions/GpioSingFunction.h"

GPIOFUNC_AUTO_REG(GpioGetFunction);
GPIOFUNC_AUTO_REG(GpioMonitorFunction);
GPIOFUNC_AUTO_REG(GpioSetFunction);
GPIOFUNC_AUTO_REG(GpioPeriodSetFunction);
GPIOFUNC_AUTO_REG(GpioSingFunction);

ADVANCED_GPIOFUNC_AUTO_REG(GpioGetFunction);
ADVANCED_GPIOFUNC_AUTO_REG(GpioMonitorFunction);
ADVANCED_GPIOFUNC_AUTO_REG(GpioSetFunction);
ADVANCED_GPIOFUNC_AUTO_REG(GpioPeriodSetFunction);
