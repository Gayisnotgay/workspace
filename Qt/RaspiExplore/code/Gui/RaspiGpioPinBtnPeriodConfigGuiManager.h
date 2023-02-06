#pragma once

#include <tuple>
#include "RaspiGpioPinBtnConfigGuiManager.h"
#include "PinVolLvlPeriodConfigWidget.h"

class RaspiGpioPinBtnPeriodConfigGuiManager : public RaspiGpioPinBtnConfigGuiManager {
    Q_OBJECT
public:
    RaspiGpioPinBtnPeriodConfigGuiManager(QButtonGroup& pinBtnsGroup, QObject* parent = nullptr);
    std::map<std::string, std::tuple<int, int, int>> GetSelectedPinPeriodConfig();

protected:
    void BuildVolLvlSelector() override;
protected slots:
    void OnPinBtnClicked(QAbstractButton *button, bool checked) override;
    void OnPinPeriodConfigChanged(QAbstractButton* pin, bool isHigh, int duration1, int duration2);
private:
    // Pin Number -> Period Pin VolLvl Config[initVolLvl, initVolLvlDuration, initOppositeVolLvlDuration]
    std::map<std::string, std::tuple<int, int, int>> m_pinPeriodConfigMap;
};