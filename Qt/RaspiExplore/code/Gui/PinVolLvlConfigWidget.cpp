#include "PinVolLvlConfigWidget.h"
#include <algorithm>
#include <vector>
#include <QtTest/QTest>

using std::for_each;
using std::max;
using std::unique_ptr;
using std::make_unique;
using std::vector;
using std::numeric_limits;

PinVolLvlConfigWidget::PinVolLvlConfigWidget(const std::vector<QIcon>& pinVolLvlIconList, QWidget* parent) : QWidget(parent), m_pinVolLvlIconList(pinVolLvlIconList), m_showStatusTimer(this)
{
    InstallMouseHoverEvent();
}

void PinVolLvlConfigWidget::InitWidget()
{
    CreateChildren();
    InitLayout();
}

void PinVolLvlConfigWidget::CreateChildren()
{
    m_volLvlLabel = make_unique<QLabel>(tr("Lvl:"), this);
    m_volLvlBtns = make_unique<QPushButton[]>(volLvlBtnNum);
    for (int btnIdx = 0; btnIdx < volLvlBtnNum; ++btnIdx) {
        m_volLvlBtns[btnIdx].setParent(this);
        m_volLvlBtns[btnIdx].setCheckable(true);
        m_volLvlBtns[btnIdx].setAutoExclusive(true);
        m_volLvlBtns[btnIdx].setIcon(m_pinVolLvlIconList[btnIdx]);
        connect(&m_volLvlBtns[btnIdx], &QAbstractButton::clicked, this, &PinVolLvlConfigWidget::OnVolLvBtnClicked);
    }

    m_showStatusTimer.setInterval(1000);
    connect(&m_showStatusTimer, &QTimer::timeout, this, &PinVolLvlConfigWidget::OnFilterTimeout);
}

void PinVolLvlConfigWidget::InitLayout()
{
    // 调整label大小
    m_volLvlLabel->adjustSize();
    int height = max(m_volLvlLabel->height(), m_volLvlLabel->width());
    vector<QWidget*> children = {m_volLvlLabel.get(), &m_volLvlBtns[0], &m_volLvlBtns[1]};

    // 调整button大小
    for (size_t idx = 0; idx < children.size(); ++idx) {
        QRect childRect(height * idx + 1, 1, height, height);
        children[idx]->setGeometry(childRect);
    }

    // 调整widget自身大小
    QRect widgetRect(geometry().left(), geometry().top(), height * 3 + 2, height + 2);
    setGeometry(widgetRect);


    // 设置widget外观：背景，边框
    setAttribute(Qt::WA_StyledBackground);
    setStyleSheet(tr("PinVolLvlConfigWidget { border: 2px outset red }"));
    setVisible(false);

    // 设置子控件外观
    m_volLvlLabel->setStyleSheet(tr("QLabel { background: red }"));
}

void PinVolLvlConfigWidget::InstallMouseHoverEvent()
{
    setAttribute(Qt::WA_Hover);
    installEventFilter(this);
}

void PinVolLvlConfigWidget::Show(QAbstractButton* btn)
{
    if ((m_showStatus == Status::SHOW) || (m_showStatus == Status::SHOW_FILTERING)) {
        return;
    }

    m_showStatusTimer.start();
    m_showStatus = Status::SHOW_FILTERING;
    m_curFilterPinBtn = btn;
}

void PinVolLvlConfigWidget::Hide(QAbstractButton* btn)
{
    if ((m_showStatus == Status::HIDE) || (m_showStatus == Status::HIDE_FILTERING)) {
        return;
    }

    m_showStatusTimer.start();
    m_showStatus = Status::HIDE_FILTERING;
    m_curFilterPinBtn = btn;
}

int PinVolLvlConfigWidget::GetSelectedVolLvl(bool& isHigh)
{
    if (m_selVolLvl == numeric_limits<int>::max()) {
        return EXIT_FAILURE;
    }

    isHigh = (m_selVolLvl == 0);
    return EXIT_SUCCESS;
}

void PinVolLvlConfigWidget::SetSelectedVolLvl(bool isHigh)
{
    m_volLvlBtns[(isHigh ? 0 : 1)].setChecked(true);
}

void PinVolLvlConfigWidget::Reset()
{
    m_selVolLvl = numeric_limits<int>::max();

    for (int idx = 0; idx < volLvlBtnNum; ++idx) {
        m_volLvlBtns[idx].setAutoExclusive(false);
    }

    for (int idx = 0; idx < volLvlBtnNum; ++idx) {
        m_volLvlBtns[idx].setChecked(false);
    }

    for (int idx = 0; idx < volLvlBtnNum; ++idx) {
        m_volLvlBtns[idx].setAutoExclusive(true);
    }
}

void PinVolLvlConfigWidget::OnFilterTimeout()
{
    if (m_showStatus == Status::HIDE_FILTERING) {
        m_showStatus = Status::HIDE;
        HideNow();
    } else if (m_showStatus == Status::SHOW_FILTERING) {
        m_showStatus = Status::SHOW;
        ShowNow();
    } else {
        qDebug() << tr("Invalid timeout");
    }
}

void PinVolLvlConfigWidget::ShowNow()
{
    m_showStatusTimer.stop();

    if (m_curFilterPinBtn == nullptr) {
        return;
    }

    Reset();
    QPoint newPos(m_curFilterPinBtn->pos().x() + m_curFilterPinBtn->width() / 2, m_curFilterPinBtn->pos().y());
    setGeometry(QRect(newPos, size()));
    setVisible(true);
}

void PinVolLvlConfigWidget::HideNow()
{
    m_showStatusTimer.stop();
    setVisible(false);
}

bool PinVolLvlConfigWidget::eventFilter(QObject *obj, QEvent *event)
{
    auto me = qobject_cast<QWidget*>(obj);
    if ((me != this) || ((event->type() != QEvent::HoverEnter) && (event->type() != QEvent::HoverMove) && (event->type() != QEvent::HoverLeave))) {
        return false;
    }

    if ((event->type() == QEvent::HoverEnter) || (event->type() == QEvent::HoverMove)) {
        m_showStatusTimer.stop();
    } else {
        m_showStatusTimer.start();
    }

    return false;
}

void PinVolLvlConfigWidget::OnVolLvBtnClicked(bool checked)
{
    for (int idx = 0; idx < volLvlBtnNum; ++idx) {
        if (m_volLvlBtns[idx].isChecked()) {
            m_selVolLvl = idx;
            emit PinVolLvlSelected(m_curFilterPinBtn, (m_selVolLvl == 0));
            break;
        }
    }
}