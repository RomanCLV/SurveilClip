#pragma once
#include <QtPlugin>
#include <QList>
#include <QImage>
#include <QObject>

class IVideoProcessorDebugable
{
public:
    enum class ImageType {
        Reference,
        Current,
        Difference,
        Result
    };

    enum class PauseType {
        BarSpace,
        FramePerFrame
    };

public:
    virtual ~IVideoProcessorDebugable() = default;

	virtual PauseType getPauseType() const = 0;
    virtual int getIncrementFrameCount() const = 0;

    virtual void addListener(const QObject* listener) = 0;
    virtual void removeListener(const QObject* listener) = 0;
};

#define IVideoProcessorDebugable_iid "com.surveilclip.IVideoProcessorDebugable/1.0"
Q_DECLARE_INTERFACE(IVideoProcessorDebugable, IVideoProcessorDebugable_iid)
