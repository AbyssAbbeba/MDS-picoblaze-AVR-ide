/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup WTextEdit
 * @file wtextedit.cpp
 */


#include <QtGui>
#include "wtextedit.h"

WTextEdit::WTextEdit(QWidget *parent, SourceType type)
    : QPlainTextEdit(parent)
{
    //qDebug() << "WTextEdit: WTextEdit()";
    this->sourceType = type;
    this->prevBlock = 0;
    this->installEventFilter(this);
    if (this->sourceType != PLAIN)
    {
        highlighter = new Highlighter(this->document(), this->sourceType);
    }
    this->setAcceptDrops(true);

    connect(this,
            SIGNAL(cursorPositionChanged()),
            this,
            SLOT(cursorPositionChangedSlot())
           );
    /*connect(this->document(),
            SIGNAL(contentsChange(int,int,int)),
            this,
            SLOT(updateUndoRedo(int,int,int))
           );*/

    //this->show();
    //this->setFocusPolicy(Qt::ClickFocus);
    //qDebug() << "WTextEdit: return WTextEdit()";
}

void WTextEdit::reloadHighlighter(SourceType type)
{
    //qDebug() << "WTextEdit: reloadHighlighter()";
    if (this->sourceType != PLAIN)
    {
        delete highlighter;
    }
    if (type != PLAIN)
    {
        highlighter = new Highlighter(this->document(), type);
    }
    this->sourceType = type;
    //qDebug() << "WTextEdit: return reloadHighlighter()";
}


bool WTextEdit::eventFilter(QObject *target, QEvent *event)
{
    //qDebug() << "WTextEdit: eventFilter()";
    //qDebug() << "WTextEdit: event type" << event->type();
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
        //breakpoint ctrl+shift+b
        if ((keyEvent->modifiers() & Qt::ShiftModifier) && (keyEvent->modifiers() & Qt::ControlModifier)
          && keyEvent->key() == Qt::Key_B)
        {
            QTextCursor cursor(this->textCursor());
            emit breakpoint(cursor.blockNumber());
            //qDebug() << "Breakpoint on line:" << cursor.blockNumber()+1;
            //qDebug() << "WTextEdit: return eventFilter()";
            return true;
        }
        //bookmark ctrl+shift+m
        if ((keyEvent->modifiers() & Qt::ShiftModifier) && (keyEvent->modifiers() & Qt::ControlModifier)
          && keyEvent->key() == Qt::Key_M)
        {
            QTextCursor cursor(this->textCursor());
            emit bookmark(cursor.blockNumber());
            //qDebug() << "Bookmark on line:" << cursor.blockNumber()+1;
            //qDebug() << "WTextEdit: return eventFilter()";
            return true;
        }
        //comment ctrl+shift+c
        if ((keyEvent->modifiers() & Qt::ShiftModifier) && (keyEvent->modifiers() & Qt::ControlModifier)
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
                //emit selectionRemovedSignal(cursor.selectionStart(), cursor.selectionEnd());
                cursor.removeSelectedText();
                //emit textChangedSignal(a, cursor.position());
                cursor.insertText(a);
            }
            else
            {
                int linePos = cursor.positionInBlock();
                cursor.setPosition(cursor.position() - linePos);
                if (cursor.block().text().startsWith(";"))
                {
                    //emit textChangedSignal(QKeySequence(Qt::Key_Backspace).toString(), cursor.position());
                    cursor.deleteChar();
                    cursor.setPosition(cursor.position() + linePos - 1);
                    //emit textChangedSignal(QKeySequence(Qt::Key_Backspace).toString(), cursor.position());
                    //cursor.deleteChar();
                }
                else
                {
                    //emit textChangedSignal(";", cursor.position());
                    cursor.insertText(";");
                    cursor.setPosition(cursor.position() + linePos);
                }
            }
            //qDebug() << "WTextEdit: return eventFilter()";
            return true;
        }
        //paste ctrl+v
        /*if ( (keyEvent->modifiers() & Qt::ControlModifier)
          && (keyEvent->key() == Qt::Key_V)
           )
        {
            if (true == this->textCursor().hasSelection())
            {
                emit selectionRemovedSignal(this->textCursor().selectionStart(), this->textCursor().selectionEnd());
                this->textCursor().removeSelectedText();
            }
            emit textChangedSignal(QApplication::clipboard()->text(), this->textCursor().position());
            this->textCursor().insertText(QApplication::clipboard()->text());
            return true;
        }
        //cut ctrl+x
        if ( (keyEvent->modifiers() & Qt::ControlModifier)
          && (keyEvent->key() == Qt::Key_X)
           )
        {
            if (true == this->textCursor().hasSelection())
            {
                QApplication::clipboard()->setText(this->textCursor().selectedText());
                emit selectionRemovedSignal(this->textCursor().selectionStart(), this->textCursor().selectionEnd());
                this->textCursor().removeSelectedText();
            }
            return true;
        }*/
        //undo/redo ctrl+z (+shift)
        /*if ( (keyEvent->modifiers() & Qt::ControlModifier)
          && (keyEvent->key() == Qt::Key_Z)
           )
        {
            //redo ctrl+shift+z
            if ((keyEvent->modifiers() & Qt::ShiftModifier))
            {
                //this->redoRequest = true;
                this->redo();
                //this->redoRequest = false;
            }
            //undo ctrl+z
            else
            {
                //this->undoRequest = true;
                this->undo();
                //this->undoRequest = false;
            }
            return true;
        }*/
        //other keys
        /*if ( ((keyEvent->key() >= Qt::Key_Space && keyEvent->key() <= Qt::Key_Z)
          && (!(keyEvent->modifiers() & Qt::ControlModifier)))
          || (keyEvent->key() == Qt::Key_Backspace)
          || (keyEvent->key() == Qt::Key_Tab)
          || (keyEvent->key() == Qt::Key_Delete)
          || (keyEvent->key() == Qt::Key_Enter)
          || (keyEvent->key() == Qt::Key_Return)
           )
        {
            bool select = this->textCursor().hasSelection();
            if (true == select)
            {
                emit selectionRemovedSignal(this->textCursor().selectionStart(), this->textCursor().selectionEnd());
                this->textCursor().removeSelectedText();
            }
            if (keyEvent->key() != Qt::Key_Backspace && keyEvent->key() != Qt::Key_Delete)
            {
                emit textChangedSignal(keyEvent->text(), this->textCursor().position());
            }
            else 
            {
                if (false == select)
                {
                    emit textChangedSignal(keyEvent->text(), this->textCursor().position());
                }
                else
                {
                    return true;
                }
            }
        }*/
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


bool WTextEdit::highlightLine(int line, QColor *color)
{
    //qDebug() << "WTextEdit: highlightLine()";
    //qDebug() << "WTextEdit: highlighted line is" << line;
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
            //lineFormat.setBackground(Qt::red);
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
        this->setTextCursor(cursor);
        this->ensureCursorVisible();
        //qDebug() << "WTextEdit: return highlightLine()";
        return true;
    }
    /*else
    {
        qDebug() << "WTextEdit: highlight failed----";
    }*/
    //qDebug() << "WTextEdit: return highlightLine()";
    return false;
}


bool WTextEdit::isLineHighlighted(int line, QColor *color)
{
    if (line >= 0 && line <= this->document()->lineCount())
    {
        QTextBlock lineBlock = this->document()->findBlockByNumber(line);
        QTextBlockFormat lineFormat = lineBlock.blockFormat();
        if (lineFormat.background().color().rgb() == color->rgb())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}


void WTextEdit::setPosition(int pos)
{
    //qDebug() << "WTextEdit: setPosition()";
    QTextCursor cursor(this->textCursor());
    cursor.setPosition(pos);
    this->setTextCursor(cursor);
    this->verticalScrollBar()->setValue(this->verticalScrollBar()->value());
    //qDebug() << "WTextEdit: return setPosition()";
}


int WTextEdit::getPosition()
{
    return this->textCursor().position();
}


void WTextEdit::scrollToLine(int line)
{
    //qDebug() << "WTextEdit: scrollToLine()";
    //qDebug() << "WTextEdit: curr value:" <<  this->verticalScrollBar()->value() << "max value:" <<  this->verticalScrollBar()->maximum() << "calculated value:" << (line / this->document()->lineCount()) * this->verticalScrollBar()->maximum();
    //qDebug() << "WTextEdit: line" << line << "line count:" << this->document()->lineCount() << "value:" << (double)line / (double)this->document()->lineCount();
    //this->ensureCursorVisible();
    this->verticalScrollBar()->setValue((int)
                                            (((double)line / (double)this->document()->lineCount())
                                            * this->verticalScrollBar()->maximum())
                                        );
    //qDebug() << "WTextEdit: return scrollToLine()";
}


void WTextEdit::deselect()
{
    QTextCursor cursor = this->textCursor();
    cursor.clearSelection();
    this->setTextCursor(cursor);
}


void WTextEdit::selectLine(int line)
{
    QTextCursor cursor(this->document()->findBlockByNumber(line-1));
    cursor.select(QTextCursor::LineUnderCursor);
    this->setTextCursor(cursor);
}


/*void WTextEdit::updateUndoRedo(int position, int charsRemoved, int charsAdded)
{
    if (true == this->undoRequest)
    {
        this->undoRequest = false;
        qDebug() << "WTextEdit: >>> undo request";
        qDebug() << "WTextEdit: position" << position;
        qDebug() << "WTextEdit: charsRemoved" << charsRemoved;
        qDebug() << "WTextEdit: charsAdded" << charsAdded;
        qDebug() << "WTextEdit: cursor prev position" << this->textCursor().position();
        if (charsAdded > 0)
        {
            QTextCursor cursor(this->textCursor());
            if (position == cursor.position())
            {
                cursor.setPosition(position+charsAdded, QTextCursor::KeepAnchor);
            }
            else
            {
                cursor.setPosition(position, QTextCursor::KeepAnchor);
            }
            qDebug() << "WTextEdit: has selection" << cursor.hasSelection();
            qDebug() << "WTextEdit: undo selection" << cursor.selectedText();
            qDebug() << "WTextEdit: cursor anchor" << cursor.anchor();
            qDebug() << "WTextEdit: cursor position" << cursor.position();
            emit textChangedSignal(cursor.selectedText(), position);
            if (charsRemoved > 0)
            {
                for (int i = 0; i < charsRemoved; i++)
                {
                    emit textChangedSignal("\177", cursor.position());
                }
            }
        }
        else if (charsRemoved > 0)
        {
            for (int i = 0; i < charsRemoved; i++)
            {
                emit textChangedSignal("\177", position);
            }
        }
        return;
    }
    if (true == this->redoRequest)
    {
        this->redoRequest = false;
        qDebug() << "WTextEdit: >>> redo request";
        qDebug() << "WTextEdit: position" << position;
        qDebug() << "WTextEdit: charsRemoved" << charsRemoved;
        qDebug() << "WTextEdit: charsAdded" << charsAdded;
        qDebug() << "WTextEdit: cursor prev position" << this->textCursor().position();
        if (charsAdded > 0)
        {
            QTextCursor cursor(this->textCursor());
            if (position == cursor.position())
            {
                cursor.setPosition(position+charsAdded, QTextCursor::KeepAnchor);
            }
            else
            {
                cursor.setPosition(position, QTextCursor::KeepAnchor);
            }
            qDebug() << "WTextEdit: has selection" << cursor.hasSelection();
            qDebug() << "WTextEdit: redo selection" << cursor.selectedText();
            qDebug() << "WTextEdit: cursor anchor" << cursor.anchor();
            qDebug() << "WTextEdit: cursor position" << cursor.position();
            emit textChangedSignal(cursor.selectedText(), position);
            if (charsRemoved > 0)
            {
                for (int i = 0; i < charsRemoved; i++)
                {
                    emit textChangedSignal("\177", cursor.position());
                }
            }
        }
        else if (charsRemoved > 0)
        {
            for (int i = 0; i < charsRemoved; i++)
            {
                emit textChangedSignal("\177", position);
            }
        }
        return;
    }
}


void WTextEdit::editedUndo()
{
    this->undoRequest = true;
    this->undo();
    this->undoRequest = false;
}


void WTextEdit::editedRedo()
{
    this->redoRequest = true;
    this->redo();
    this->redoRequest = false;
}


void WTextEdit::editedPaste()
{
    if (true == this->textCursor().hasSelection())
    {
        emit selectionRemovedSignal(this->textCursor().selectionStart(), this->textCursor().selectionEnd());
        this->textCursor().removeSelectedText();
    }
    emit textChangedSignal(QApplication::clipboard()->text(), this->textCursor().position());
    this->textCursor().insertText(QApplication::clipboard()->text());
}


void WTextEdit::editedCut()
{
    if (true == this->textCursor().hasSelection())
    {
        QApplication::clipboard()->setText(this->textCursor().selectedText());
        emit selectionRemovedSignal(this->textCursor().selectionStart(), this->textCursor().selectionEnd());
        this->textCursor().removeSelectedText();
    }
}*/


void WTextEdit::cursorPositionChangedSlot()
{
    if (this->textCursor().blockNumber() != this->prevBlock)
    {
        this->prevBlock = this->textCursor().blockNumber();
        emit updateLineCounter();
    }
}