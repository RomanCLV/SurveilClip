#include "VideoProcessorDebugable.h"

VideoProcessorDebugable::VideoProcessorDebugable(QObject* parent) :
	VideoProcessorBase(parent),
	m_listeners(),
	m_pauseType(PauseType::BarSpace),
	m_incrementFrameCount(MIN_INCREMENT_FRAME_COUNT)
{
}

void VideoProcessorDebugable::addListener(const QObject* listener)
{
	if (!m_listeners.contains(listener))
	{
		m_listeners.append(listener);
	}
}

void VideoProcessorDebugable::removeListener(const QObject* listener)
{
	if (m_listeners.contains(listener))
	{
		m_listeners.removeOne(listener);
	}
}

bool VideoProcessorDebugable::hasListeners() const
{
	return !m_listeners.isEmpty();
}


int VideoProcessorDebugable::getIncrementFrameCount() const
{
	return m_incrementFrameCount;
}

void VideoProcessorDebugable::setIncrementFrameCount(int incrementFrameCount)
{
	if (incrementFrameCount >= MIN_INCREMENT_FRAME_COUNT)
	{
		m_incrementFrameCount = incrementFrameCount;
	}
}

IVideoProcessorDebugable::PauseType VideoProcessorDebugable::getPauseType() const
{
	return m_pauseType;
}

void VideoProcessorDebugable::setPauseType(PauseType pauseType)
{
	m_pauseType = pauseType;
}

void VideoProcessorDebugable::cancelRequested()
{
	cancel();
}