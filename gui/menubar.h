#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>
#include <QHBoxLayout>
#include <QLinearGradient>
#include <QColor>
#include <QPainter>
#include <QBrush>
#include <QAction>
#include <QWidget>
#include <iostream>

class MenuBar : public QMenuBar
{
public:
    MenuBar(QWidget *parent = 0);
    ~MenuBar();

protected:
    void paintEvent(QPaintEvent *pe);

private:
    int m_height;
    int m_marginRight;
    int m_clickedMenu;
    QHBoxLayout m_layout;
};

#endif // MENUBAR_H
