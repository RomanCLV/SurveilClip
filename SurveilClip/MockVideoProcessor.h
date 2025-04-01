#pragma once

#include "VideoProcessorBase.h"

#include <QObject>
#include <QTimer>

class MockVideoProcessor : public VideoProcessorBase
{
    Q_OBJECT

public:
    explicit MockVideoProcessor(QObject* parent = nullptr);

    void setDimensions(int width, int height) override;
    void processVideo(const QString& filePath) override;

private slots:
    void updateProgress();

private:
    QTimer m_timer;
};
