#include "GpioSetFunctionWidget.h"
#include <algorithm>
#include <QtWidgets/QMessageBox>
#include <QtTest/QTest>
#include "RaspiGpioPinBtnConfigGuiManager.h"
#include "AutoRegFactory/AutoRegFactory.h"
#include "SmallTools/HelperDefine.h"

using std::for_each;
using std::unique_ptr;
using std::make_unique;
using std::to_string;
using std::string;
using std::vector;

GpioSetFunctionWidget::GpioSetFunctionWidget(QWidget* parent) : GpioGetFunctionWidget(parent)
{
    m_funName = tr("GpioSetFunction");
}

unique_ptr<RaspiGpioPinBtnSelectGuiManager> GpioSetFunctionWidget::CreatePinBtnGuiMng(QButtonGroup* gpioPinsBtnGroup)
{
    return unique_ptr<RaspiGpioPinBtnSelectGuiManager>(new RaspiGpioPinBtnConfigGuiManager(*gpioPinsBtnGroup, this));
}

void GpioSetFunctionWidget::OnBtnRunClicked(bool checked)
{
    qDebug() << tr("GpioSetFunctionWidget OnBtnRunClicked");

    // 组装查询的参数列表
    vector<string> args{m_funName.toStdString(), m_selGpioDrv.toStdString()};
    auto selPinVolLvl = qobject_cast<RaspiGpioPinBtnConfigGuiManager*>(m_pinsBtnMng.get())->GetSelectedPinVolLvl();
    for_each(selPinVolLvl.begin(), selPinVolLvl.end(), [&args](auto& pinVolPair) {
        args.emplace_back(pinVolPair.first);
        args.emplace_back(to_string(pinVolPair.second));
    });

    // 执行命令
    string output;
    int result = FuncFactory<int, const vector<string>&, string&, int*>::Instance().RunFunc(m_funName.toStdString(), args, output, nullptr);
    QString resultInfo(tr("GpioSetFunction run result[%1], args[%2], output[%3]").arg(result).arg(QString::fromStdString(GetSequenceContents(args.begin(), args.end()))).arg(QString::fromStdString(output)));
    qDebug() << resultInfo;
    if (result != EXIT_SUCCESS) {
        QMessageBox::critical(this, tr("Execute failed"), resultInfo, QMessageBox::Ok);
    }
}

void GpioSetFunctionWidget::OnPinConfigChanged()
{
    qDebug() << tr("GpioSetFunctionWidget OnPinConfigChanged");

    auto selPinVolLvl = qobject_cast<RaspiGpioPinBtnConfigGuiManager*>(m_pinsBtnMng.get())->GetSelectedPinVolLvl();
    m_btnFunctionRun->setEnabled(!selPinVolLvl.empty());
}