#include "BoolParameterWidget.h"
#include <QVBoxLayout>

BoolParameterWidget::BoolParameterWidget(QWidget* parent) :
    QWidget(parent),
    m_checkBox(new QCheckBox("Bool Parameter", this))
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_checkBox);

    connect(m_checkBox, &QCheckBox::toggled, this, &BoolParameterWidget::valueChanged);
}

QString BoolParameterWidget::name() const
{
    return m_checkBox->text();
}

void BoolParameterWidget::setName(const QString& newName)
{
    m_checkBox->setText(newName);
}

void BoolParameterWidget::setValue(bool value)
{
    if (m_checkBox->isChecked() != value)
    {
        m_checkBox->setChecked(value);
    }
}

bool BoolParameterWidget::getValue() const
{
    return m_checkBox->isChecked();
}
