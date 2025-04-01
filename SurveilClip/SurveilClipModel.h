#pragma once

#include "VideoProcessorBase.h"

#include <QObject>
#include <QMediaPlayer>
#include <QMediaMetaData>
#include <QAudioOutput>
#include <QVideoWidget>
#include <QString>

class VideoData
{
public:
    QString absolutePath;
    QString name;
    QString extension;
    QString fileFormat;

    qint64  sizeByte;
    quint64 durationMs;
    double  fps;
    int     width;
    int     height;

    QString videoCodec;
    QString audioCodec;
};

class SurveilClipModel : public QObject
{
    Q_OBJECT

public:
    SurveilClipModel(VideoProcessorBase* processor, QObject* parent = nullptr);
    ~SurveilClipModel();

    // public Getters & Setters
public:
    QMediaPlayer* getMediaPlayer() const;
    const VideoData* getVideoData() const;
    const void* getResult() const;
    IVideoProcessor* getVideoProcessor() const;

    // private methods
private:
    void destroyVideoData();

    // public methods
public:
    void setVideoOutput(QVideoWidget* videoWidget) const;
    void setSource(const QString& filePath);
    void play() const;
    void stop() const;
    void generate();
    void cancel();

    bool isWorking() const;

private slots:
    void onVideoErrorThrowed(QMediaPlayer::Error error, const QString& errorString);
    void loadVideoData();

    // signals
signals:
    void sourceChanged();
    void processStatusChanged(IVideoProcessor::Status status);
    void progressUpdated(int progress);
    void processDurationChanged(int seconds);
    void errorThrowed(const QString& error, const QString& message);

private:
    VideoProcessorBase* m_videoProcessor;
    QMediaPlayer* m_player;
    QAudioOutput* m_audioOutput;

    VideoData* m_videoData;
};
