#include "ParameterBase.h"

ParameterBase::ParameterBase(const QString& name, QObject* parent) :
	QObject(parent),
	m_name(name),
	m_isLocked(false)
{
}

QString ParameterBase::getName() const
{
	return m_name;
}

bool ParameterBase::getIsLocked() const
{
	return m_isLocked;
}

void ParameterBase::setIsLocked(bool isLocked)
{
	if (isLocked != m_isLocked)
	{
		m_isLocked = isLocked;
		emit isLockedChanged(m_isLocked);
	}
}
