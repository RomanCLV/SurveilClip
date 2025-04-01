#pragma once
#include "ParameterBase.h"
#include <QObject>

class BoolParameter : public ParameterBase
{
	Q_OBJECT

public:
	explicit BoolParameter(const QString& name, bool value = false, QObject* parent = nullptr);
	~BoolParameter() = default;

	bool getValue() const;

public slots:
	void setValue(bool value);

signals:
	void valueChanged(bool value);

private:
	bool m_value;
};
