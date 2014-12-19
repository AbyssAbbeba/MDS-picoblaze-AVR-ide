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
#include "../../../enums/enums.h"



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
        void setQuery(QString query);
        void reloadFocus();

    signals:
        void find(QString query, bool next, bool caseSensitive);
        void closeWidget(CodeEditBottomWidget widget);

    private slots:
        void findNext();
        void findPrevious();
        void closeClicked();

    private:
        Ui_Find ui;

    protected:
        bool eventFilter(QObject *target, QEvent *event);
};

#endif
