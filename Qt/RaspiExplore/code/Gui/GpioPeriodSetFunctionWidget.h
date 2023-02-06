#pragma once
#include <thread>
#include "GpioSetFunctionWidget.h"

class GpioPeriodSetFunctionWidget : public GpioSetFunctionWidget {
    Q_OBJECT
public:
    GpioPeriodSetFunctionWidget(QWidget* parent = nullptr);
    ~GpioPeriodSetFunctionWidget() override;
    void Initialize(QAbstractButton *btnFunctionRun, QAbstractButton *btnFunctionStop, QButtonGroup* gpioPinsBtnGroup);

protected:
    std::unique_ptr<RaspiGpioPinBtnSelectGuiManager> CreatePinBtnGuiMng(QButtonGroup* gpioPinsBtnGroup) override;
    QAbstractButton *m_btnFunctionStop = nullptr;

protected slots:
    void OnPinConfigChanged() override;
    void OnBtnRunClicked(bool checked) override;
    void OnBtnStopClicked(bool checked);

private:
    void PeriodSetProcess();

    std::thread m_periodSetProcess;
    int m_ctrlFlag = 0;
};