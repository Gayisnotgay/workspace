#pragma once
#include "GpioGetFunctionWidget.h"

class GpioSetFunctionWidget : public GpioGetFunctionWidget {
    Q_OBJECT
public:
    GpioSetFunctionWidget(QWidget* parent = nullptr);
    ~GpioSetFunctionWidget() override = default;
protected:
    std::unique_ptr<RaspiGpioPinBtnSelectGuiManager> CreatePinBtnGuiMng(QButtonGroup* gpioPinsBtnGroup) override;
protected slots:
    void OnBtnRunClicked(bool checked) override;
    void OnPinConfigChanged() override;

};