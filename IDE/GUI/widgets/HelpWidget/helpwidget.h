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
 * @file helpwidget.h
 */


#ifndef HELPWIDGET_H
#define HELPWIDGET_H


#include <QWidget>

class HelpBrowser;



class HelpWidget : public QWidget
{
    public:
        HelpWidget(QWidget *parent, int width = 600, int height = 800);

    private:
        HelpBrowser *textBrowser;
};





#endif