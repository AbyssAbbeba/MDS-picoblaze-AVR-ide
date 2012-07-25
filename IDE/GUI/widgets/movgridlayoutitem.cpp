#include "movgridlayoutitem.h"

MovGridLayoutItem::MovGridLayoutItem(QWidget *parent, QWidget *widget, int x, int y, int w, int h)
{
    this->parent = parent;
    this->widget = widget;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}
