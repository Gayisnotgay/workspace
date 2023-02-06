#pragma once

#include <memory>
#include <limits>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtCore/QTimer>

class PinVolLvlConfigWidget : public QWidget {
    Q_OBJECT
public:
    PinVolLvlConfigWidget(const std::vector<QIcon>& pinVolLvlIconList, QWidget* parent = nullptr);
    ~PinVolLvlConfigWidget() override = default;
    void InitWidget();
    void Show(QAbstractButton* btn);
    void Hide(QAbstractButton* btn);
    int GetSelectedVolLvl(bool& isHigh);
    void SetSelectedVolLvl(bool isHigh);
signals:
    void PinVolLvlSelected(QAbstractButton* btn, bool lvlHigh);
protected slots:
    virtual void OnVolLvBtnClicked(bool checked);
    void OnFilterTimeout();
protected:
    virtual void CreateChildren();
    virtual void InitLayout();
    virtual void Reset();

    void ShowNow();
    void HideNow();
    bool eventFilter(QObject *obj, QEvent *event) override;

    std::unique_ptr<QLabel> m_volLvlLabel;
    static const int volLvlBtnNum = 2;
    std::unique_ptr<QPushButton[]> m_volLvlBtns;
    const std::vector<QIcon>& m_pinVolLvlIconList;
    int m_selVolLvl = std::numeric_limits<int>::max();

    enum class Status {
        HIDE = 0,
        SHOW_FILTERING,
        SHOW,
        HIDE_FILTERING,
    };

    Status m_showStatus = Status::HIDE;
    QAbstractButton* m_curFilterPinBtn = nullptr;
    QTimer m_showStatusTimer;

private:
    void InstallMouseHoverEvent();
};