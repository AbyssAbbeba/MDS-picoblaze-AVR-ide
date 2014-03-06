/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
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