#pragma once

#include "RaspiGpioPinBtnSelectGuiManager.h"
#include "PinVolLvlConfigWidget.h"

class RaspiGpioPinBtnConfigGuiManager : public RaspiGpioPinBtnSelectGuiManager {
    Q_OBJECT
public:
    RaspiGpioPinBtnConfigGuiManager(QButtonGroup& pinBtnsGroup, QObject* parent = nullptr);
    std::map<std::string, int> GetSelectedPinVolLvl();
    void Init() override;

protected:
    virtual void BuildVolLvlSelector();
    bool eventFilter(QObject *obj, QEvent *event) override;
    std::unique_ptr<PinVolLvlConfigWidget> m_volSelector;
protected slots:
    void OnPinBtnClicked(QAbstractButton *button, bool checked) override;
    void OnPinVolLvlSelected(QAbstractButton* btn, bool lvlHigh);
private:
    void InstallMouseHoverEvent();
    void SetPinsCheckable(bool checkable);

    // Pin Number -> Pin Level
    std::map<std::string, int> m_pinLvlMap;
};