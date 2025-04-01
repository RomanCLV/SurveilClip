#include "AppStore.h"

VideoProcessorBase* AppStore::s_videoProcessor = nullptr;

void AppStore::setVideoProcessor(VideoProcessorBase* processor)
{
    s_videoProcessor = processor;
}

VideoProcessorBase* AppStore::getVideoProcessor()
{
    return s_videoProcessor;
}
