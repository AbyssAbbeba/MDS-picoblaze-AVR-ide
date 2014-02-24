/**
 * @brief C++ file for BreakpointList class
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 *
 */


#include <QtGui>
#include "breakpointlist.h"



/**
 * @brief Constructor. Inits sorting by line number.
 * @param parent Parent QWidget.
 */
BreakpointList::BreakpointList(QWidget *parent)
    : QListWidget(parent)
{
    this->setSortingEnabled(true);
}


/**
 * @brief Reloads list (clear and load).
 * @param breakpointList QList of breakpoints' line numbers.
 */
void BreakpointList::reload(QList<int> breakpointList)
{
    this->clear();
    QList<int>::iterator i;
    for (i = breakpointList.begin(); i != breakpointList.end(); i++)
    {
        QListWidgetItem *newItem = new QListWidgetItem(QString::number(*i+1, 10), this);
        this->addItem(newItem);
    }
    //qDebug() << "breakpointlist - reload";
}


/**
 * @brief Adds breakpoint to the list.
 * @param line Line number of added breakpoint.
 */
void BreakpointList::breakpointListAddSlot(int line)
{
    QListWidgetItem *newItem = new QListWidgetItem(QString::number(line+1, 10), this);
    this->addItem(newItem);
    this->sortItems();
    //qDebug() << "breakpointlist - add";
}


/**
 * @brief Removes breakpoint from the list.
 * @param line Line number of removed breakpoint.
 */
void BreakpointList::breakpointListRemoveSlot(int line)
{
    QList<QListWidgetItem*> list = this->findItems(QString::number(line+1, 10), Qt::MatchExactly);
    QList<QListWidgetItem*>::iterator i;
    for (i = list.begin(); i != list.end(); i++)
    {
        this->removeItemWidget(*i);
        delete *i;
        *i = NULL;
    }
    //qDebug() << "breakpointlist - remove";
}