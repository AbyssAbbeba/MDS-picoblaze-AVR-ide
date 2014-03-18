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
 * @file breakpointlist.h
 */


#ifndef BREAKPOINTLIST_H
#define BREAKPOINTLIST_H

#include <QTreeWidget>



/**
 * @brief QTreeWidget of breakpoints added in Project
 * @ingroup GUI
 * @class BreakpointList
 */
class BreakpointList : public QTreeWidget
{
    Q_OBJECT   
    public:
        BreakpointList(QWidget *parent);
        void reload(QList<QPair<QString, unsigned int>> breakpointList);


    public slots:
        void breakpointListAddSlot(QString file, int line);
        void breakpointListRemoveSlot(QString file, int line);
        void breakpointListUpdateSlot(QString file, int linesAdded);

};

#endif
