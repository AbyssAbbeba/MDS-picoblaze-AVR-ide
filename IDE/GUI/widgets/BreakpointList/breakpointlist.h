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
        void reload(QList<QPair<QString, QSet<unsigned int>>> *breakpointList);
        void breakpointListAdd(QString file, int line);
        void breakpointListRemove(QString file, int line);
        void breakpointsAddLines(QString file, int line, int linesAdded);
        void breakpointsRemoveLines(QString file, int line, int linesRemoved);

    private slots:
        void breakpointClickedSlot(QTreeWidgetItem *item, int column);

    signals:
        void breakpointClicked(QString file, int line);

};

#endif
