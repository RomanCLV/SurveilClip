#include "SurveilClipWindow.h"
#include "Tools.h"

#include <QFileDialog>
#include <QVBoxLayout>
#include <QMessageBox>

SurveilClipWindow::SurveilClipWindow(QWidget* parent) :
    QMainWindow(parent),
    m_ui(new Ui::SurveilClipUI()),
    m_videoWidget(new SurveilClipVideoWidget()),
    m_isWorking(false)
{
    m_ui->setupUi(this);

    // Connect buttons to personalized signals
    connect(m_ui->selectButton, &QPushButton::clicked, this, &SurveilClipWindow::selectButtonClicked);
    connect(m_ui->watchButton, &QPushButton::clicked, this, &SurveilClipWindow::watchButtonClicked);
    connect(m_ui->generateButton, &QPushButton::clicked, this, &SurveilClipWindow::generateButtonClicked);
    connect(m_ui->cancelButton, &QPushButton::clicked, this, &SurveilClipWindow::cancelButtonClicked);
    connect(m_ui->watchResultButton, &QPushButton::clicked, this, &SurveilClipWindow::watchResultButtonClicked);
    connect(m_ui->saveButton, &QPushButton::clicked, this, &SurveilClipWindow::saveButtonClicked);
    connect(m_ui->parametersButton, &QPushButton::clicked, this, &SurveilClipWindow::parametersButtonClicked);

    resetUi();

    m_videoWidget->resize(640, 480);
}

SurveilClipWindow::~SurveilClipWindow()
{
    if (m_videoWidget)
    {
        m_videoWidget->close(); 
        delete m_videoWidget;
        m_videoWidget = nullptr;
    }

    delete m_ui;
}

void SurveilClipWindow::closeEvent(QCloseEvent* event)
{
    event->ignore();

    if (m_videoWidget)
    {
        m_videoWidget->close();
    }

    if (!m_isWorking ||
        (QMessageBox::warning(this, "Process still working!", "The video process is still running. Do you really want to exit?", QMessageBox::StandardButton::Yes, QMessageBox::StandardButton::No) == QMessageBox::StandardButton::Yes))
    {
        //event->accept();
        emit closing();
    }
}

#pragma region private methods

void SurveilClipWindow::resetUi() const
{
    m_ui->titleLabel->setText("");
    m_ui->sizeLabel->setText("");
    m_ui->durationLabel->setText("");
    m_ui->fpsLabel->setText("");
    m_ui->resolutionLabel->setText("");

    m_ui->formatLabel->setText("");
    m_ui->videoCodecLabel->setText("");
    m_ui->audioCodecLabel->setText("");

    m_ui->watchButton->setEnabled(false);
    m_ui->generateButton->setEnabled(false);
    m_ui->cancelButton->setEnabled(false);
    m_ui->watchResultButton->setEnabled(false);
    m_ui->saveButton->setEnabled(false);

    m_ui->progressBar->setVisible(false);
    m_ui->statusBar->clearMessage();
}

#pragma endregion

#pragma region public Getters & Setters

SurveilClipVideoWidget* SurveilClipWindow::getVideoWidget() const
{
    return m_videoWidget;
}

#pragma endregion

#pragma region public methods

QString SurveilClipWindow::selectVideoFile()
{
    return QFileDialog::getOpenFileName(this, "Select Video", "", "Videos (*.mp4 *.avi *.mkv)");
}

QString SurveilClipWindow::saveVideoFile()
{
    return QFileDialog::getSaveFileName(this, "Save Video", "", "Videos (*.mp4)");
}

void SurveilClipWindow::messageBoxError(const QString& title, const QString& message)
{
    QMessageBox::critical(this, title, message);
}

void SurveilClipWindow::updateVideoData(const VideoData* videoData) const
{
    if (videoData)
    {
        m_ui->titleLabel->setText(videoData->name);
        m_ui->sizeLabel->setText(QString::number((double)videoData->sizeByte / (1024.0 * 1024.0), 'f', 2) + " MB");
        m_ui->durationLabel->setText(Tools::formatTimeMs(videoData->durationMs));
        m_ui->fpsLabel->setText(QString::number(videoData->fps, 'f', 2) + " fps");
        m_ui->resolutionLabel->setText(QString::number(videoData->width) + " x " + QString::number(videoData->height));
        
        m_ui->formatLabel->setText(videoData->fileFormat);
        m_ui->videoCodecLabel->setText(videoData->videoCodec);
        m_ui->audioCodecLabel->setText(videoData->audioCodec);

        m_ui->generateButton->setEnabled(true);
        m_ui->watchButton->setEnabled(true);
        m_ui->statusBar->clearMessage();
    }
    else
    {
        resetUi();
    }
}

#pragma endregion

#pragma region public _slots

void SurveilClipWindow::updateUiProcessWorking()
{
    m_isWorking = true;
    m_ui->generateButton->setEnabled(false);
    m_ui->generateButton->setText("Generating...");
    m_ui->cancelButton->setEnabled(true);
    m_ui->progressBar->setVisible(true);

    m_ui->saveButton->setEnabled(false);
    m_ui->watchResultButton->setEnabled(false);
}

void SurveilClipWindow::updateUiProcessNotWorking(bool processFinishedSuccesfuly)
{
    m_isWorking = false;
    m_ui->generateButton->setEnabled(true);
    m_ui->generateButton->setText("Generate");
    m_ui->cancelButton->setEnabled(false);
    m_ui->progressBar->setVisible(false);
    m_ui->statusBar->clearMessage();

    if (processFinishedSuccesfuly)
    {
        m_ui->saveButton->setEnabled(true);
        m_ui->watchResultButton->setEnabled(true);
    }
}

void SurveilClipWindow::updateProgressBar(int value)
{
    m_ui->progressBar->setValue(value);
}

void SurveilClipWindow::onProcessDurationChanged(int seconds)
{
    m_ui->statusBar->showMessage("Process duration: " + Tools::formatTime(seconds));
}

#pragma endregion
