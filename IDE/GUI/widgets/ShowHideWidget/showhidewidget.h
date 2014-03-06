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
 * @file showhidewidget.h
 */


#ifndef SHOWHIDEWIDGET_H
#define SHOWHIDEWIDGET_H


#include <QWidget>

class ShowHideWidget : public QWidget
{
    Q_OBJECT
    public:
        ShowHideWidget(QWidget *parent);

    signals:
        void show(bool enable);

    protected:
        virtual void paintEvent ( QPaintEvent * event );

    private:
        bool isShown;
};



#endif