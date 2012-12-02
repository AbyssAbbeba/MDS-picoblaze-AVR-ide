/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 *
 */



#include "movgridlayoutitem.h"

MovGridLayoutItem::MovGridLayoutItem(QWidget *parent, QWidget *widget, int w, int h, int index)
{
    this->parent = parent;
    this->widget = widget;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->index = index;
}


void MovGridLayoutItem::move(int x, int y, int realX, int realY)
{
    this->x = x;
    this->y = y;
    this->widget->move(realX, realY);
}
