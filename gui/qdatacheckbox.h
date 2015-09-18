#ifndef QDATACHECKBOX_H
#define QDATACHECKBOX_H

#include <QCheckBox>
#include <QString>
#include <QVariant>

class QDataCheckBox : public QCheckBox
{
public:
    QDataCheckBox(QWidget *parent = 0);
    QDataCheckBox(const QString &text, QWidget *parent = 0);
    QDataCheckBox(const QString &text, QVariant userData, QWidget *parent = 0);

    QVariant data();
    void setData(QVariant data);

private:
    QVariant m_data;
};

#endif // QDATACHECKBOX_H
