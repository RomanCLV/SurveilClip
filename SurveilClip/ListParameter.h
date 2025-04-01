#pragma once
#include "ListParameterBase.h"

template <typename T>
class ListParameter : public ListParameterBase
{
public:
    explicit ListParameter(const QString& name, QObject* parent = nullptr) :
        ListParameterBase(name, parent) 
    {
    }

    ~ListParameter() override = default;

    bool addItem(const QString& key, const T& value)
    {
        int i;
        for (i = 0; i < m_keys.size(); i++)
        {
            if (m_keys[i] == key)
            {
                break;
            }
        }
        bool added = i == m_keys.size();
        if (added)
        {
            m_keys.append(key);
            m_values.append(QVariant::fromValue(value));
            if (m_selectedIndex == -1)
            {
                m_selectedIndex = 0;
                emit selectedIndexChanged(m_selectedIndex);
            }
        }
        
        return added;
    }

    const QStringList& getKeys() const override
    {
        return m_keys;
    }

    QVariant getSelectedValue() const override
    {
        return (m_selectedIndex >= 0 && m_selectedIndex < m_values.size()) ?
            m_values[m_selectedIndex] :
            QVariant();
    }

    T getSelectedTypedValue() const
    {
        return (m_selectedIndex >= 0 && m_selectedIndex < m_values.size()) ?
            m_values[m_selectedIndex].value<T>() :
            T();
    }

    void selectValueByIndex(int index) override
    {
        if (index >= 0 && index < m_values.size() && index != m_selectedIndex)
        {
            m_selectedIndex = index;
            emit selectedIndexChanged(m_selectedIndex);
        }
    }

    void selectValue(const T& value)
    {
        for (int i = 0; i < m_values.size(); i++)
        {
            if (m_values[i].value<T>() == value)
            {
                selectValueByIndex(i);
            }
        }
    }
};
