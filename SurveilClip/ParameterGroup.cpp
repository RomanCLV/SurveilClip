#include "ParameterGroup.h"

ParameterGroup::ParameterGroup(const QString& name, QObject* parent) :
	ParameterBase(name, parent),
	m_parameters()
{
}

ParameterGroup::~ParameterGroup()
{
	clearParameters();
}

void ParameterGroup::setIsLocked(bool isLocked)
{
	if (getIsLocked() != isLocked)
	{
		for (auto& parameter : m_parameters)
		{
			parameter->setIsLocked(isLocked);
		}
		ParameterBase::setIsLocked(isLocked);
	}
}

void ParameterGroup::addParameter(ParameterBase* parameter)
{
	if (parameter && !m_parameters.contains(parameter))
	{
		m_parameters.append(parameter);
	}
}

void ParameterGroup::removeParameter(ParameterBase* parameter)
{
	if (parameter && m_parameters.contains(parameter))
	{
		m_parameters.removeOne(parameter);
	}
}

void ParameterGroup::clearParameters()
{
	m_parameters.clear();
}

QList<const ParameterBase*> ParameterGroup::getParameters() const
{
	QList<const ParameterBase*> paramers(m_parameters.size());
	int i = 0;
	for (const auto& parameter : m_parameters)
	{
		paramers[i++] = parameter;
	}
	return paramers;
}