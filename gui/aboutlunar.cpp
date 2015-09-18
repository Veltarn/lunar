#include "aboutlunar.h"
#include "ui_aboutlunar.h"

AboutLunar::AboutLunar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutLunar)
{
    ui->setupUi(this);

    initWindow();

    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(accept()));
}

AboutLunar::~AboutLunar()
{
    delete ui;
}

void AboutLunar::paintEvent(QPaintEvent *pe)
{
    Q_UNUSED(pe);

    QPainter painter(this);

    QColor bgColor(255, 255, 255);
    QBrush brush(bgColor);
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);

    painter.drawRect(0, 0, this->width(), this->height());
}

void AboutLunar::initWindow()
{
    m_mainLayout = new QVBoxLayout;
    m_buttonLayout = new QHBoxLayout;

    setWindowTitle(tr("About Lunar"));
    m_text = new QLabel(this);
    m_closeButton = new Button(tr("Close"), this);
    QFont font;
    font.setPointSize(10);
    m_text->setFont(font);
    m_text->setText("<p align='center'><b>Lunar " + SOFTWARE_VERSION + " </b></p><p>Copyright (C) 2013 Lunar <br/> \
                    \
                    This program is free software: you can redistribute it and/or modify <br/> \
                    it under the terms of the GNU General Public License as published by <br/> \
                    the Free Software Foundation, either version 3 of the License, or <br/> \
                    (at your option) any later version. <br/> \
                    \
                    This program is distributed in the hope that it will be useful,<br/> \
                    but WITHOUT ANY WARRANTY; without even the implied warranty of<br/> \
                    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the<br/> \
                    GNU General Public License for more details.<br/> \
                    \
                    You should have received a copy of the GNU General Public License<br/> \
                    along with this program.");

    m_buttonLayout->addWidget(m_closeButton, 1, Qt::AlignRight);
    m_mainLayout->addWidget(m_text, 1, Qt::AlignTop);
    m_mainLayout->addLayout(m_buttonLayout);

    this->setLayout(m_mainLayout);
}
