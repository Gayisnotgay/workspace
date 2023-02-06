#pragma once
#include <memory>
#include <QtWidgets/QWidget>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QButtonGroup>
#include "RaspiGpioPinBtnSelectGuiManager.h"

class GpioGetFunctionWidget : public QWidget {
    Q_OBJECT
public:
    GpioGetFunctionWidget(QWidget* parent = nullptr);
    ~GpioGetFunctionWidget() override = default;
    void Initialize(QAbstractButton *btnFunctionRun, QButtonGroup* gpioPinsBtnGroup);
public slots:
    void OnGpioDrvChanged(const QString &newDrv);

protected slots:
    virtual void OnBtnRunClicked(bool checked);
    virtual void OnPinConfigChanged();
protected:
    virtual std::unique_ptr<RaspiGpioPinBtnSelectGuiManager> CreatePinBtnGuiMng(QButtonGroup* gpioPinsBtnGroup);

    QString m_funName;
    QString m_selGpioDrv;
    std::unique_ptr<RaspiGpioPinBtnSelectGuiManager> m_pinsBtnMng;
    QAbstractButton *m_btnFunctionRun = nullptr;
};