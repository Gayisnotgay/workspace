#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <QtWidgets/QWidget>
#include <QtGui/QPixmap>
#include "ui_ExploreRaspiAppGui.h"

class ExploreRaspiAppGuiWidget : public QWidget {
    Q_OBJECT

public:
    ExploreRaspiAppGuiWidget(QWidget* parent = nullptr);
    ~ExploreRaspiAppGuiWidget() override = default;

    Q_PROPERTY(std::string funList MEMBER m_funList WRITE SetFunList NOTIFY OnFunListChanged)
    Q_PROPERTY(std::string gpioDrvList MEMBER m_gpioDrvList WRITE SetGpioDrvList NOTIFY OnGpioDrvListChanged)

signals:
    void OnFunListChanged(const std::string& funList);
    void OnGpioDrvListChanged(const std::string& gpioDrvList);

public slots:
    void SetFunList(const std::string& funList);
    void SetGpioDrvList(const std::string& gpioDrvList);

private slots:
    void on_funBox_currentTextChanged(const QString &newFun);
    void on_gpioDrvBox_currentTextChanged(const QString &newDrv);
    void on_configApplyBtn_clicked(bool checked);

private:
    void UiAdditionalConfig();
    void InitGpioFunctions();
    void UpdateRaspiBasicInfo();

    Ui::Form m_ui;
    std::unique_ptr<QPixmap> m_raspi400Logo;
    std::string m_funList;
    std::string m_gpioDrvList;
    QString m_selFun;
    QString m_selGpioDrv;
    // 功能界面是否初始化过
    std::unique_ptr<bool[]> m_isFuncInitList;
};