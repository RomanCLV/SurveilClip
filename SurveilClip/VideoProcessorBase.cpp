#include "VideoProcessorBase.h"
#include "IVideoProcessor.h"

VideoProcessorBase::VideoProcessorBase(QObject* parent) :
	QObject(parent),
	m_status(Status::None),
	m_progress(0),
	m_cancelRequest(false)
{
}

VideoProcessorBase::~VideoProcessorBase()
{
}

IVideoProcessor::Status VideoProcessorBase::getStatus() const
{
	return m_status;
}

void VideoProcessorBase::cancel()
{
	m_cancelRequest = true;
}

void VideoProcessorBase::setStatus(Status status)
{
	if (m_status != status)
	{
		m_status = status;
		emit statusChanged(m_status);
	}
}

bool VideoProcessorBase::getCancelRequest() const
{
	return m_cancelRequest;
}

void VideoProcessorBase::consumeCancelRequest()
{
	m_cancelRequest = false;
}

int VideoProcessorBase::getProgress() const
{
	return m_progress;
}

void VideoProcessorBase::resetProgress()
{
	m_progress = 0;
	emit progressUpdated(m_progress);
}

void VideoProcessorBase::incrementProgress(int inc)
{
	m_progress += inc;
	emit progressUpdated(m_progress);
}
