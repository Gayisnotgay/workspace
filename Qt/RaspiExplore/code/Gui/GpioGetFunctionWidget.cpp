#include "GpioGetFunctionWidget.h"
#include <vector>
#include <string>
#include <QtWidgets/QMessageBox>
#include <QtTest/QTest>
#include "ParseConfig/ParseConfig.h"
#include "AutoRegFactory/AutoRegFactory.h"
#include "SmallTools/HelperDefine.h"

using std::unique_ptr;
using std::make_unique;
using std::vector;
using std::string;

GpioGetFunctionWidget::GpioGetFunctionWidget(QWidget* parent) : QWidget(parent), m_funName("GpioGetFunction") {}

void GpioGetFunctionWidget::Initialize(QAbstractButton *btnFunctionRun, QButtonGroup* gpioPinsBtnGroup)
{
    if ((btnFunctionRun == nullptr) || (gpioPinsBtnGroup == nullptr)) {
        qDebug() << tr("GpioGetFunctionWidget Initialize failed, input nullptr");
        return;
    }

    if ((m_btnFunctionRun != nullptr) && m_pinsBtnMng) {
        qDebug() << tr("GpioGetFunctionWidget Initialize cannot be called repeatedly");
        return;
    }

    m_btnFunctionRun = btnFunctionRun;
    m_pinsBtnMng = CreatePinBtnGuiMng(gpioPinsBtnGroup);
    m_pinsBtnMng->Init();

    connect(btnFunctionRun, &QAbstractButton::clicked, this, &GpioGetFunctionWidget::OnBtnRunClicked);
    connect(m_pinsBtnMng.get(), &RaspiGpioPinBtnSelectGuiManager::PinConfigChanged, this, &GpioGetFunctionWidget::OnPinConfigChanged);

    // 初始禁用功能运行按钮
    m_btnFunctionRun->setEnabled(false);
}

unique_ptr<RaspiGpioPinBtnSelectGuiManager> GpioGetFunctionWidget::CreatePinBtnGuiMng(QButtonGroup* gpioPinsBtnGroup)
{
    return make_unique<RaspiGpioPinBtnSelectGuiManager>(*gpioPinsBtnGroup, this);
}

void GpioGetFunctionWidget::OnGpioDrvChanged(const QString &newDrv)
{
    qDebug() << QObject::tr("%1 OnGpioDrvChanged to ").arg(metaObject()->className()) << newDrv;
    m_selGpioDrv = newDrv;
}

void GpioGetFunctionWidget::OnBtnRunClicked(bool checked)
{
    qDebug() << tr("GpioGetFunctionWidget OnBtnRunClicked");

    // 组装查询的参数列表
    vector<string> args{m_funName.toStdString(), m_selGpioDrv.toStdString()};
    auto selPinBtnNos = m_pinsBtnMng->GetSelectedPinNumbers();
    args.insert(args.end(), selPinBtnNos.begin(), selPinBtnNos.end());

    // 执行命令
    string output;
    int result = FuncFactory<int, const vector<string>&, string&, int*>::Instance().RunFunc(m_funName.toStdString(), args, output, nullptr);
    QString resultInfo(tr("GpioGetFunction run result[%1], args[%2], output[%3]").arg(result).arg(QString::fromStdString(GetSequenceContents(args.begin(), args.end()))).arg(QString::fromStdString(output)));
    qDebug() << resultInfo;
    if (result != EXIT_SUCCESS) {
        QMessageBox::critical(this, tr("Execute failed"), resultInfo, QMessageBox::Ok);
        return;
    }

    // 解析结果
    auto values = ParseConfig::SplitCfgStr(output);
    result = m_pinsBtnMng->SetPinsVolLevel(selPinBtnNos, values);
    if (result != EXIT_SUCCESS) {
        return;
    }
}

void GpioGetFunctionWidget::OnPinConfigChanged()
{
    // 维护btnGpioGetFunctionRun按钮状态，没有选中pin不让运行
    m_btnFunctionRun->setEnabled(!m_pinsBtnMng->GetSelectedPinNumbers().empty());
}