#pragma once

#include "FrameDebug.h"
#include "opencv2/opencv.hpp"

#include <QObject>
#include <QString>
#include <QPixmap>
#include <QImage>
#include <QSize>

class Tools : public QObject
{
	Q_OBJECT

public:

	static QString formatTimeMs(qint64 milliseconds);
	static QString formatTime(int seconds);
	static QString generateRandomString(int length = 16);
	static QImage matToQImage(const cv::Mat& mat);
	static QImage matToQImageResized(const cv::Mat& mat, const QSize& targetSize);
	static QPixmap frameDebugToQPixmap(const FrameDebug* frameDebug, const QSize& targetSize);
};
