/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file movgridlayoutitem.h
 */


#ifndef MOVGRIDLAYOUTITEM_H
#define MOVGRIDLAYOUTITEM_H

#include <QWidget>


/**
 * @brief
 * @ingroup GUI
 * @class MovGridLayoutItem
 */
class MovGridLayoutItem
{
    public:
        MovGridLayoutItem(QWidget *parent, QWidget *widget, int w, int h, int index);
        void move(int x, int y, int realX, int realY);

        int x;
        int y;
        int w;
        int h;
        int index;
        QWidget *widget;


    private:
        QWidget *parent;
};


#endif
