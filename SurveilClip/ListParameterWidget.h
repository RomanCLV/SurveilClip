#pragma once
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>

class ListParameterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ListParameterWidget(QWidget* parent = nullptr);

    ~ListParameterWidget() = default;

    void setName(const QString& name);
    void setOptions(const QStringList& keys);
    void setCurrentIndex(int index);

signals:
    void indexChanged(int index);

private:
    QLabel* m_label;
    QComboBox* m_comboBox;
};