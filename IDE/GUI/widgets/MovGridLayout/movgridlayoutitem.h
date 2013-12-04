/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
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
