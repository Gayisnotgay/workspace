#include "GpioDrvWrapperWiringPi.h"
#include <algorithm>
#include <map>
#include "wiringPi.h"
#include "MessageCenter/MessageCenter.h"
#include "TerminalCtrl/TerminalCtrl.h"

using std::for_each;
using std::vector;
using std::map;
using std::endl;

GPIODRV_AUTO_REG(GpioDrvWrapperWiringPi, "wiringPi");

GpioDrvWrapperWiringPi::GpioDrvWrapperWiringPi(const std::string& devName)
{
    Initialize();
}

void GpioDrvWrapperWiringPi::Initialize()
{
    int err = wiringPiSetupGpio();
    if (err != EXIT_SUCCESS) {
        MessageCenter::Instance() << TerminalCtrl::FontColor(TerminalCtrl::Color::RED)  << "wiringPiSetupGpio failed, errno: " << err << endl;
    }
}

int GpioDrvWrapperWiringPi::SetPinMode(unsigned int pinBcmOffset, PinMode mode)
{
    const map<PinMode, int> modeMapping = {
        {PinMode::MODE_INPUT, INPUT},
        {PinMode::MODE_OUTPUT, OUTPUT},
    };

    if (modeMapping.find(mode) == modeMapping.end()) {
        MessageCenter::Instance() << "GpioDrvWrapperWiringPi::SetPinsMode failure, unknown PinMode:" << static_cast<int>(mode) << endl;
        return EXIT_FAILURE;
    }

    pinMode(pinBcmOffset, modeMapping.at(mode));

    return EXIT_SUCCESS;
}

int GpioDrvWrapperWiringPi::SetPinValue(unsigned int pinBcmOffset, bool isActive)
{
    digitalWrite(pinBcmOffset, static_cast<int>(isActive));

    return EXIT_SUCCESS;
}

bool GpioDrvWrapperWiringPi::GetPinValue(unsigned int pinBcmOffset)
{
    return (digitalRead(pinBcmOffset) != 0);
}

int GpioDrvWrapperWiringPi::SetPinsMode(const vector<unsigned int> &pinsBcmOffset, PinMode mode)
{
    int result = EXIT_SUCCESS;
    for_each(pinsBcmOffset.begin(), pinsBcmOffset.end(), [this, &result, mode](unsigned int offset){ result += SetPinMode(offset, mode); });
    return result;
}

int GpioDrvWrapperWiringPi::SetPinsValues(const vector<unsigned int> &pinsBcmOffset, const vector<bool>& isActive)
{
    if (pinsBcmOffset.empty() || (pinsBcmOffset.size() != isActive.size())) {
        MessageCenter::Instance() << "GpioDrvWrapperWiringPi::SetPinsValues failure, pin and value size not match:" << pinsBcmOffset.size() << " Vs " << isActive.size() << endl;
        return EXIT_FAILURE;
    }

    int result = EXIT_SUCCESS;
    for (size_t index = 0; index < pinsBcmOffset.size(); ++index) {
        result += SetPinValue(pinsBcmOffset[index], isActive[index]);
    }

    return result;
}

vector<bool> GpioDrvWrapperWiringPi::GetPinsValues(const vector<unsigned int> &pinsBcmOffset)
{
    vector<bool> pinValues(pinsBcmOffset.size(), false);
    if (pinsBcmOffset.empty()) {
        MessageCenter::Instance() << "GpioDrvWrapperWiringPi::GetPinsValues failure, pin size is empty" << endl;
        return pinValues;
    }

    for (size_t index = 0; index < pinsBcmOffset.size(); ++index) {
        pinValues[index] = GetPinValue(pinsBcmOffset[index]);
    }

    return pinValues;
}
