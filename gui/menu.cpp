#include "menu.h"

Menu::Menu(QWidget *parent):
    QMenu(parent)
{}

Menu::Menu(const QString &title, QWidget *parent):
    QMenu(title, parent)
{}

Menu::~Menu()
{}

void Menu::paintEvent(QPaintEvent *pe)
{
    Q_UNUSED(pe);
    QPainter painter(this);

    painter.setPen(QPen(QBrush(QColor(66, 142, 255)), 2.0));
    painter.setRenderHint(QPainter::Antialiasing);

    QColor bgMenuColor(250, 250, 250);
    painter.setBrush(bgMenuColor);

    painter.drawRect(0, 0, this->width(), this->height());

    QStyleOptionMenuItem menuOptions;
    menuOptions.initFrom(this);
    for(int i = 0; i < this->actions().size(); i++)
    {
        QAction *action = this->actions().at(i);
        QRect actionRect = this->actionGeometry(action);

        initStyleOption(&menuOptions, action);

        //We draw a simple line if the action is a separator
        if(action->isSeparator())
        {
            //reajusting the y position of the separator
            actionRect.setY(actionRect.y() + 4);
            painter.save();
            QLinearGradient lineColor(0, actionRect.y() + 4, 0, actionRect.y() + 5);
            lineColor.setSpread(QGradient::RepeatSpread);

            lineColor.setColorAt(0.0, QColor(81, 81, 81));
            lineColor.setColorAt(1.0, QColor(217, 217, 217));

            QBrush brush(lineColor);
            painter.setPen(QPen(brush, 1));

            QPoint p1(actionRect.x(), actionRect.y());
            QPoint p2(actionRect.x() + actionRect.width(), actionRect.y());

            painter.drawLine(p1, p2);
            painter.restore();
        }
        else
        {
            actionRect.setY(actionRect.y() - 3);
            bool selected = menuOptions.state & QStyle::State_Selected;

            actionRect.setY(actionRect.y() + 3);

            /*
             *  If the menu is selected
             */
            if(selected)
            {
                painter.save();
                QColor bgColorItemMenu(255, 148, 16);

                QBrush brushItemMenu(bgColorItemMenu);
                painter.setBrush(brushItemMenu);
                painter.setPen(Qt::NoPen);

                painter.drawRect(actionRect);

                painter.restore();
            }

            /*
             * Displaying menu's icon
             */
            if(!action->icon().isNull())
            {
                QRect iconRect = actionRect;
                iconRect.setWidth(16);
                iconRect.setHeight(16);
                iconRect.setY(iconRect.y() + 4);

                //Scaling icon
                QImage img(action->icon().pixmap(24, 24).toImage());
                painter.drawImage(iconRect, img);
            }

            actionRect.setX(actionRect.x() + 30);
            painter.save();

            //Drawing text
            QColor textColor;
            if(action->isEnabled())
                textColor = QColor(60, 60, 60);
            else
                textColor = QColor(175, 175, 175);

            QRect textRect = actionRect;
            textRect.setY(actionRect.y() + 3);
            painter.setPen(QPen(QBrush(textColor), 2.0));

            painter.drawText(textRect, action->text());
            painter.restore();

            //Drawing sub-menus arrow
            if(menuOptions.menuItemType == QStyleOptionMenuItem::SubMenu)
            {
                QImage arrow("resources/imgs/right_arrow.png");
                QRect rectArrow = actionRect;
                rectArrow.setX(rectArrow.width() + 18);
                rectArrow.setWidth(16);
                rectArrow.setHeight(16);

                painter.drawImage(rectArrow, arrow);
            }
        }
    }
}
