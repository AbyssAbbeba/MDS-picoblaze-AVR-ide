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

#include <QListWidget>



/**
 * @brief QListWidget of breakpoints added in CodeEdit
 * @ingroup GUI
 * @class BreakpointList
 */
class BreakpointList : public QListWidget
{
    Q_OBJECT   
    public:
        BreakpointList(QWidget *parent);
        void reload(QList<int> breakpointList);


    private slots:
        void breakpointListAddSlot(int line);
        void breakpointListRemoveSlot(int line);

};

#endif
