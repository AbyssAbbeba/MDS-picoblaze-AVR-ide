#include "movgridlayout.h"
#include "movgridlayoutitem.h"
//#include <QtGui>

MovGridLayout::MovGridLayout(QWidget *parentWidget)
{
    //free whole grid
    for (int i = 0; i<SIZEX; i++)
        for (int j = 0; j<SIZEY; j++)
            grid[i][j]=-1;
    //calculate size of row and column
    sizeRow = this->width()/SIZEX;
    sizeCol = this->height()/SIZEY;
    this->parent = parentWidget;
}

bool MovGridLayout::addWidget(QWidget *widget)
{
    //position - x, y, see typedef struct XY in movgridlayout.h
    XY xy = calcXY(widget);
    //if grid is full
    if (xy.x == -1 || xy.y == -1)
        return false;
    MovGridLayoutItem *item = new MovGridLayoutItem(this, widget, xy.x, xy.y, widget->width()/sizeRow, widget->height()/sizeCol);
    gridWidgets.append(item);
    for (int i = item->x; i<item->x+item->w; i++)
        for (int j = item->y; j<item->y+item->h; j++)
            grid[i][j] = gridWidgets.size()-1;
    return true;
}

void MovGridLayout::addWidget(QWidget *widget, int x, int y)
{
    MovGridLayoutItem *item = new MovGridLayoutItem(this, widget, x, y, widget->width()/sizeCol, widget->height()/sizeCol);
    gridWidgets.append(item);
}

void MovGridLayout::loadGridWidgets()
{
    //load from xml, here or where project-loading takes place
    //xml format:
    //  WIDGET_ID, x, y (, w, h?)
}

XY MovGridLayout::calcXY(QWidget *widget)
{
    int w = widget->width()/sizeRow;
    int h = widget->height()/sizeCol;
    for (int i = 0; i+w<SIZEX; i++)
        for (int j = 0; j+h<SIZEY; j++)
            if (grid[i][j] == -1 &&
                grid [i][j+h] == -1)
            {
                XY xysucc;
                xysucc.x = i;
                xysucc.y = j;
                return xysucc;
            }
     XY xyfalse;
     xyfalse.x = -1;
     xyfalse.y = -1; 
     return xyfalse;
}

//use pointer to project file or its path
void MovGridLayout::saveGridWidgets()
{

}
