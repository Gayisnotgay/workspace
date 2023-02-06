#include "GpioDrvWrapperGpiod.h"
#include <algorithm>
#include "MessageCenter/MessageCenter.h"

using std::for_each;
using std::vector;
using std::map;
using std::make_unique;
using std::endl;
using std::lock_guard;
using gpiod::chip;
using gpiod::line;
using gpiod::line_request;

GPIODRV_AUTO_REG(GpioDrvWrapperGpiod, "libgpiod");

GpioDrvWrapperGpiod::GpioDrvWrapperGpiod(const std::string& devName) : m_devName(devName), m_requester("GpioDrvWrapperGpiod")
{
    Initialize();
}

GpioDrvWrapperGpiod::~GpioDrvWrapperGpiod()
{
    for_each(m_linebulkCache.begin(), m_linebulkCache.end(), [](auto& linebulk) { linebulk.second.release(); });
    m_linebulkCache.clear();
}

void GpioDrvWrapperGpiod::Initialize()
{
    m_chip = make_unique<chip>(m_devName);
}

int GpioDrvWrapperGpiod::SetPinMode(unsigned int pinBcmOffset, PinMode mode)
{
    return SetPinsMode({pinBcmOffset}, mode);
}

int GpioDrvWrapperGpiod::SetPinValue(unsigned int pinBcmOffset, bool isActive)
{
   return SetPinsValues({pinBcmOffset}, {isActive});
}

bool GpioDrvWrapperGpiod::GetPinValue(unsigned int pinBcmOffset)
{
    auto activeList = GetPinsValues({pinBcmOffset});
    
    return activeList.empty() ? false : activeList[0];
}

// int GpioDrvWrapperGpiod::SetPinsMode(const std::vector<unsigned int> &pinsBcmOffset, PinMode mode)
// {
//     lock_guard guard(m_lineBulkCacheMutex);

//     auto linebulkIt = m_linebulkCache.find(pinsBcmOffset);
//     if (linebulkIt != m_linebulkCache.end()) {
//         linebulkIt->second.release();
//         m_linebulkCache.erase(linebulkIt);
//     }

//     const map<PinMode, int> modeMapping = {
//         {PinMode::MODE_INPUT, line_request::DIRECTION_INPUT},
//         {PinMode::MODE_OUTPUT, line_request::DIRECTION_OUTPUT},
//     };

//     if (modeMapping.find(mode) == modeMapping.end()) {
//         MessageCenter::Instance() << "GpioDrvWrapperGpiod::SetPinsMode failure, unknown PinMode:" << static_cast<int>(mode) << endl;
//         return EXIT_FAILURE;
//     }

//     auto newLinebulk = m_chip->get_lines(pinsBcmOffset);
//     newLinebulk.request({m_requester, modeMapping.at(mode), 0});
//     m_linebulkCache.emplace(pinsBcmOffset, std::move(newLinebulk));

//     return EXIT_SUCCESS;
// }

int GpioDrvWrapperGpiod::SetPinsMode(const std::vector<unsigned int> &pinsBcmOffset, PinMode mode)
{
    lock_guard guard(m_lineBulkCacheMutex);

    auto linebulkIt = m_linebulkCache.find(pinsBcmOffset);
    if (linebulkIt != m_linebulkCache.end()) {
        linebulkIt->second.release();
        m_linebulkCache.erase(linebulkIt);
    }

    const map<PinMode, int> modeMapping = {
        {PinMode::MODE_INPUT, line_request::DIRECTION_INPUT},
        {PinMode::MODE_OUTPUT, line_request::DIRECTION_OUTPUT},
    };

    if (modeMapping.find(mode) == modeMapping.end()) {
        MessageCenter::Instance() << "GpioDrvWrapperGpiod::SetPinsMode failure, unknown PinMode:" << static_cast<int>(mode) << endl;
        return EXIT_FAILURE;
    }

    auto newLinebulk = m_chip->get_lines(pinsBcmOffset);
    newLinebulk.request({m_requester, modeMapping.at(mode), 0});
    m_linebulkCache.emplace(pinsBcmOffset, std::move(newLinebulk));

    return EXIT_SUCCESS;
}

int GpioDrvWrapperGpiod::SetPinsValues(const std::vector<unsigned int> &pinsBcmOffset, const std::vector<bool>& isActive)
{
    lock_guard guard(m_lineBulkCacheMutex);

    auto linebulkIt = m_linebulkCache.find(pinsBcmOffset);
    if (linebulkIt == m_linebulkCache.end()) {
        MessageCenter::Instance() << "GpioDrvWrapperGpiod::SetPinValue failure, pinMode not configured, pinsBcmOffset " << pinsBcmOffset << endl;
        return EXIT_FAILURE;
    }

    vector<int> values;
    for_each(isActive.begin(), isActive.end(), [&values](bool active) { values.emplace_back(active ? 1 : 0); });
    linebulkIt->second.set_values(values);

    return EXIT_SUCCESS;
}

std::vector<bool> GpioDrvWrapperGpiod::GetPinsValues(const std::vector<unsigned int> &pinsBcmOffset)
{
    lock_guard guard(m_lineBulkCacheMutex);
    
    auto linebulkIt = m_linebulkCache.find(pinsBcmOffset);
    if (linebulkIt == m_linebulkCache.end()) {
        MessageCenter::Instance() << "GpioDrvWrapperGpiod::GetPinsValues failure, pinMode not configured, pinsBcmOffset " << pinsBcmOffset << endl;
        return {false};
    }

    auto values = linebulkIt->second.get_values();
    vector<bool> activeList;
    for_each(values.begin(), values.end(), [&activeList](int value) { activeList.emplace_back(value == 1); });

    return activeList;
}