#include "SurveilClipController.h"
#include "ViewFactory.h"
#include "IVideoProcessor.h"
#include "VideoProcessorDebugable.h"

SurveilClipController::SurveilClipController(SurveilClipWindow* view, SurveilClipModel* model, QObject* parent) :
	QObject(parent),
	m_view(view),
	m_model(model),
	m_parametersWindow(nullptr),
	m_isViewClosing(false)
{
	SurveilClipVideoWidget* videoWidget = m_view->getVideoWidget();

	connect(m_view, &SurveilClipWindow::closing, this, &SurveilClipController::onViewClosing);
	connect(m_view, &SurveilClipWindow::selectButtonClicked, this, &SurveilClipController::onSelectButtonClicked);
	connect(m_view, &SurveilClipWindow::watchButtonClicked, this, &SurveilClipController::onWatchButtonClicked);
	connect(m_view, &SurveilClipWindow::generateButtonClicked, this, &SurveilClipController::onGenerateButtonClicked);
	connect(m_view, &SurveilClipWindow::cancelButtonClicked, this, &SurveilClipController::onCancelButtonClicked);
	connect(m_view, &SurveilClipWindow::watchResultButtonClicked, this, &SurveilClipController::onWatchResultButtonClicked);
	connect(m_view, &SurveilClipWindow::saveButtonClicked, this, &SurveilClipController::onSaveButtonClicked);
	connect(m_view, &SurveilClipWindow::parametersButtonClicked, this, &SurveilClipController::onParametersButtonClicked);

	connect(m_model, &SurveilClipModel::errorThrowed, this, &SurveilClipController::onMediaErrorThrowed);
	connect(m_model, &SurveilClipModel::sourceChanged, this, &SurveilClipController::onMediaSourceChanged);
	connect(m_model, &SurveilClipModel::processStatusChanged, this, &SurveilClipController::onMediaProcessStatusChanged);
	connect(m_model, &SurveilClipModel::progressUpdated, m_view, &SurveilClipWindow::updateProgressBar);
	connect(m_model, &SurveilClipModel::processDurationChanged, m_view, &SurveilClipWindow::onProcessDurationChanged);

	connect(videoWidget, &SurveilClipVideoWidget::closing, this, &SurveilClipController::onVideoWidgetClosing);

	m_model->setVideoOutput(videoWidget);
}

SurveilClipController::~SurveilClipController()
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
	if (m_parametersWindow)
	{
		delete m_parametersWindow;
		m_parametersWindow = nullptr;
	}
}

#pragma region private methods

#pragma endregion

#pragma region _slots

void SurveilClipController::onViewClosing()
{
	if (m_parametersWindow)
	{
		m_parametersWindow->close(); // DebugController takes care of deleting the DebugWindow
		m_parametersWindow = nullptr;
	}
	if (m_model->isWorking())
	{
		m_model->cancel();
		m_isViewClosing = true;
	}
	else
	{
		deleteLater();
	}

}

void SurveilClipController::onVideoWidgetClosing()
{
	m_model->stop();
}

void SurveilClipController::onParametersWindowClosing()
{
	m_parametersWindow = nullptr;
}

void SurveilClipController::onSelectButtonClicked()
{
	QString filePath = m_view->selectVideoFile();
	if (!filePath.isEmpty())
	{
		m_model->setSource(filePath);
	}
}

void SurveilClipController::onWatchButtonClicked()
{
	SurveilClipVideoWidget* videoWidget = m_view->getVideoWidget();
	videoWidget->setWindowTitle(m_model->getVideoData()->name);
	videoWidget->show();
	m_model->play();
}

void SurveilClipController::onParametersButtonClicked()
{
	IVideoProcessor* videoProcessor = m_model->getVideoProcessor();
	VideoProcessorDebugable* debugable = dynamic_cast<VideoProcessorDebugable*>(videoProcessor);

	if (debugable)
	{
		if (m_parametersWindow)
		{
			m_parametersWindow->activateWindow();
		}
		else
		{
			m_parametersWindow = ViewFactory::createParametersWindow(debugable, m_model);
			connect(m_parametersWindow, &ParametersWindow::closing, this, &SurveilClipController::onParametersWindowClosing);
			m_parametersWindow->show();
		}
	}
	else
	{
		m_view->messageBoxError("Video processor not debugable", "The current video processor is not debugable.");
	}
}

void SurveilClipController::onGenerateButtonClicked()
{
	m_model->generate();
}

void SurveilClipController::onCancelButtonClicked()
{
	m_model->cancel();
}

void SurveilClipController::onWatchResultButtonClicked()
{
	const void* result = m_model->getResult();
	if (result)
	{

	}
	else
	{
		m_view->messageBoxError("No result", "Result is not available.");
	}
}

void SurveilClipController::onSaveButtonClicked()
{
	const void* result = m_model->getResult();
	if (result)
	{

	}
	else
	{
		m_view->messageBoxError("No result", "Result is not available.");
	}
}

void SurveilClipController::onMediaErrorThrowed(const QString& error, const QString& message)
{
	m_view->messageBoxError(error, message);
}

void SurveilClipController::onMediaSourceChanged()
{
	auto data = m_model->getVideoData();
	if (data)
	{
		m_model->getVideoProcessor()->setDimensions(data->width, data->height);
	}
	m_view->updateVideoData(data);
}

void SurveilClipController::onMediaProcessStatusChanged(IVideoProcessor::Status status)
{
	switch (status)
	{
	case IVideoProcessor::Status::Working:
		m_view->updateUiProcessWorking();
		break;
	case IVideoProcessor::Status::None:
	case IVideoProcessor::Status::Error:
	case IVideoProcessor::Status::Canceled:
		m_view->updateUiProcessNotWorking(false);
		break;
	case IVideoProcessor::Status::Finished:
		m_view->updateUiProcessNotWorking(true);
		break;
	default:
		break;
	}
	if (m_isViewClosing)
	{
		deleteLater();
	}
}

#pragma endregion
