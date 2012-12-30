/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 * @ingroup GUI
 * @file breakpointlist.h
 */


#ifndef BREAKPOINTLIST_H
#define BREAKPOINTLIST_H

#include <QListWidget>



/**
 * @brief
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
