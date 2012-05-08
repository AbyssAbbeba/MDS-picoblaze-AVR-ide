#include "movgridlayout.h"
#include <QtGui>

MovGridLayout::MovGridLayout(QWidget *parent)
{
    for (int i = 0; i<SIZEX; i++)
        for (int j = 0; j<SIZEY; j++)
            grid[i][j]=false;
}

void MovGridLayout::addWidget(QWidget *widget)
{
    gridWidgets.append(widget);
}

void MovGridLayout::loadGridWidgets()
{
    QLabel *a = new QLabel(this);
    QLabel *b = new QLabel(this);
    a->setText("aaa");
    b->setText("bbb");
    addWidget(a);
    addWidget(b);
}
