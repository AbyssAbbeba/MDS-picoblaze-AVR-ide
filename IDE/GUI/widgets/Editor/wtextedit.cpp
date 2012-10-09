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
    highlighter = new Highlighter(this->document());
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
    else if (target == this && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if ((keyEvent->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier))
          && keyEvent->key() == Qt::Key_B)
        {
            qDebug() << "Breakpoint";
            QTextCursor cursor(this->textCursor());
            emit breakpoint(cursor.blockNumber());
            return true;
        }
        else if ((keyEvent->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier))
          && keyEvent->key() == Qt::Key_M)
        {
            qDebug() << "bookMark";
            return true;
        }
    }
    return QWidget::eventFilter(target, event);
}

void WTextEdit::highlightCurrentLine()
{
    QTextCursor cursor(this->textCursor());
    QTextBlockFormat lineFormat = cursor.blockFormat();
    if (lineFormat.background() == Qt::green)
    {
        QPalette palette = this->palette();
        lineFormat.setBackground(palette.color(QPalette::Base));
    }
    else
        lineFormat.setBackground(Qt::green);
    cursor.setBlockFormat(lineFormat);    
}

void WTextEdit::highlightLine(int line)
{
    QTextBlock lineBlock = this->document()->findBlockByNumber(line);
    QTextBlockFormat lineFormat = lineBlock.blockFormat();
    if (lineFormat.background() == Qt::green)
    {
        QPalette palette = this->palette();
        lineFormat.clearBackground();
    }
    else
        lineFormat.setBackground(Qt::green);
    QTextCursor cursor(lineBlock);
    qDebug() << "position: " << cursor.position();
    cursor.setBlockFormat(lineFormat);
}
