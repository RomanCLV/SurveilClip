#pragma once

#include <QWidget>
#include <QCheckBox>

class BoolParameterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BoolParameterWidget(QWidget* parent = nullptr);
    ~BoolParameterWidget() = default;

    QString name() const;
    void setName(const QString& newName);

    bool getValue() const;

public slots:
    void setValue(bool value);

signals:
    void valueChanged(bool value);

private:
    QCheckBox* m_checkBox;
};
