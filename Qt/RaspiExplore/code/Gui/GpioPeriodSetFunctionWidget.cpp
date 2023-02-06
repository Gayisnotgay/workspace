#include "GpioPeriodSetFunctionWidget.h"
#include <algorithm>
#include <bitset>
#include <QtWidgets/QMessageBox>
#include <QtTest/QTest>
#include "RaspiGpioPinBtnPeriodConfigGuiManager.h"
#include "AutoRegFactory/AutoRegFactory.h"
#include "SmallTools/HelperDefine.h"

using std::for_each;
using std::bitset;
using std::unique_ptr;
using std::get;
using std::make_unique;
using std::to_string;
using std::string;
using std::vector;

GpioPeriodSetFunctionWidget::GpioPeriodSetFunctionWidget(QWidget* parent) : GpioSetFunctionWidget(parent)
{
    m_funName = tr("GpioPeriodSetFunction");
}

GpioPeriodSetFunctionWidget::~GpioPeriodSetFunctionWidget()
{
    if (m_periodSetProcess.joinable()) {
        m_ctrlFlag = 1;
        m_periodSetProcess.join();
    }
}

void GpioPeriodSetFunctionWidget::Initialize(QAbstractButton *btnFunctionRun, QAbstractButton *btnFunctionStop, QButtonGroup* gpioPinsBtnGroup)
{
    GpioSetFunctionWidget::Initialize(btnFunctionRun, gpioPinsBtnGroup);

    if (btnFunctionStop == nullptr) {
        qDebug() << tr("GpioPeriodSetFunctionWidget Initialize failed, btnFunctionStop is nullptr");
        return;
    }

    if (m_btnFunctionStop != nullptr) {
        qDebug() << tr("GpioPeriodSetFunctionWidget Initialize cannot be called repeatedly");
        return;
    }

    m_btnFunctionStop = btnFunctionStop;

    connect(btnFunctionStop, &QAbstractButton::clicked, this, &GpioPeriodSetFunctionWidget::OnBtnStopClicked);

    // 初始禁用功能运行按钮
    m_btnFunctionStop->setEnabled(false);
}

unique_ptr<RaspiGpioPinBtnSelectGuiManager> GpioPeriodSetFunctionWidget::CreatePinBtnGuiMng(QButtonGroup* gpioPinsBtnGroup)
{
    return unique_ptr<RaspiGpioPinBtnSelectGuiManager>(new RaspiGpioPinBtnPeriodConfigGuiManager(*gpioPinsBtnGroup, this));
}

void GpioPeriodSetFunctionWidget::OnBtnRunClicked(bool checked)
{
    m_btnFunctionRun->setEnabled(false);
    m_pinsBtnMng->SetPinsBtnEnable(false);

    m_ctrlFlag = 0;
    m_periodSetProcess = std::thread([this](){ return PeriodSetProcess(); });
    m_btnFunctionStop->setEnabled(true);
}

void GpioPeriodSetFunctionWidget::OnBtnStopClicked(bool checked)
{
    m_btnFunctionStop->setEnabled(false);

    bitset<32> ctrlBitFlag(0);
    ctrlBitFlag[0] = true;
    m_ctrlFlag = static_cast<int>(ctrlBitFlag.to_ulong());
    m_periodSetProcess.join();

    m_btnFunctionRun->setEnabled(true);
    m_pinsBtnMng->SetPinsBtnEnable(true);
}

void GpioPeriodSetFunctionWidget::PeriodSetProcess()
{
    // 组装查询的参数列表
    vector<string> args{m_funName.toStdString(), m_selGpioDrv.toStdString()};
    auto selPinPeriodConfig = qobject_cast<RaspiGpioPinBtnPeriodConfigGuiManager*>(m_pinsBtnMng.get())->GetSelectedPinPeriodConfig();
    for_each(selPinPeriodConfig.begin(), selPinPeriodConfig.end(), [&args](auto& pinConfig) {
        if ((get<1>(pinConfig.second) <= 0) || (get<1>(pinConfig.second) >= 1000) ||
            (get<2>(pinConfig.second) <= 0) || (get<2>(pinConfig.second) >= 1000)) {
            qDebug() << tr("PeriodConfig[pin:%1, initVol:%2, duration1:%3, duration2:%4] is ignored due to incomplete data").arg(QString::fromStdString(pinConfig.first)).arg(get<0>(pinConfig.second)).arg(get<1>(pinConfig.second)).arg(get<2>(pinConfig.second));
            return;
        }
        args.emplace_back(pinConfig.first);
        args.emplace_back(to_string(get<0>(pinConfig.second)));
        args.emplace_back(to_string(get<1>(pinConfig.second)));
        args.emplace_back(to_string(get<2>(pinConfig.second)));
    });

    // 执行命令
    string output;
    int result = FuncFactory<int, const vector<string>&, string&, int*>::Instance().RunFunc(m_funName.toStdString(), args, output, &m_ctrlFlag);
    QString resultInfo(tr("GpioPeriodSetFunction run result[%1], args[%2], output[%3]").arg(result).arg(QString::fromStdString(GetSequenceContents(args.begin(), args.end()))).arg(QString::fromStdString(output)));
    qDebug() << resultInfo;
    if (result != EXIT_SUCCESS) {
        QMessageBox::critical(this, tr("Execute failed"), resultInfo, QMessageBox::Ok);
    }

    qDebug() << tr("PeriodSetProcess exit");
}

void GpioPeriodSetFunctionWidget::OnPinConfigChanged()
{
    auto selPinConfig = qobject_cast<RaspiGpioPinBtnPeriodConfigGuiManager*>(m_pinsBtnMng.get())->GetSelectedPinPeriodConfig();
    bool hasValidConfig = false;
    for (auto& cfg : selPinConfig) {
        if ((get<1>(cfg.second) > 0) && (get<1>(cfg.second) < 1000) &&
            (get<2>(cfg.second) > 0) && (get<2>(cfg.second) < 1000)) {
            hasValidConfig = true;
            break;
        } else {
            qDebug() << tr("PeriodConfig[pin:%1, initVol:%2, duration1:%3, duration2:%4] is incomplete").arg(QString::fromStdString(cfg.first)).arg(get<0>(cfg.second)).arg(get<1>(cfg.second)).arg(get<2>(cfg.second));
        }
    }

    m_btnFunctionRun->setEnabled(hasValidConfig);
}