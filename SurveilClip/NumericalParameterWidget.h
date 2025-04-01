#pragma once

#include <QtWidgets/QWidget>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QObject>

class NumericalParameterWidget : public QWidget
{
    Q_OBJECT
    //Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    //Q_PROPERTY(int minimum READ minimum WRITE setMinimum NOTIFY minimumChanged)
    //Q_PROPERTY(int maximum READ maximum WRITE setMaximum NOTIFY maximumChanged)
    //Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)

public:
    NumericalParameterWidget(QWidget *parent = nullptr);

    QString name() const;
    void setName(const QString& newName);

    int value() const;
    int minimum() const;
    int maximum() const;

public slots:
    void setValue(int val);
    void setMaximum(int max);
    void setMinimum(int min);

signals:
    void nameChanged(const QString& name);
    void minimumChanged(int minimum);
    void maximumChanged(int maximum);
    void valueChanged(int newValue);

private:
    QLabel* m_label;
    QSlider* m_slider;
    QSpinBox* m_spinBox;
    int m_minValue;
    int m_maxValue;
};
