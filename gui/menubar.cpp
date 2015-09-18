#include "menubar.h"

MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent),
    m_height(22),
    m_marginRight(15),
    m_clickedMenu(-1)
{
    this->setFixedHeight(m_height);
}

MenuBar::~MenuBar()
{}

void MenuBar::paintEvent(QPaintEvent *pe)
{
    Q_UNUSED(pe);

    QPainter painter(this);

    painter.setPen(Qt::NoPen);

    QLinearGradient gradientBg(0, 0, 0, this->height());
    gradientBg.setSpread(QGradient::PadSpread);

    gradientBg.setColorAt(0.0, QColor(0, 120, 227));
    gradientBg.setColorAt(1.0, QColor(22, 145, 255));

    QBrush brush(gradientBg);
    painter.setBrush(brush);

    painter.drawRect(0, 0, this->parentWidget()->width(), this->height());
    /**
     * Displaying menus
     */

    for(int i = 0; i < this->actions().size(); i++)
    {
        QAction *action = this->actions().at(i);
        QRect actionRect = this->actionGeometry(action);

        painter.save();
        painter.setRenderHint(QPainter::Antialiasing);

        //Displaying background color of the menu if it's active
        if(action->menu()->isVisible())
        {
            painter.setPen(QPen(QBrush(QColor(0, 59, 126)), 1.0));
            QColor bgColorAction(255, 148, 16);
            QBrush brushAction(bgColorAction);
            painter.setBrush(brushAction);
        }

        painter.drawRect(actionRect);
        QRect rectText = actionRect;
        rectText.setY(3);
        rectText.setX(rectText.x() + 7);

        painter.setPen(QPen(QBrush(QColor(255, 255, 255)), 2.0));
        painter.drawText(rectText, action->text());
        painter.restore();
    }
}
