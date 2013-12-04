/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2013
 * @ingroup GUI
 * @file graphwidget.h
 */


#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H


#include <QWidget>

class GraphWidget : public QWidget
{
    public:
        GraphWidget(QWidget *parent);


    protected:
        void paintEvent(QPaintEvent *);
};



#endif