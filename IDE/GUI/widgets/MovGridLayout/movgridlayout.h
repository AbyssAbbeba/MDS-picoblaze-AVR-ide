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
 * @file movgridlayout.h
 */


#ifndef MOVGRIDLAYOUT_H
#define MOVGRIDLAYOUT_H

#include <QWidget>
#include <QList>
#include <vector>

#define SIZEX 2
#define SIZEY 2

class MovGridLayoutItem;

//using namespace std;


//position - x, y
typedef struct XY
{
    int x;
    int y;
} XY;


//width, height
typedef struct WH
{
    int w;
    int h;
} WH;

//blank widget (for checking position on mouse move)
typedef struct BLANK
{
    XY pos;
    WH geom;
} BLANK;


/**
 * @brief
 * @ingroup GUI
 * @class MovGridLayout
 */
class MovGridLayout : public QWidget
{
    Q_OBJECT
    public:
        //constructor, pointer to parent project maybe?
        MovGridLayout(QWidget *parentWidget);
        //add widget, automatic position
        bool addWidget(QWidget *widget);
        //add widget, specific position
        void addWidget(QWidget *widget, int x, int y);
        //load layout from project xml file
        void loadGridWidgets();
        //save layout to project xml file
        void saveGridWidgets();

    private:
        //check availability of position and geometry of given widget
        bool checkPosition(XY position, WH geometry, int index = -1);
        //calculate valid position for widget
        XY calcXY(QWidget *widget);

        QWidget *parent;
        //if mousebtn is down
        bool grab;
        //index of grabbed widget
        int movingIndex;
        //list of widgets
        QList<MovGridLayoutItem*> gridWidgets;
        //index grid
        //vector< vector<int> >grid;
        int** grid;
        //grid width
        int gridWidth;
        //grid heigth
        int gridHeight;
        //size of row in pixels
        int sizeRow;
        //size of column in pixels
        int sizeCol;
	//params of grabbed widget
	BLANK movingWidget;
        //old geometry on resize
        int oldWidth;
        int oldHeight;

    protected:
        virtual bool eventFilter(QObject *target, QEvent *event);
        void paintEvent(QPaintEvent *);

};

#endif
