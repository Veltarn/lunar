#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include <QWidget>
#include <QScrollBar>

class ScrollBar : public QScrollBar
{
public:
    ScrollBar(QWidget *parent = 0);
};

#endif // SCROLLBAR_H
