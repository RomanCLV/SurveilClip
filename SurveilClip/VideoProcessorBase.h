#pragma once
#include "IVideoProcessor.h"
#include <QtPlugin>
#include <QObject>

class VideoProcessorBase : public QObject, public IVideoProcessor
{
    Q_OBJECT
    Q_INTERFACES(IVideoProcessor)

public:
    explicit VideoProcessorBase(QObject* parent = nullptr);
    ~VideoProcessorBase();

public:
    Status getStatus() const override;
    void cancel() override;

protected:
    void setStatus(Status status);
    
    bool getCancelRequest() const;
    void consumeCancelRequest();

    int getProgress() const;
    void resetProgress();
    void incrementProgress(int inc = 1);

private:
    Status m_status;
    int m_progress;
    bool m_cancelRequest;

signals:
    void statusChanged(Status status);
    void processingError(const QString& error, const QString& message);
    void progressUpdated(int progress);
    void processDurationChanged(int seconds);
};
