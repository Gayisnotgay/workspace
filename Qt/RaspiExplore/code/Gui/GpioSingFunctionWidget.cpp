#include "GpioSingFunctionWidget.h"
#include <bitset>
#include <vector>
#include <string>
#include <QtWidgets/QMessageBox>
#include <QtTest/QTest>
#include "GpioFunctions/GpioFunction.h"
#include "ParseConfig/ParseConfig.h"
#include "AutoRegFactory/AutoRegFactory.h"
#include "SmallTools/HelperDefine.h"
#include "SmallTools/TimeTool.h"

using std::bitset;
using std::unique_ptr;
using std::make_unique;
using std::vector;
using std::string;

GpioSingFunctionWidget::GpioSingFunctionWidget(QWidget* parent) : GpioGetFunctionWidget(parent)
{
    m_funName = tr("GpioSingFunction");
}

GpioSingFunctionWidget::~GpioSingFunctionWidget()
{
    if (m_thdPlaySong.joinable()) {
        m_stopPlay = true;
        m_thdPlaySong.join();
    }
}

void GpioSingFunctionWidget::Initialize(QAbstractButton *btnFunctionRun, QAbstractButton *btnFunctionStop, QButtonGroup* gpioPinsBtnGroup, const SongPlayGuiComponents& components)
{
    GpioGetFunctionWidget::Initialize(btnFunctionRun, gpioPinsBtnGroup);

    if ((btnFunctionStop == nullptr) || (m_btnFunctionStop != nullptr)) {
        return;
    }
    m_btnFunctionStop = btnFunctionStop;
    // 初始禁用功能运行按钮
    m_btnFunctionStop->setEnabled(false);

    m_songGuiComponents = components;
    m_songSelector = make_unique<QFileDialog>(this, tr("Select song file"), m_songGuiComponents.songName->text());
    m_songSelector->setFileMode(QFileDialog::ExistingFile);

    connect(btnFunctionStop, &QAbstractButton::clicked, this, &GpioSingFunctionWidget::OnBtnStopClicked);
    connect(m_songGuiComponents.songName, &QLineEdit::textChanged, this, &GpioSingFunctionWidget::OnPinConfigChanged);
    connect(m_songGuiComponents.songSelect, &QToolButton::clicked, m_songSelector.get(), &QDialog::open);
    connect(m_songSelector.get(), &QFileDialog::finished, this, &GpioSingFunctionWidget::OnSongFileSelected);
}

void GpioSingFunctionWidget::OnBtnRunClicked(bool checked)
{
    m_btnFunctionRun->setEnabled(false);
    m_pinsBtnMng->SetPinsBtnEnable(false);
    m_songGuiComponents.songSelect->setEnabled(false);

    // 组装查询的参数列表
    vector<string> args{m_funName.toStdString(), m_selGpioDrv.toStdString()};
    auto selPinBtnNos = m_pinsBtnMng->GetSelectedPinNumbers();
    args.insert(args.end(), selPinBtnNos.begin(), selPinBtnNos.end());
    args.emplace_back(m_songGuiComponents.songName->text().toStdString());

    // 创建执行命令的进程
    m_thdPlaySong = std::thread([this](const vector<string>& songPara) { SingProcess(songPara); }, args);

    m_btnFunctionStop->setEnabled(true);
}

void GpioSingFunctionWidget::OnBtnStopClicked(bool checked)
{
    m_btnFunctionStop->setEnabled(false);

    m_stopPlay = true;
    m_thdPlaySong.join();

    m_stopPlay = false;
    m_songGuiComponents.songPlayIndicator->setValue(0);
    m_btnFunctionRun->setEnabled(true);
    m_pinsBtnMng->SetPinsBtnEnable(true);
    m_songGuiComponents.songSelect->setEnabled(true);
}

void GpioSingFunctionWidget::SingProcess(const vector<string>& songPara)
{
    do {
        // 发送命令执行请求
        string output;
        auto runProcess = FuncFactory<unique_ptr<GpioFunction>>::Instance().RunFunc(m_funName.toStdString());
        int ctrlFlag = 0;
        int result = runProcess->Run(songPara, output, &ctrlFlag);
        QString resultInfo(tr("GpioSingFunctionWidget run result[%1], args[%2], output[%3]").arg(result).arg(QString::fromStdString(GetSequenceContents(songPara.begin(), songPara.end()))).arg(QString::fromStdString(output)));
        qDebug() << resultInfo;
        if (result != EXIT_SUCCESS) {
            QMessageBox::critical(this, tr("Execute failed"), resultInfo, QMessageBox::Ok);
            return;
        }

        // 等待Gpio操作进程退出后，自身再退出
        while (ctrlFlag < 0x7ffffffd) {
            unsigned int progress = (static_cast<unsigned int>(ctrlFlag) >> 24) & 0xff;
            if (progress <= 100) {
                m_songGuiComponents.songPlayIndicator->setValue(progress);
            }
            
            // 音乐播放完之前终止播放
            if (m_stopPlay) {
                bitset<32> ctrlBitFlag(static_cast<unsigned long>(ctrlFlag));
                ctrlBitFlag[0] = true;
                ctrlFlag = static_cast<int>(ctrlBitFlag.to_ulong());
            }
            TimeTool::millisleep(100);
        }
    } while (m_songGuiComponents.repeatPlay->isChecked() && (!m_stopPlay));

    qDebug() << tr("SingProcess exit");

    m_btnFunctionStop->click();
}

void GpioSingFunctionWidget::OnSongFileSelected(int selectResult)
{
    if (selectResult != QDialog::Accepted) {
        return;
    }

    auto selectedFile = m_songSelector->selectedFiles();
    if (selectedFile.isEmpty()) {
        qDebug() << tr("selectedFile is empty");
        return;
    }

    m_songGuiComponents.songName->setText(selectedFile[0]);
}

void GpioSingFunctionWidget::OnPinConfigChanged()
{
    auto selPinBtnNos = m_pinsBtnMng->GetSelectedPinNumbers();
    if (selPinBtnNos.empty() || m_songGuiComponents.songName->text().isEmpty()) {
        m_btnFunctionRun->setEnabled(false);
    } else {
        m_btnFunctionRun->setEnabled(true);
    }
}