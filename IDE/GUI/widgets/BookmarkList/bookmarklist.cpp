/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup BookmarkList
 * @file bookmarklist.cpp
 */


#include <QtGui>
#include "bookmarklist.h"



/**
 * @brief Constructor. Inits sorting by line number.
 * @param parent Parent QWidget.
 */
BookmarkList::BookmarkList(QWidget *parent)
    : QTreeWidget(parent)
{
    //this->setSortingEnabled(true);
    QStringList labels;
    labels << "Line" << "File";
    this->setHeaderLabels(labels);
    connect(this,
            SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
            this,
            SLOT(bookmarkClickedSlot(QTreeWidgetItem*, int))
           );
}


/**
 * @brief Reloads list (clear and load).
 * @param bookmarkList QList of bookmarks' line numbers.
 */
void BookmarkList::reload(QList<QPair<QString, QSet<unsigned int>>> *bookmarkList)
{
    this->clear();
    QList<unsigned int> values;
    for (int i = 0; i < bookmarkList->count(); i++)
    {
        values = bookmarkList->at(i).second.values();
        for (int j = 0; j < values.count(); j++)
        {
            QTreeWidgetItem *newItem = new QTreeWidgetItem(this);
            newItem->setText(0, QString::number(values.at(j), 10));
            newItem->setText(1, bookmarkList->at(i).first.section('/', -1));
            newItem->setToolTip(1, bookmarkList->at(i).first);
            this->addTopLevelItem(newItem);
        }
    }
    this->sortItems(0, Qt::AscendingOrder);
    this->sortItems(1, Qt::AscendingOrder);
    //qDebug() << "bookmarklist - reload";
}


/**
 * @brief Adds bookmark to the list.
 * @param line Line number of added bookmark.
 */
void BookmarkList::bookmarkListAdd(QString file, int line)
{
    QTreeWidgetItem *newItem = new QTreeWidgetItem(this);
    newItem->setText(0, QString::number(line, 10));
    newItem->setText(1, file.section('/', -1));
    newItem->setToolTip(1, file);
    this->addTopLevelItem(newItem);
    //qDebug() << "bookmarklist - add";
}


/**
 * @brief Removes bookmark from the list.
 * @param line Line number of removed bookmark.
 */
void BookmarkList::bookmarkListRemove(QString file, int line)
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
        qDebug() << "BookmarkList: compare given line" << line << "with item line" << this->topLevelItem(i)->text(0).toInt();
        qDebug() << "BookmarkList: compare given file" << file << "with item file" << this->topLevelItem(i)->toolTip(1);
        if (file == this->topLevelItem(i)->toolTip(1) && line == this->topLevelItem(i)->text(0).toInt())
        {
            QTreeWidgetItem *item = this->takeTopLevelItem(i);
            delete item;
            item = NULL;
            return;
        }
    }
    //qDebug() << "bookmarklist - remove";
}


/**
 * @brief Updates bookmarks for selected file on linecount change
 */
void BookmarkList::bookmarkListUpdate(QString file, int fromLine, int linesAdded)
{
    QList<QTreeWidgetItem*> list = this->findItems(file, Qt::MatchExactly);
    QList<QTreeWidgetItem*>::iterator i;
    for (i = list.begin(); i != list.end(); i++)
    {
        (*i)->setText(0, QString::number((*i)->text(1).toInt() + linesAdded, 10));
    }
    //qDebug() << "bookmarklist - remove";
}


void BookmarkList::bookmarkClickedSlot(QTreeWidgetItem *item, int column)
{
    emit bookmarkClicked(item->toolTip(1), item->text(0).toInt());
}