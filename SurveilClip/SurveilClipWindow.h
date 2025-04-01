#pragma once

#include "ui_SurveilClipUI.h"
#include "SurveilClipModel.h"
#include "SurveilClipVideoWidget.h"

#include <QtWidgets/QMainWindow>
#include <QVideoWidget>
#include <QCloseEvent>
#include <QString>

class SurveilClipWindow : public QMainWindow
{
    Q_OBJECT

public:
    SurveilClipWindow(QWidget* parent = nullptr);
    ~SurveilClipWindow();

    // protected methods
protected:
    void closeEvent(QCloseEvent* event) override;

    // private methods
private:
    void resetUi() const;

    // public Getters & Setters
public:
    SurveilClipVideoWidget* getVideoWidget() const;

    // public methods
public:
    QString selectVideoFile();
    QString saveVideoFile();

    void messageBoxError(const QString& title, const QString& message);

    // methods to update UI
    void updateVideoData(const VideoData* videoData) const;
    void updateUiProcessWorking();
    void updateUiProcessNotWorking(bool processFinishedSuccesfuly);

    // public slots
public slots:
    void updateProgressBar(int value);
    void onProcessDurationChanged(int seconds);

signals:
    void closing();
    void selectButtonClicked();
    void watchButtonClicked();
    void generateButtonClicked();
    void cancelButtonClicked();
    void watchResultButtonClicked();
    void saveButtonClicked();
    void parametersButtonClicked();
    
private:
    Ui::SurveilClipUI* m_ui;
    SurveilClipVideoWidget* m_videoWidget;  // Widget pour afficher la vidéo
    bool m_isWorking;
};
