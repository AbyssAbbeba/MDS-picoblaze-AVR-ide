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
    labels << "Line" << "File";
    this->setHeaderLabels(labels);
    this->setSortingEnabled(false);
    connect(this,
            SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
            this,
            SLOT(breakpointClickedSlot(QTreeWidgetItem*, int))
           );
}


/**
 * @brief Reloads list (clear and load).
 * @param breakpointList QList of breakpoints' line numbers.
 */
void BreakpointList::reload(QList<QPair<QString, QSet<unsigned int>>> *breakpointList)
{
    this->clear();
    QList<unsigned int> values; 
    for (int i = 0; i < breakpointList->count(); i++)
    {
        values = breakpointList->at(i).second.values();
        for (int j = 0; j < values.count(); j++)
        {
            QTreeWidgetItem *newItem = new QTreeWidgetItem(this);
            newItem->setText(0, QString::number(values.at(j), 10));
            newItem->setText(1, breakpointList->at(i).first.section('/', -1));
            newItem->setToolTip(1, breakpointList->at(i).first);
            this->addTopLevelItem(newItem);
        }
    }
    //qDebug() << "breakpointlist - reload";
}


/**
 * @brief Adds breakpoint to the list.
 * @param line Line number of added breakpoint.
 */
void BreakpointList::breakpointListAdd(QString file, int line)
{
    QTreeWidgetItem *newItem = new QTreeWidgetItem();
    newItem->setText(0, QString::number(line, 10));
    newItem->setText(1, file.section('/', -1));
    newItem->setToolTip(1, file);
    QTreeWidgetItem *curItem;
    int index = 0;
    for (; index < this->topLevelItemCount(); index++)
    {
        curItem = this->topLevelItem(index);
        if (curItem->toolTip(1) == file)
        {
            if (curItem->text(0).toInt() > line)
            {
                //qDebug() << "BreakpointList: file found, breaking on index" << index;
                //qDebug() << "BreakpointList: file" << file << "tooltip" << curItem->toolTip(1);
                break;
            }
        }
        else
        {
            if (curItem->toolTip(1) > file)
            {
                //qDebug() << "BreakpointList: breaking on index" << index;
                //qDebug() << "BreakpointList: file" << file << "tooltip" << curItem->toolTip(1);
                break;
            }
        }
    }
    //qDebug() << "BreakpointList: inserting on index" << index;
    //qDebug() << "BreakpointList: total items" << this->topLevelItemCount();
    this->insertTopLevelItem(index, newItem);
    //this->sortItems(0, Qt::AscendingOrder);
    //this->sortItems(1, Qt::AscendingOrder);
    //qDebug() << "breakpointlist - add";
}


/**
 * @brief Removes breakpoint from the list.
 * @param line Line number of removed breakpoint.
 */
void BreakpointList::breakpointListRemove(QString file, int line)
{
    /*QList<QTreeWidgetItem*> list = this->findItems(file, Qt::MatchExactly);
    QList<QTreeWidgetItem*>::iterator i;
    for (i = list.begin(); i != list.end(); i++)
    {
        this->removeItemWidget(*i, 0);
        delete *i;
        *i = NULL;
    }*/
    for (int i = 0; i < this->topLevelItemCount(); i++)
    {
        //qDebug() << "BreakpointList: compare given line" << line << "with item line" << this->topLevelItem(i)->text(0).toInt();
        //qDebug() << "BreakpointList: compare given file" << file << "with item file" << this->topLevelItem(i)->toolTip(1);
        if (file == this->topLevelItem(i)->toolTip(1) && line == this->topLevelItem(i)->text(0).toInt())
        {
            QTreeWidgetItem *item = this->takeTopLevelItem(i);
            delete item;
            item = NULL;
            return;
        }
    }
    //this->sortItems(0, Qt::AscendingOrder);
    //this->sortItems(1, Qt::AscendingOrder);
    //qDebug() << "breakpointlist - remove";
}


void BreakpointList::breakpointsAddLines(QString file, int line, int linesAdded)
{
    for (int i = 0; i < this->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = this->topLevelItem(i);
        if (item->toolTip(1) == file)
        {
            if (item->text(0).toInt() > line)
            {
                item->setText(0, QString::number(item->text(0).toInt() + linesAdded, 10));
            }
        }
    }
}


void BreakpointList::breakpointsRemoveLines(QString file, int line, int linesRemoved)
{
    //qDebug() << "BreakpointList: breakpointsRemoveLines" << file << line << linesRemoved;
    QList<QTreeWidgetItem*> itemsToRemove;
    for (int i = 0; i < this->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = this->topLevelItem(i);
        if (item->toolTip(1) == file)
        {
            if ( item->text(0).toInt() >= line
            && item->text(0).toInt() < line + linesRemoved
            )
            {
                itemsToRemove.append(item);
            }
            else
            {
                if (item->text(0).toInt() >= line + linesRemoved)
                {
                    item->setText(0, QString::number(item->text(0).toInt() - linesRemoved, 10));
                }
            }
        }
    }
    if (false == itemsToRemove.empty())
    {
        QList<QTreeWidgetItem*>::iterator i;
        for (i = itemsToRemove.begin(); i != itemsToRemove.end(); i++)
        { 
            //this->removeItemWidget(*i, 0);
            delete *i;
            *i = NULL;
        }
    }
}


void BreakpointList::breakpointClickedSlot(QTreeWidgetItem *item, int /*column*/)
{
    emit breakpointClicked(item->toolTip(1), item->text(0).toInt());
}