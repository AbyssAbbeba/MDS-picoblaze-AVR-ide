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
 * @file helpdockwidget.h
 */


#ifndef HELPDOCKWIDGET_H
#define HELPDOCKWIDGET_H


#include <QWidget>
#include <QUrl>




class HelpDockWidget : public QWidget
{
    Q_OBJECT
    public:
        HelpDockWidget(QWidget *parent);

    signals:
        void showHelpContent(const QUrl &url);
};





#endif