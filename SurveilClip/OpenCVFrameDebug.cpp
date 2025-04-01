#include "OpenCVFrameDebug.h"

OpenCVFrameDebug::OpenCVFrameDebug(const cv::Mat& i, IVideoProcessorDebugable::ImageType t, int f) :
	FrameDebug(t, f),
	image(i.clone())
{
}
