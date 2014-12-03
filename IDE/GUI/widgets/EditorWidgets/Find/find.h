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
 * @file find.h
 */


#ifndef FIND_H
#define FIND_H

#include <QWidget>
#include "ui_find.h"



/**
 * @brief
 * @ingroup GUI
 * @class Find
 */
class Find : public QWidget
{
    Q_OBJECT
    public:
        Find(QWidget *parent, QString query = QString());

    signals:
        void find(QString query, bool next, bool caseSensitive);

    private slots:
        void findNext();
        void findPrevious();

    private:
        Ui_Find ui;
};

#endif
