#include "Tools.h"
#include "OpenCVFrameDebug.h"
#include <QRandomGenerator>

QString Tools::formatTimeMs(qint64 milliseconds)
{
    qint64 hours = milliseconds / (1000 * 60 * 60);
    milliseconds %= (1000 * 60 * 60);

    qint64 minutes = milliseconds / (1000 * 60);
    milliseconds %= (1000 * 60);

    qint64 seconds = milliseconds / 1000;
    milliseconds %= 1000;

    QString timeString;

    if (hours > 0)
    {
        timeString = QString("%1:%2:%3.%4")
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'))
            .arg(milliseconds, 3, 10, QChar('0'));
    }
    else
    {
        timeString = QString("%1:%2.%3")
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'))
            .arg(milliseconds, 3, 10, QChar('0'));
    }
    return timeString;
}

QString Tools::formatTime(int seconds)
{
    int hours = seconds / (60 * 60);
    seconds -= 60 * 60 * hours;

    int minutes = seconds / 60;
    seconds -= 60 * minutes;

    return QString("%1:%2:%3")
        .arg(hours, 2, 10, QChar('0'))
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));
}

QString Tools::generateRandomString(int length) 
{
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    QString randomString;

    for (int i = 0; i < length; ++i) {
        int index = QRandomGenerator::global()->bounded(chars.size());
        randomString.append(chars.at(index));
    }

    return randomString;
}

QImage Tools::matToQImage(const cv::Mat& mat)
{
    if (mat.empty())
    {
        return QImage();
    }

    switch (mat.type())
    {
    case CV_8UC3: // 8-bit unsigned, 3 channels (RGB)
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888).rgbSwapped();

    case CV_8UC1: // 8-bit unsigned, 1 channel (Grayscale)
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);

    case CV_8UC4: // 8-bit unsigned, 4 channels (RGBA)
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);

    default:
        return QImage(); // Unsupported format
    }
}

QImage Tools::matToQImageResized(const cv::Mat& mat, const QSize& targetSize)
{
    if (mat.empty()) 
        return QImage();

    QImage::Format format;
    cv::Mat convertedMat;

    switch (mat.type()) {
    case CV_8UC1:  // Grayscale
        format = QImage::Format_Grayscale8;
        convertedMat = mat; // Pas besoin de conversion
        break;

    case CV_8UC3:  // RGB
        format = QImage::Format_RGB888;
        cv::cvtColor(mat, convertedMat, cv::COLOR_BGR2RGB); // OpenCV utilise BGR par défaut
        break;

    case CV_8UC4:  // RGBA
        format = QImage::Format_ARGB32;
        convertedMat = mat; // Déjà compatible
        break;

    default:
        return QImage();
    }

    // Création d'un QImage sans copie de mémoire
    QImage qimg(convertedMat.data, convertedMat.cols, convertedMat.rows, convertedMat.step, format);

    // Redimensionne uniquement si nécessaire
    return qimg.size() != targetSize ? qimg.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation) : qimg;
}

QPixmap Tools::frameDebugToQPixmap(const FrameDebug* frameDebug, const QSize& targetSize)
{
    const OpenCVFrameDebug* opencvFrameDebug = dynamic_cast<const OpenCVFrameDebug*>(frameDebug);
    if (opencvFrameDebug)
    {
        return QPixmap::fromImage(Tools::matToQImageResized(opencvFrameDebug->image, targetSize));
    }

    return QPixmap();
}
