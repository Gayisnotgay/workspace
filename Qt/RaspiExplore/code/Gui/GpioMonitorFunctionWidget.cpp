#include "GpioMonitorFunctionWidget.h"
#include <bitset>
#include <vector>
#include <string>
#include <unistd.h>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QLayout>
#include <QtGui/QResizeEvent>
#include <QtTest/QTest>
#include "ParseConfig/ParseConfig.h"
#include "AutoRegFactory/AutoRegFactory.h"
#include "SmallTools/HelperDefine.h"
#include "SmallTools/TimeTool.h"

using std::unique_ptr;
using std::make_unique;
using std::vector;
using std::string;
using std::bitset;
using std::thread;

GpioMonitorFunctionWidget::GpioMonitorFunctionWidget(QWidget* parent) : GpioGetFunctionWidget(parent) 
{
    m_funName = tr("GpioMonitorFunction");
}

GpioMonitorFunctionWidget::~GpioMonitorFunctionWidget()
{
    if (m_thdMonitor.joinable()) {
        m_ctrlFlag = 1;
        m_thdMonitor.join();
    }
}

void GpioMonitorFunctionWidget::Initialize(QAbstractButton *btnFunctionRun, QAbstractButton *btnFunctionStop, QButtonGroup* gpioPinsBtnGroup)
{
    GpioGetFunctionWidget::Initialize(btnFunctionRun, gpioPinsBtnGroup);

    if (btnFunctionStop == nullptr) {
        qDebug() << tr("GpioMonitorFunctionWidget Initialize failed, btnFunctionStop is nullptr");
        return;
    }

    if (m_btnFunctionStop != nullptr) {
        qDebug() << tr("GpioMonitorFunctionWidget Initialize cannot be called repeatedly");
        return;
    }

    m_btnFunctionStop = btnFunctionStop;

    connect(btnFunctionStop, &QAbstractButton::clicked, this, &GpioMonitorFunctionWidget::OnBtnStopClicked);

    // 初始禁用功能运行按钮
    m_btnFunctionStop->setEnabled(false);

    InitBackgroundLabel();
}

int GpioMonitorFunctionWidget::InitBackgroundLabel()
{
    m_background = make_unique<QLabel>(this);
    m_background->lower();
    return EXIT_SUCCESS;
}

void GpioMonitorFunctionWidget::ResizeBackgroundLabel()
{
    // 设置label的backgroud大小为pin按钮区域大小
    auto layoutMng = layout();
    qDebug() << tr("layoutMng has %1 items").arg(layoutMng->count());
    auto pinBtnLayout = layoutMng->itemAt(0);
    m_background->setGeometry(pinBtnLayout->geometry());

    // qDebug() << tr("Widget geometry:") << qobject_cast<QWidget*>(parent())->geometry();
    qDebug() << tr("GpioMonitorFunctionWidget geometry:") << geometry();
    qDebug() << tr("GpioMonitorFunctionWidget layout geometry:") << layoutMng->geometry();
    qDebug() << tr("GpioMonitorFunctionWidget pinBtns layout geometry:") << pinBtnLayout->geometry();
    qDebug() << tr("m_background geometry:") << m_background->geometry();
}

void GpioMonitorFunctionWidget::resizeEvent(QResizeEvent *event)
{
    qDebug() << tr("GpioMonitorFunctionWidget::resizeEvent, changed from ") << event->oldSize() << tr(" to ") << event->size();
    ResizeBackgroundLabel();
}

void GpioMonitorFunctionWidget::OnBtnRunClicked(bool checked)
{
    m_btnFunctionRun->setEnabled(false);
    m_background->raise();

    bitset<32> ctrlBitFlag(0);
    ctrlBitFlag[1] = true;
    m_ctrlFlag = static_cast<int>(ctrlBitFlag.to_ulong());
    m_thdMonitor = std::thread([this](const vector<string>& pins){ ProcessPinBtnStatus(pins, m_ctrlFlag); }, m_pinsBtnMng->GetSelectedPinNumbers());

    m_btnFunctionStop->setEnabled(true);
}

void GpioMonitorFunctionWidget::OnBtnStopClicked(bool checked)
{
    m_btnFunctionStop->setEnabled(false);

    bitset<32> ctrlBitFlag(static_cast<unsigned long>(m_ctrlFlag));
    ctrlBitFlag[0] = true;
    m_ctrlFlag = static_cast<int>(ctrlBitFlag.to_ulong());
    m_thdMonitor.join();

    m_background->lower();
    m_btnFunctionRun->setEnabled(true);
}

unique_ptr<GpioFunction> GpioMonitorFunctionWidget::SendFunctionRequest(const vector<string>& selPins, int& ctrlFlag, string& output)
{
    // 组装查询的参数列表
    vector<string> args{m_funName.toStdString(), m_selGpioDrv.toStdString()};
    args.insert(args.end(), selPins.begin(), selPins.end());

    // 执行命令
    auto monitorFunction = FuncFactory<unique_ptr<GpioFunction>>::Instance().RunFunc(m_funName.toStdString());
    int result = monitorFunction->Run(args, output, &ctrlFlag);
    QString resultInfo(tr("[%1] run result[%2], args[%3], output[%4]").arg(m_funName).arg(result).arg(QString::fromStdString(GetSequenceContents(args.begin(), args.end()))).arg(QString::fromStdString(output)));
    qDebug() << resultInfo;

    return monitorFunction;
}

void GpioMonitorFunctionWidget::UpdateGpioPinsVolLevel(const vector<string>& selPins, int& ctrlFlag, string& output)
{
    // 解析结果
    bitset<32> monitorFlag = static_cast<unsigned long>(ctrlFlag);
    while (!monitorFlag[0]) {
        monitorFlag = static_cast<unsigned long>(ctrlFlag);
        if (!monitorFlag[2]) {
            TimeTool::millisleep(100);
            continue;
        }
        auto values = ParseConfig::SplitCfgStr(output);
        int result = m_pinsBtnMng->SetPinsVolLevel(selPins, values);
        if (result != EXIT_SUCCESS) {
            QMessageBox::warning(nullptr, tr("Internal Warning"), tr("SetPinsVolLevel failed, pins number[%1], values[%2]").arg(QString::fromStdString(GetSequenceContents(selPins.begin(), selPins.end()))).arg(QString::fromStdString(GetSequenceContents(values.begin(), values.end()))), QMessageBox::Ok);
            break;
        }

        monitorFlag[2] = false;
        ctrlFlag = static_cast<int>(monitorFlag.to_ulong());
    }
}
void GpioMonitorFunctionWidget::ProcessPinBtnStatus(const vector<string>& selPins, int& ctrlFlag)
{
    string output;
    auto monitorFunction = SendFunctionRequest(selPins, ctrlFlag, output);
    UpdateGpioPinsVolLevel(selPins, ctrlFlag, output);
    // 等待MonitorProcess退出后，自身再退出
    while (ctrlFlag < 0x7ffffffd) {
        TimeTool::millisleep(100);
    }
    qDebug() << tr("ProcessPinBtnStatus exit");
}