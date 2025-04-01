#include "MockVideoProcessor.h"

MockVideoProcessor::MockVideoProcessor(QObject* parent) : 
    VideoProcessorBase(parent),
    m_timer(QTimer())
{
    connect(&m_timer, &QTimer::timeout, this, &MockVideoProcessor::updateProgress);
}

void MockVideoProcessor::setDimensions(int width, int height)
{
}

void MockVideoProcessor::processVideo(const QString& filePath)
{
    setStatus(Status::Working);

    resetProgress();

    m_timer.start(30); // 30 ms x 100 = 3 secondes
}

void MockVideoProcessor::updateProgress()
{
    if (getCancelRequest())
    {
        m_timer.stop();
        setStatus(Status::Canceled);
        consumeCancelRequest();
    }
    else
    {
        incrementProgress();
        if (getProgress() > 100)
        {
            m_timer.stop();
            setStatus(Status::Finished);
        }
    }
}