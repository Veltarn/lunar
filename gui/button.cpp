#include "button.h"

using namespace std;

Button::Button(QWidget *parent) :
    QPushButton(parent),
    m_hovered(false),
    m_pressed(false),
    m_roundness(5)
{}

Button::Button(const QString text, QWidget *parent) :
    QPushButton(text, parent),
    m_hovered(false),
    m_pressed(false),
    m_roundness(1)
{}

Button::Button(const QIcon &icon, const QString text, QWidget *parent):
    QPushButton(icon, text, parent),
    m_hovered(false),
    m_pressed(false),
    m_roundness(1)
{}

Button::~Button()
{}

void Button::paintEvent(QPaintEvent *pe)
{
    Q_UNUSED(pe);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QLinearGradient buttonColor;
    QRect buttonRect = this->geometry();

    painter.save();
    if(this->isEnabled())
    {
        QLinearGradient grd(0, 0, 0, this->height());
        grd.setSpread(QGradient::PadSpread);

        if(m_hovered)
        {
            grd.setColorAt(0.0, QColor(48, 160, 255));
            grd.setColorAt(1.0, QColor(62, 164, 255));
        }
        else // If the button is not hovered nor pressed
        {
            grd.setColorAt(0.0, QColor(0, 120, 227));
            grd.setColorAt(1.0, QColor(22, 145, 255));
        }


        if(m_pressed)
        {
            grd.setColorAt(0.0, QColor(1, 106, 200));
            grd.setColorAt(1.0, QColor(0, 120, 227));
        }

        buttonColor = grd;
        painter.setPen(QPen(QBrush(QColor(24, 143, 249)), 2.0));
    }
    else
    {
        QLinearGradient grd(0, 0, 0, this->height());
        grd.setSpread(QGradient::PadSpread);

        grd.setColorAt(0.0, QColor(158, 167, 182));
        grd.setColorAt(1.0, QColor(173, 182, 193));
        painter.setPen(QPen(QBrush(QColor(158, 167, 182)), 2.0));
        buttonColor = grd;
    }
    QBrush btnColor(buttonColor);
    //Drawing button
    painter.setBrush(btnColor);
    painter.drawRoundedRect(0, 0, buttonRect.width(), buttonRect.height(), m_roundness, m_roundness);
    painter.restore();

    //Text
    if(!this->text().isNull())
    {
        painter.setFont(this->font());
        painter.setPen(Qt::white);
        //The coordinates are relative to the button and not to the window
        if(this->icon().isNull())
        {
            painter.drawText(0, 0, buttonRect.width(), buttonRect.height(), Qt::AlignCenter, this->text());
        }
        else
        {
            painter.drawText(8, 0, buttonRect.width(), buttonRect.height(), Qt::AlignCenter, this->text());
        }
    }

    //Icon
    if(!this->icon().isNull())
    {
        QSize iconSize = this->iconSize();
        QRect iconRect = this->calculateIconPosition(buttonRect, iconSize);
        painter.drawPixmap(4, 4, iconRect.width(), iconRect.height(), this->icon().pixmap(iconSize));
    }
}

QRect Button::calculateIconPosition(QRect button_rect, QSize icon_size)
 {
     int x = button_rect.x() + 2;
     int y = button_rect.y() + 2;
     int width = icon_size.width();
     int height = icon_size.height();

     QRect icon_position;
     icon_position.setX(x);
     icon_position.setY(y);
     icon_position.setWidth(width);
     icon_position.setHeight(height);

     return icon_position;
 }

void Button::enterEvent(QEvent *e)
{
    m_hovered = true;
    this->repaint();

    QPushButton::enterEvent(e);
}

void Button::leaveEvent(QEvent *e)
{
    m_hovered = false;
    this->repaint();

    QPushButton::leaveEvent(e);
}

void Button::mousePressEvent(QMouseEvent *e)
{
    m_pressed = true;
    this->repaint();

    QPushButton::mousePressEvent(e);
}

void Button::mouseReleaseEvent(QMouseEvent *e)
{
    m_pressed = false;
    this->repaint();

    QPushButton::mouseReleaseEvent(e);
}
