#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <iostream>
#include <QListView>
#include <QPainter>
#include <QWidget>

class ListView : public QListView
{
public:
    ListView(QWidget *parent = 0);
    virtual ~ListView();
protected:
    //void paintEvent(QPaintEvent *e);
};

#endif // LISTVIEW_H
