#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>
#include <QPushButton>
#include <QLinearGradient>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QDebug>

class Button : public QPushButton
{
public:
    Button(QWidget *parent = 0);
    Button(const QString text, QWidget *parent = 0);
    Button(const QIcon &icon, const QString text, QWidget *parent = 0);
    virtual ~Button();

protected:
    void paintEvent(QPaintEvent *pe);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
private:
    QRect calculateIconPosition(QRect button_rect, QSize icon_size);
private:
    bool m_hovered;
    bool m_pressed;
    int m_roundness;
};

#endif // BUTTON_H
