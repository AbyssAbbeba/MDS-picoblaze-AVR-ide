#include <QtGui>
#include "breakpointlist.h"

BreakpointList::BreakpointList(QWidget *parent)
    : QListWidget(parent)
{
    this->setSortingEnabled(true);
}

void BreakpointList::reload(QList<int> breakpointList)
{
    this->clear();
    QList<int>::iterator i;
    for (i = breakpointList.begin(); i != breakpointList.end(); i++)
    {
        QListWidgetItem *newItem = new QListWidgetItem(QString::number(*i+1, 10), this);
        this->addItem(newItem);
    }
    qDebug() << "breakpointlist - reload";
}

void BreakpointList::breakpointListAddSlot(int line)
{
    QListWidgetItem *newItem = new QListWidgetItem(QString::number(line+1, 10), this);
    this->addItem(newItem);
    this->sortItems();
    qDebug() << "breakpointlist - add";
}

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
    qDebug() << "breakpointlist - remove";
}
