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
 * @file callwatcher.h
 */


#ifndef CALLITEMWIDGET_H
#define CALLITEMWIDGET_H


#include <QWidget>
#include <QString>
#include "ui_callitemwidget.h"



class CallItemWidget : public QWidget
{
    public:
        CallItemWidget(QWidget *parent, QString event, int from, int to);

    private:
        Ui_CallItemWidget ui;
        int numWidth;
};




#endif