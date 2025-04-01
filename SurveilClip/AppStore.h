#pragma once
#include "VideoProcessorBase.h"

class AppStore
{
public:
    static void setVideoProcessor(VideoProcessorBase* processor);
    static VideoProcessorBase* getVideoProcessor();

private:
    static VideoProcessorBase* s_videoProcessor;
};
