#pragma once
#include "ParametersModel.h"
#include "ParametersWindow.h"
#include "SurveilClipModel.h"
#include "IVideoProcessor.h"
#include "IVideoProcessorDebugable.h"

#include <QObject>


class ParametersController : public QObject
{
    Q_OBJECT

public:
    ParametersController(ParametersWindow* view, ParametersModel* model, SurveilClipModel* surveilClipModel, QObject* parent = nullptr);
    ~ParametersController();

private slots:
    void onViewClosing();
	void onPauseButtonClicked();
	void onContinueButtonClicked();
	void onStopButtonClicked();
	void onGenerateButtonClicked();
	void saveParameters();
	void resetParameters();

	void setPauseType(IVideoProcessorDebugable::PauseType pauseType);
	void setIncrementFrameCount(int incrementFrameCount);

	void onSourceChanged();

private:
    ParametersWindow* m_view;
    ParametersModel* m_model;
	SurveilClipModel* m_surveilClipModel;
};
