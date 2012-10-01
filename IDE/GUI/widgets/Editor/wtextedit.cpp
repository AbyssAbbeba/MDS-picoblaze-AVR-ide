/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 *
 */



#include <QtGui>
#include "wtextedit.h"

WTextEdit::WTextEdit(QWidget *parent)
    : QTextEdit(parent)
{
    installEventFilter(this);
}

WTextEdit::eventFilter(QObject *target, QEvent *event)
{
    if (event->type() == QEvent::FocusIn)
    {
        if (target == this)
        {
            return true;
        }
    }
    return QWidget::eventFilter(target, event);
}
