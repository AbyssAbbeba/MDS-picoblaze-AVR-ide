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
 * @file bookmarklist.h
 */


#ifndef BOOKMARKLIST_H
#define BOOKMARKLIST_H

#include <QListWidget>



/**
 * @brief
 * @ingroup GUI
 * @class BookmarkList
 */
class BookmarkList : public QListWidget
{
    Q_OBJECT   
    public:
        BookmarkList(QWidget *parent);
        void reload(QList<int> bookmarkList);


    private slots:
        void bookmarkListAddSlot(int line);
        void bookmarkListRemoveSlot(int line);

};

#endif
