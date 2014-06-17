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

#include <QTreeWidget>



/**
 * @brief QTreeWidget of bookmarks added in Project
 * @ingroup GUI
 * @class BookmarkList
 */
class BookmarkList : public QTreeWidget
{
    Q_OBJECT
    public:
        BookmarkList(QWidget *parent);
        void reload(QList<QPair<QString, QSet<unsigned int>>> *bookmarkList);
        void bookmarkListAdd(QString file, int line);
        void bookmarkListRemove(QString file, int line);
        void bookmarksAddLines(QString file, int line, int linesAdded);
        void bookmarksRemoveLines(QString file, int line, int linesRemoved);

    private slots:
        void bookmarkClickedSlot(QTreeWidgetItem *item, int column);

    signals:
        void bookmarkClicked(QString file, int line);

};

#endif
