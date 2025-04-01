#pragma once
#include "SurveilClipWindow.h"
#include "ParametersWindow.h"
#include "VideoProcessorDebugable.h"

class ViewFactory
{
public:
	static SurveilClipWindow* createSurveilClipWindow();
	static ParametersWindow* createParametersWindow(VideoProcessorDebugable* videoProcessorDebugable, SurveilClipModel* surveilClipModel);
};
