#ifndef MOVGRIDLAYOUT_H
#define MOVGRIDLAYOUT_H

#include <QWidget>
#include <QList>

#define SIZEX 50
#define SIZEY 30

class MovGridLayoutItem;

typedef struct XY
{
    int x;
    int y;
} XY;


class MovGridLayout : QWidget
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
        //calculate valid position for widget
        XY calcXY(QWidget *widget);
        //save layout to project xml file
        void saveGridWidgets();
    private:
         QWidget *parent;
        //if mousebtn is down
        bool grab;
        //list of widgets
        QList<MovGridLayoutItem*> gridWidgets;
        //static, for now
        int grid[SIZEX][SIZEY];
        //size of row in pixels
        int sizeRow;
        //size of column in pixels
        int sizeCol;
};

#endif
