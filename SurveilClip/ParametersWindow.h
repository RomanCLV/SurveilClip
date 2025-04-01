#pragma once
#include "ui_ParametersWindowUI.h"
#include "IVideoProcessor.h"
#include "IVideoProcessorDebugable.h"
#include "IVideoProcessorConfigurable.h"
#include "ParameterGroup.h"
#include "ParameterBase.h"
#include "BoolParameter.h"
#include "NumericalParameter.h"
#include "ListParameterBase.h"
#include "NumericalParameterWidget.h"
#include "BoolParameterWidget.h"
#include "ListParameterWidget.h"
#include "FrameDebug.h"

#include <QMainWindow>
#include <QCloseEvent>
#include <QList>
#include <QGroupBox>
#include <QImage>
#include <opencv2/opencv.hpp>

class ParametersWindow : public QMainWindow
{
	Q_OBJECT

public:
	ParametersWindow(QWidget *parent = nullptr);
	~ParametersWindow();

public:
	void defineVideoProcessor(const IVideoProcessorDebugable* debugable);

protected:
	void closeEvent(QCloseEvent* event) override;

private:
	void setParameters(const QList<const ParameterGroup*>& groups);

	QGroupBox* createParameterGroupWidget(const ParameterGroup* parameter);
	QWidget* createParameterWidget(const ParameterBase* parameter);
	NumericalParameterWidget* createNumericalParameterWidget(const NumericalParameter* parameter);
	BoolParameterWidget* createBoolParameterWidget(const BoolParameter* parameter);
	ListParameterWidget* createListParameterWidget(const ListParameterBase* parameter);

public slots:
	void onVideoProcessorStatusChanged(IVideoProcessor::Status status);
	void onPauseTypeChanged(IVideoProcessorDebugable::PauseType pauseType);
	void onIncrementFrameCountChanged(int incrementFrameCount);
	void onActivityDetectedChanged(bool detected, int frame, int detections);
	void onImageProcessingChanged(const FrameDebug* frameDebug);
	void onImagesProcessingChanged(const QList<FrameDebug*>& frameDebugList);
	void onProcessedFrameTimeChanged(int milliseconds);
	void onProcessDurationChanged(int seconds);
	void onSourceChanged(bool hasSource);
	void onProcessPaused();
	void onProcessResumed();

private slots:
	void onSpaceBarRadioButtonClicked();
	void onFrameRadioButtonClicked();
	
signals:
	void closing();
	void pauseButtonClicked();
	void continueButtonClicked();
	void generateButtonClicked();
	void stopButtonClicked();
	void saveClicked();
	void resetClicked();

	void pauseTypeChanged(IVideoProcessorDebugable::PauseType pauseType);
	void framePerFrameValueChanged(int value);

private:
	Ui::ParametersWindowUI*m_ui;
	bool m_videoProcessorIsConfigurable;
};
