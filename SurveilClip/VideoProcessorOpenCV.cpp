#include "VideoProcessorOpenCV.h"
#include "Tools.h"
#include "FrameDebug.h"
#include "OpenCVFrameDebug.h"

#include <QtConcurrent>
#include <QFile>
#include <QDateTime>

#include <exception>
#include <typeinfo>

QString VideoProcessorOpenCV::s_tmpFilename;

VideoProcessorOpenCV::VideoProcessorOpenCV(QObject* parent) :
	VideoProcessorDebugable(parent),
	m_groups(),
	m_parameters(new ParameterGroup("Parameters", this)),
	m_resultParameters(new ParameterGroup("Result frame", this)),
	m_threshold(new NumericalParameter("Threshold", 0., this)),
	m_minWidth(new NumericalParameter("Minimum Width", 50., this)),
	m_minHeight(new NumericalParameter("Minimum Height", 50., this)),
	m_overlayTimecode(new ListParameter<IVideoProcessorConfigurable::OverlayTimecode>("Overlay timecode", this)),
	m_showRectanglesDetection(new BoolParameter("Show detection rectangles", false, this)),
	m_continueRequested(false),
	m_pauseRequested(false),
	m_inPause(false),
	m_pv()
{
	m_threshold->setMaximum(255.);
	m_threshold->setValue(127.);

	m_parameters->addParameter(m_threshold);
	m_parameters->addParameter(m_minWidth);
	m_parameters->addParameter(m_minHeight);

	m_overlayTimecode->addItem("No", IVideoProcessorConfigurable::OverlayTimecode::None);
	m_overlayTimecode->addItem("Top left", IVideoProcessorConfigurable::OverlayTimecode::TopLeft);
	m_overlayTimecode->addItem("Top right", IVideoProcessorConfigurable::OverlayTimecode::TopRight);
	m_overlayTimecode->addItem("Bottom left", IVideoProcessorConfigurable::OverlayTimecode::BottomLeft);
	m_overlayTimecode->addItem("Bottom right", IVideoProcessorConfigurable::OverlayTimecode::BottomRight);

	m_resultParameters->addParameter(m_showRectanglesDetection);
	m_resultParameters->addParameter(m_overlayTimecode);

	m_groups.append(m_parameters);
	m_groups.append(m_resultParameters);
}

VideoProcessorOpenCV::~VideoProcessorOpenCV()
{
	m_groups.clear();
	deleteTempFile();
}

void VideoProcessorOpenCV::processVideo(const QString& filePath)
{
	(void)QtConcurrent::run([this, filePath]() {
		internalProcess(filePath);
		});
}

QList<const ParameterGroup*> VideoProcessorOpenCV::getGroups() const
{
	QList<const ParameterGroup*> groups(m_groups.size());
	int i = 0;
	for (const auto& group : m_groups)
	{
		groups[i++] = (group);
	}
	return groups;
}

QList<const ParameterBase*> VideoProcessorOpenCV::getParameters() const
{
	QList<const ParameterBase*> parameters;
	for (const auto& group : m_groups)
	{
		for (const auto& parameter : group->getParameters())
		{
			parameters.append(parameter);
		}
	}
	return parameters;
}

bool VideoProcessorOpenCV::saveParameters() const
{
	return false;
}

void VideoProcessorOpenCV::resetParameters()
{
	m_threshold->setValue(127.);
	m_minWidth->setValue(0.);
	m_minHeight->setValue(0.);
	m_showRectanglesDetection->setValue(false);
	m_overlayTimecode->selectValue(OverlayTimecode::None);
}

void VideoProcessorOpenCV::setDimensions(int width, int height)
{
	m_minHeight->setMaximum(height);
	m_minWidth->setMaximum(width);
}

void VideoProcessorOpenCV::internalProcess(const QString& filePath)
{
	cv::VideoCapture cap(filePath.toStdString(), cv::CAP_FFMPEG);
	if (cap.isOpened())
	{
		m_parameters->setIsLocked(true);
		m_resultParameters->setIsLocked(true);
		try
		{
			detectActivity(cap);
		}
		catch (const std::exception& e)
		{
			setStatus(Status::Error);
			emit processingError(typeid(e).name(), e.what());
		}
		if (m_pv.videoWritter.isOpened())
		{
			m_pv.videoWritter.release();
		}

		m_parameters->setIsLocked(false);
		m_resultParameters->setIsLocked(false);
	}
	else
	{
		setStatus(Status::Error);
		emit processingError("Unknown error", "Failed to open video file.");
	}
}

void VideoProcessorOpenCV::pauseProcessingRequested()
{
	m_pauseRequested = true;
}

void VideoProcessorOpenCV::continueProcessingRequested()
{
	m_continueRequested = true;
}

void VideoProcessorOpenCV::detectActivity(cv::VideoCapture& cap)
{
	deleteTempFile();
	setStatus(Status::Working);
	resetProgress();

	int fps = cap.get(cv::CAP_PROP_FPS);
	int totalFrames = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
	int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
	int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));

	bool mustResize;
	cv::Size newSize = getNewSize(width, height, mustResize);

	s_tmpFilename = Tools::generateRandomString() + ".avi";
	m_pv.videoWritter = cv::VideoWriter(s_tmpFilename.toStdString(), cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, newSize);

	m_pv.activityDetected = false;
	m_pv.activityDetectedInFrame = false;
	m_pv.firstFrame = true;
	m_pv.referenceFrameIndex = 0;
	m_pv.currentFrameIndex = 0;
	m_pv.boundaries = QList<cv::Rect>();
	m_pv.contours = std::vector<std::vector<cv::Point>>();
	m_pv.hierarchy = std::vector<cv::Vec4i>();
	m_pv.frameDuration = 0;
	m_pv.lastFrameDuration = 0;

	m_pv.redColor = cv::Scalar(0, 0, 255);
	m_pv.minHeigth = mustResize ? m_minHeight->getValue() * ((float)newSize.height / (float)height) : m_minHeight->getValue();
	m_pv.minWidth = mustResize ? m_minWidth->getValue() * ((float)newSize.width / (float)width) : m_minWidth->getValue();

	int processedFrames = 0;
	int newProgress;

	Status resultStatus = Status::Finished;

	std::chrono::steady_clock::time_point processStart = std::chrono::high_resolution_clock::now();
	std::chrono::steady_clock::time_point processEnd;
	int processDurationSeconds = 0;
	int newProcessDurationSeconds;

	while (cap.read(m_pv.originalFrame))
	{
		m_pv.frameStart = std::chrono::high_resolution_clock::now();
		if (getCancelRequest())
		{
			resultStatus = Status::Canceled;
			deleteTempFile();
			consumeCancelRequest();
			break;
		}
		m_pv.currentFrameIndex++;

		// resize if needed
		if (mustResize)
		{
			cv::resize(m_pv.originalFrame, m_pv.rgbFrame, newSize, 0, 0, cv::INTER_LINEAR);
		}
		else
		{
			m_pv.originalFrame.copyTo(m_pv.rgbFrame);
		}

		// BGR -> gray
		cv::cvtColor(m_pv.rgbFrame, m_pv.grayFrame, cv::COLOR_BGR2GRAY);

		if (hasListeners())
		{
			processFrameForListeners();
		}
		else
		{
			processFrameOptimized();
		}

		// update progress 
		processedFrames++;
		newProgress = static_cast<int>((processedFrames * 100.0) / totalFrames);
		if (newProgress > getProgress())
		{
			incrementProgress(newProgress - getProgress());
		}

		// update process duration
		newProcessDurationSeconds = (int)(std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - processStart).count() / 1000.);
		if (newProcessDurationSeconds != processDurationSeconds)
		{
			processDurationSeconds = newProcessDurationSeconds;
			emit processDurationChanged(processDurationSeconds);
		}
	}
	m_pv.videoWritter.release();

	emit setStatus(resultStatus);
}

cv::Size VideoProcessorOpenCV::getNewSize(int width, int height, bool& mustResize)
{
	float widthF = float(width);
	float dwFactor = 480.f / widthF;
	int newWidth = (int)(widthF * dwFactor);
	mustResize = dwFactor < 1.f;
	return mustResize ? cv::Size(newWidth, (int)(float(height) * dwFactor)) : cv::Size(width, height);
}

void VideoProcessorOpenCV::processFrameOptimized()
{

}

void VideoProcessorOpenCV::processFrameForListeners()
{
	if (m_pv.firstFrame)
	{
		m_pv.firstFrame = false;

		m_pv.rgbFrame.copyTo(m_pv.referenceRgbFrame);
		m_pv.grayFrame.copyTo(m_pv.referenceGrayFrame);

		m_pv.referenceFrameIndex = m_pv.currentFrameIndex;
		m_pv.frameDuration = (int)std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - m_pv.frameStart).count();

		m_pv.framesDebug.append(new OpenCVFrameDebug(m_pv.referenceRgbFrame, ImageType::Reference, m_pv.referenceFrameIndex));
	}
	else
	{
		// apply diff
		cv::absdiff(m_pv.referenceGrayFrame, m_pv.grayFrame, m_pv.differenceFrame);

		cv::threshold(m_pv.differenceFrame, m_pv.differenceFrame, m_threshold->getValue(), 255, cv::THRESH_BINARY);

		m_pv.contours.clear();
		m_pv.hierarchy.clear();
		m_pv.boundaries.clear();

		cv::findContours(m_pv.differenceFrame, m_pv.contours, m_pv.hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		for (const auto& contour : m_pv.contours)
		{
			cv::Rect rect = cv::boundingRect(contour);
			if (rect.width >= m_pv.minWidth && rect.height >= m_pv.minHeigth)
			{
				m_pv.boundaries.append(rect);
			}
		}

		m_pv.activityDetectedInFrame = m_pv.boundaries.size() > 0;

		if (m_pv.activityDetectedInFrame)
		{
			cv::cvtColor(m_pv.differenceFrame, m_pv.differenceRgbFrame, cv::COLOR_GRAY2BGR);
			for (const auto& rect : m_pv.boundaries)
			{
				cv::rectangle(m_pv.differenceRgbFrame, rect, m_pv.redColor, 2);
			}

			if (m_showRectanglesDetection->getValue())
			{
				m_pv.rgbFrame.copyTo(m_pv.rgbFrameFrameRectangles);
				for (const auto& rect : m_pv.boundaries)
				{
					cv::rectangle(m_pv.rgbFrameFrameRectangles, rect, m_pv.redColor, 2);
				}
				m_pv.videoWritter.write(m_pv.rgbFrameFrameRectangles);
			}
			else
			{
				m_pv.videoWritter.write(m_pv.rgbFrame); // utiliser la frame rgb du début
			}
		}
		else
		{
			m_pv.rgbFrame.copyTo(m_pv.referenceRgbFrame);   // update rgb reference frame with current rgb frame
			m_pv.grayFrame.copyTo(m_pv.referenceGrayFrame); // update gray reference frame with current gray frame
			m_pv.referenceFrameIndex = m_pv.currentFrameIndex;
			m_pv.framesDebug.append(new OpenCVFrameDebug(m_pv.referenceRgbFrame, ImageType::Reference, m_pv.referenceFrameIndex));
		}

		// capture du temps de process de la frame
		m_pv.frameDuration = (int)std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - m_pv.frameStart).count();

		// event activity detected changed
		if (m_pv.activityDetected != m_pv.activityDetectedInFrame)
		{
			m_pv.activityDetected = m_pv.activityDetectedInFrame;
			emit activityDetectedChanged(m_pv.activityDetected, m_pv.currentFrameIndex, m_pv.boundaries.size());
		}

		// event image processing changed
		m_pv.framesDebug.append(new OpenCVFrameDebug(m_pv.rgbFrame, ImageType::Current, m_pv.currentFrameIndex));
		if (m_showRectanglesDetection->getValue())
		{
			m_pv.framesDebug.append(new OpenCVFrameDebug(m_pv.differenceRgbFrame, ImageType::Difference, m_pv.currentFrameIndex));
			m_pv.framesDebug.append(new OpenCVFrameDebug(m_pv.rgbFrameFrameRectangles, ImageType::Result, m_pv.currentFrameIndex));
		}
		else
		{
			m_pv.framesDebug.append(new OpenCVFrameDebug(m_pv.differenceFrame, ImageType::Difference, m_pv.currentFrameIndex));
			m_pv.framesDebug.append(new OpenCVFrameDebug(m_pv.rgbFrame, ImageType::Result, m_pv.currentFrameIndex));
		}
	}

	emit imagesProcessingChanged(m_pv.framesDebug);
	m_pv.framesDebug.clear();

	// event processed frame time changed
	if (m_pv.lastFrameDuration != m_pv.frameDuration)
	{
		m_pv.lastFrameDuration = m_pv.frameDuration;
		emit processedFrameTimeChanged(m_pv.frameDuration);
	}
}

void VideoProcessorOpenCV::deleteTempFile()
{
	if (!s_tmpFilename.isEmpty() && QFile::exists(s_tmpFilename))
	{
		QFile::remove(s_tmpFilename);
	}
}
