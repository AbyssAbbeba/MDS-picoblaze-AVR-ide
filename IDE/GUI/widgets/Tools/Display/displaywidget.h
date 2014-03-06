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
        void activate(bool active);
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
