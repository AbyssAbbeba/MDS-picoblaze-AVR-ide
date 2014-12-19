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
#include "../../../enums/enums.h"



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
        void setQuery(QString query);

    signals:
        void find(QString query, bool next, bool caseSensitive);
        void findAndReplace(QString find, QString replace, bool all, bool caseSensitive);
        void closeWidget(CodeEditBottomWidget widget);

    private slots:
        void findNext();
        void findPrevious();
        void replace();
        void replaceAll();
        void closeClicked();

    private:
        Ui_FindAndReplace ui;

    protected:
        bool eventFilter(QObject *target, QEvent *event);

};

#endif
