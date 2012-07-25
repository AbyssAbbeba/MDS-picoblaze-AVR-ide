#ifndef MOVGRIDLAYOUTITEM_H
#define MOVGRIDLAYOUTITEM_H

#include <QWidget>


class MovGridLayoutItem
{
    public:
        MovGridLayoutItem(QWidget *parent, QWidget *widget, int x, int y, int w, int h);
        int x;
        int y;
        int w;
        int h;
    private:
        QWidget *parent;
        QWidget *widget;
};


#endif
