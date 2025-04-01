#include "ParametersModel.h"

ParametersModel::ParametersModel(VideoProcessorDebugable* debugable, QObject* parent) :
	QObject(parent),
	m_debugable(debugable)
{
	m_debugable->addListener(this);
	connect(this, &ParametersModel::pauseProcessingRequested, m_debugable, &VideoProcessorDebugable::pauseProcessingRequested);
	connect(this, &ParametersModel::continueProcessingRequested, m_debugable, &VideoProcessorDebugable::continueProcessingRequested);
	connect(this, &ParametersModel::cancelRequested, m_debugable, &VideoProcessorDebugable::cancelRequested);

	connect(m_debugable, &VideoProcessorDebugable::activityDetectedChanged, this, &ParametersModel::activityDetectedChanged);
	connect(m_debugable, &VideoProcessorDebugable::imageProcessingChanged, this, &ParametersModel::imageProcessingChanged);
	connect(m_debugable, &VideoProcessorDebugable::imagesProcessingChanged, this, &ParametersModel::imagesProcessingChanged);
	connect(m_debugable, &VideoProcessorDebugable::processedFrameTimeChanged, this, &ParametersModel::processedFrameTimeChanged);
	connect(m_debugable, &VideoProcessorDebugable::processDurationChanged, this, &ParametersModel::processDurationChanged);
	connect(m_debugable, &VideoProcessorDebugable::processPaused, this, &ParametersModel::processPaused);
	connect(m_debugable, &VideoProcessorDebugable::processResumed, this, &ParametersModel::processResumed);
}

ParametersModel::~ParametersModel()
{
	m_debugable->removeListener(this);
}

VideoProcessorDebugable* ParametersModel::getDebugable() const
{
	return m_debugable;
}

IVideoProcessorDebugable::PauseType ParametersModel::getPauseType() const
{
	return m_debugable->getPauseType();
}

int ParametersModel::getIncrementFrameCount() const
{
	return m_debugable->getIncrementFrameCount();
}

void ParametersModel::setPauseType(IVideoProcessorDebugable::PauseType pauseType)
{
	m_debugable->setPauseType(pauseType);
}

void ParametersModel::setIncrementFrameCount(int incrementFrameCount)
{
	m_debugable->setIncrementFrameCount(incrementFrameCount);
}