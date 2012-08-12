#ifndef MOVGRIDLAYOUTITEM_H
#define MOVGRIDLAYOUTITEM_H

#include <QWidget>


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
