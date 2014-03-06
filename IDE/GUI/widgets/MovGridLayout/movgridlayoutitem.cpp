/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup MovGridLayoutItem
 * @file movgridlayoutitem.cpp
 */


#include <QtGui>
#include "movgridlayoutitem.h"

MovGridLayoutItem::MovGridLayoutItem(QWidget *parent, QWidget *widget, int w, int h, int index)
{
    this->parent = parent;
    this->widget = widget;
    this->x = 0;
    this->y = 0;
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
