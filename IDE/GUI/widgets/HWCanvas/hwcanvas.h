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
 * @file hwcanvas.h
 */


#ifndef HWCANVAS_H
#define HWCANVAS_H


#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWidget>


class HWCanvas : public QWidget
{
    public:
        HWCanvas(QWidget *parent);
};


#endif