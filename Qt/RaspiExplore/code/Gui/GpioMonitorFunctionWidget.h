#pragma once
#include <thread>
#include <vector>
#include <string>
#include <QtWidgets/QLabel>
#include "GpioFunctions/GpioFunction.h"
#include "GpioGetFunctionWidget.h"

class GpioMonitorFunctionWidget : public GpioGetFunctionWidget {
    Q_OBJECT
public:
    GpioMonitorFunctionWidget(QWidget* parent = nullptr);
    ~GpioMonitorFunctionWidget() override;
    void Initialize(QAbstractButton *btnFunctionRun, QAbstractButton *btnFunctionStop, QButtonGroup* gpioPinsBtnGroup);

protected slots:
    void OnBtnRunClicked(bool checked) override;
    void OnBtnStopClicked(bool checked);

protected:
    void resizeEvent(QResizeEvent *event) override;
    QAbstractButton *m_btnFunctionStop = nullptr;

private:
    void ProcessPinBtnStatus(const std::vector<std::string>& selPins, int& exitFlag);
    std::unique_ptr<GpioFunction> SendFunctionRequest(const std::vector<std::string>& selPins, int& ctrlFlag, std::string& output);
    void UpdateGpioPinsVolLevel(const std::vector<std::string>& selPins, int& ctrlFlag, std::string& output);
    int InitBackgroundLabel();
    void ResizeBackgroundLabel();

    int m_ctrlFlag = 0;
    std::thread m_thdMonitor;
    std::unique_ptr<QLabel> m_background = nullptr;
};