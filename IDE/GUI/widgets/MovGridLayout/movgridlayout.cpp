/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 *
 */




#include "movgridlayout.h"
#include "movgridlayoutitem.h"
#include "../../dialogs/errordlg.h"
#include <QtGui>
#include <QtXml>


MovGridLayout::MovGridLayout(QWidget *parentWidget)
    :QWidget(parentWidget)
{
    this->setMinimumWidth(900);
    this->setMinimumHeight(600);
    this->show();
    sizeRow = SIZEX;
    sizeCol = SIZEY;
    gridWidth = this->width()/sizeRow;
    gridHeight = this->height()/sizeCol;
    //grid.resize(gridWidth, vector<int>(gridHeight, -1));
    grid = new int*[gridWidth];
    for (int i = 0; i < gridWidth; i++)
    {
        grid[i] = new int[gridHeight];
    }
    //free whole grid
    for (int i = 0; i<gridWidth; i++)
    {
        for (int j = 0; j<gridHeight; j++)
        {
            grid[i][j]=-1;
        }
    }
    //FOR STATIC ALLOCATION
    //calculate size of row and column
    //sizeRow = this->width()/SIZEX;
    //sizeCol = this->height()/SIZEY;
    this->parent = parentWidget;
    this->installEventFilter(this);
    grab = false;
    oldWidth = this->width();
    oldHeight = this->height();
    //loadGridWidgets();
}


//add widget to layout, automatic position
bool MovGridLayout::addWidget(QWidget *widget)
{
    //position - x, y, see typedef struct XY in movgridlayout.h
    //qDebug() << "calc start";
    XY xy = calcXY(widget);
    //if grid is full
    if (xy.x == -1 || xy.y == -1)
    {
        return false;
    }
    MovGridLayoutItem *item = new MovGridLayoutItem(this, widget, widget->width()/sizeRow, widget->height()/sizeCol, gridWidgets.size());
    //qDebug() << "add";
    gridWidgets.append(item);
    item->move(xy.x, xy.y, xy.x*sizeRow, xy.y*sizeCol);
    for (int i = item->x; i<item->x+item->w; i++)
    {
        for (int j = item->y; j<item->y+item->h; j++)
        {
            grid[i][j] = item->index;
        }
    }
    //qDebug() << "add done";
    return true;
}


//add widget to layout with given index, automatic position
bool MovGridLayout::addItem(MovGridLayoutItem *item)
{
    //position - x, y, see typedef struct XY in movgridlayout.h
    //qDebug() << "calc start";
    XY xy = calcXY(item->widget);
    //if grid is full
    if (xy.x == -1 || xy.y == -1)
    {
        return false;
    }
    MovGridLayoutItem *newItem = new MovGridLayoutItem(this, item->widget, item->w, item->h, item->index);
    //qDebug() << "add";
    gridWidgets.append(newItem);
    newItem->move(xy.x, xy.y, xy.x*sizeRow, xy.y*sizeCol);
    for (int i = newItem->x; i<newItem->x+newItem->w; i++)
    {
        for (int j = newItem->y; j<newItem->y+newItem->h; j++)
        {
            grid[i][j] = newItem->index;
        }
    }
    newItem->widget->show();
    //qDebug() << "add done";
    return true;
}


//add widget to layout with given index and position
void MovGridLayout::addItem(MovGridLayoutItem *item, int x, int y)
{
    MovGridLayoutItem *newItem = new MovGridLayoutItem(this, item->widget, item->w, item->h, item->index);
    //qDebug() << "add";
    gridWidgets.append(newItem);
    newItem->move(x, y, x*sizeRow, y*sizeCol);
    for (int i = newItem->x; i<newItem->x+newItem->w; i++)
    {
        for (int j = newItem->y; j<newItem->y+newItem->h; j++)
        {
            grid[i][j] = newItem->index;
        }
    }
    newItem->widget->show();
    //qDebug() << "add done";
}


//add widget to layout on exact position
void MovGridLayout::addWidget(QWidget *widget, int x, int y)
{
    MovGridLayoutItem *item = new MovGridLayoutItem(this, widget, widget->width()/sizeRow, widget->height()/sizeCol, gridWidgets.size());
    gridWidgets.append(item);
    item->move(x, y, x*sizeRow, y*sizeCol);
    for (int i = item->x; i<item->x+item->w; i++)
    {
        for (int j = item->y; j<item->y+item->h; j++)
        {
            grid[i][j] = item->index;
        }
    }
}


//load order of widgets from project xml
void MovGridLayout::loadGridWidgets(QString path)
{
    //load from xml, here or where project-loading takes place
    //xml format:
    //  WIDGET_ID, x, y
    QFile file(path);
    QDomDocument domDoc("MMProject");
    if (!domDoc.setContent(&file))
    {
        error(ERR_XML_ASSIGN);
    }
    else
    {
        QDomElement xmlRoot = domDoc.documentElement();
        if (xmlRoot.tagName() != "MMProject")
        {
            error(ERR_XML_CONTENT);
        }
        else
        {
            QDomNode xmlNode = xmlRoot.firstChild();
            QDomElement xmlElement;
            while (!xmlNode.isNull())
            {
                xmlElement = xmlNode.toElement();
                if (!xmlElement.isNull())
                {
                    if (xmlElement.tagName() == "GridLayout")
                    {
                        QDomNode xmlNode2 = xmlElement.firstChild();
                        QDomElement xmlElement2;
                        while (!xmlNode2.isNull())
                        {
                            xmlElement2 = xmlNode2.toElement();
                            if (!xmlElement2.isNull())
                            {
                                if (xmlElement2.tagName() == "MovGridLayoutItem")
                                {
                                    qDebug() << "MovGridLayout: MovGridLayoutItem tag in xml with index" << xmlElement2.attribute("index");
                                    MovGridLayoutItem *item = tempGridWidgets.at(xmlElement2.attribute("index").toInt());
                                    this->addItem(item, xmlElement2.attribute("x").toInt(), xmlElement2.attribute("y").toInt());
                                }
                            }
                            xmlNode2 = xmlNode2.nextSibling();
                        }
                    }
                }
                xmlNode = xmlNode.nextSibling();
            }
        }
    }
}



//calculate position of widget (= find first free position)
XY MovGridLayout::calcXY(QWidget *widget)
{
    int w = (widget->width())/sizeRow;
    int h = (widget->height())/sizeCol;
    //qDebug() << "width: " << w << " height: " << h;
    XY xysucc;
    //bool succ = true;
    for (int j = 1; j+h<gridHeight; j++)
    {
        for (int i = 1; i+w<gridWidth; i++)
        {
            if (grid[i+w][j] == -1 &&
                grid[i][j+h] == -1 &&
                grid[i+w][j+h] == -1 &&
                grid[i][j] == -1 &&
                grid[(i+i+w)/2][(j+j+h)/2] == -1 &&
                grid[i][(j+j+h)/2] == -1 &&
                grid[(i+i+w)/2][j] == -1 &&
                grid[i+w][(j+j+h)/2] == -1 &&
                grid[(i+i+w)/2][j+h] == -1)
            {
                //qDebug() << "return";
                xysucc.x = i;
                xysucc.y = j;
                return xysucc;
            }
            /*succ = true;
            for (int i2 = i; i2 < i+w && succ == true; i2++)
            {
                for (int j2 = j; j2 < j+h && succ == true; j2++)
                {
                    if (grid[i2][j2] != -1)
                    {
                        succ = false;
                        qDebug() << "false" << i2 << j2;
                    }
                }
            }
            if (succ == true)
            {
                qDebug() << "return";
                xysucc.x = i;
                xysucc.y = j;
                return xysucc;
            }*/
        }
    }
    qDebug() << "MovGridLayout: resize";
    this->resize(this->width(), this->height() + widget->height()*4);
    gridWidth = this->width()/sizeRow;
    gridHeight = this->height()/sizeCol;
    //grid.resize(gridWidth, vector<int>(gridHeight));
    int **newGrid = (int**) new int*[gridWidth];
    for (int i = 0; i < gridWidth; i++)
    {
        newGrid[i] = (int*) new int[gridHeight];
    }
    for (int i = 0; i<oldWidth/sizeRow; i++)
    {
        for (int j = 0; j<oldHeight/sizeCol; j++)
        {
            newGrid[i][j] = grid[i][j];
        }
    }
    for (int i = 0; i < gridWidth; i++)
    {
        for (int j = 0; j < gridHeight; j++)
        {
            //qDebug() << oldWidth/sizeRow << " new: " << gridWidth;
            //qDebug() << oldHeight/sizeRow << " new: " << gridHeight;
            //qDebug() << i << " " << j;
            if (i >= oldWidth/sizeRow || j >= oldHeight/sizeCol)
                newGrid[i][j] = -1;
        }
    }
    delete grid;
    grid = newGrid;
    //qDebug() << "reallocating";
    oldWidth = this->width();
    oldHeight = this->height();
    for (int i = 0; i+w<gridWidth; i++)
    {
        for (int j = 0; j+h<gridHeight; j++)
        {
            if (grid[i][j] == -1 &&
                grid [i][j+h] == -1)
            {
                //qDebug() << "succ";
                xysucc.x = i;
                xysucc.y = j;
                return xysucc;
            }
        }
    }
    qDebug() << "MovGridLayout: false";
    XY xyfalse;
    xyfalse.x = -1;
    xyfalse.y = -1; 
    return xyfalse;
}


bool MovGridLayout::checkPosition(XY position, WH geometry, int index)
{
    //check grid range
    if (position.x + geometry.w > gridWidth || position.y + geometry.h > gridHeight || position.x < 0 || position.y < 0)
    {
        return false;
    }
    //check grid
    for (int i = position.x; i<position.x+geometry.w; i++)
    {
        for (int j = position.y; j<position.y+geometry.h; j++)
        {
            if (grid[i][j] != -1 && grid[i][j] != index)
            {
                return false;
            }
        }
    }
    return true;
}


//use pointer to project file or its path
void MovGridLayout::saveGridWidgets(QString path)
{
    QFile file(path);
    //qDebug() << "path:" << path;
    QDomDocument domDoc("MMProject");
    QDomElement xmlRoot;
    if (!domDoc.setContent(&file))
    {
        error(ERR_XML_ASSIGN);
        return;
    }
    else
    {
        xmlRoot = domDoc.documentElement();
        if (xmlRoot.tagName() != "MMProject")
        {
            error(ERR_XML_CONTENT);
            return;
        }
        else
        {
            QDomNode xmlNode = xmlRoot.firstChild();
            QDomElement xmlElement;
            while (!xmlNode.isNull())
            {
                xmlElement = xmlNode.toElement();
                if (!xmlElement.isNull())
                {
                    if (xmlElement.tagName() == "GridLayout")
                    {
                        xmlRoot.removeChild(xmlNode);
                        break;
                    }
                }
                xmlNode = xmlNode.nextSibling();
            }
        }
    }

    
    //QDomDocument domDoc("GridLayout");
    QDomElement xmlGrid = domDoc.createElement("GridLayout");
    xmlRoot.appendChild(xmlGrid);

    for (int i = 0; i < gridWidgets.count(); i++)
    {
        QDomElement xmlWidget = domDoc.createElement("MovGridLayoutItem");
        MovGridLayoutItem * actItem = gridWidgets.at(i);
        xmlWidget.setAttribute("index", actItem->index);
        xmlWidget.setAttribute("x", actItem->x);
        xmlWidget.setAttribute("y", actItem->y);
        //xmlWidget.setAttribute("w", actItem->w);
        //xmlWidget.setAttribute("h", actItem->h);
        xmlGrid.appendChild(xmlWidget);
    }
    file.close();
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream xmlStream(&file);
    xmlStream << domDoc.toString();
    file.close();
}


bool MovGridLayout::eventFilter(QObject *target, QEvent *event)
{
    if (target == this)
    {
        //on resize recalculate size of row and col
        /*if (event->type() == QEvent::Resize)
        {
            //FOR STATIC ALLOCATION
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
            }
            //FOR DYNAMIC ALLOCATION
            qDebug() << "resize event";
            gridWidth = this->width()/sizeRow;
            gridHeight = this->height()/sizeCol;
            grid = new (grid) int*[gridWidth];
            for (int i = 0; i < gridWidth; i++)
                grid[i] = new (grid[i]) int[gridHeight];
            if (oldWidth <= this->width() && oldHeight <= this->height())
            {
                for (int i = oldWidth/sizeRow; i < gridWidth; i++)
                    for (int j = oldHeight/sizeCol; j < gridHeight; j++)
                        grid[i][j] = -1;
            }
            qDebug() << "reallocating";
            this->update();
            oldWidth = this->width();
            oldHeight = this->height();
        }*/

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
                int listIndex = findIndex(movingIndex);
                movingWidget.geom.w = gridWidgets.at(listIndex)->w;
                movingWidget.geom.h = gridWidgets.at(listIndex)->h;
                movingWidget.pos.x = mouseEvent->x();
                movingWidget.pos.y = mouseEvent->y();
                grab = true;
                qDebug() << "MovGridLayout: start grab index: " << movingIndex;
                qDebug() << "MovGridLayout: act index: " << listIndex;
                emit dragSignal(gridWidgets.at(listIndex));
            }
            return true;
        }
        //mouse up
        if (event->type() == QEvent::MouseButtonRelease && movingIndex != -1)
        {
            if (grab == true)
            {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
                grab = false;
                qDebug() << "MovGridLayout: stop grab";
                //emit dragSignal(NULL);
                XY position;
                position.x = mouseEvent->x()/sizeRow;
                position.y = mouseEvent->y()/sizeCol;
                WH geometry;
                int listIndex = findIndex(movingIndex);
                geometry.w = gridWidgets.at(listIndex)->w;
                geometry.h = gridWidgets.at(listIndex)->h;
                if (checkPosition(position, geometry, movingIndex) == true)
                {
                    //null previous position
                    //change to gridWidgets.at(movingIndex)->x and y
                    //faster
                    for (int i = 0; i<gridWidth; i++)
                    {
                        for (int j = 0; j<gridHeight; j++)
                        {
                            if (grid[i][j] == movingIndex)
                            {
                                grid[i][j] = -1;
                            }
                        }
                    }
                    //add new position
                    for (int i = position.x; i<position.x+geometry.w; i++)
                    {
                        for (int j = position.y; j<position.y+geometry.h; j++)
                        {
                            grid[i][j] = movingIndex;
                        }
                    }
                    gridWidgets.at(listIndex)->move(position.x, position.y, position.x*sizeRow, position.y*sizeCol);
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
    //for (int i=0; i<gridWidgets.size(); i++)
    //{
        //gridWidgets.at(i)->widget->show();
        //paint.drawRect(gridWidgets.at(i)->x*sizeRow, gridWidgets.at(i)->y*sizeCol, gridWidgets.at(i)->w*sizeRow, gridWidgets.at(i)->h*sizeCol);
        //paint.drawRect(gridWidgets.at(i)->x*sizeRow -1, gridWidgets.at(i)->y*sizeCol -1, gridWidgets.at(i)->w*sizeRow +2, gridWidgets.at(i)->h*sizeCol +2);
    //}
    if (grab == true)
    {
        //fill current mouse position (= new position)
        QBrush brush(Qt::red);
        if (checkPosition(movingWidget.pos, movingWidget.geom, movingIndex) == true)
        {
            brush.setColor(Qt::green);
        }
        paint.fillRect(movingWidget.pos.x*sizeRow -1, movingWidget.pos.y*sizeCol -1, movingWidget.geom.w*sizeRow +2, movingWidget.geom.h*sizeCol +2, brush);
        //fill grabbed widget (= old position)
        brush.setColor(Qt::darkCyan);
        int listIndex = findIndex(movingIndex);
        paint.fillRect(gridWidgets.at(listIndex)->x*sizeRow -1, gridWidgets.at(listIndex)->y*sizeCol -1, gridWidgets.at(listIndex)->w*sizeRow +2, gridWidgets.at(listIndex)->h*sizeCol +2, brush);
    }
    paint.end();
}


int MovGridLayout::actualIndex()
{
    return gridWidgets.size();
}


int MovGridLayout::findIndex(int index)
{
    for (int i = 0; i < gridWidgets.size(); i++)
    {
        qDebug() << "MovGridLayout: find index: " << gridWidgets.at(i)->index;
        if (gridWidgets.at(i)->index == index)
        {
            return i;
        }
    }
    qDebug() << "movgridlayout: index not found";
    qDebug() << "index is: " << index;
    return -1;
}


int MovGridLayout::findTempIndex(int index)
{
    for (int i = 0; i < tempGridWidgets.size(); i++)
    {
        qDebug() << "find temp index: " << tempGridWidgets.at(i)->index;
        if (tempGridWidgets.at(i)->index == index)
        {
            return i;
        }
    }
    qDebug() << "movgridlayout: temp index not found";
    qDebug() << "temp index is: " << index;
    return -1;
}


int MovGridLayout::getX(int index)
{
    return gridWidgets.at(index)->x;
}


int MovGridLayout::getY(int index)
{
    return gridWidgets.at(index)->y;
}


void MovGridLayout::addTempWidget(QWidget *widget)
{
    MovGridLayoutItem *item = new MovGridLayoutItem(this, widget, widget->width()/sizeRow, widget->height()/sizeCol, tempGridWidgets.size());
    widget->hide();
    tempGridWidgets.append(item);
}


MovGridLayoutItem* MovGridLayout::getTempGridWidget(int index)
{
    return tempGridWidgets.at(index);
}


void MovGridLayout::cancelDrag()
{
    this->grab = false;
    movingIndex = -1;
    
}