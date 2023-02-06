#pragma once
#include <thread>
#include "GpioGetFunctionWidget.h"
#include <QtWidgets/QToolButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFileDialog>

class GpioSingFunctionWidget : public GpioGetFunctionWidget {
    Q_OBJECT
public:
    GpioSingFunctionWidget(QWidget* parent = nullptr);
    ~GpioSingFunctionWidget() override;
    struct SongPlayGuiComponents {
        QToolButton* songSelect;
        QLineEdit* songName;
        QCheckBox* repeatPlay;
        QProgressBar* songPlayIndicator;
    };
    void Initialize(QAbstractButton *btnFunctionRun, QAbstractButton *btnFunctionStop, QButtonGroup* gpioPinsBtnGroup, const SongPlayGuiComponents& components);

protected slots:
    void OnBtnRunClicked(bool checked) override;
    void OnPinConfigChanged() override;
    void OnBtnStopClicked(bool checked);
    void OnSongFileSelected(int selectResult);
protected:
    SongPlayGuiComponents m_songGuiComponents = {nullptr, nullptr, nullptr};
    std::unique_ptr<QFileDialog> m_songSelector;
    void SingProcess(const std::vector<std::string>& songPara);
    QAbstractButton *m_btnFunctionStop = nullptr;

    bool m_stopPlay = false;
    std::thread m_thdPlaySong;
};