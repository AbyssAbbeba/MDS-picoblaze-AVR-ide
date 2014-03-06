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
 * @file bookmarklist.h
 */


#ifndef BOOKMARKLIST_H
#define BOOKMARKLIST_H

#include <QListWidget>



/**
 * @brief QListWidget of bookmarks added to CodeEdit
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
