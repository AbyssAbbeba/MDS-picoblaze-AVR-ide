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




/**
 * @brief
 * @ingroup GUI
 * @class JumpToLine
 */
class JumpToLine : public QWidget
{
    Q_OBJECT
    public:
        JumpToLine(QWidget *parent, int maxLines, int currLine);

    signals:
        void jmptoline(int line);

    private slots:
        void emitJump();

    private:
        Ui_JumpToLine ui;

};

#endif
