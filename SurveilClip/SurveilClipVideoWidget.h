#pragma once

#include <QVideoWidget>
#include <QCloseEvent>

class SurveilClipVideoWidget : public QVideoWidget
{
    Q_OBJECT

public:
    explicit SurveilClipVideoWidget(QWidget* parent = nullptr) : QVideoWidget(parent) {}

signals:
    void closing();

protected:
    void closeEvent(QCloseEvent* event) override
    {
        emit closing();
        QVideoWidget::closeEvent(event);
    }
};
