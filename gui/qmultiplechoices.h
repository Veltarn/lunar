#ifndef QMULTIPLECHOICES_H
#define QMULTIPLECHOICES_H

#include <QGroupBox>
#include <QCheckBox>
#include <QVariant>
#include <QList>
#include <cmath>
#include "QGridLayout"
#include "qdatacheckbox.h"

class QMultipleChoices : public QWidget
{
    Q_OBJECT
public:
    explicit QMultipleChoices(QWidget *parent = 0);
    QMultipleChoices(QString groupName, QWidget *parent = 0);

    void addItem(const QString &label, QVariant data, bool isChecked = false);

    QDataCheckBox *findItemByLabel(const QString &label);
    QDataCheckBox *findItemByData(QVariant &data);

    void setAllCheckboxes(bool checked);

    /**
     * @brief This method will find if "data" is present, if so, it will set the checkbox state to "checked" value. It won't do anything otherwise
     * @param data
     * @param checked
     */
    void setCheckboxStateByData(QVariant data, bool checked);

    /**
     * @brief This is an overload of setCheckboxState. Rather than searching for data, this method will look after the label of the checkboxes
     * @param label
     * @param checked
     */
    void setCheckboxState(QString &label, bool checked);

    QList<QDataCheckBox *> checkedBox();
signals:

public slots:

protected:
    void updateLayout();

private:
    void init();
    QDataCheckBox *createItem(const QString &label, QVariant data, bool isChecked = false);
private:
    QGroupBox *m_multipleChoicesBox;
    QList<QDataCheckBox*> m_checkBoxList;
    QGridLayout *m_groupBoxLayout;

};

#endif // QMULTIPLECHOICES_H
