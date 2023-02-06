#include "ExploreRaspiAppGuiWidget.h"
#include <algorithm>
#include <tuple>
#include <unistd.h>
#include <map>
#include <QtTest/QTest>
#include <QtWidgets/QMessageBox>
#include <QtGui/QIcon>
#include "ParseConfig/ParseConfig.h"
#include "SmallTools/HelperDefine.h"
#include "SmallTools/ShellCommand.h"

using std::for_each;
using std::tuple;
using std::get;
using std::map;
using std::unique_ptr;
using std::make_unique;
using std::string;
using std::vector;
using std::stoul;

ExploreRaspiAppGuiWidget::ExploreRaspiAppGuiWidget(QWidget* parent) : QWidget(parent)
{
    m_ui.setupUi(this);
    UiAdditionalConfig();
    InitGpioFunctions();
}

void ExploreRaspiAppGuiWidget::UiAdditionalConfig()
{    
    // 设置应用程序图标
    QString logoFile(":/RaspBerryPi400.png");
    m_raspi400Logo = make_unique<QPixmap>(logoFile);
    if ((!m_raspi400Logo) || !(*m_raspi400Logo)) {
        qDebug() << tr("can not open %1").arg(logoFile);
        return;
    }
    if (m_ui.raspiLogoLabel != nullptr) {
        // 将图片缩放至和label一样大小
        m_ui.raspiLogoLabel->setPixmap(m_raspi400Logo->scaled(m_ui.raspiLogoLabel->size(), Qt::KeepAspectRatio));
    }

    UpdateRaspiBasicInfo();
}

void ExploreRaspiAppGuiWidget::InitGpioFunctions()
{
    // 设置功能区背景
    m_ui.funcWidgets->setStyleSheet(tr("border-image:url(:/RaspberryBackgroud.png)"));

    connect(m_ui.gpioDrvBox, &QComboBox::currentTextChanged, m_ui.pageGpioGetFunction, &GpioGetFunctionWidget::OnGpioDrvChanged);
    connect(m_ui.gpioDrvBox, &QComboBox::currentTextChanged, m_ui.pageGpioMonitorFunction, &GpioGetFunctionWidget::OnGpioDrvChanged);
    connect(m_ui.gpioDrvBox, &QComboBox::currentTextChanged, m_ui.pageGpioSetFunction, &GpioGetFunctionWidget::OnGpioDrvChanged);
    connect(m_ui.gpioDrvBox, &QComboBox::currentTextChanged, m_ui.pageGpioPeriodSetFunction, &GpioGetFunctionWidget::OnGpioDrvChanged);
    connect(m_ui.gpioDrvBox, &QComboBox::currentTextChanged, m_ui.pageGpioSingFunction, &GpioGetFunctionWidget::OnGpioDrvChanged);

    m_ui.pageGpioGetFunction->Initialize(m_ui.btnGpioGetFunctionRun, m_ui.btnGrpGpioGetFunction);
    m_ui.pageGpioMonitorFunction->Initialize(m_ui.btnGpioMonitorFunctionRun, m_ui.btnGpioMonitorFunctionStop, m_ui.btnGrpGpioMonitorFunction);
    m_ui.pageGpioSetFunction->Initialize(m_ui.btnGpioSetFunctionRun, m_ui.btnGrpGpioSetFunction);
    m_ui.pageGpioPeriodSetFunction->Initialize(m_ui.btnGpioPeriodSetFunctionRun, m_ui.btnGpioPeriodSetFunctionStop, m_ui.btnGrpGpioPeriodSetFunction);
    m_ui.pageGpioSingFunction->Initialize(m_ui.btnGpioSingFunctionRun, m_ui.btnGpioSingFunctionStop, m_ui.btnGrpGpioSingFunction, {m_ui.songFileSelectBtn, m_ui.songFileNameEdit, m_ui.repeatPlayCheckBox, m_ui.songPlayProgressBar});
}

void ExploreRaspiAppGuiWidget::UpdateRaspiBasicInfo()
{
    auto basicInfo = ShellCommand::Exec("pinout");

    // <item, pattern, value>
    vector<tuple<string, string, string>> basicInfoCfg = {
        {"BoardName",           "(Raspberry Pi [\\w\\s]*) Rev", ""},  
        {"Revision",            "Revision \\s*:\\s*([\\w]*)", ""},  
        {"SoC",                 "SoC \\s*:\\s*([\\w]*)", ""},  
        {"RAM",                 "RAM \\s*:\\s*([\\w]*)", ""},
        {"Storage",             "Storage \\s*:\\s*([\\w]*)", ""},
        {"USB ports",           "USB ports \\s*:\\s*([\\w]*)", ""},
        {"Ethernet ports",      "Ethernet ports \\s*:\\s*([\\w]*)", ""},
        {"Wi-fi",               "Wi-fi \\s*:\\s*([\\w]*)", ""},
        {"Bluetooth",           "Bluetooth \\s*:\\s*([\\w]*)", ""},
        {"Camera ports (CSI)",  "Camera ports \\(CSI\\)\\s*:\\s*([\\w]*)", ""},
        {"Display ports (DSI)", "Display ports \\(DSI\\)\\s*:\\s*([\\w]*)", ""},
    };
    
    QString basicInfoShow;
    for_each(basicInfoCfg.begin(), basicInfoCfg.end(), [&basicInfo, &basicInfoShow](auto& elem) {
        auto value = ParseConfig::ParsePattern(basicInfo, get<1>(elem));
        if (!value.empty()) {
            QString elemInfo = tr("<b>%1</b>: <i>%2</i><br>").arg(QString::fromStdString(get<0>(elem))).arg(QString::fromStdString(value[0]));
            get<2>(elem) = value[0];
            basicInfoShow += elemInfo;
        }
    });

    m_ui.raspiBasicInfoBrowser->setText(basicInfoShow);
}

void ExploreRaspiAppGuiWidget::on_funBox_currentTextChanged(const QString &newFun)
{
    qDebug() << QObject::tr("function changed to ") << newFun;
    m_selFun = newFun;
    auto curFuncIndex = m_ui.funBox->currentIndex();
    if (curFuncIndex < m_ui.funcWidgets->count()) {
        m_ui.funcWidgets->setCurrentIndex(curFuncIndex);
        if (!m_isFuncInitList[curFuncIndex]) {
            m_ui.funcWidgets->currentWidget()->setVisible(false);
        }
    }
}

void ExploreRaspiAppGuiWidget::on_gpioDrvBox_currentTextChanged(const QString &newDrv)
{
    qDebug() << QObject::tr("gpioDrv changed to ") << newDrv;
    m_selGpioDrv = newDrv;
}

void ExploreRaspiAppGuiWidget::on_configApplyBtn_clicked(bool checked)
{
    // 设置功能区背景
    m_ui.funcWidgets->setStyleSheet(tr(""));
    m_ui.funcWidgets->currentWidget()->setVisible(true);
    m_isFuncInitList[m_ui.funcWidgets->currentIndex()] = true;
}

void ExploreRaspiAppGuiWidget::SetFunList(const std::string& newFunList)
{
    if (newFunList == m_funList) {
        return;
    }

    if (m_ui.funBox != nullptr) {
        auto funInfoSplitParts = ParseConfig::ParsePattern(newFunList, "^\\s*\\d+\\.(\\w+)");
        if (funInfoSplitParts.empty()) {
            m_ui.funBox->clear();
            return;
        }
        m_isFuncInitList = make_unique<bool[]>(funInfoSplitParts.size());
        memset(m_isFuncInitList.get(), 0, sizeof(bool) * funInfoSplitParts.size());
        for_each(funInfoSplitParts.begin(), funInfoSplitParts.end(), [this](auto& fun) { m_ui.funBox->addItem(QString::fromUtf8(fun.c_str())); });
    }

    m_funList = newFunList;

    emit OnFunListChanged(newFunList);
}

void ExploreRaspiAppGuiWidget::SetGpioDrvList(const std::string& newGpioDrvList)
{
    if (newGpioDrvList == m_gpioDrvList) {
        return;
    }

    if (m_ui.gpioDrvBox != nullptr) {
        auto gpioDrvSplitParts = ParseConfig::ParsePattern(newGpioDrvList, "^\\s*\\d+\\.(\\w+)");
        if (gpioDrvSplitParts.empty()) {
            m_ui.gpioDrvBox->clear();
            return;
        }
        for_each(gpioDrvSplitParts.begin(), gpioDrvSplitParts.end(), [this](auto& gpioDrv) { m_ui.gpioDrvBox->addItem(QString::fromUtf8(gpioDrv.c_str())); });
    }

    m_gpioDrvList = newGpioDrvList;
    emit OnGpioDrvListChanged(newGpioDrvList);
}