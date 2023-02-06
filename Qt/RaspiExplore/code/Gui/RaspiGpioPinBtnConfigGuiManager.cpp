#include <algorithm>
#include "RaspiGpioPinBtnConfigGuiManager.h"
#include <QtTest/QTest>

using std::for_each;
using std::make_unique;
using std::map;
using std::string;

RaspiGpioPinBtnConfigGuiManager::RaspiGpioPinBtnConfigGuiManager(QButtonGroup& pinBtnsGroup, QObject* parent) : RaspiGpioPinBtnSelectGuiManager(pinBtnsGroup, parent) {}

void RaspiGpioPinBtnConfigGuiManager::Init()
{
    RaspiGpioPinBtnSelectGuiManager::Init();
    BuildVolLvlSelector();
    InstallMouseHoverEvent();
    SetPinsCheckable(false);
}

void RaspiGpioPinBtnConfigGuiManager::BuildVolLvlSelector()
{
    auto btns = m_pinBtnsGroup.buttons();
    if (btns.empty()) {
        qDebug() << tr("RaspiGpioPinBtnConfigGuiManager::BuildVolLvlSelector failed due to empty pin list");
        return;
    }

    m_volSelector = make_unique<PinVolLvlConfigWidget>(m_pinVolLvlIconList, qobject_cast<QWidget*>(btns[0]->parent()));
    m_volSelector->InitWidget();
    connect(m_volSelector.get(), &PinVolLvlConfigWidget::PinVolLvlSelected, this, &RaspiGpioPinBtnConfigGuiManager::OnPinVolLvlSelected);
}

void RaspiGpioPinBtnConfigGuiManager::InstallMouseHoverEvent()
{
    auto btns = m_pinBtnsGroup.buttons();
    if (btns.empty()) {
        qDebug() << tr("RaspiGpioPinBtnConfigGuiManager::InstallMouseHoverEvent failed due to empty pin list");
        return;
    }

    for_each(btns.begin(), btns.end(), [this](QAbstractButton* btn) {
        btn->installEventFilter(this);
    });
}

void RaspiGpioPinBtnConfigGuiManager::SetPinsCheckable(bool checkable)
{
    auto buttons = m_pinBtnsGroup.buttons();
    for_each(buttons.begin(), buttons.end(), [checkable](auto& btn) { btn->setCheckable(checkable); });
}

map<string, int> RaspiGpioPinBtnConfigGuiManager::GetSelectedPinVolLvl()
{
    return m_pinLvlMap;
}

bool RaspiGpioPinBtnConfigGuiManager::eventFilter(QObject *obj, QEvent *event)
{
    auto pinBtn = qobject_cast<QAbstractButton*>(obj);
    if ((pinBtn == nullptr) || (event == nullptr) || ((event->type() != QEvent::HoverEnter) && (event->type() != QEvent::HoverLeave))) {
        return false;
    }

    auto btnIndex = m_pinBtnsGroup.id(pinBtn);
    if (m_btnIndexPinNoMap.find(btnIndex) == m_btnIndexPinNoMap.end()) {
        return false;
    }

    if (event->type() == QEvent::HoverEnter) {
        m_volSelector->Show(pinBtn);
    } else {
        m_volSelector->Hide(pinBtn);
    }

    return false;
}

void RaspiGpioPinBtnConfigGuiManager::OnPinBtnClicked(QAbstractButton *btn, bool checked)
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
    auto pinVolLvlIt = m_pinLvlMap.find(pinNo);
    if (pinVolLvlIt != m_pinLvlMap.end()) {
        btn->setCheckable(false);
        m_pinLvlMap.erase(pinVolLvlIt);
        emit PinConfigChanged();
    }
}

void RaspiGpioPinBtnConfigGuiManager::OnPinVolLvlSelected(QAbstractButton* btn, bool lvlHigh)
{
    if (btn == nullptr) {
        qDebug() << tr("RaspiGpioPinBtnConfigGuiManager::OnPinVolLvlSelected failed due to null btn");
        return;
    }

    int btnIndex = m_pinBtnsGroup.id(btn);
    auto pinNoIt = m_btnIndexPinNoMap.find(btnIndex);
    if (pinNoIt == m_btnIndexPinNoMap.end()) {
        qDebug() << tr("RaspiGpioPinBtnConfigGuiManager::OnPinVolLvlSelected failed due to invalid pinNo");
        return;
    }

    string pinNo = pinNoIt->second;
    auto pinVolLvlIt = m_pinLvlMap.find(pinNo);
    if (pinVolLvlIt == m_pinLvlMap.end()) {
        m_pinLvlMap.emplace(pinNo, (lvlHigh ? 1 : 0));
    } else {
        pinVolLvlIt->second = (lvlHigh ? 1 : 0);
    }

    SetPinsVolLevel({pinNo}, {(lvlHigh ? "High" : "Low")});
    btn->setCheckable(true);
    btn->setChecked(true);
    
    emit PinConfigChanged();
}