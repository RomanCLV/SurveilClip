#include "VideoProcessorOpenCV.h"
#include "MockVideoProcessor.h"
#include "ViewFactory.h"
#include "AppStore.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    VideoProcessorOpenCV videoProcessor;
    //MockVideoProcessor videoProcessor;
    AppStore::setVideoProcessor(&videoProcessor);

    SurveilClipWindow* view = ViewFactory::createSurveilClipWindow();
    view->show();

    return a.exec();
}
