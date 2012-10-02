/**
 * @brief Modif. QTextEdit
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
    this->installEventFilter(this);
    //this->setFocusPolicy(Qt::ClickFocus);
}

bool WTextEdit::eventFilter(QObject *target, QEvent *event)
{
    //track focus event
    if (event->type() == QEvent::FocusIn)
    {
        if (target == this)
        {
            //emit signal for CodeEdit parent
            emit focusIn();
        }
    }
    return QWidget::eventFilter(target, event);
}
