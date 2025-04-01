#include "ListParameterWidget.h"

ListParameterWidget::ListParameterWidget(QWidget* parent) :
    QWidget(parent),
    m_label(new QLabel(this)),
    m_comboBox(new QComboBox(this))
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_label);
    mainLayout->addWidget(m_comboBox);

    connect(m_comboBox, &QComboBox::currentIndexChanged, this, &ListParameterWidget::indexChanged);
}

void ListParameterWidget::setName(const QString& name) 
{ 
    m_label->setText(name);
}

void ListParameterWidget::setOptions(const QStringList& keys) 
{ 
    m_comboBox->addItems(keys);
}

void ListParameterWidget::setCurrentIndex(int index) 
{ 
    m_comboBox->setCurrentIndex(index); 
}
