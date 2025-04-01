#include "BoolParameter.h"

BoolParameter::BoolParameter(const QString& name, bool value, QObject* parent) :
	ParameterBase(name, parent),
	m_value(value)
{
}

bool BoolParameter::getValue() const
{
	return m_value;
}

void BoolParameter::setValue(bool value)
{
	if (!getIsLocked() && m_value != value)
	{
		m_value = value;
		emit valueChanged(m_value);
	}
}

