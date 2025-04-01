#pragma once
#include "ParameterBase.h"
#include <QObject>

class NumericalParameter : public ParameterBase
{
	Q_OBJECT

public:
	explicit NumericalParameter(const QString& name, double value=50., QObject* parent = nullptr);
	~NumericalParameter() = default;

	double getValue() const;
	double getMinimum() const;
	double getMaximum() const;
	int	   getIncrement() const;

public slots:
	void setValue(double value);
	void setMinimum(double minimum);
	void setMaximum(double maximum);
	void setIncrement(int increment);

private:
	void internalSetValue(double value);

signals:
	void valueChanged(double value);
	void minimumChanged(double minimum);
	void maximumChanged(double maximum);
	void incrementChanged(double increment);

private:
	double m_value;
	double m_minimum;
	double m_maximum;
	int    m_increment;
};
