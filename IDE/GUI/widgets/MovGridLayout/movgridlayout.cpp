#include "movgridlayout.h"
#include "movgridlayoutitem.h"
#include <QtGui>


MovGridLayout::MovGridLayout(QWidget *parentWidget)
    :QWidget(parentWidget)
{
    this->setMinimumWidth(100);
    this->setMinimumHeight(100);
    sizeRow = SIZEX;
    sizeCol = SIZEY;
    gridWidth = this->width()/sizeRow;
    gridHeight = this->height()/sizeCol;
    grid = NULL;
    grid = new int*[gridWidth];
    for (int i = 0; i < gridWidth; i++)
        grid[i] = new int[gridHeight];
    //free whole grid
    for (int i = 0; i<gridWidth; i++)
        for (int j = 0; j<gridHeight; j++)
            grid[i][j]=-1;
    //FOR STATIC ALLOCATION
    //calculate size of row and column
    //sizeRow = this->width()/SIZEX;
    //sizeCol = this->height()/SIZEY;
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
    //  WIDGET_ID, x, y
}


XY MovGridLayout::calcXY(QWidget *widget)
{
    //calculate position of widget (= find first free position)
    int w = (widget->width()+2)/sizeRow;
    int h = (widget->height()+2)/sizeCol;
    qDebug() << "width + height:";
    qDebug() << w;
    qDebug() << h;
    for (int i = 0; i+w<gridWidth; i++)
        for (int j = 0; j+h<gridHeight; j++)
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


bool MovGridLayout::checkPosition(XY position, WH geometry, int index)
{
    //check grid range
    if (position.x + geometry.w > gridWidth || position.y + geometry.h > gridHeight || position.x < 0 || position.y < 0)
        return false;
    //check grid
    for (int i = position.x; i<position.x+geometry.w; i++)
        for (int j = position.y; j<position.y+geometry.h; j++)
            if (grid[i][j] != -1 && grid[i][j] != index)
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
        //on resize recalculate size of row and col
        if (event->type() == QEvent::Resize)
        {
            /*FOR STATIC ALLOCATION
            //calculate new sizeRow, sizeCol
            sizeRow = this->width()/SIZEX;
            sizeCol = this->height()/SIZEY;
            qDebug() << "sizeRow + sizeCol:";
            qDebug() << sizeRow;
            qDebug() << sizeCol;
            //recalcutale w, h
            for (int i = 0; i < gridWidgets.size(); i++)
            {
                gridWidgets.at(i)->w = gridWidgets.at(i)->widget->width() / sizeRow;
                gridWidgets.at(i)->h = gridWidgets.at(i)->widget->height() / sizeCol;
                gridWidgets.at(i)->widget->move(gridWidgets.at(i)->x*sizeRow, gridWidgets.at(i)->y*sizeCol);
            }*/
            //FOR DYNAMIC ALLOCATION
            gridWidth = this->width()/sizeRow;
            gridHeight = this->height()/sizeCol;
            grid = new (grid) int*[gridWidth];
            for (int i = 0; i < gridWidth; i++)
                grid[i] = new (grid[i]) int[gridHeight];
            qDebug() << "reallocating";
            this->update();
        }

        //
        //mouse events: down, up, move
        //
        //mouse down
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            movingIndex = grid[mouseEvent->x()/sizeRow][mouseEvent->y()/sizeCol];
            //check if user clicked on widget (blank area == -1)
            if (movingIndex != -1)
            {
                movingWidget.geom.w = gridWidgets.at(movingIndex)->w;
                movingWidget.geom.h = gridWidgets.at(movingIndex)->h;
                movingWidget.pos.x = mouseEvent->x();
                movingWidget.pos.y = mouseEvent->y();
                grab = true;
                qDebug() << "start grab index: " + movingIndex;
            }
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
	        if (checkPosition(position, geometry, movingIndex) == true)
	        {
	            //null previous position
                    //change to gridWidgets.at(movingIndex)->x and y
                    //faster
	            for (int i = 0; i<gridWidth; i++)
	                for (int j = 0; j<gridHeight; j++)
	                    if (grid[i][j] == movingIndex)
	                        grid[i][j] = -1;
	            //add new position
                    for (int i = position.x; i<position.x+geometry.w; i++)
                        for (int j = position.y; j<position.y+geometry.h; j++)
                            grid[i][j] = movingIndex;
                    gridWidgets.at(movingIndex)->move(position.x, position.y, position.x*sizeRow, position.y*sizeCol);
	        }
                this->update();
            }
            return true;
        }
        //mouse move
        if (event->type() == QEvent::MouseMove)
        {
            if (grab == true)
            {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
                movingWidget.pos.x = mouseEvent->x()/sizeRow;
                movingWidget.pos.y = mouseEvent->y()/sizeCol;
                this->update();
            }
            return true;
        }
    }
    return QWidget::eventFilter(target, event);
}


void MovGridLayout::paintEvent(QPaintEvent *)
{

    QPainter paint;
    paint.begin(this);
    paint.fillRect(0, 0, this->width(), this->height(), paint.background());
    paint.setOpacity(0.4);
    for (int i=0; i<gridWidgets.size(); i++)
    {
        gridWidgets.at(i)->widget->show();
        paint.drawRect(gridWidgets.at(i)->x*sizeRow -1, gridWidgets.at(i)->y*sizeCol -1, gridWidgets.at(i)->w*sizeRow +2, gridWidgets.at(i)->h*sizeCol +2);
    }
    if (grab == true)
    {
        //fill current mouse position (= new position)
        QBrush brush(Qt::red);
        if (checkPosition(movingWidget.pos, movingWidget.geom, movingIndex) == true)
            brush.setColor(Qt::green);
        paint.fillRect(movingWidget.pos.x*sizeRow -1, movingWidget.pos.y*sizeCol -1, movingWidget.geom.w*sizeRow +2, movingWidget.geom.h*sizeCol +2, brush);
        //fill grabbed widget (= old position)
        brush.setColor(Qt::darkCyan);
        paint.fillRect(gridWidgets.at(movingIndex)->x*sizeRow -1, gridWidgets.at(movingIndex)->y*sizeCol -1, gridWidgets.at(movingIndex)->w*sizeRow +2, gridWidgets.at(movingIndex)->h*sizeCol +2, brush);
    }
    paint.end();
}
