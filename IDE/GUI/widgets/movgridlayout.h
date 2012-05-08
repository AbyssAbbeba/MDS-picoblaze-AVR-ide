#ifndef MOVGRIDLAYOUT_H
#define MOVGRIDLAYOUT_H

#include <QWidget>
#include <QList>

#define SIZEX 50
#define SIZEY 30

class MovGridLayout : public QWidget
{
    Q_OBJECT   
    public:
        MovGridLayout(QWidget *parent);
        void addWidget(QWidget *widget);
        void loadGridWidgets();
    private:
        bool grab;
        QList<QWidget*> gridWidgets;
        //bool **grid;
        bool grid[SIZEX][SIZEY];
};

#endif
