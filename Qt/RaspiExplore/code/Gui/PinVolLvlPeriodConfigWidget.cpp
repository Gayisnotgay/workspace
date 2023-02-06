#include "PinVolLvlPeriodConfigWidget.h"
#include <algorithm>
#include <tuple>
#include <vector>
#include <QtTest/QTest>

using std::for_each;
using std::tuple;
using std::get;
using std::tie;
using std::max;
using std::min;
using std::unique_ptr;
using std::make_unique;
using std::vector;
using std::numeric_limits;

PinVolLvlPeriodConfigWidget::PinVolLvlPeriodConfigWidget(const std::vector<QIcon>& pinVolLvlIconList, QWidget* parent) : PinVolLvlConfigWidget(pinVolLvlIconList, parent), m_durationValidator(100, 900) {}

void PinVolLvlPeriodConfigWidget::CreateChildren()
{
    // 继承父系控件    
    PinVolLvlConfigWidget::CreateChildren();
    m_volLvlLabel->setText(tr("InitVolLvl:"));
    m_volLvlLabel->adjustSize();

    // 创建子类控件：持续时间
    m_durationLabel = make_unique<QLabel[]>(volLvlBtnNum);
    m_durationEdits = make_unique<QLineEdit[]>(volLvlBtnNum);
    for (int editIdx = 0; editIdx < volLvlBtnNum; ++editIdx) {
        m_durationLabel[editIdx].setParent(this);
        m_durationLabel[editIdx].setText(tr("Duration%1").arg(editIdx + 1));
        m_durationLabel[editIdx].adjustSize();
        m_durationEdits[editIdx].setParent(this);
        m_durationEdits[editIdx].setValidator(&m_durationValidator);
        connect(&m_durationEdits[editIdx], &QLineEdit::editingFinished, this, &PinVolLvlPeriodConfigWidget::OnDurationConfigFinished);
    }
}

void PinVolLvlPeriodConfigWidget::InitLayout()
{
    // 调整布局
    int height = max(m_volLvlLabel->height(), m_durationLabel[0].height());
    int labelWidth = max(m_volLvlLabel->width(), m_durationLabel[0].width());
    vector<tuple<QWidget*, QRect>> layoutInfoList = {
        {m_volLvlLabel.get(),   QRect(1, 1, labelWidth, height)},
        {&m_volLvlBtns[0],      QRect(1 + labelWidth, 1, height, height)},
        {&m_volLvlBtns[1],      QRect(1 + labelWidth + height, 1, height, height)},

        {&m_durationLabel[0],   QRect(1, 1 + height, labelWidth, height)},
        {&m_durationEdits[0],   QRect(1 + labelWidth, 1 + height, 2 * height, height)},
        {&m_durationLabel[1],   QRect(1, 1 + 2 * height, labelWidth, height)},
        {&m_durationEdits[1],   QRect(1 + labelWidth, 1 + 2 * height, 2 * height, height)},

        {this,                  QRect(geometry().left(), geometry().top(), 2 + labelWidth + 2 * height, 2 + 3 * height)}
    };
    for_each(layoutInfoList.begin(), layoutInfoList.end(), [](auto& layoutInfo){ get<0>(layoutInfo)->setGeometry(get<1>(layoutInfo)); });


    // 设置widget外观
    setAttribute(Qt::WA_StyledBackground);
    setStyleSheet(tr("PinVolLvlPeriodConfigWidget { border: 2px outset red }"));
    setVisible(false);

    // 设置子控件外观
    vector<QLabel*> labels = {m_volLvlLabel.get(), &m_durationLabel[0], &m_durationLabel[1]};
    for_each(labels.begin(), labels.end(), [](auto& label){ label->setStyleSheet(tr("QLabel { background: red }")); });
}

void PinVolLvlPeriodConfigWidget::Reset()
{
    PinVolLvlConfigWidget::Reset();
    for (int editIdx = 0; editIdx < volLvlBtnNum; ++editIdx) {
        m_curDuationTime[editIdx] = numeric_limits<int>::max();
        m_durationEdits[editIdx].setText(tr(""));
    }
}

void PinVolLvlPeriodConfigWidget::OnVolLvBtnClicked(bool checked)
{
    PinVolLvlConfigWidget::OnVolLvBtnClicked(checked);
    emit PinVolLvlPeriodConfigChanged(m_curFilterPinBtn, (m_selVolLvl == 0), m_curDuationTime[0], m_curDuationTime[1]);
}

void PinVolLvlPeriodConfigWidget::OnDurationConfigFinished()
{
    for (int editIdx = 0; editIdx < volLvlBtnNum; ++editIdx) {
        m_curDuationTime[editIdx] = m_durationEdits[editIdx].text().toInt();
    }

    emit PinVolLvlPeriodConfigChanged(m_curFilterPinBtn, (m_selVolLvl == 0), m_curDuationTime[0], m_curDuationTime[1]);
}