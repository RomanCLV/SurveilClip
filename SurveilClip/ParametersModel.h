#pragma once
#include "VideoProcessorDebugable.h"
#include "SurveilClipModel.h"
#include "ParameterGroup.h"
#include "FrameDebug.h"

#include "opencv2/opencv.hpp"

#include <QObject>
#include <QList>
#include <QImage>

class ParametersModel : public QObject
{
	Q_OBJECT

public:
	ParametersModel(VideoProcessorDebugable* debugable, QObject* parent = nullptr);
	~ParametersModel();

	VideoProcessorDebugable* getDebugable() const;

	IVideoProcessorDebugable::PauseType getPauseType() const;
	int getIncrementFrameCount() const;

public slots:
	void setPauseType(IVideoProcessorDebugable::PauseType pauseType);
	void setIncrementFrameCount(int incrementFrameCount);

signals:
	void pauseProcessingRequested();
	void continueProcessingRequested();
	void cancelRequested();
	void processPaused();
	void processResumed();
	void activityDetectedChanged(bool detected, int frame, int detections);
	void imageProcessingChanged(const FrameDebug* frameDebug);
	void imagesProcessingChanged(const QList<FrameDebug*>& frameDebugList);
	void processedFrameTimeChanged(int milliseconds);
	void processDurationChanged(int seconds);

private:
	VideoProcessorDebugable* m_debugable;
};
