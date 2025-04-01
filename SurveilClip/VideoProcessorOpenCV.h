#pragma once

#include "VideoProcessorDebugable.h"
#include "IVideoProcessorConfigurable.h"

#include "ParameterGroup.h"
#include "NumericalParameter.h"
#include "BoolParameter.h"
#include "ListParameter.h"

#include <QObject>
#include <QImage>
#include <QList>

#include <chrono>
#include <opencv2/opencv.hpp>
//#include <opencv2/core.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui.hpp>

class VideoProcessorOpenCV : public VideoProcessorDebugable, public IVideoProcessorConfigurable
{
    Q_OBJECT
    Q_INTERFACES(IVideoProcessorConfigurable)

public:
    explicit VideoProcessorOpenCV(QObject* parent = nullptr);
    ~VideoProcessorOpenCV();

    void processVideo(const QString& filePath) override;

    QList<const ParameterGroup*> getGroups() const override;
    QList<const ParameterBase*> getParameters() const override;
    bool saveParameters() const override;
    void resetParameters()  override;

    void setDimensions(int width, int height) override;

public slots:
    void pauseProcessingRequested() override;
    void continueProcessingRequested() override;

private:
    typedef struct {
        bool activityDetected;
        bool activityDetectedInFrame;
        bool firstFrame;
        int referenceFrameIndex;
        int currentFrameIndex;
        QList<cv::Rect> boundaries;
        int minHeigth;
        int minWidth;

        std::chrono::steady_clock::time_point time1;
        std::chrono::steady_clock::time_point time2;
        double deltaTime;
        std::chrono::steady_clock::time_point frameStart;
        std::chrono::steady_clock::time_point frameEnd;
        int frameDuration;
        int lastFrameDuration;

        cv::Mat originalFrame;
        cv::Mat rgbFrame;
        cv::Mat grayFrame;
        cv::Mat referenceRgbFrame;
        cv::Mat referenceGrayFrame;
        cv::Mat differenceFrame;
        cv::Mat differenceRgbFrame;
        cv::Mat rgbFrameFrameRectangles;

        cv::VideoWriter videoWritter;

        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::Scalar redColor;
        QList<FrameDebug*> framesDebug;
    } ProcessVariables;

    void internalProcess(const QString& filePath);
    void detectActivity(cv::VideoCapture& cap);

    cv::Size getNewSize(int width, int height, bool& mustResize);
    void processFrameOptimized();
    void processFrameForListeners();

    static void deleteTempFile();

private:
	QList<ParameterGroup*> m_groups;
    ParameterGroup* m_parameters;
    ParameterGroup* m_resultParameters;
    NumericalParameter* m_threshold;
    NumericalParameter* m_minWidth;
    NumericalParameter* m_minHeight;
    BoolParameter* m_showRectanglesDetection;
    ListParameter<IVideoProcessorConfigurable::OverlayTimecode>* m_overlayTimecode;

    bool m_pauseRequested;
    bool m_continueRequested;
    bool m_inPause;

    ProcessVariables m_pv;

    static QString s_tmpFilename;
};
