#include "RaspiGpioPinBtnPeriodConfigGuiManager.h"
#include <algorithm>
#include <QtTest/QTest>

using std::for_each;
using std::unique_ptr;
using std::make_unique;
using std::map;
using std::string;
using std::tuple;
using std::get;

RaspiGpioPinBtnPeriodConfigGuiManager::RaspiGpioPinBtnPeriodConfigGuiManager(QButtonGroup& pinBtnsGroup, QObject* parent) : RaspiGpioPinBtnConfigGuiManager(pinBtnsGroup, parent){}

void RaspiGpioPinBtnPeriodConfigGuiManager::BuildVolLvlSelector()
{
    auto btns = m_pinBtnsGroup.buttons();
    if (btns.empty()) {
        qDebug() << tr("RaspiGpioPinBtnConfigGuiManager::BuildVolLvlSelector failed due to empty pin list");
        return;
    }

    auto pinMng = new PinVolLvlPeriodConfigWidget(m_pinVolLvlIconList, qobject_cast<QWidget*>(btns[0]->parent()));
    pinMng->InitWidget();
    connect(pinMng, &PinVolLvlPeriodConfigWidget::PinVolLvlSelected, this, &RaspiGpioPinBtnPeriodConfigGuiManager::OnPinVolLvlSelected);
    connect(pinMng, &PinVolLvlPeriodConfigWidget::PinVolLvlPeriodConfigChanged, this, &RaspiGpioPinBtnPeriodConfigGuiManager::OnPinPeriodConfigChanged);
    m_volSelector.reset(pinMng);
}

map<string, tuple<int, int, int>> RaspiGpioPinBtnPeriodConfigGuiManager::GetSelectedPinPeriodConfig()
{
    return m_pinPeriodConfigMap;
}

void RaspiGpioPinBtnPeriodConfigGuiManager::OnPinBtnClicked(QAbstractButton *btn, bool checked)
{
    if ((btn == nullptr) || checked) {
        return;
    }

    btn->setIcon(QIcon());

    int btnIndex = m_pinBtnsGroup.id(btn);
    auto pinNoIt = m_btnIndexPinNoMap.find(btnIndex);
    if (pinNoIt == m_btnIndexPinNoMap.end()) {
        return;
    }

    string pinNo = pinNoIt->second;
    auto pinConfigIt = m_pinPeriodConfigMap.find(pinNo);
    if (pinConfigIt != m_pinPeriodConfigMap.end()) {
        btn->setCheckable(false);
        m_pinPeriodConfigMap.erase(pinConfigIt);
        emit PinConfigChanged();
    }
}

void RaspiGpioPinBtnPeriodConfigGuiManager::OnPinPeriodConfigChanged(QAbstractButton* pin, bool isHigh, int duration1, int duration2)
{
    if (pin == nullptr) {
        qDebug() << tr("RaspiGpioPinBtnPeriodConfigGuiManager::OnPinPeriodConfigChanged failed due to null btn");
        return;
    }

    int btnIndex = m_pinBtnsGroup.id(pin);
    auto pinNoIt = m_btnIndexPinNoMap.find(btnIndex);
    if (pinNoIt == m_btnIndexPinNoMap.end()) {
        qDebug() << tr("RaspiGpioPinBtnPeriodConfigGuiManager::OnPinPeriodConfigChanged failed due to invalid pinNo");
        return;
    }

    tuple<int, int, int> periodCfg = {(isHigh ? 1 : 0), duration1, duration2};
    string pinNo = pinNoIt->second;
    auto pinVolLvlIt = m_pinPeriodConfigMap.find(pinNo);
    if (pinVolLvlIt == m_pinPeriodConfigMap.end()) {
        m_pinPeriodConfigMap.emplace(pinNo, periodCfg);
    } else {
        pinVolLvlIt->second = periodCfg;
    }

    SetPinsVolLevel({pinNo}, {"Wave"});
    pin->setCheckable(true);
    pin->setChecked(true);

    emit PinConfigChanged();

    qDebug() << tr("RaspiGpioPinBtnPeriodConfigGuiManager::OnPinPeriodConfigChanged, Pin:%1, initVolLvl:%2, duration1:%3, duration2:%4")\
        .arg(QString::fromStdString(pinNo))\
        .arg(get<0>(periodCfg))\
        .arg(get<1>(periodCfg))\
        .arg(get<2>(periodCfg));
}