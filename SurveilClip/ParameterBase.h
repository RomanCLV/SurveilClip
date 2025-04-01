#pragma once
#include <QObject>
#include <QString>

class ParameterBase : public QObject
{
	Q_OBJECT

public:
	explicit ParameterBase(const QString& name, QObject* parent = nullptr);
	virtual ~ParameterBase() = default;
	
	QString getName() const;
	bool getIsLocked() const;

public slots:
	virtual void setIsLocked(bool isLocked);

signals:
	void isLockedChanged(bool isLocked);

private:
	QString m_name;
	bool m_isLocked;
};
