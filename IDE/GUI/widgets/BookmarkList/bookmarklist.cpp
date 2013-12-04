/**
 * @brief C++ file for BookmarkList class
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
#include "bookmarklist.h"



/**
 * @brief Constructor. Inits sorting by line numbers.
 * @param parent Parent QWidget.
 */
BookmarkList::BookmarkList(QWidget *parent)
    : QListWidget(parent)
{
    this->setSortingEnabled(true);
}


/**
 * @brief Reloads list (clear and load).
 * @param bookmarkList QList of bookmarks' line numbers.
 */
void BookmarkList::reload(QList<int> bookmarkList)
{
    this->clear();
    QList<int>::iterator i;
    for (i = bookmarkList.begin(); i != bookmarkList.end(); i++)
    {
        QListWidgetItem *newItem = new QListWidgetItem(QString::number(*i+1, 10), this);
        this->addItem(newItem);
    }
    qDebug() << "bookmarklist - reload";
}


/**
 * @brief Adds bookmark to the list.
 * @param line Line number of added bookmark.
 */
void BookmarkList::bookmarkListAddSlot(int line)
{
    QListWidgetItem *newItem = new QListWidgetItem(QString::number(line+1, 10), this);
    this->addItem(newItem);
    this->sortItems();
    qDebug() << "bookmarklist - add";
}


/**
 * @brief Removes bookmark from the list.
 * @param line Line number of removed bookmark.
 */
void BookmarkList::bookmarkListRemoveSlot(int line)
{
    QList<QListWidgetItem*> list = this->findItems(QString::number(line+1, 10), Qt::MatchExactly);
    QList<QListWidgetItem*>::iterator i;
    for (i = list.begin(); i != list.end(); i++)
    {
        this->removeItemWidget(*i);
        delete *i;
        *i = NULL;
    }
    qDebug() << "bookmarklist - remove";
}