#ifndef ABOUTLUNAR_H
#define ABOUTLUNAR_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <QBrush>
#include "../constants.h"
#include "button.h"

namespace Ui {
class AboutLunar;
}

class AboutLunar : public QDialog
{
    Q_OBJECT
    
public:
    explicit AboutLunar(QWidget *parent = 0);
    ~AboutLunar();
protected:
    void paintEvent(QPaintEvent *pe);
private:
    void initWindow();
    Ui::AboutLunar *ui;

    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_buttonLayout;
    QLabel *m_text;
    Button *m_closeButton;

};

#endif // ABOUTLUNAR_H
