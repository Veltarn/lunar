#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <QMenu>
#include <QHBoxLayout>
#include <QColor>
#include <QLinearGradient>
#include <QPainter>
#include <QBrush>
#include <QStyle>
#include <QPoint>
#include <QStyleOptionMenuItem>

class Menu : public QMenu
{
public:
    Menu(QWidget *parent = 0);
    Menu(const QString &title, QWidget *parent = 0);
    virtual ~Menu();
    QString title();
    void setTitle(const QString &title);

protected:
    void paintEvent(QPaintEvent *pe);
};

#endif // MENU_H
