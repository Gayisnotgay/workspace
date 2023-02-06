#pragma once

#include <QtWidgets/QLineEdit>
#include <QtGui/QIntValidator>
#include "PinVolLvlConfigWidget.h"

class PinVolLvlPeriodConfigWidget : public PinVolLvlConfigWidget {
    Q_OBJECT
public:
    PinVolLvlPeriodConfigWidget(const std::vector<QIcon>& pinVolLvlIconList, QWidget* parent = nullptr);
    ~PinVolLvlPeriodConfigWidget() override = default;

signals:
    void PinVolLvlPeriodConfigChanged(QAbstractButton* pin, bool isHigh, int duration1, int duration2);
protected slots:
    void OnVolLvBtnClicked(bool checked) override;
    void OnDurationConfigFinished();
protected:
    void CreateChildren() override;
    void InitLayout() override;
    void Reset() override;
private:
    std::unique_ptr<QLabel[]> m_durationLabel;
    std::unique_ptr<QLineEdit[]> m_durationEdits;
    // duration1, duration2
    int m_curDuationTime[volLvlBtnNum] = {std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
    QIntValidator m_durationValidator;
};