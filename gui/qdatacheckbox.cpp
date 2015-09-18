#include "qdatacheckbox.h"

QDataCheckBox::QDataCheckBox(QWidget *parent) : QCheckBox(parent)
{

}

QDataCheckBox::QDataCheckBox(const QString &text, QWidget *parent) : QCheckBox(text, parent)
{

}

QDataCheckBox::QDataCheckBox(const QString &text, QVariant userData, QWidget *parent) : QCheckBox(text, parent), m_data(userData)
{

}

QVariant QDataCheckBox::data()
{
    return m_data;
}

void QDataCheckBox::setData(QVariant data)
{
    m_data = data;
}


