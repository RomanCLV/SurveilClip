#pragma once
#include "ParameterBase.h"

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariantList>
#include <QVariant>

class ListParameterBase : public ParameterBase
{
    Q_OBJECT

public:
    explicit ListParameterBase(const QString& name, QObject* parent = nullptr) :
        ParameterBase(name, parent),
        m_keys(),
        m_values(),
        m_selectedIndex(-1)
    {
    }
    virtual ~ListParameterBase() = default;

    virtual const QStringList& getKeys() const = 0;
    virtual QVariant getSelectedValue() const = 0;
    virtual void selectValueByIndex(int index) = 0;
    int getSelectedIndex() const { return m_selectedIndex; }

signals:
    void selectedIndexChanged(int index);

protected:
    QStringList m_keys;
    QVariantList m_values;
    int m_selectedIndex;
};
