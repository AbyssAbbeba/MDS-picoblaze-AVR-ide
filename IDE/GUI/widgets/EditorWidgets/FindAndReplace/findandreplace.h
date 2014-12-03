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
 * @file findandreplace.h
 */


#ifndef FINDANDREPLACE_H
#define FINDANDREPLACE_H

#include <QWidget>
#include "ui_findandreplace.h"



/**
 * @brief
 * @ingroup GUI
 * @class FindAndReplace
 */
class FindAndReplace : public QWidget
{
    Q_OBJECT
    public:
        FindAndReplace(QWidget *parent, QString query = QString());

    signals:
        void find(QString query, bool next, bool caseSensitive);
        void findAndReplace(QString find, QString replace, bool all, bool caseSensitive);

    private slots:
        void findNext();
        void findPrevious();
        void replace();
        void replaceAll();

    private:
        Ui_FindAndReplace ui;

};

#endif
