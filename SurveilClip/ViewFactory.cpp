#include "ViewFactory.h"
#include "AppStore.h"

#include "SurveilClipController.h"
#include "SurveilClipModel.h"

#include "ParametersController.h"
#include "ParametersModel.h"

SurveilClipWindow* ViewFactory::createSurveilClipWindow()
{
	VideoProcessorBase* videoProcessor = AppStore::getVideoProcessor();

	auto* model = new SurveilClipModel(videoProcessor);
	auto* view = new SurveilClipWindow();
	auto* controller = new SurveilClipController(view, model);
	return view;
}

ParametersWindow* ViewFactory::createParametersWindow(VideoProcessorDebugable* videoProcessorDebugable, SurveilClipModel* surveilClipModel)
{
	auto* model = new ParametersModel(videoProcessorDebugable);
	auto* view = new ParametersWindow();
	auto* controller = new ParametersController(view, model, surveilClipModel);
	return view;
}
