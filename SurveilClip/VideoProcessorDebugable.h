#pragma once
#include "VideoProcessorBase.h"
#include "IVideoProcessorDebugable.h"
#include "FrameDebug.h"
#include <QObject>
#include <QList>
#include <QImage>
#include <opencv2/opencv.hpp>

class VideoProcessorDebugable : public VideoProcessorBase, public IVideoProcessorDebugable
{
	Q_OBJECT
	Q_INTERFACES(IVideoProcessorDebugable)

public:
	explicit VideoProcessorDebugable(QObject* parent = nullptr);
	virtual ~VideoProcessorDebugable() = default;

	void addListener(const QObject* listener) override;
	void removeListener(const QObject* listener) override;

	PauseType getPauseType() const override;
	int getIncrementFrameCount() const override;

public slots:
	virtual void pauseProcessingRequested() = 0;
	virtual void continueProcessingRequested() = 0;
	void cancelRequested();
	void setIncrementFrameCount(int incrementFrameCount);
	void setPauseType(PauseType pauseType);

protected:
	bool hasListeners() const;

signals:
	void imageProcessingChanged(const FrameDebug* frameDebug);
	void imagesProcessingChanged(const QList<FrameDebug*>& frameDebugList);

	void activityDetectedChanged(bool detected, int frame, int detections);
	void processedFrameTimeChanged(int milliseconds);

	void processPaused();
	void processResumed();

protected:
	const int MIN_INCREMENT_FRAME_COUNT = 1;

private:
	QList<const QObject*> m_listeners;
	PauseType m_pauseType;
	int m_incrementFrameCount;
};
