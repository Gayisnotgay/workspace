#include "RaspiGpioPinBtnSelectGuiManager.h"
#include <algorithm>
#include <QtWidgets/QMessageBox>
#include "ParseConfig/ParseConfig.h"

using std::string;
using std::vector;
using std::map;
using std::for_each;
using std::make_unique;

RaspiGpioPinBtnSelectGuiManager::RaspiGpioPinBtnSelectGuiManager(QButtonGroup& pinBtnsGroup, QObject* parent) : QObject(parent), m_pinBtnsGroup(pinBtnsGroup) {}

void RaspiGpioPinBtnSelectGuiManager::Init()
{
    BuildPinBtnIndexPinNoMap();
    BuildPinBtnIcons();
    connect(&m_pinBtnsGroup, &QButtonGroup::buttonToggled, this, &RaspiGpioPinBtnSelectGuiManager::OnPinBtnClicked);
}

std::vector<std::string> RaspiGpioPinBtnSelectGuiManager::GetSelectedPinNumbers()
{
    vector<string> selPinsNo; // 选中的按钮在组内的id集合
    auto allPinBtns = m_pinBtnsGroup.buttons();
    for_each(allPinBtns.begin(), allPinBtns.end(), [this, &selPinsNo](QAbstractButton* btn) {
        if ((btn == nullptr) || (!btn->isCheckable()) || (!btn->isChecked())) {
            return;
        }
        
        auto it = m_btnIndexPinNoMap.find(m_pinBtnsGroup.id(btn));
        if (it == m_btnIndexPinNoMap.end()) {
            return;
        }

        selPinsNo.emplace_back(it->second);
    });

    return selPinsNo;
}

int RaspiGpioPinBtnSelectGuiManager::SetPinsVolLevel(const std::vector<std::string>& pinsNum, const std::vector<std::string>& pinsVolLevel)
{
    static map<string, int> volLvlIconIndexMapping = {
        {"High", 0},
        {"Low",  1},
        {"Wave", 2},
    };

    if (pinsNum.empty() || (pinsNum.size() != pinsVolLevel.size())) {
        QMessageBox::warning(nullptr, tr("Internal Warning"), tr("pinsNum's size[%1] is not equal to pinsVolLevel's size[%2]").arg(pinsNum.size()).arg(pinsVolLevel.size()), QMessageBox::Ok);
        return EXIT_FAILURE;
    }

    for (size_t index = 0; index < pinsNum.size(); ++index) {
        auto itPinBtnId = m_pinNoBtnIndexMap.find(pinsNum[index]);
        if (itPinBtnId == m_pinNoBtnIndexMap.end()) {
            QMessageBox::warning(nullptr, tr("Internal Warning"), tr("cannot find btn[pinNo:%1]").arg(QString::fromStdString(pinsNum[index])), QMessageBox::Ok);
            continue;
        }
        auto btn = m_pinBtnsGroup.button(itPinBtnId->second);
        if (btn == nullptr) {
            QMessageBox::critical(nullptr, tr("Internal Warning"), tr("cannot find btn[id:%1]").arg(itPinBtnId->second), QMessageBox::Ok);
            continue;
        }

        if (volLvlIconIndexMapping.find(pinsVolLevel[index]) == volLvlIconIndexMapping.end()) {
            QMessageBox::critical(nullptr, tr("Internal Warning"), tr("pin[%1]'s value[%2] is not valid").arg(btn->text()).arg(QString::fromStdString(pinsVolLevel[index])), QMessageBox::Ok);
            continue;
        }

        btn->setIcon(m_pinVolLvlIconList[volLvlIconIndexMapping[pinsVolLevel[index]]]);
    }

    return EXIT_SUCCESS;
}

void RaspiGpioPinBtnSelectGuiManager::SetPinsBtnEnable(bool enable)
{
    auto allPinBtns = m_pinBtnsGroup.buttons();
    for_each(allPinBtns.begin(), allPinBtns.end(), [&enable](QAbstractButton* btn) {btn->setEnabled(enable);});
}

void RaspiGpioPinBtnSelectGuiManager::OnPinBtnClicked(QAbstractButton *button, bool checked)
{
    if (button == nullptr) {
        return;
    }

    if (!checked) {
        // 取消勾选的按钮清除电平高低图标
        button->setIcon(QIcon());
    }

    emit PinConfigChanged();

    qDebug() << QObject::tr("Button %1(id:%2) is %3").arg(button->text()).arg(m_pinBtnsGroup.id(button)).arg((checked ? tr("selected") : tr("unselected")));
}

// 创建Btn index到PinNo的索引列表
void RaspiGpioPinBtnSelectGuiManager::BuildPinBtnIndexPinNoMap()
{
    auto btnList = m_pinBtnsGroup.buttons();
    for_each(btnList.begin(), btnList.end(), [this](QAbstractButton *btn) {
        auto pinNameSplitParts = ParseConfig::ParsePattern(btn->text().toStdString(), "GPIO(\\d+)");
        if (pinNameSplitParts.empty()) {
            // qDebug() << tr("CalcPinBtnIndexPinNoMap failed for Pin(btnName:%1) because the btnName does not obey the rule \"GPIOXX\" which XX should be numbers").arg(btn->text());
            return;
        }

        m_btnIndexPinNoMap.emplace(m_pinBtnsGroup.id(btn), pinNameSplitParts[0]);
        m_pinNoBtnIndexMap.emplace(pinNameSplitParts[0], m_pinBtnsGroup.id(btn));

    });
}

// 创建Pin高低电平图标
void RaspiGpioPinBtnSelectGuiManager::BuildPinBtnIcons()
{
    auto buttons = m_pinBtnsGroup.buttons();
    if (buttons.empty()) {
        QMessageBox::critical(nullptr, tr("Internal Error"), tr("there is no pin btn in this button group"), QMessageBox::Ok);
        return;
    }

    QList<QString> iconFileList = {
        tr(":/VoltageHighIcon.ico"),
        tr(":/VoltageLowIcon.ico"),
        tr(":/VoltageWaveIcon.ico"),
    };

    m_pinVolLvlIconList.resize(iconFileList.size());
    for(qsizetype index = 0; index < iconFileList.size(); ++index) {
        m_pinVolLvlIconList[index].addFile(iconFileList[index],QSize(buttons[0]->size().height(), buttons[0]->size().height()));
    }
}