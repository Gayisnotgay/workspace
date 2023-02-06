#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <QtWidgets/QWidget>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QButtonGroup>

class RaspiGpioPinBtnSelectGuiManager : public QObject {
    Q_OBJECT
public:
    RaspiGpioPinBtnSelectGuiManager(QButtonGroup& pinBtnsGroup, QObject* parent = nullptr);
    virtual void Init();
    std::vector<std::string> GetSelectedPinNumbers();
    int SetPinsVolLevel(const std::vector<std::string>& pinsNum, const std::vector<std::string>& pinsVolLevel);
    void SetPinsBtnEnable(bool enable);
protected slots:
    virtual void OnPinBtnClicked(QAbstractButton *button, bool checked);
signals:
    void PinConfigChanged();
protected:
    // 创建Btn index到PinNo的索引列表
    void BuildPinBtnIndexPinNoMap();
    // 创建Pin高低电平图标
    virtual void BuildPinBtnIcons();

    QButtonGroup& m_pinBtnsGroup;
    // Button Index -> Pin Number
    std::map<int, std::string> m_btnIndexPinNoMap;
    // Pin Number -> Button Index
    std::map<std::string, int> m_pinNoBtnIndexMap;
    // Pin Button电平高低指示的ICON
    std::vector<QIcon> m_pinVolLvlIconList;
};