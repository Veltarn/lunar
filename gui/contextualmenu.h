#ifndef CONTEXTUALMENU_H
#define CONTEXTUALMENU_H

#include <QWidget>
#include "menu.h"

class ContextualMenu : public Menu
{
public:
    ContextualMenu(QWidget *parent = 0);
};

#endif // CONTEXTUALMENU_H
