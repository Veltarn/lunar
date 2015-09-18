#include "qmultiplechoices.h"

QMultipleChoices::QMultipleChoices(QWidget *parent) : QWidget(parent)
{
    init();
}

QMultipleChoices::QMultipleChoices(QString groupName, QWidget *parent) : QWidget(parent)
{
    init();

    m_multipleChoicesBox->setTitle(groupName);
}

void QMultipleChoices::addItem(const QString &label, QVariant data, bool isChecked)
{
    QDataCheckBox *checkBox = this->createItem(label, data, isChecked);
    m_checkBoxList.append(checkBox);
    this->updateLayout();
}

QDataCheckBox *QMultipleChoices::findItemByLabel(const QString &label)
{
    foreach(QDataCheckBox* checkbox, m_checkBoxList) {
        if(checkbox->text() == label) {
            return checkbox;
        }
    }

    return NULL;
}

QDataCheckBox *QMultipleChoices::findItemByData(QVariant &data)
{
    foreach(QDataCheckBox *checkbox, m_checkBoxList) {
        if(checkbox->data() == data) {
            return checkbox;
        }
    }
    return NULL;
}

void QMultipleChoices::setAllCheckboxes(bool checked)
{
    foreach(QDataCheckBox *checkbox, m_checkBoxList) {
        checkbox->setChecked(checked);
    }
}

void QMultipleChoices::setCheckboxStateByData(QVariant data, bool checked)
{
    QDataCheckBox *checkbox = this->findItemByData(data);

    if(checkbox != NULL) {
        checkbox->setChecked(checked);
    }
}

void QMultipleChoices::setCheckboxState(QString &label, bool checked)
{
    QDataCheckBox *checkbox = this->findItemByLabel(label);

    if(checkbox != NULL) {
        checkbox->setChecked(checked);
    }
}

QList<QDataCheckBox *> QMultipleChoices::checkedBox()
{
    QList<QDataCheckBox *> checkboxList = QList<QDataCheckBox *>();

    foreach(QDataCheckBox *checkbox, m_checkBoxList) {
        if(checkbox->isChecked()) {
            checkboxList.append(checkbox);
        }
    }
    return checkboxList;
}

void QMultipleChoices::updateLayout()
{
    int row = 0;

    QLayoutItem *item;
    //Clearing layout
    while((item = m_groupBoxLayout->takeAt(0)) != 0){}

    int checkboxesNumber = m_checkBoxList.size();

    //Continuing if checkboxesNumber is different than 0
    if(checkboxesNumber > 0) {
        /*
         * Dividing number of checkboxes by 3 (3 per row)and ceilling it to get
         * the number of rows
         */

        int rowsNumber = ceil(checkboxesNumber / 3.d);

        QList<QDataCheckBox*>::Iterator dataCheckboxIterator = m_checkBoxList.begin();
        //Iterate through rows number
        for(int i = 0; i < rowsNumber; i++) {
            //Iterate through columns
            for(int j = 0; j < 3; j++) {
                //If dataCheckboxIterator equals end() then we are at the end of the list
                if(dataCheckboxIterator != m_checkBoxList.end()) {
                    QDataCheckBox *checkbox = *dataCheckboxIterator;
                    m_groupBoxLayout->addWidget(checkbox, i, j);

                    ++dataCheckboxIterator;
                }
            }
        }
    }
}

void QMultipleChoices::init()
{
    m_multipleChoicesBox = new QGroupBox(this);
    m_groupBoxLayout = new QGridLayout;

    m_multipleChoicesBox->setLayout(m_groupBoxLayout);
}

QDataCheckBox *QMultipleChoices::createItem(const QString &label, QVariant data, bool isChecked)
{
    QDataCheckBox *dataCheckBox = new QDataCheckBox(label, data);
    dataCheckBox->setChecked(isChecked);

    return dataCheckBox;
}

