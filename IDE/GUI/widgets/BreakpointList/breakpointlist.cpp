/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup BreakpointList
 * @file breakpointlist.cpp
 */


#include <QtGui>
#include "breakpointlist.h"



/**
 * @brief Constructor. Inits sorting by line number.
 * @param parent Parent QWidget.
 */
BreakpointList::BreakpointList(QWidget *parent)
    : QTreeWidget(parent)
{
    //this->setSortingEnabled(true);
    QStringList labels;
    labels << "File" << "Line";
    this->setHeaderLabels(labels);
}


/**
 * @brief Reloads list (clear and load).
 * @param breakpointList QList of breakpoints' line numbers.
 */
void BreakpointList::reload(QList<QPair<QString, unsigned int>> breakpointList)
{
    this->clear();
    for (int i = 0; i != breakpointList.count(); i++)
    {
        QTreeWidgetItem *newItem = new QTreeWidgetItem(this);
        newItem->setText(0, breakpointList.at(i).first);
        newItem->setText(1, QString::number(breakpointList.at(i).second+1, 10));
        this->addTopLevelItem(newItem);
    }
    //qDebug() << "breakpointlist - reload";
}


/**
 * @brief Adds breakpoint to the list.
 * @param line Line number of added breakpoint.
 */
void BreakpointList::breakpointListAddSlot(QString file, int line)
{
    QTreeWidgetItem *newItem = new QTreeWidgetItem(this);
    this->addTopLevelItem(newItem);
    newItem->setText(0, file);
    newItem->setText(1, QString::number(line+1, 10));
    this->sortItems(0, Qt::AscendingOrder);
    //qDebug() << "breakpointlist - add";
}


/**
 * @brief Removes breakpoint from the list.
 * @param line Line number of removed breakpoint.
 */
void BreakpointList::breakpointListRemoveSlot(QString file, int line)
{
    QList<QTreeWidgetItem*> list = this->findItems(file, Qt::MatchExactly);
    QList<QTreeWidgetItem*>::iterator i;
    for (i = list.begin(); i != list.end(); i++)
    {
        this->removeItemWidget(*i, 0);
        delete *i;
        *i = NULL;
    }
    //qDebug() << "breakpointlist - remove";
}


/**
 * @brief Updates breakpoints for selected file on linecount change
 */
void BreakpointList::breakpointListUpdateSlot(QString file, int linesAdded)
{
    QList<QTreeWidgetItem*> list = this->findItems(file, Qt::MatchExactly);
    QList<QTreeWidgetItem*>::iterator i;
    for (i = list.begin(); i != list.end(); i++)
    {
        (*i)->setText(1, QString::number((*i)->text(1).toInt() + linesAdded, 10));
    }
    //qDebug() << "breakpointlist - remove";
}