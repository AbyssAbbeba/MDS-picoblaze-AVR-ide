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
 * @file tabbar.h
 */


#ifndef TABBAR_H
#define TABBAR_H


#include <QTabBar>
#include <QList>


/**
 * @brief TabBar uses QTabBar as base, adding some functionality (repaint, extended tab management)
 * @ingroup GUI
 * @class TabBar
 */
class TabBar : public QTabBar
{
    Q_OBJECT
    public:
        TabBar(QWidget *parent);
        void tabAdded();
        void tabRemoved(int index);
        void tabChanged(int index, bool changed);
    private slots:
        void tabStatsMoved(int from, int to);
    private:
        QList<bool> tabStats;
    //protected:
    //    void paintEvent(QPaintEvent *);
};


#endif