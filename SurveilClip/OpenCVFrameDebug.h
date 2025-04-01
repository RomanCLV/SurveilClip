#pragma once
#include "FrameDebug.h"
#include "opencv2/opencv.hpp"

class OpenCVFrameDebug : public FrameDebug
{
public:
    OpenCVFrameDebug(const cv::Mat& i, IVideoProcessorDebugable::ImageType t, int f);

    const cv::Mat image;
};
