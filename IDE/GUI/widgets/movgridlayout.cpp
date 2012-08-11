#include "movgridlayout.h"
#include "movgridlayoutitem.h"
#include <QtGui>

MovGridLayout::MovGridLayout(QWidget *parentWidget)
    :QWidget(parentWidget)
{
    //free whole grid
    for (int i = 0; i<SIZEX; i++)
        for (int j = 0; j<SIZEY; j++)
            grid[i][j]=-1;
    //calculate size of row and column
    this->setMinimumWidth(500);
    this->setMinimumHeight(500);
    sizeRow = this->width()/SIZEX;
    sizeCol = this->height()/SIZEY;
    //sizeRow = SIZEX;
    //sizeCol = SIZEY;
    qDebug() << sizeRow;
    qDebug() << sizeCol;
    this->parent = parentWidget;
    this->installEventFilter(this);
    grab = false;
}

bool MovGridLayout::addWidget(QWidget *widget)
{
    //position - x, y, see typedef struct XY in movgridlayout.h
    XY xy = calcXY(widget);
    //if grid is full
    if (xy.x == -1 || xy.y == -1)
        return false;
    MovGridLayoutItem *item = new MovGridLayoutItem(this, widget, widget->width()/sizeRow, widget->height()/sizeCol, gridWidgets.size());
    qDebug() << "add";
    gridWidgets.append(item);
    item->move(xy.x, xy.y, xy.x*sizeRow, xy.y*sizeCol);
    for (int i = item->x; i<item->x+item->w; i++)
        for (int j = item->y; j<item->y+item->h; j++)
            grid[i][j] = item->index;
    return true;
}

void MovGridLayout::addWidget(QWidget *widget, int x, int y)
{
    MovGridLayoutItem *item = new MovGridLayoutItem(this, widget, widget->width()/sizeRow, widget->height()/sizeCol, gridWidgets.size());
    gridWidgets.append(item);
    item->move(x, y, x*sizeRow, y*sizeCol);
    for (int i = item->x; i<item->x+item->w; i++)
        for (int j = item->y; j<item->y+item->h; j++)
            grid[i][j] = item->index;
}

void MovGridLayout::loadGridWidgets()
{
    //load from xml, here or where project-loading takes place
    //xml format:
    //  WIDGET_ID, x, y (, w, h?)
}

XY MovGridLayout::calcXY(QWidget *widget)
{
    //calculate position of widget (= find first free position)
    int w = widget->width()/sizeRow;
    int h = widget->height()/sizeCol;
    qDebug() << "width + height:";
    qDebug() << w;
    qDebug() << h;
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


bool MovGridLayout::checkPosition(XY position, WH geometry)
{
    //check grid range
    if (position.x + geometry.w > SIZEX || position.y + geometry.h > SIZEY)
        return false;
    //check grid
    for (int i = position.x; i<position.x+geometry.w; i++)
        for (int j = position.y; j<position.y+geometry.h; j++)
            if (grid[i][j] != -1)
                return false;     
    return true;
}

//use pointer to project file or its path
void MovGridLayout::saveGridWidgets()
{

}


bool MovGridLayout::eventFilter(QObject *target, QEvent *event)
{
    if (target == this)
    {
        //mouse down
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            movingIndex = grid[mouseEvent->x()/sizeRow][mouseEvent->y()/sizeCol];
            grab = true;
            qDebug() << "start grab";
            return true;
        }
        //mouse up
        if (event->type() == QEvent::MouseButtonRelease)
        {
            if (grab == true)
            {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
	        grab = false;
                qDebug() << "stop grab";
	        XY position;
	        position.x = mouseEvent->x()/sizeRow;
	        position.y = mouseEvent->y()/sizeCol;
	        WH geometry;
	        geometry.w = gridWidgets.at(movingIndex)->w;
	        geometry.h = gridWidgets.at(movingIndex)->h;
	        if (checkPosition(position, geometry) == true)
	        {
	            //null previous position
                    //change to gridWidgets.at(movingIndex)->x and y
                    //faster
	            for (int i = 0; i<SIZEX; i++)
	                for (int j = 0; j<SIZEY; j++)
	                    if (grid[i][j] == movingIndex)
	                        grid[i][j] = -1;
	            //add new position
                    for (int i = position.x; i<position.x+geometry.w; i++)
                        for (int j = position.y; j<position.y+geometry.h; j++)
                            grid[i][j] = movingIndex;
                    gridWidgets.at(movingIndex)->move(position.x, position.y, position.x*sizeRow, position.y*sizeCol);
	        }
            }
            return true;
        }
    }
    return QWidget::eventFilter(target, event);
}


void MovGridLayout::paintEvent(QPaintEvent *)
{
     for (int i=0; i<gridWidgets.size(); i++)
        gridWidgets.at(i)->widget->show();
}
