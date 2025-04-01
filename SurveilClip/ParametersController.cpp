#include "ParametersController.h"
#include "VideoProcessorDebugable.h"
#include "IVideoProcessorConfigurable.h"

ParametersController::ParametersController(ParametersWindow* view, ParametersModel* model, SurveilClipModel* surveilClipModel, QObject* parent) :
	QObject(parent),
	m_view(view),
	m_model(model),
	m_surveilClipModel(surveilClipModel)
{
	m_view->onPauseTypeChanged(m_model->getPauseType());
	m_view->onIncrementFrameCountChanged(m_model->getIncrementFrameCount());

	connect(m_view, &ParametersWindow::closing, this, &ParametersController::onViewClosing);

	connect(m_view, &ParametersWindow::pauseButtonClicked, this, &ParametersController::onPauseButtonClicked);
	connect(m_view, &ParametersWindow::continueButtonClicked, this, &ParametersController::onContinueButtonClicked);
	connect(m_view, &ParametersWindow::stopButtonClicked, this, &ParametersController::onStopButtonClicked);
	connect(m_view, &ParametersWindow::generateButtonClicked, this, &ParametersController::onGenerateButtonClicked);
	connect(m_view, &ParametersWindow::pauseTypeChanged, this, &ParametersController::setPauseType);
	connect(m_view, &ParametersWindow::framePerFrameValueChanged, this, &ParametersController::setIncrementFrameCount);

	connect(m_model, &ParametersModel::activityDetectedChanged, m_view, &ParametersWindow::onActivityDetectedChanged);
	connect(m_model, &ParametersModel::processedFrameTimeChanged, m_view, &ParametersWindow::onProcessedFrameTimeChanged);
	connect(m_model, &ParametersModel::processDurationChanged, m_view, &ParametersWindow::onProcessDurationChanged);
	connect(m_model, &ParametersModel::imageProcessingChanged, m_view, &ParametersWindow::onImageProcessingChanged);
	connect(m_model, &ParametersModel::imagesProcessingChanged, m_view, &ParametersWindow::onImagesProcessingChanged);
	connect(m_model, &ParametersModel::processPaused, m_view, &ParametersWindow::onProcessPaused);
	connect(m_model, &ParametersModel::processResumed, m_view, &ParametersWindow::onProcessResumed);

	connect(m_surveilClipModel, &SurveilClipModel::sourceChanged, this, &ParametersController::onSourceChanged);

	const VideoProcessorDebugable* debugable = m_model->getDebugable();
	m_view->defineVideoProcessor(debugable);	
	m_view->onVideoProcessorStatusChanged(debugable->getStatus());
	m_view->onSourceChanged(surveilClipModel->getVideoData() != nullptr);

	connect(debugable, &VideoProcessorBase::statusChanged, m_view, &ParametersWindow::onVideoProcessorStatusChanged);

	const IVideoProcessorConfigurable* configurable = dynamic_cast<const IVideoProcessorConfigurable*>(debugable);
	if (configurable)
	{
		connect(m_view, &ParametersWindow::saveClicked, this, &ParametersController::saveParameters);
		connect(m_view, &ParametersWindow::resetClicked, this, &ParametersController::resetParameters);
	}
}

ParametersController::~ParametersController()
{
	if (m_view)
	{
		delete m_view;
		m_view = nullptr;
	}
	if (m_model)
	{
		delete m_model;
		m_model = nullptr;
	}
}

void ParametersController::onViewClosing()
{
	deleteLater();
}

void ParametersController::onPauseButtonClicked()
{
	m_model->pauseProcessingRequested();
}

void ParametersController::onContinueButtonClicked()
{
	m_model->continueProcessingRequested();
}

void ParametersController::onStopButtonClicked()
{
	m_model->cancelRequested();
}

void ParametersController::onGenerateButtonClicked()
{
	m_surveilClipModel->generate();
}

void ParametersController::saveParameters()
{
	IVideoProcessorConfigurable* configurable = dynamic_cast<IVideoProcessorConfigurable*>(m_model->getDebugable());
	if (configurable)
	{
		bool saved = configurable->saveParameters();
	}
}

void ParametersController::resetParameters()
{
	IVideoProcessorConfigurable* configurable = dynamic_cast<IVideoProcessorConfigurable*>(m_model->getDebugable());
	if (configurable)
	{
		configurable->resetParameters();
	}
}

void ParametersController::setPauseType(IVideoProcessorDebugable::PauseType pauseType)
{
	m_model->setPauseType(pauseType);
}

void ParametersController::setIncrementFrameCount(int incrementFrameCount)
{
	m_model->setIncrementFrameCount(incrementFrameCount);
}

void ParametersController::onSourceChanged()
{
	m_view->onSourceChanged(m_surveilClipModel->getVideoData() != nullptr);
}
