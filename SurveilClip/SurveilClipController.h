#pragma once
#include "SurveilClipWindow.h"
#include "SurveilClipModel.h"
#include "IVideoProcessor.h"
#include "ParametersWindow.h"

#include <QObject>
#include <QString>

class SurveilClipController : public QObject
{
    Q_OBJECT

public:
    SurveilClipController(SurveilClipWindow* view, SurveilClipModel* model, QObject* parent = nullptr);
    ~SurveilClipController();

private slots:
    void onViewClosing();
    void onVideoWidgetClosing();
    void onParametersWindowClosing();

    void onSelectButtonClicked();
    void onWatchButtonClicked();
    void onGenerateButtonClicked();
    void onWatchResultButtonClicked();
    void onSaveButtonClicked();
    void onCancelButtonClicked();
    void onParametersButtonClicked();


    void onMediaErrorThrowed(const QString& error, const QString& message);
    void onMediaSourceChanged();
    void onMediaProcessStatusChanged(IVideoProcessor::Status status);

private:
    SurveilClipWindow* m_view;
    SurveilClipModel* m_model;

    ParametersWindow* m_parametersWindow;
    bool m_isViewClosing;
};

