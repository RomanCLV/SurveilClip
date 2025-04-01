#include "NumericalParameterWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

NumericalParameterWidget::NumericalParameterWidget(QWidget* parent) :
	QWidget(parent),
	m_minValue(0),
	m_maxValue(100)
{
	m_label = new QLabel("Numerical Parameter", this);
	m_slider = new QSlider(Qt::Horizontal, this);
	m_spinBox = new QSpinBox(this);

	m_slider->setRange(m_minValue, m_maxValue);
	m_spinBox->setRange(m_minValue, m_maxValue);

	QHBoxLayout* layoutControls = new QHBoxLayout;
	layoutControls->addWidget(m_slider);
	layoutControls->addWidget(m_spinBox);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(m_label);
	mainLayout->addLayout(layoutControls);

	connect(m_slider, &QSlider::valueChanged, m_spinBox, &QSpinBox::setValue);
	connect(m_spinBox, QOverload<int>::of(&QSpinBox::valueChanged), m_slider, &QSlider::setValue);
	connect(m_slider, &QSlider::valueChanged, this, &NumericalParameterWidget::valueChanged);
}

QString NumericalParameterWidget::name() const { return m_label->text(); }
void NumericalParameterWidget::setName(const QString& newName)
{
	if (m_label->text() != newName)
	{
		m_label->setText(newName);
		emit nameChanged(newName);
	}
}

int NumericalParameterWidget::minimum() const { return m_minValue; }
void NumericalParameterWidget::setMinimum(int min)
{
	if (m_minValue != min)
	{
		m_minValue = min;
		m_slider->setMinimum(min);
		m_spinBox->setMinimum(min);
		emit minimumChanged(min);
	}
}

int NumericalParameterWidget::maximum() const { return m_maxValue; }
void NumericalParameterWidget::setMaximum(int max)
{
	if (m_maxValue != max)
	{
		m_maxValue = max;
		m_slider->setMaximum(max);
		m_spinBox->setMaximum(max);
		emit maximumChanged(max);
	}
}

int NumericalParameterWidget::value() const { return m_slider->value(); }
void NumericalParameterWidget::setValue(int val) { m_slider->setValue(val); }
