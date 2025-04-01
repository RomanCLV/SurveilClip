#pragma once
#include "ParameterBase.h"
#include <QObject>
#include <QList>

class ParameterGroup : public ParameterBase
{
	Q_OBJECT

public:
	explicit ParameterGroup(const QString& name, QObject* parent = nullptr);
	~ParameterGroup();

	void addParameter(ParameterBase* parameter);
	void removeParameter(ParameterBase* parameter);
	void clearParameters();
	QList<const ParameterBase*> getParameters() const;

public slots:
	void setIsLocked(bool isLocked) override;

private:
	QList<ParameterBase*> m_parameters;
};
