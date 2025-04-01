#pragma once
#include "IVideoProcessorDebugable.h"
#include <QObject>

class FrameDebug : public QObject
{
    Q_OBJECT

public:
    FrameDebug(IVideoProcessorDebugable::ImageType t, int f);
    virtual ~FrameDebug() = default;

    IVideoProcessorDebugable::ImageType type;
    int frame;
};
