#include "SurveilClipModel.h"

#include <QFile>
#include <QFileInfo>
#include <QMetaEnum>

SurveilClipModel::SurveilClipModel(VideoProcessorBase* processor, QObject* parent) :
    QObject(parent),
    m_videoProcessor(processor),
    m_player(new QMediaPlayer(this)),
    m_audioOutput(new QAudioOutput(this)),
    m_videoData(nullptr)
{
    m_audioOutput->setVolume(50.f);
    m_player->setAudioOutput(m_audioOutput);

    connect(m_player, &QMediaPlayer::errorOccurred, this, &SurveilClipModel::onVideoErrorThrowed);
    connect(m_player, &QMediaPlayer::metaDataChanged, this, &SurveilClipModel::loadVideoData);

    connect(m_videoProcessor, &VideoProcessorBase::statusChanged, this, &SurveilClipModel::processStatusChanged);
    connect(m_videoProcessor, &VideoProcessorBase::progressUpdated, this, &SurveilClipModel::progressUpdated);
    connect(m_videoProcessor, &VideoProcessorBase::processDurationChanged, this, &SurveilClipModel::processDurationChanged);
    connect(m_videoProcessor, &VideoProcessorBase::processingError, this, &SurveilClipModel::errorThrowed);
}

SurveilClipModel::~SurveilClipModel()
{
    if (m_audioOutput)
    {
        delete m_audioOutput;
        m_audioOutput = nullptr;
    }
    if (m_player) 
    {
        stop();
        delete m_player;
        m_player = nullptr;
    }

    destroyVideoData();
}

#pragma region public Getters & Setters

QMediaPlayer* SurveilClipModel::getMediaPlayer() const
{
    return m_player;
}

const VideoData* SurveilClipModel::getVideoData() const
{
    return m_videoData;
}

const void* SurveilClipModel::getResult() const
{
    return nullptr;
}

IVideoProcessor* SurveilClipModel::getVideoProcessor() const
{
    return m_videoProcessor;
}

#pragma endregion

#pragma region private methods

void SurveilClipModel::destroyVideoData()
{
    if (m_videoData)
    {
        delete m_videoData;
        m_videoData = nullptr;
    }
}

void SurveilClipModel::loadVideoData()
{
    QUrl source = m_player->source();
    QString filePath = source.toLocalFile();

    destroyVideoData();
    if (!filePath.isEmpty())
    {
        if (QFile::exists(filePath))
        {
            m_videoData = new VideoData();
            QMediaMetaData metaData = m_player->metaData();
            QFileInfo fileInfo(filePath);

            m_videoData->absolutePath = fileInfo.absolutePath();
            m_videoData->name = fileInfo.fileName();
            m_videoData->extension = fileInfo.suffix();
            m_videoData->sizeByte = fileInfo.size();

            m_videoData->fileFormat = metaData.value(QMediaMetaData::FileFormat).toString();

            m_videoData->fps = metaData.value(QMediaMetaData::VideoFrameRate).toDouble();
            m_videoData->durationMs = metaData.value(QMediaMetaData::Duration).toLongLong();
            m_videoData->width = metaData.value(QMediaMetaData::Resolution).toSize().width();
            m_videoData->height = metaData.value(QMediaMetaData::Resolution).toSize().height();

            m_videoData->videoCodec = metaData.value(QMediaMetaData::VideoCodec).toString();
            m_videoData->audioCodec = metaData.value(QMediaMetaData::AudioCodec).toString();
        }
        else
        {
            emit errorThrowed("File not found", "The file " + filePath + " does not exist.");
        }
    }
    emit sourceChanged();
}

#pragma endregion

#pragma region public methods

void SurveilClipModel::setVideoOutput(QVideoWidget* videoWidget) const
{
    if (m_player)
    {
        m_player->setVideoOutput(videoWidget);
    }
}

void SurveilClipModel::setSource(const QString& filePath)
{
    if (QFile::exists(filePath))
    {
        m_player->setSource(QUrl::fromLocalFile(filePath));
    }
    else
    {
        emit errorThrowed("File not found", "The file: " + filePath + " does not exist.");
    }
}

void SurveilClipModel::play() const
{
    if (m_player && !m_player->isPlaying())
    {
        m_player->play();
    }
}

void SurveilClipModel::stop() const
{
    if (m_player && m_player->isPlaying())
    {
        m_player->stop();
    }
}

void SurveilClipModel::generate()
{
    if (m_videoProcessor)
    {
        m_videoProcessor->processVideo(m_player->source().toLocalFile());
    }
    else
    {
        emit errorThrowed("Video processor not selected", "The process cannot start because there is no video processor selected.");
    }
}

void SurveilClipModel::cancel()
{
    m_videoProcessor->cancel();
}

bool SurveilClipModel::isWorking() const
{
    return m_videoProcessor->getStatus() == IVideoProcessor::Status::Working;
}

#pragma endregion

void SurveilClipModel::onVideoErrorThrowed(QMediaPlayer::Error error, const QString& errorString)
{
    emit errorThrowed(QString::fromUtf8(QMetaEnum::fromType<QMediaPlayer::Error>().valueToKey(error)), errorString);
}
