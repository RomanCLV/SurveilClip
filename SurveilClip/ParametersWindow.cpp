#include "ParametersWindow.h"
#include "Tools.h"

#include <QLabel>
#include <QPixmap>

ParametersWindow::ParametersWindow(QWidget* parent) :
	QMainWindow(parent),
	m_ui(new Ui::ParametersWindowUI()),
	m_videoProcessorIsConfigurable(false)
{
	m_ui->setupUi(this);

	connect(m_ui->pauseButton, &QPushButton::clicked, this, &ParametersWindow::pauseButtonClicked);
	connect(m_ui->continueButton, &QPushButton::clicked, this, &ParametersWindow::continueButtonClicked);
	connect(m_ui->stopButton, &QPushButton::clicked, this, &ParametersWindow::stopButtonClicked);
	connect(m_ui->generateButton, &QPushButton::clicked, this, &ParametersWindow::generateButtonClicked);
	connect(m_ui->actionSave, &QAction::triggered, this, &ParametersWindow::saveClicked);
	connect(m_ui->actionReset, &QAction::triggered, this, &ParametersWindow::resetClicked);

	connect(m_ui->spaceBarRadioButton, &QRadioButton::clicked, this, &ParametersWindow::onSpaceBarRadioButtonClicked);
	connect(m_ui->framePerFrameRadioButton, &QRadioButton::clicked, this, &ParametersWindow::onFrameRadioButtonClicked);
	connect(m_ui->framePerFrameSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ParametersWindow::framePerFrameValueChanged);

	m_ui->actionReset->setEnabled(false);
	m_ui->actionSave->setEnabled(false);
}

ParametersWindow::~ParametersWindow()
{
	delete m_ui;
}

void ParametersWindow::closeEvent(QCloseEvent* event)
{
	event->accept();
	emit closing();
}

void ParametersWindow::onSpaceBarRadioButtonClicked()
{
	onPauseTypeChanged(IVideoProcessorDebugable::PauseType::BarSpace);
	emit pauseTypeChanged(IVideoProcessorDebugable::PauseType::BarSpace);
}

void ParametersWindow::onFrameRadioButtonClicked()
{
	onPauseTypeChanged(IVideoProcessorDebugable::PauseType::FramePerFrame);
	emit pauseTypeChanged(IVideoProcessorDebugable::PauseType::FramePerFrame);
}

void ParametersWindow::onProcessPaused()
{
	m_ui->pauseButton->setEnabled(false);
	m_ui->continueButton->setEnabled(true);
}

void ParametersWindow::onProcessResumed()
{
	m_ui->pauseButton->setEnabled(true);
	m_ui->continueButton->setEnabled(false);
}

void ParametersWindow::onPauseTypeChanged(IVideoProcessorDebugable::PauseType pauseType)
{
	switch (pauseType)
	{
	case IVideoProcessorDebugable::PauseType::BarSpace:
		m_ui->spaceBarRadioButton->setChecked(true);
		break;
	case IVideoProcessorDebugable::PauseType::FramePerFrame:
		m_ui->framePerFrameRadioButton->setChecked(true);
		break;
	}
	m_ui->framePerFrameSpinBox->setEnabled(pauseType == IVideoProcessorDebugable::PauseType::FramePerFrame);
}

void ParametersWindow::onIncrementFrameCountChanged(int incrementFrameCount)
{
	m_ui->framePerFrameSpinBox->setValue(incrementFrameCount);
}

void ParametersWindow::onActivityDetectedChanged(bool detected, int frame, int detections)
{
	m_ui->activityLabel->setText(detected ? "Activity detected!" : "No activity");
	m_ui->detectionCountLabel->setText(QString::number(detections));
}

void ParametersWindow::onImageProcessingChanged(const FrameDebug* frameDebug)
{
	switch (frameDebug->type)
	{
	case IVideoProcessorDebugable::ImageType::Reference:
		m_ui->referenceFrameLabel->setText(QString::number(frameDebug->frame));
		m_ui->referenceImageLabel->clear();
		m_ui->referenceImageLabel->setPixmap(Tools::frameDebugToQPixmap(frameDebug, m_ui->referenceImageLabel->size()));
		break;
	case IVideoProcessorDebugable::ImageType::Current:
		m_ui->currentFrameLabel->setText(QString::number(frameDebug->frame));
		m_ui->currentImageLabel->clear();
		m_ui->currentImageLabel->setPixmap(Tools::frameDebugToQPixmap(frameDebug, m_ui->currentImageLabel->size()));

		break;
	case IVideoProcessorDebugable::ImageType::Difference:
		m_ui->differenceFrameLabel->setText(QString::number(frameDebug->frame));
		m_ui->differenceImageLabel->clear();
		m_ui->differenceImageLabel->setPixmap(Tools::frameDebugToQPixmap(frameDebug, m_ui->differenceImageLabel->size()));
		break;
	case IVideoProcessorDebugable::ImageType::Result:
		m_ui->resultFrameLabel->setText(QString::number(frameDebug->frame));
		m_ui->resultImageLabel->clear();
		m_ui->resultImageLabel->setPixmap(Tools::frameDebugToQPixmap(frameDebug, m_ui->resultImageLabel->size()));
		break;
	}
	delete frameDebug;
}

void ParametersWindow::onImagesProcessingChanged(const QList<FrameDebug*>& frameDebugList)
{
	for (const auto& frameDebug : frameDebugList)
	{
		onImageProcessingChanged(frameDebug);
	}
}

void ParametersWindow::onProcessedFrameTimeChanged(int milliseconds)
{
	m_ui->timeLabel->setText(QString::number(milliseconds));
}

void ParametersWindow::onProcessDurationChanged(int seconds)
{
	m_ui->statusBar->showMessage("Process duration: " + Tools::formatTime(seconds));
}

void ParametersWindow::onSourceChanged(bool hasSource)
{
	m_ui->generateButton->setEnabled(hasSource);
}

void ParametersWindow::onVideoProcessorStatusChanged(IVideoProcessor::Status status)
{
	if (status == IVideoProcessor::Status::Working)
	{
		m_ui->processPauseGroupBox->setEnabled(true);
		m_ui->pauseButton->setEnabled(true);
		m_ui->continueButton->setEnabled(true);
		m_ui->stopButton->setEnabled(true);
		m_ui->generateButton->setEnabled(false);
	}
	else
	{
		m_ui->pauseButton->setEnabled(false);
		m_ui->continueButton->setEnabled(false);
		m_ui->stopButton->setEnabled(false);
		m_ui->generateButton->setEnabled(true);
		m_ui->statusBar->clearMessage();

		// reset frames ui
		m_ui->referenceFrameLabel->setText("0");
		m_ui->referenceImageLabel->clear();
		m_ui->currentFrameLabel->setText("0");
		m_ui->currentImageLabel->clear();
		m_ui->differenceFrameLabel->setText("0");
		m_ui->differenceImageLabel->clear();
		m_ui->resultFrameLabel->setText("0");
		m_ui->resultImageLabel->clear();
		onProcessedFrameTimeChanged(0);
		onActivityDetectedChanged(false, 0, 0);
	}
}

void ParametersWindow::defineVideoProcessor(const IVideoProcessorDebugable* debugable)
{
	const IVideoProcessorConfigurable* configurable = dynamic_cast<const IVideoProcessorConfigurable*>(debugable);
	m_videoProcessorIsConfigurable = configurable != nullptr;

	m_ui->actionReset->setEnabled(m_videoProcessorIsConfigurable);
	m_ui->actionSave->setEnabled(m_videoProcessorIsConfigurable);
	if (m_videoProcessorIsConfigurable)
	{
		setParameters(configurable->getGroups());
	}
}

void ParametersWindow::setParameters(const QList<const ParameterGroup*>& groups)
{
	int index = 0;
	for (const ParameterGroup* group : groups)
	{
		QGroupBox* groupBox = createParameterGroupWidget(group);

		for (const ParameterBase* parameter : group->getParameters())
		{
			groupBox->layout()->addWidget(createParameterWidget(parameter));
		}

		((QVBoxLayout*)(m_ui->lateralWidget->layout()))->insertWidget(index++, groupBox);
	}
}

QGroupBox* ParametersWindow::createParameterGroupWidget(const ParameterGroup* parameter)
{
	QGroupBox* groupBox = new QGroupBox(parameter->getName());
	QVBoxLayout* groupLayout = new QVBoxLayout();
	groupBox->setLayout(groupLayout);
	return groupBox;
}

QWidget* ParametersWindow::createParameterWidget(const ParameterBase* parameter)
{
	const NumericalParameter* numericalParameter = dynamic_cast<const NumericalParameter*>(parameter);
	if (numericalParameter)
	{
		return createNumericalParameterWidget(numericalParameter);
	}
	const BoolParameter* boolParameter = dynamic_cast<const BoolParameter*>(parameter);
	if (boolParameter)
	{
		return createBoolParameterWidget(boolParameter);
	}
	const ListParameterBase* listParameter = dynamic_cast<const ListParameterBase*>(parameter);
	if (listParameter)
	{
		return createListParameterWidget(listParameter);
	}
	return new QLabel(parameter->getName());
}

NumericalParameterWidget* ParametersWindow::createNumericalParameterWidget(const NumericalParameter* parameter)
{
	NumericalParameterWidget* widget = new NumericalParameterWidget();

	widget->setName(parameter->getName());
	widget->setMinimum(parameter->getMinimum());
	widget->setMaximum(parameter->getMaximum());
	widget->setValue(parameter->getValue());

	connect(widget, &NumericalParameterWidget::valueChanged, parameter, &NumericalParameter::setValue);
	connect(widget, &NumericalParameterWidget::minimumChanged, parameter, &NumericalParameter::setMinimum);
	connect(widget, &NumericalParameterWidget::maximumChanged, parameter, &NumericalParameter::setMaximum);

	connect(parameter, &ParameterBase::isLockedChanged, widget, &QWidget::setDisabled);
	connect(parameter, &NumericalParameter::valueChanged, widget, &NumericalParameterWidget::setValue);
	connect(parameter, &NumericalParameter::minimumChanged, widget, &NumericalParameterWidget::setMinimum);
	connect(parameter, &NumericalParameter::maximumChanged, widget, &NumericalParameterWidget::setMaximum);

	return widget;
}

BoolParameterWidget* ParametersWindow::createBoolParameterWidget(const BoolParameter* parameter)
{
	BoolParameterWidget* widget = new BoolParameterWidget();
	widget->setName(parameter->getName());
	widget->setValue(parameter->getValue());

	connect(widget, &BoolParameterWidget::valueChanged, parameter, &BoolParameter::setValue);

	connect(parameter, &ParameterBase::isLockedChanged, widget, &QWidget::setDisabled);
	connect(parameter, &BoolParameter::valueChanged, widget, &BoolParameterWidget::setValue);

	return widget;
}

ListParameterWidget* ParametersWindow::createListParameterWidget(const ListParameterBase* parameter)
{
	ListParameterWidget* widget = new ListParameterWidget();
	widget->setName(parameter->getName());
	widget->setOptions(parameter->getKeys());
	widget->setCurrentIndex(parameter->getSelectedIndex());

	connect(widget, &ListParameterWidget::indexChanged, parameter, &ListParameterBase::selectValueByIndex);

	connect(parameter, &ParameterBase::isLockedChanged, widget, &QWidget::setDisabled);
	connect(parameter, &ListParameterBase::selectedIndexChanged, widget, &ListParameterWidget::setCurrentIndex);

	return widget;
}
