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
 * @file jmptoline.h
 */


#ifndef JUMPTOLINE_H
#define JUMPTOLINE_H

#include <QWidget>
#include "ui_jumptoline.h"
#include "../../../enums/enums.h"




/**
 * @brief
 * @ingroup GUI
 * @class JumpToLine
 */
class JumpToLine : public QWidget
{
    Q_OBJECT
    public:
        JumpToLine(QWidget *parent, int currLine, int maxLines);

    signals:
        void jmpToLine(int line);
        void closeWidget(CodeEditBottomWidget widget);

    private slots:
        void emitJump();
        void closeClicked();

    private:
        Ui_JumpToLine ui;

    protected:
        bool eventFilter(QObject *target, QEvent *event);

};

#endif
