#include "listview.h"

ListView::ListView(QWidget *parent) : QListView(parent)
{
    QString stylesheet = "";
    stylesheet += "QListView { border: 0; background: 0 }";
    stylesheet += "QListView::item { height: 32px; margin: 2px 2px } QScrollBar::handle:vertical{ background: #ff0000; }";
    this->setStyleSheet(stylesheet);
}

ListView::~ListView()
{}

/*void ListView::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QListView::paintEvent(e);

    QPainter painter(this->viewport());
    painter.save();
    QRect listViewRect = this->rect();

    QColor bgColor(255, 255, 255, 30);
    QBrush brush(bgColor);
    painter.setPen(QPen(QBrush(QColor(255, 255, 255)), 1.0));
    painter.setBrush(brush);

    painter.drawRect(listViewRect);
    painter.restore();

    if(model() == NULL || model()->rowCount() == 0)
    {
        QRect textRect = listViewRect;
        painter.drawText(textRect, Qt::AlignCenter, tr("No games..."));
    }
    QListView::paintEvent(e);
}*/
