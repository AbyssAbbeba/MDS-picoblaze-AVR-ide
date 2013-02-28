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

WTextEdit::WTextEdit(QWidget *parent, SourceType type)
    : QTextEdit(parent)
{
    this->installEventFilter(this);
    highlighter = new Highlighter(this->document(), type);
    //this->sourceType = type;
    //this->setFocusPolicy(Qt::ClickFocus);
}

void WTextEdit::reloadHighlighter(SourceType type)
{
    delete highlighter;
    highlighter = new Highlighter(this->document(), type);
    //this->sourceType = type;
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
        if ((keyEvent->modifiers() & Qt::ShiftModifier) && (keyEvent->modifiers() & Qt::ControlModifier)
          && keyEvent->key() == Qt::Key_B)
        {
            QTextCursor cursor(this->textCursor());
            emit breakpoint(cursor.blockNumber());
            qDebug() << "Breakpoint on line:" << cursor.blockNumber()+1;
            return true;
        }
        else if ((keyEvent->modifiers() & Qt::ShiftModifier) && (keyEvent->modifiers() & Qt::ControlModifier)
          && keyEvent->key() == Qt::Key_M)
        {
            QTextCursor cursor(this->textCursor());
            emit bookmark(cursor.blockNumber());
            qDebug() << "Bookmark on line:" << cursor.blockNumber()+1;
            return true;
        }
        else if ((keyEvent->modifiers() & Qt::ShiftModifier) && (keyEvent->modifiers() & Qt::ControlModifier)
          && keyEvent->key() == Qt::Key_C)
        {
            QTextCursor cursor(this->textCursor());
            if (cursor.hasSelection())
            {
                QString a(cursor.selectedText());
                if (a.startsWith("/*") && a.endsWith("*/"))
                {
                    a.remove(0, 2);
                    a.remove(a.size() - 2, 2);
                }
                else if (a.startsWith("/*"))
                    a = a + "*/";
                else if (a.endsWith("*/"))
                    a = "/*" + a;
                else
                    a = "/*" + a + "*/";
                cursor.removeSelectedText();
                cursor.insertText(a); 
            }
            else
            {
                int linePos = cursor.positionInBlock();
                cursor.setPosition(cursor.position() - linePos);
                if (cursor.block().text().startsWith("//"))
                {
                    cursor.deleteChar();
                    cursor.deleteChar();
                }
                else
                    cursor.insertText("//");
                cursor.setPosition(cursor.position() + linePos);
            }
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
        //QPalette palette = this->palette();
        //lineFormat.setBackground(palette.color(QPalette::Base));
        lineFormat.clearBackground();
    }
    else
    {
        lineFormat.setBackground(Qt::green);
    }
    cursor.setBlockFormat(lineFormat);
}


void WTextEdit::highlightLine(int line, QColor *color, QColor *origColor)
{
    qDebug() << "WTextEdit: highlighted line is" << line;
    //origColor = NULL;
    if (line >= 0 && line <= this->document()->lineCount())
    {
        QTextBlock lineBlock = this->document()->findBlockByNumber(line);
        QTextBlockFormat lineFormat = lineBlock.blockFormat();
        if (color == NULL)
        {
            //QPalette palette = this->palette();
            //lineFormat.setBackground(palette.color(QPalette::Base));
            lineFormat.clearBackground();
        }
        else
        {
            QColor orig = lineFormat.background().color();
            origColor = &orig;
            lineFormat.setBackground(*color);
        }
        QTextCursor cursor(lineBlock);
        //qDebug() << "position: " << cursor.position();
        cursor.setBlockFormat(lineFormat);
    }
    else
    {
        qDebug() << "fail--------------------------------";
    }
}


void WTextEdit::setPosition(int pos)
{
    QTextCursor cursor(this->textCursor());
    cursor.setPosition(pos);
    this->setTextCursor(cursor);
    this->verticalScrollBar()->setValue(this->verticalScrollBar()->value());
}

int WTextEdit::getPosition()
{
    return this->textCursor().position();
}
