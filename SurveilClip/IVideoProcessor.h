#pragma once
#include <QString>
#include <QtPlugin>

class IVideoProcessor
{
public:
    enum class Status {
        None,
        Working,
        Error,
        Canceled,
        Finished
    };

public:
    virtual ~IVideoProcessor() = default;

    virtual void setDimensions(int width, int height) = 0;
    virtual void processVideo(const QString& filePath) = 0;
    virtual void cancel() = 0;
    virtual Status getStatus() const = 0;
};

#define IVideoProcessor_iid "com.surveilclip.IVideoProcessor/1.0"
Q_DECLARE_INTERFACE(IVideoProcessor, IVideoProcessor_iid)
