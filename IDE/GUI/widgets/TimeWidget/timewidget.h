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
 * @file timewidget.h
 */


#ifndef TIMEWIDGET_H
#define TIMEWIDGET_H


#include <QWidget>
#include "ui_timewidget.h"

class TimeWidget : public QWidget
{
    public:
        TimeWidget(QWidget *parent);
        void setTime(long double time);

    private:
        Ui_TimeWidget ui;
};




#endif