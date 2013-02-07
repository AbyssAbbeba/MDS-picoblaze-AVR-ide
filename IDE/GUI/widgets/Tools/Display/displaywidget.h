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
 * @file displaywidget.h
 */


#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H


#include <QWidget>


/**
 * @brief
 * @ingroup GUI
 * @class DisplayWidget
 */
class DisplayWidget : public QWidget
{
    Q_OBJECT
    public:
        DisplayWidget(QWidget *parent, int index, bool align, bool comma);
        //~DisplayWidget();

    protected:
        virtual void mousePressEvent(QMouseEvent *event);
        virtual void paintEvent(QPaintEvent *event);

    private:
        int index;
        bool activated;
        QPolygonF *widgetPolygon;

    signals:
        void pressed(int index);
};




#endif
