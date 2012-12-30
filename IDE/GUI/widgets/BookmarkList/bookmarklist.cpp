#include <QtGui>
#include "bookmarklist.h"

BookmarkList::BookmarkList(QWidget *parent)
    : QListWidget(parent)
{
    this->setSortingEnabled(true);
}

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

void BookmarkList::bookmarkListAddSlot(int line)
{
    QListWidgetItem *newItem = new QListWidgetItem(QString::number(line+1, 10), this);
    this->addItem(newItem);
    this->sortItems();
    qDebug() << "bookmarklist - add";
}

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
