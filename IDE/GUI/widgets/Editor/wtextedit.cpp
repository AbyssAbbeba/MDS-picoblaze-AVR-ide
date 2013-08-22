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
    qDebug() << "WTextEdit: WTextEdit()";
    this->sourceType = type;
    this->installEventFilter(this);
    if (this->sourceType != PLAIN)
    {
        highlighter = new Highlighter(this->document(), this->sourceType);
    }
    //this->setFocusPolicy(Qt::ClickFocus);
    qDebug() << "WTextEdit: return WTextEdit()";
}

void WTextEdit::reloadHighlighter(SourceType type)
{
    qDebug() << "WTextEdit: reloadHighlighter()";
    if (this->sourceType != PLAIN)
    {
        delete highlighter;
    }
    if (type != PLAIN)
    {
        highlighter = new Highlighter(this->document(), type);
    }
    this->sourceType = type;
    qDebug() << "WTextEdit: return reloadHighlighter()";
}


bool WTextEdit::eventFilter(QObject *target, QEvent *event)
{
    //qDebug() << "WTextEdit: eventFilter()";
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
            //qDebug() << "WTextEdit: return eventFilter()";
            return true;
        }
        else if ((keyEvent->modifiers() & Qt::ShiftModifier) && (keyEvent->modifiers() & Qt::ControlModifier)
          && keyEvent->key() == Qt::Key_M)
        {
            QTextCursor cursor(this->textCursor());
            emit bookmark(cursor.blockNumber());
            qDebug() << "Bookmark on line:" << cursor.blockNumber()+1;
            //qDebug() << "WTextEdit: return eventFilter()";
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
                {
                    a = a + "*/";
                }
                else if (a.endsWith("*/"))
                {
                    a = "/*" + a;
                }
                else
                {
                    a = "/*" + a + "*/";
                }
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
                {
                    cursor.insertText("//");
                }
                cursor.setPosition(cursor.position() + linePos);
            }
            //qDebug() << "WTextEdit: return eventFilter()";
            return true;
        }
        emit textChangedSignal(keyEvent->text(), this->textCursor().position());
    }
    //qDebug() << "WTextEdit: return eventFilter()";
    return QWidget::eventFilter(target, event);
}

void WTextEdit::highlightCurrentLine()
{
    qDebug() << "WTextEdit: highlightCurrentLine()";
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
    qDebug() << "WTextEdit: return highlightCurrentLine()";
}


void WTextEdit::highlightLine(int line, QColor *color)
{
    qDebug() << "WTextEdit: highlightLine()";
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
            //QColor orig = lineFormat.background().color();
            //origColor = &orig;
            lineFormat.setBackground(*color);
        }
        QTextCursor cursor(lineBlock);
        //qDebug() << "position: " << cursor.position();
        cursor.setBlockFormat(lineFormat);
    }
    else
    {
        qDebug() << "WTextEdit: highlight failed----";
    }
    qDebug() << "WTextEdit: return highlightLine()";
}


void WTextEdit::setPosition(int pos)
{
    qDebug() << "WTextEdit: setPosition()";
    QTextCursor cursor(this->textCursor());
    cursor.setPosition(pos);
    this->setTextCursor(cursor);
    this->verticalScrollBar()->setValue(this->verticalScrollBar()->value());
    qDebug() << "WTextEdit: return setPosition()";
}

int WTextEdit::getPosition()
{
    return this->textCursor().position();
}
