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
#include "../Highlighter/highlighter.h"
#include "../../errordialog/errordlg.h"


WTextEdit::WTextEdit(QWidget *parent, SourceType type)
    : QPlainTextEdit(parent)
{
    //qDebug() << "WTextEdit: WTextEdit()";
    //qDebug() << "WTextEdit: sourceType" << type;
    highlighter = NULL;
    this->sourceType = type;
    //qDebug() << "WTextEdit: present sourceType" << this->sourceType;
    this->prevBlock = 0;
    this->cursorLineColor = new QColor(0xE9, 0xE9, 0xE9);
    this->installEventFilter(this);
    if (this->sourceType != PLAIN)
    {
        highlighter = new Highlighter(this->document(), this->sourceType);
    }
    //this->setAcceptDrops(true);
    this->makeMenu();

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
    //this->setFocusPolicy(Qt::StrongFocus);
    //qDebug() << "WTextEdit: return WTextEdit()";
}

void WTextEdit::reloadHighlighter(SourceType type)
{
    //qDebug() << "WTextEdit: reloadHighlighter()";
    //qDebug() << "WTextEdit: sourceType" << type;
    //qDebug() << "WTextEdit: present sourceType" << this->sourceType;
    if (this->sourceType != PLAIN && highlighter != NULL)
    {
        delete highlighter;
        highlighter = NULL;
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
        //new line
        //check for selection
        if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
        {
            QTextCursor cursor(this->textCursor());
            if (false == cursor.hasSelection())
            {
                emit breakpointsAddLines(cursor.blockNumber(), 1);
                emit bookmarksAddLines(cursor.blockNumber(), 1);
            }
            else
            {
                QTextCursor cursorSelBeg(this->document());
                QTextCursor cursorSelEnd(this->document());
                cursorSelBeg.setPosition(cursor.selectionStart());
                cursorSelEnd.setPosition(cursor.selectionEnd());
                if (cursorSelBeg.blockNumber() != cursorSelEnd.blockNumber())
                {
                    emit breakpointsRemoveLines(cursorSelBeg.blockNumber() + 1,
                                                cursorSelEnd.blockNumber() - cursorSelBeg.blockNumber()
                                               );
                    emit bookmarksRemoveLines(cursorSelBeg.blockNumber() + 1,
                                                cursorSelEnd.blockNumber() - cursorSelBeg.blockNumber()
                                               );
                }
                emit breakpointsAddLines(cursorSelBeg.blockNumber(), 1);
                emit bookmarksAddLines(cursorSelBeg.blockNumber(), 1);
            }
        }
        //backspace on the beginning of the line
        //check for selection
        if (keyEvent->key() == Qt::Key_Backspace)
        {
            QTextCursor cursor(this->textCursor());
            if (false == cursor.hasSelection())
            {
                if (true == cursor.atBlockStart() && cursor.blockNumber() > 1) //bug
                {
                    emit breakpointsRemoveLines(cursor.blockNumber(), 1);
                    emit bookmarksRemoveLines(cursor.blockNumber(), 1);
                }
            }
            else
            {
                QTextCursor cursorSelBeg(this->document());
                QTextCursor cursorSelEnd(this->document());
                cursorSelBeg.setPosition(cursor.selectionStart());
                cursorSelEnd.setPosition(cursor.selectionEnd());
                if (cursorSelBeg.blockNumber() != cursorSelEnd.blockNumber())
                {
                    if (true == cursorSelBeg.atBlockStart())
                    {
                        emit breakpointsRemoveLines(cursorSelBeg.blockNumber(),
                                                    //cursorSelEnd.blockNumber() - cursorSelBeg.blockNumber() + 1
                                                    cursorSelEnd.blockNumber() - cursorSelBeg.blockNumber()
                                                   );
                        emit bookmarksRemoveLines(cursorSelBeg.blockNumber(),
                                                    //cursorSelEnd.blockNumber() - cursorSelBeg.blockNumber() + 1
                                                    cursorSelEnd.blockNumber() - cursorSelBeg.blockNumber()
                                                   );
                    }
                    else
                    {
                        emit breakpointsRemoveLines(cursorSelBeg.blockNumber() + 1,
                                                    cursorSelEnd.blockNumber() - cursorSelBeg.blockNumber()
                                                   );
                        emit bookmarksRemoveLines(cursorSelBeg.blockNumber() + 1,
                                                    cursorSelEnd.blockNumber() - cursorSelBeg.blockNumber()
                                                   );
                    }
                }
            }
        }
        //delete on the end of the line
        //check for selection
        if (keyEvent->key() == Qt::Key_Delete)
        {
            QTextCursor cursor(this->textCursor());
            if (false == cursor.hasSelection())
            {
                if (true == cursor.atBlockEnd())
                {
                    emit breakpointsRemoveLines(cursor.blockNumber() + 1, 1);
                    emit bookmarksRemoveLines(cursor.blockNumber() + 1, 1);
                }
            }
            else
            {
                QTextCursor cursorSelBeg(this->document());
                QTextCursor cursorSelEnd(this->document());
                cursorSelBeg.setPosition(cursor.selectionStart());
                cursorSelEnd.setPosition(cursor.selectionEnd());
                if (cursorSelBeg.blockNumber() != cursorSelEnd.blockNumber())
                {
                    if (true == cursorSelBeg.atBlockStart())
                    {
                        emit breakpointsRemoveLines(cursorSelBeg.blockNumber(),
                                                    cursorSelEnd.blockNumber() - cursorSelBeg.blockNumber() + 1
                                                   );
                        emit bookmarksRemoveLines(cursorSelBeg.blockNumber(),
                                                    cursorSelEnd.blockNumber() - cursorSelBeg.blockNumber() + 1
                                                   );
                    }
                    else
                    {
                        emit breakpointsRemoveLines(cursorSelBeg.blockNumber() + 1,
                                                    cursorSelEnd.blockNumber() - cursorSelBeg.blockNumber()
                                                   );
                        emit bookmarksRemoveLines(cursorSelBeg.blockNumber() + 1,
                                                    cursorSelEnd.blockNumber() - cursorSelBeg.blockNumber()
                                                   );
                    }
                }
            }
        }
        if (keyEvent->key() == Qt::Key_Backtab)
        {
            QTextCursor cursor(this->textCursor());
            if (true == cursor.hasSelection())
            {
                if (true == this->tabToSpaces)
                {
                    //qDebug() << "WTextEdit: backtab";
                    int prevPosStart = cursor.selectionStart();
                    int prevPosEnd = cursor.selectionEnd();
                    QString line;
                    int spaces, spacesToInsert;
                    int finalSpaces = 0;
                    int firstSpaces = 0;
                    cursor.setPosition(prevPosStart);
                    int blockCount = cursor.blockNumber();
                    QTextBlock block = cursor.block();
                    cursor.setPosition(prevPosEnd);
                    blockCount = cursor.blockNumber() - blockCount + 1;
                    for (int i = 0; i < blockCount; i++)
                    {
                        cursor.setPosition(block.position());
                        line = block.text();
                        spaces = 0;
                        for (int j = 0; j < line.size(); j++)
                        {
                            if (line.at(j) == ' ')
                            {
                                spaces++;
                            }
                            else
                            {
                                break;
                            }
                        }
                        if (spaces > 0)
                        {
                            if (spaces%this->spacesInTab != 0)
                            {
                                spacesToInsert = spaces%this->spacesInTab;
                            }
                            else
                            {
                                spacesToInsert = this->spacesInTab;
                            }
                        }
                        else
                        {
                            spacesToInsert = 0;
                        }
                        for (int j = 0; j < spacesToInsert; j++)
                        {
                            cursor.deleteChar();
                        }
                        finalSpaces = finalSpaces + spacesToInsert;
                        if (i == 0)
                        {
                            firstSpaces = spacesToInsert;
                        }
                        block = block.next();
                    }
                    cursor.setPosition(prevPosStart-firstSpaces);
                    cursor.setPosition(prevPosEnd-finalSpaces, QTextCursor::KeepAnchor);
                    this->setTextCursor(cursor);
                }
                else
                {
                    int prevPosStart = cursor.selectionStart();
                    int prevPosEnd = cursor.selectionEnd();
                    cursor.setPosition(prevPosStart);
                    bool onStart = cursor.atBlockStart();
                    bool firstInt = false;
                    int blockCount = cursor.blockNumber();
                    int blankLines = 0;
                    QTextBlock block = cursor.block();
                    cursor.setPosition(prevPosEnd);
                    blockCount = cursor.blockNumber() - blockCount + 1;
                    for (int i = 0; i < blockCount; i++)
                    {
                        cursor.setPosition(block.position());
                        cursor.setPosition(cursor.position()+1, QTextCursor::KeepAnchor);
                        if (cursor.selectedText() == "\t")
                        {
                            cursor.removeSelectedText();
                            if (i == 0)
                            {
                                firstInt = true;
                            }
                        }
                        else
                        {
                            blankLines++;
                        }
                        block = block.next();
                    }
                    /*if (true == onStart && blankLines == 0)
                    {
                        prevPosStart++;
                    }
                    if (blockCount == 1)
                    {
                        prevPosStart--;
                    }*/
                    blockCount = blockCount - blankLines;
                    if (0 == blockCount || true == onStart || false == firstInt)
                    {
                        cursor.setPosition(prevPosStart);
                    }
                    else
                    {
                        cursor.setPosition(prevPosStart-1);
                    }
                    cursor.setPosition(prevPosEnd-blockCount, QTextCursor::KeepAnchor);
                    this->setTextCursor(cursor);
                }
            }
            else
            {
                if (true == this->tabToSpaces)
                {
                    int prevPos = cursor.position();
                    QString line;
                    int spaces, spacesToDelete;
                    bool onStart = cursor.atBlockStart();
                    cursor.setPosition(cursor.block().position());
                    line = cursor.block().text();
                    spaces = 0;
                    for (int i = 0; i < line.size(); i++)
                    {
                        if (line.at(i) == ' ')
                        {
                            spaces++;
                        }
                        else
                        {
                            break;
                        }
                    }
                    spacesToDelete = spaces%this->spacesInTab;
                    if (spacesToDelete == 0 && spaces > 0)
                    {
                        if (spaces/this->spacesInTab > 0)
                        {
                            spacesToDelete = this->spacesInTab;
                        }
                        else
                        {
                            spacesToDelete = spaces;
                        }
                    }
                    for (int i = 0; i < spacesToDelete; i++)
                    {
                        cursor.deleteChar();
                    }
                    if (true == onStart)
                    {
                        cursor.setPosition(prevPos);
                    }
                    else
                    {
                        cursor.setPosition(prevPos-spacesToDelete);
                    }
                    this->setTextCursor(cursor);
                }
                else
                {
                    int prevPos = cursor.position();
                    //bool onStart = cursor.atBlockStart();
                    cursor.setPosition(cursor.position() - cursor.positionInBlock());
                    cursor.setPosition(cursor.position()+1, QTextCursor::KeepAnchor);
                    if (cursor.selectedText() == "\t")
                    {
                        cursor.removeSelectedText();
                        //if (true == onStart)
                        //{
                            prevPos--;
                        //}
                    }
                    cursor.setPosition(prevPos);
                    this->setTextCursor(cursor);
                }
            }
            return true;
        }
        if (keyEvent->key() == Qt::Key_Escape)
        {
            if (true == this->textCursor().hasSelection())
            {
                deselect();
                return true;
            }
        }
        if (keyEvent->key() == Qt::Key_Tab)
        {
            if (true == this->tabToSpaces)
            {
                QTextCursor cursor(this->textCursor());
                if (true == cursor.hasSelection())
                {
                    int prevPosStart = cursor.selectionStart();
                    int prevPosEnd = cursor.selectionEnd();
                    QString line;
                    int spaces, spacesToInsert;
                    int finalSpaces = 0;
                    int firstSpaces = 0;
                    cursor.setPosition(prevPosStart);
                    int blockCount = cursor.blockNumber();
                    QTextBlock block = cursor.block();
                    cursor.setPosition(prevPosEnd);
                    blockCount = cursor.blockNumber() - blockCount + 1;
                    for (int i = 0; i < blockCount; i++)
                    {
                        cursor.setPosition(block.position());
                        line = block.text();
                        spaces = 0;
                        for (int i = 0; i < line.size(); i++)
                        {
                            if (line.at(i) == ' ')
                            {
                                spaces++;
                            }
                            else
                            {
                                break;
                            }
                        }
                        spacesToInsert = this->spacesInTab-spaces%this->spacesInTab;
                        for (int i = 0; i < spacesToInsert; i++)
                        {
                            cursor.insertText(" ");
                        }
                        finalSpaces = finalSpaces + spacesToInsert;
                        if (i == 0)
                        {
                            firstSpaces = spacesToInsert;
                        }
                        block = block.next();
                    }
                    cursor.setPosition(prevPosStart+firstSpaces);
                    cursor.setPosition(prevPosEnd+finalSpaces, QTextCursor::KeepAnchor);
                    this->setTextCursor(cursor);
                }
                else
                {
                    int charsToAdd;
                    if (cursor.positionInBlock()%this->spacesInTab > 0)
                    {
                        charsToAdd = this->spacesInTab - cursor.positionInBlock()%this->spacesInTab;
                    }
                    else
                    {
                        charsToAdd = this->spacesInTab;
                    }
                    for (int i = 0; i < charsToAdd; i++)
                    {
                        cursor.insertText(" ");
                    }
                    this->setTextCursor(cursor);
                }
                return true;
            }
            else
            {
                QTextCursor cursor(this->textCursor());
                if (true == cursor.hasSelection())
                {
                    int prevPosStart = cursor.selectionStart();
                    int prevPosEnd = cursor.selectionEnd();
                    cursor.setPosition(prevPosStart);
                    //bool onStart = cursor.atBlockStart();
                    int blockCount = cursor.blockNumber();
                    QTextBlock block = cursor.block();
                    cursor.setPosition(prevPosEnd);
                    blockCount = cursor.blockNumber() - blockCount + 1;
                    for (int i = 0; i < blockCount; i++)
                    {
                        cursor.setPosition(block.position());
                        cursor.insertText("\t");
                        block = block.next();
                    }
                    prevPosStart++;
                    cursor.setPosition(prevPosStart);
                    cursor.setPosition(prevPosEnd+blockCount, QTextCursor::KeepAnchor);
                    this->setTextCursor(cursor);
                }
                else
                {
                    cursor.insertText("\t");
                    this->setTextCursor(cursor);
                }
                return true;
            }
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

/*void WTextEdit::highlightCurrentLine()
{
    qDebug() << "WTextEdit: highlightCurrentLine(), obsolete";

    qDebug() << "WTextEdit: return highlightCurrentLine()";
}*/


bool WTextEdit::highlightLine(int line, QColor *color)
{

    //qDebug() << "WTextEdit: highlightLine()";
    //qDebug() << "WTextEdit: highlighted line is" << line;
    //origColor = NULL;
    if (line >= 0 && line <= this->document()->lineCount())
    {
        QTextBlock lineBlock = this->document()->findBlockByNumber(line);
        QTextCursor cursor(lineBlock);

        QList<QTextEdit::ExtraSelection> extraSelections = this->extraSelections();

        if (color != NULL)
        {
            int i = 0;
            foreach (const QTextEdit::ExtraSelection &selection, extraSelections)
            {
                if (selection.cursor == cursor)
                {
                    //qDebug() << "WTextEdit: removing extra selection";
                    extraSelections.removeAt(i);
                }
                i++;
            }
            QTextEdit::ExtraSelection selection;
            selection.format.setBackground(*color);
            selection.format.setProperty(QTextFormat::FullWidthSelection, true);
            selection.cursor = cursor;
            selection.cursor.clearSelection();
            extraSelections.append(selection);
        }
        else
        {
            int i = 0;
            foreach (const QTextEdit::ExtraSelection &selection, extraSelections)
            {
                if (selection.cursor == cursor)
                {
                    //qDebug() << "WTextEdit: removing extra selection, NULL";
                    extraSelections.removeAt(i);
                }
                i++;
            }
        }
        this->setExtraSelections(extraSelections);

        this->setTextCursor(cursor);
        this->ensureCursorVisible();
        //qDebug() << "WTextEdit: return highlightLine()";
        return true;
    }
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


void WTextEdit::setPositionToStart()
{
    //qDebug() << "WTextEdit: scroll bar value" << this->verticalScrollBar()->value();
    this->moveCursor(QTextCursor::Start);
    this->verticalScrollBar()->setValue(0);
    //qDebug() << "WTextEdit: scroll bar new value" << this->verticalScrollBar()->value();
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
    int scrollBarValue = this->verticalScrollBar()->value();
    QTextCursor cursor = this->textCursor();
    cursor.clearSelection();
    this->setTextCursor(cursor);
    this->verticalScrollBar()->setValue(scrollBarValue);
}


void WTextEdit::selectLine(int line)
{
    QTextCursor cursor(this->document()->findBlockByNumber(line-1));
    cursor.select(QTextCursor::LineUnderCursor);
    this->setTextCursor(cursor);
}


void WTextEdit::cursorPositionChangedSlot()
{
    if (this->textCursor().blockNumber() != this->prevBlock)
    {
        if (false == this->isReadOnly())
        {
            QList<QTextEdit::ExtraSelection> extraSelections;

            QTextEdit::ExtraSelection selection;

            selection.format.setBackground(*cursorLineColor);
            selection.format.setProperty(QTextFormat::FullWidthSelection, true);
            selection.cursor = this->textCursor();
            selection.cursor.clearSelection();
            extraSelections.append(selection);
            this->setExtraSelections(extraSelections);
        }
        this->prevBlock = this->textCursor().blockNumber();
        emit updateLineCounter();
    }
    if (false == this->isReadOnly())
    {
        emit updateStatusBar();
    }
}


void WTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    //if (target == textEdit)
    //qDebug() << "WTextEdit: contextMenuEvent";
    if (false == this->isReadOnly())
    {
        if (this->textCursor().selectedText() == NULL)
        {
            cutAct->setEnabled(false);
            copyAct->setEnabled(false);
            deselectAct->setEnabled(false);
        }
        else
        {
            cutAct->setEnabled(true);
            copyAct->setEnabled(true);
            deselectAct->setEnabled(true);
        }
        editorPopup->popup(event->globalPos());
    }
}


void WTextEdit::makeMenu()
{
    //qDebug() << "CodeEdit: makeMenu()";
    editorPopup = new QMenu(this);
    cutAct = new QAction(QIcon(":resources/icons/cut.png"), "Cut", editorPopup);
    cutAct->setIconVisibleInMenu(true);
    copyAct = new QAction(QIcon(":resources/icons/page_copy.png"), "Copy", editorPopup);
    copyAct->setIconVisibleInMenu(true);
    QAction *pasteAct = new QAction("Paste", editorPopup);
    QAction *undoAct = new QAction(QIcon(":resources/icons/arrow_undo.png"), "Undo", editorPopup);
    undoAct->setIconVisibleInMenu(true);
    QAction *redoAct = new QAction(QIcon(":resources/icons/arrow_redo.png"), "Redo", editorPopup);
    redoAct->setIconVisibleInMenu(true);
    QAction *selectAllAct = new QAction("Select All", editorPopup);
    deselectAct = new QAction("Deselect", editorPopup);
    //QAction *splitHorizontalAct = new QAction("Split horizontal", editorPopup);
    //QAction *splitVerticalAct = new QAction("Split vertical", editorPopup);


    editorPopup->addAction(cutAct);
    editorPopup->addAction(copyAct);
    editorPopup->addAction(pasteAct);
    editorPopup->addSeparator();
    editorPopup->addAction(undoAct);
    editorPopup->addAction(redoAct);
    editorPopup->addSeparator();
    editorPopup->addAction(selectAllAct);
    editorPopup->addAction(deselectAct);
    //editorPopup->addSeparator();
    //editorPopup->addAction(splitHorizontalAct);
    //editorPopup->addAction(splitVerticalAct);


    /*connect(cutAct,
            SIGNAL(triggered()),
            this,
            SLOT(editedCut())
           );
    connect(copyAct,
            SIGNAL(triggered()),
            this,
            SLOT(copy()));
    connect(pasteAct,
            SIGNAL(triggered()),
            this,
            SLOT(editedPaste())
           );
    connect(undoAct,
            SIGNAL(triggered()),
            this,
            SLOT(editedUndo())
           );
    connect(redoAct,
            SIGNAL(triggered()),
            this,
            SLOT(editedRedo())
           );connect(cutAct,
            SIGNAL(triggered()),
            this,
            SLOT(editedCut())
           );*/
    connect(cutAct,
            SIGNAL(triggered()),
            this,
            SLOT(cut())
           );
    connect(copyAct,
            SIGNAL(triggered()),
            this,
            SLOT(copy())
           );
    connect(pasteAct,
            SIGNAL(triggered()),
            this,
            SLOT(paste())
           );
    connect(undoAct,
            SIGNAL(triggered()),
            this,
            SLOT(undo())
           );
    connect(redoAct,
            SIGNAL(triggered()),
            this,
            SLOT(redo())
           );
    connect(selectAllAct,
            SIGNAL(triggered()),
            this,
            SLOT(selectAll())
           );
    connect(deselectAct,
            SIGNAL(triggered()),
            this,
            SLOT(deselect())
           );
    //connect(splitHorizontalAct, SIGNAL(triggered()), this, SLOT(splitHorizontal()));
    //connect(splitVerticalAct, SIGNAL(triggered()), this, SLOT(splitVertical()));
    //qDebug() << "CodeEdit: return makeMenu()";
}


void WTextEdit::setTabToSpaces(bool enabled)
{
    this->tabToSpaces = enabled;
}


void WTextEdit::setSpacesInTab(int spacesInTab)
{
    this->spacesInTab = spacesInTab;
}


bool WTextEdit::highlightLineAppend(int line, QColor *color)
{
    //qDebug() << "Highlight";
    if (line >= 0 && line <= this->document()->lineCount())
    {
        QTextBlock lineBlock = this->document()->findBlockByNumber(line);
        QTextCursor cursor(lineBlock);

        QList<QTextEdit::ExtraSelection> extraSelections = this->extraSelections();

        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(*color);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = cursor;
        selection.cursor.clearSelection();
        extraSelections.append(selection);

        this->setExtraSelections(extraSelections);

        /*QTextBlockFormat lineFormat = lineBlock.blockFormat();
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
        }*/
        /*QTextCursor cursor(lineBlock);
        //qDebug() << "position: " << cursor.position();
        cursor.setBlockFormat(lineFormat);*/
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


void WTextEdit::clearHighlight()
{
    //qDebug() << "WTextEdit: clearHighlight()";
    this->setExtraSelections(QList<QTextEdit::ExtraSelection>());
}


/*SourceType WTextEdit::getSourceType()
{
    return this->sourceType;
}


void WTextEdit::setSourceType(SourceType type)
{
    this->sourceType = type;
}*/


void WTextEdit::deleteHighlighter()
{
    if (highlighter != NULL)
    {
        delete highlighter;
        this->sourceType = PLAIN;
        highlighter = NULL;
    }
}


void WTextEdit::jumpToLine(int lineToJmp)
{
    /*if (lineToJmp < cursor.block().blockNumber())
    {
        while (cursor.block().blockNumber() < lineToJmp)
        {
            cursor.movePosition(QTextCursor::NextBlock);
        }
    }
    else
    {
        while (cursor.block().blockNumber() > lineToJmp)
        {
            cursor.movePosition(QTextCursor::PreviousBlock);
        }
    }*/
    if (this->document()->blockCount()-1 >= lineToJmp)
    {
        QTextCursor cursor(this->document()->findBlockByNumber(lineToJmp));
        /*QList<QTextEdit::ExtraSelection> extraSelections = this->extraSelections();
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(*cursorLineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = cursor;
        selection.cursor.clearSelection();
        extraSelections.append(selection);

        this->setExtraSelections(extraSelections);*/
        this->setTextCursor(cursor);
        this->ensureCursorVisible();
    }
}


void WTextEdit::findAndMark(QString query, bool next, bool caseSensitive)
{
    QTextDocument::FindFlags options;
    if (false == next)
    {
        options |= QTextDocument::FindBackward;
    }
    if (true == caseSensitive)
    {
        options |= QTextDocument::FindCaseSensitively;
    }

    QTextCursor cur = this->document()->find(query, this->textCursor(), options);
    if (!cur.isNull())
    {
        this->setTextCursor(cur);
    }
    else
    {
        if (true == next)
        {
            QString text = "End of document reached.\nDo you want to continue searching from the start of the document?";
            if (QMessageBox::Yes == QMessageBox::question(this, "", text, QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes))
            {
                QTextCursor curAgain = this->document()->find(query, 0, options);
                if (!curAgain.isNull())
                {
                    this->setTextCursor(curAgain);
                }
                else
                {
                    text = "String \"" + query + "\" was not found in the document.";
                    QMessageBox::information(this, "", text, QMessageBox::Ok, QMessageBox::Ok);
                }
            }
        }
        else
        {
            QString text = "Start of document reached.\nDo you want to continue searching from the end of the document?";
            if (QMessageBox::Yes == QMessageBox::question(this, "", text, QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes))
            {
                QTextCursor curAgain = this->document()->find(query, this->document()->characterCount(), options);
                if (!curAgain.isNull())
                {
                    this->setTextCursor(curAgain);
                }
                else
                {
                    text = "String \"" + query + "\" was not found in the document.";
                    QMessageBox::information(this, "", text, QMessageBox::Ok, QMessageBox::Ok);
                }
            }
        }
    }
}


void WTextEdit::findAndReplace(QString query, QString replace, bool all, bool caseSensitive)
{
    QTextDocument::FindFlags options;
    if (true == caseSensitive)
    {
        options |= QTextDocument::FindCaseSensitively;
    }

    if (false == all)
    {
        QTextCursor currCursor = this->textCursor();
        if (true == currCursor.hasSelection())
        {
            currCursor.setPosition(currCursor.selectionStart());
        }

        QTextCursor cur = this->document()->find(query, currCursor, options);
        if (!cur.isNull())
        {
            cur.removeSelectedText();
            cur.insertText(replace);
            this->setTextCursor(cur);
        }
        else
        {
            QString text = "End of document reached.\nDo you want to continue searching from the start of the document?";
            if (QMessageBox::Yes == QMessageBox::question(this, "", text, QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes))
            {
                QTextCursor curAgain = this->document()->find(query, 0, options);
                if (!curAgain.isNull())
                {
                    curAgain.removeSelectedText();
                    curAgain.insertText(replace);
                    this->setTextCursor(curAgain);
                }
                else
                {
                    text = "String \"" + query + "\" was not found in the document.";
                    QMessageBox::information(this, "", text, QMessageBox::Ok, QMessageBox::Ok);
                }
            }
        }
    }
    else
    {
        QTextCursor cur = this->document()->find(query, 0, options);
        while (!cur.isNull())
        {
            cur.removeSelectedText();
            cur.insertText(replace);
            this->setTextCursor(cur);
            cur = this->document()->find(query, this->textCursor(), options);
        }
    }
}


void WTextEdit::queryReadOnly(bool readOnly)
{
    this->setReadOnly(readOnly);
    this->clearHighlight();
    shctBreakpoint->setEnabled(!readOnly);
    shctBookmark->setEnabled(!readOnly);
    /*shctCopy->setEnabled(!readOnly);
    shctPaste->setEnabled(!readOnly);
    shctCut->setEnabled(!readOnly);
    shctSelectAll->setEnabled(!readOnly);
    shctDeselect->setEnabled(!readOnly);
    shctComment->setEnabled(!readOnly);
    shctDeleteComment->setEnabled(!readOnly);
    shctJmpToBookmarkNext->setEnabled(!readOnly);
    shctJmpToBookmarkPrev->setEnabled(!readOnly);
    shctJmpToLine->setEnabled(!readOnly);
    shctFind->setEnabled(!readOnly);
    shctReplace->setEnabled(!readOnly);*/
    shctMoveLineUp->setEnabled(!readOnly);
    shctMoveLineDown->setEnabled(!readOnly);
    shctMoveWordLeft->setEnabled(!readOnly);
    shctMoveWordRight->setEnabled(!readOnly);
    shctToUpper->setEnabled(!readOnly);
    shctToLower->setEnabled(!readOnly);
    shctFirstToUpper->setEnabled(!readOnly);
    shctDeleteLine->setEnabled(!readOnly);
    shctSwitchChars->setEnabled(!readOnly);
    shctSelectWordUnder->setEnabled(!readOnly);
    shctSelectWordLeft->setEnabled(!readOnly);
    shctSelectWordRight->setEnabled(!readOnly);
    /*shctUndo->setEnabled(!readOnly);
    shctRedo->setEnabled(!readOnly);
    shctFindNext->setEnabled(!readOnly);
    shctFindPrevious->setEnabled(!readOnly);*/
    emit editorReadOnly(readOnly);
    if (false == this->isReadOnly())
    {
        QList<QTextEdit::ExtraSelection> extraSelections;

        QTextEdit::ExtraSelection selection;

        selection.format.setBackground(*cursorLineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = this->textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
        this->setExtraSelections(extraSelections);
    }
}


void WTextEdit::setShortcuts()
{
    shctBreakpoint = new QShortcut(this);
    shctBreakpoint->setKey(Qt::CTRL + Qt::SHIFT + Qt::Key_B);
    connect(shctBreakpoint, SIGNAL(activated()), this, SLOT(shortcutBreakpoint()));
    shctBookmark = new QShortcut(this);
    shctBookmark->setKey(Qt::CTRL + Qt::Key_B);
    connect(shctBookmark, SIGNAL(activated()), this, SLOT(shortcutBookmark()));
    /*shctCopy = new QShortcut(this);
    shctCopy->setKey(Qt::CTRL + Qt::Key_C);
    connect(shctCopy, SIGNAL(activated()), this, SLOT(shortcutCopy()));*/
    /*shctPaste = new QShortcut(this);
    shctPaste->setKey(Qt::CTRL + Qt::Key_V);
    connect(shctPaste, SIGNAL(activated()), this, SLOT(shortcutPaste()));
    shctCut = new QShortcut(this);
    shctCut->setKey(Qt::CTRL + Qt::Key_X);
    connect(shctCut, SIGNAL(activated()), this, SLOT(shortcutCut()));
    shctSelectAll = new QShortcut(this);
    shctSelectAll->setKey(Qt::CTRL + Qt::Key_A);
    connect(shctSelectAll, SIGNAL(activated()), this, SLOT(shortcutSelectAll()));
    shctDeselect = new QShortcut(this);
    shctDeselect->setKey(Qt::CTRL + Qt::SHIFT + Qt::Key_A);
    connect(shctDeselect, SIGNAL(activated()), this, SLOT(shortcutDeselect()));
    shctComment = new QShortcut(this);
    shctComment->setKey(Qt::CTRL + Qt::Key_D);
    connect(shctComment, SIGNAL(activated()), this, SLOT(shortcutComment()));
    shctDeleteComment = new QShortcut(this);
    shctDeleteComment->setKey(Qt::CTRL + Qt::SHIFT + Qt::Key_D);
    connect(shctDeleteComment, SIGNAL(activated()), this, SLOT(shortcutDeleteComment()));*/
    /*shctJmpToBookmarkNext = new QShortcut(this);
    shctJmpToBookmarkNext->setKey(Qt::ALT + Qt::Key_PageUp);
    connect(shctJmpToBookmarkNext, SIGNAL(activated()), this, SLOT(shortcutJmpToBookmarkNext()));
    shctJmpToBookmarkPrev = new QShortcut(this);
    shctJmpToBookmarkPrev->setKey(Qt::ALT + Qt::Key_PageDown);
    connect(shctJmpToBookmarkPrev, SIGNAL(activated()), this, SLOT(shortcutJmpToBookmarkPrev()));*/
    /*shctJmpToLine = new QShortcut(this);
    shctJmpToLine->setKey(Qt::CTRL + Qt::Key_G);
    connect(shctJmpToLine, SIGNAL(activated()), this, SLOT(shortcutJmpToLine()));*/
    /*shctFind = new QShortcut(this);
    shctFind->setKey(Qt::CTRL + Qt::Key_F);
    connect(shctFind, SIGNAL(activated()), this, SLOT(shortcutFind()));*/
    /*shctReplace = new QShortcut(this);
    shctReplace->setKey(Qt::CTRL + Qt::Key_R);
    connect(shctReplace, SIGNAL(activated()), this, SLOT(shortcutReplace()));*/
    shctMoveLineUp = new QShortcut(this);
    shctMoveLineUp->setKey(Qt::CTRL + Qt::Key_Up);
    connect(shctMoveLineUp, SIGNAL(activated()), this, SLOT(shortcutMoveLineUp()));
    shctMoveLineDown = new QShortcut(this);
    shctMoveLineDown->setKey(Qt::CTRL + Qt::Key_Down);
    connect(shctMoveLineDown, SIGNAL(activated()), this, SLOT(shortcutMoveLineDown()));
    shctMoveWordLeft = new QShortcut(this);
    shctMoveWordLeft->setKey(Qt::CTRL + Qt::Key_Left);
    connect(shctMoveWordLeft, SIGNAL(activated()), this, SLOT(shortcutMoveWordLeft()));
    shctMoveWordRight = new QShortcut(this);
    shctMoveWordRight->setKey(Qt::CTRL + Qt::Key_Right);
    connect(shctMoveWordRight, SIGNAL(activated()), this, SLOT(shortcutMoveWordRight()));
    shctToUpper = new QShortcut(this);
    shctToUpper->setKey(Qt::CTRL + Qt::Key_U);
    connect(shctToUpper, SIGNAL(activated()), this, SLOT(shortcutToUpper()));
    shctToLower = new QShortcut(this);
    shctToLower->setKey(Qt::CTRL + Qt::SHIFT + Qt::Key_U);
    connect(shctToLower, SIGNAL(activated()), this, SLOT(shortcutToLower()));
    shctFirstToUpper = new QShortcut(this);
    shctFirstToUpper->setKey(Qt::CTRL + Qt::ALT + Qt::Key_U);
    connect(shctFirstToUpper, SIGNAL(activated()), this, SLOT(shortcutFirstToUpper()));
    shctDeleteLine = new QShortcut(this);
    shctDeleteLine->setKey(Qt::CTRL + Qt::Key_K);
    connect(shctDeleteLine, SIGNAL(activated()), this, SLOT(shortcutDeleteLine()));
    shctSwitchChars = new QShortcut(this);
    shctSwitchChars->setKey(Qt::CTRL + Qt::Key_T);
    connect(shctSwitchChars, SIGNAL(activated()), this, SLOT(shortcutSwitchChars()));
    shctSelectWordUnder = new QShortcut(this);
    shctSelectWordUnder->setKey(Qt::CTRL + Qt::Key_H);
    connect(shctSelectWordUnder, SIGNAL(activated()), this, SLOT(shortcutSelectWordUnder()));
    shctSelectWordLeft = new QShortcut(this);
    shctSelectWordLeft->setKey(Qt::CTRL + Qt::SHIFT + Qt::Key_Left);
    connect(shctSelectWordLeft, SIGNAL(activated()), this, SLOT(shortcutSelectWordLeft()));
    shctSelectWordRight = new QShortcut(this);
    shctSelectWordRight->setKey(Qt::CTRL + Qt::SHIFT + Qt::Key_Right);
    connect(shctSelectWordRight, SIGNAL(activated()), this, SLOT(shortcutSelectWordRight()));
    shctSwitchLinesUp = new QShortcut(this);
    shctSwitchLinesUp->setKey(Qt::CTRL + Qt::SHIFT + Qt::Key_Up);
    connect(shctSwitchLinesUp, SIGNAL(activated()), this, SLOT(shortcutSwitchLinesUp()));
    shctSwitchLinesDown = new QShortcut(this);
    shctSwitchLinesDown->setKey(Qt::CTRL + Qt::SHIFT + Qt::Key_Down);
    connect(shctSwitchLinesDown, SIGNAL(activated()), this, SLOT(shortcutSwitchLinesDown()));
    /*shctUndo = new QShortcut(this);
    shctUndo->setKey(Qt::CTRL + Qt::Key_Z);
    connect(shctUndo, SIGNAL(activated()), this, SLOT(shortcutUndo()));
    shctRedo = new QShortcut(this);
    shctRedo->setKey(Qt::CTRL + Qt::SHIFT + Qt::Key_Z);
    connect(shctRedo, SIGNAL(activated()), this, SLOT(shortcutRedo()));*/
    /*shctFindNext = new QShortcut(this);
    shctFindNext->setKey(Qt::Key_F3);
    connect(shctFindNext, SIGNAL(activated()), this, SLOT(shortcutFindNext()));
    shctFindPrevious = new QShortcut(this);
    shctFindPrevious->setKey(Qt::SHIFT + Qt::Key_F3);
    connect(shctFindPrevious, SIGNAL(activated()), this, SLOT(shortcutFindPrevious()));*/
    ////TODO: move lines up, down
}


void WTextEdit::shortcutBreakpoint()
{
    QTextCursor cursor(this->textCursor());
    emit breakpoint(cursor.blockNumber());
}


void WTextEdit::shortcutBookmark()
{
    QTextCursor cursor(this->textCursor());
    emit bookmark(cursor.blockNumber());
}


void WTextEdit::shortcutCopy()
{
    this->copy();
}


void WTextEdit::shortcutPaste()
{
    this->paste();
}


void WTextEdit::shortcutCut()
{
    this->cut();
}


void WTextEdit::shortcutSelectAll()
{
    this->selectAll();
}


void WTextEdit::shortcutDeselect()
{
    this->deselect();
}


void WTextEdit::shortcutComment()
{
    QTextCursor cursor(this->textCursor());
    if (true == cursor.hasSelection())
    {
        int curStart = cursor.selectionStart();
        int startBlockPos = this->document()->findBlock(curStart).position();
        QString blockText = this->document()->findBlock(curStart).text();
        int curEnd = cursor.selectionEnd();
        QRegExp whiteRegExp("[\\s]*");
        if ( this->document()->findBlock(curStart).blockNumber() == this->document()->findBlock(curEnd).blockNumber()
          && true == whiteRegExp.exactMatch(blockText.left(curStart - startBlockPos))
           )
        {
            cursor.setPosition(startBlockPos);
            cursor.insertText(";");
            cursor.setPosition(curStart+1);
            cursor.setPosition(curEnd+1, QTextCursor::KeepAnchor);
            this->setTextCursor(cursor);
        }
        else
        {
            QString selection = cursor.selectedText();
            selection = "/*" + selection + "*/";
            cursor.removeSelectedText();
            cursor.insertText(selection);
            cursor.setPosition(curStart);
            cursor.setPosition(curEnd+4, QTextCursor::KeepAnchor);
            this->setTextCursor(cursor);
        }
        //emit selectionRemovedSignal(cursor.selectionStart(), cursor.selectionEnd());
        //emit textChangedSignal(a, cursor.position());
    }
    else
    {
        int linePos = cursor.positionInBlock();
        cursor.setPosition(cursor.position() - linePos);
        cursor.insertText(";");
        cursor.setPosition(cursor.position() + linePos);
        this->setTextCursor(cursor);
    }
}


void WTextEdit::shortcutDeleteComment()
{
    QTextCursor cursor(this->textCursor());
    if (true == cursor.hasSelection())
    {
        if (cursor.selectedText().startsWith("/*") && cursor.selectedText().endsWith("*/"))
        {
            QString text = cursor.selectedText();
            text.remove(0, 2);
            text.remove(text.size() - 2, 2);
            cursor.removeSelectedText();
            cursor.insertText(text);
        }
        else
        {
            QTextBlock block = this->document()->findBlock(cursor.selectionStart());
            QTextBlock blockEnd = this->document()->findBlock(cursor.selectionEnd());
            int startPos = cursor.selectionStart();
            int endPos = cursor.selectionEnd();
            cursor.setPosition(cursor.selectionStart());
            bool notOnBlockStart = (cursor.positionInBlock() > 0);

            cursor.setPosition(cursor.position() - cursor.positionInBlock());
            if (cursor.block().text().startsWith(";"))
            {
                cursor.deleteChar();
                if (true == notOnBlockStart)
                {
                    startPos--;
                }
                endPos--;
            }
            while (block != blockEnd)
            {
                block = block.next();
                cursor.setPosition(block.position());
                if (block.text().startsWith(";"))
                {
                    cursor.deleteChar();
                    endPos--;
                }
            }
            cursor.setPosition(startPos);
            cursor.setPosition(endPos, QTextCursor::KeepAnchor);
        }
    }
    else
    {
        int linePos = cursor.positionInBlock();
        if (cursor.block().text().startsWith(";"))
        {
            cursor.setPosition(cursor.position() - linePos);
            cursor.deleteChar();
            cursor.setPosition(cursor.position() + linePos - 1);
        }
        else
        {
            cursor.setPosition(cursor.position() + linePos);
        }
    }
    this->setTextCursor(cursor);
}


void WTextEdit::shortcutJmpToBookmarkNext()
{
    emit requestScrollToBookmark(this->textCursor().blockNumber(), true);
}


void WTextEdit::shortcutJmpToBookmarkPrev()
{
    emit requestScrollToBookmark(this->textCursor().blockNumber(), false);
}


void WTextEdit::shortcutJmpToLine()
{
    emit jumpToLineDialog(this->textCursor().blockNumber(), this->document()->blockCount());
}


void WTextEdit::shortcutFind()
{
    emit findDialog(this->textCursor().selectedText());
}


void WTextEdit::shortcutReplace()
{
    emit findAndReplaceDialog(this->textCursor().selectedText());
}


void WTextEdit::shortcutMoveLineUp()
{
    //qDebug() << "WTextEdit: scroll value" << this->verticalScrollBar()->value();
    this->verticalScrollBar()->setValue(this->verticalScrollBar()->value()-1);
}


void WTextEdit::shortcutMoveLineDown()
{
    this->verticalScrollBar()->setValue(this->verticalScrollBar()->value()+1);
}


void WTextEdit::shortcutMoveWordLeft()
{
    QTextCursor cursor(this->textCursor());
    cursor.movePosition(QTextCursor::WordLeft);
    this->setTextCursor(cursor);
}


void WTextEdit::shortcutMoveWordRight()
{
    QTextCursor cursor(this->textCursor());
    cursor.movePosition(QTextCursor::WordRight);
    this->setTextCursor(cursor);
}


void WTextEdit::shortcutToUpper()
{
    QTextCursor cursor(this->textCursor());
    if (true == cursor.hasSelection())
    {
        int selStart = cursor.selectionStart();
        int selEnd = cursor.selectionEnd();
        QString text = cursor.selectedText();
        cursor.removeSelectedText();
        text = text.toUpper();
        cursor.insertText(text);
        cursor.setPosition(selStart);
        cursor.setPosition(selEnd, QTextCursor::KeepAnchor);
        this->setTextCursor(cursor);
    }
}


void WTextEdit::shortcutToLower()
{
    QTextCursor cursor(this->textCursor());
    if (true == cursor.hasSelection())
    {
        int selStart = cursor.selectionStart();
        int selEnd = cursor.selectionEnd();
        QString text = cursor.selectedText();
        cursor.removeSelectedText();
        text = text.toLower();
        cursor.insertText(text);
        cursor.setPosition(selStart);
        cursor.setPosition(selEnd, QTextCursor::KeepAnchor);
        this->setTextCursor(cursor);
    }
}


void WTextEdit::shortcutFirstToUpper()
{
    QTextCursor cursor(this->textCursor());
    if (false == cursor.hasSelection())
    {
        int curPos = cursor.position();
        cursor.select(QTextCursor::WordUnderCursor);
        QString text = cursor.selectedText();
        cursor.removeSelectedText();
        text = text.at(0).toUpper() + text.mid(1).toLower();
        cursor.insertText(text);
        cursor.setPosition(curPos);
        this->setTextCursor(cursor);
    }
    else
    {
        int selStart = cursor.selectionStart();
        int selEnd = cursor.selectionEnd();
        QString text = cursor.selectedText();
        cursor.removeSelectedText();
        text = text.at(0).toUpper() + text.mid(1).toLower();
        cursor.insertText(text);
        cursor.setPosition(selStart);
        cursor.setPosition(selEnd, QTextCursor::KeepAnchor);
        this->setTextCursor(cursor);
    }
}


void WTextEdit::shortcutDeleteLine()
{
    QTextCursor cursor(this->textCursor());
    if (false == cursor.hasSelection())
    {
        cursor.select(QTextCursor::LineUnderCursor);
        cursor.removeSelectedText();
        if (cursor.blockNumber() != this->document()->blockCount()-1)
        {
            cursor.deleteChar();
        }
        else if (cursor.blockNumber() != 0)
        {
            cursor.deletePreviousChar();
        }
        this->setTextCursor(cursor);
    }
}


void WTextEdit::shortcutSwitchChars()
{
    QTextCursor cursor1(this->textCursor());
    if (false == cursor1.movePosition(QTextCursor::PreviousCharacter))
    {
        return;
    }
    QTextCursor cursor2(this->textCursor());
    if ( false == this->textCursor().hasSelection()
      && cursor1.position() != cursor2.position()
      && cursor1.blockNumber() == cursor2.blockNumber()
       )
    {
        QString text = QString(cursor2.block().text().at(cursor2.positionInBlock()));
        text += QString(cursor2.block().text().at(cursor1.positionInBlock()));
        cursor2.deletePreviousChar();
        cursor2.deletePreviousChar();
        cursor2.insertText(text);
        this->setTextCursor(cursor2);
    }
}


void WTextEdit::shortcutSelectWordUnder()
{
    QTextCursor cursor(this->textCursor());
    cursor.select(QTextCursor::WordUnderCursor);
    this->setTextCursor(cursor);
}


void WTextEdit::shortcutSelectWordLeft()
{
    QTextCursor cursor(this->textCursor());
    cursor.movePosition(QTextCursor::WordLeft, QTextCursor::KeepAnchor);
    this->setTextCursor(cursor);
}


void WTextEdit::shortcutSelectWordRight()
{
    QTextCursor cursor(this->textCursor());
    cursor.movePosition(QTextCursor::WordRight, QTextCursor::KeepAnchor);
    this->setTextCursor(cursor);
}


void WTextEdit::shortcutUndo()
{
    this->undo();
}


void WTextEdit::shortcutRedo()
{
    this->redo();
}


void WTextEdit::shortcutFindNext()
{
    QTextCursor cursor = this->textCursor();
    if (false == cursor.hasSelection())
    {
        cursor.select(QTextCursor::WordUnderCursor);
    }
    findAndMark(cursor.selectedText(), true, true);
}


void WTextEdit::shortcutFindPrevious()
{
    QTextCursor cursor = this->textCursor();
    if (false == cursor.hasSelection())
    {
        cursor.select(QTextCursor::WordUnderCursor);
    }
    findAndMark(cursor.selectedText(), false, true);
}


void WTextEdit::shortcutSwitchLinesUp()
{
    QTextCursor cursor(this->textCursor());
    if (false == cursor.hasSelection())
    {
        if (cursor.blockNumber() != 0)
        {
            int posInLine = cursor.positionInBlock();
            cursor.select(QTextCursor::LineUnderCursor);
            QString text1 = cursor.selectedText();
            cursor.removeSelectedText();
            cursor.movePosition(QTextCursor::PreviousBlock);
            cursor.select(QTextCursor::LineUnderCursor);
            QString text2 = cursor.selectedText();
            cursor.removeSelectedText();
            cursor.insertText(text1);
            cursor.movePosition(QTextCursor::NextBlock);
            cursor.insertText(text2);
            cursor.movePosition(QTextCursor::PreviousBlock);
            cursor.setPosition(cursor.block().position() + posInLine);
            this->setTextCursor(cursor);
        }
    }
    else
    {
        if (this->document()->findBlock(cursor.selectionStart()).blockNumber() != 0)
        {
            QTextCursor cursor2(cursor);
            int selectOffset = 0;
            int selStart = cursor.selectionStart();
            int selEnd = cursor.selectionEnd();
            QString text1;
            QString text2;
            cursor2.setPosition(cursor.selectionStart());
            int lineCount = this->document()->findBlock(cursor.selectionEnd()).blockNumber();
            lineCount -= this->document()->findBlock(cursor.selectionStart()).blockNumber();
            lineCount++;
            for (int i = 0; i < lineCount; i++)
            {
                cursor2.select(QTextCursor::LineUnderCursor);
                text1 = cursor2.selectedText();
                cursor2.removeSelectedText();
                cursor2.movePosition(QTextCursor::PreviousBlock);
                cursor2.select(QTextCursor::LineUnderCursor);
                text2 = cursor2.selectedText();
                if (0 == i)
                {
                    selectOffset = text2.length()+1;
                }
                cursor2.removeSelectedText();
                cursor2.insertText(text1);
                cursor2.movePosition(QTextCursor::NextBlock);
                cursor2.insertText(text2);
                cursor2.movePosition(QTextCursor::NextBlock);
            }
            cursor2.setPosition(selStart - selectOffset);
            cursor2.setPosition(selEnd - selectOffset, QTextCursor::KeepAnchor);
            this->setTextCursor(cursor2);
        }
    }
}


void WTextEdit::shortcutSwitchLinesDown()
{
    QTextCursor cursor(this->textCursor());
    if (false == cursor.hasSelection())
    {
        if (cursor.blockNumber() != this->document()->blockCount()-1)
        {
            int posInLine = cursor.positionInBlock();
            cursor.select(QTextCursor::LineUnderCursor);
            QString text1 = cursor.selectedText();
            cursor.removeSelectedText();
            cursor.movePosition(QTextCursor::NextBlock);
            cursor.select(QTextCursor::LineUnderCursor);
            QString text2 = cursor.selectedText();
            cursor.removeSelectedText();
            cursor.insertText(text1);
            cursor.movePosition(QTextCursor::PreviousBlock);
            cursor.insertText(text2);
            cursor.movePosition(QTextCursor::NextBlock);
            cursor.setPosition(cursor.block().position() + posInLine);
            this->setTextCursor(cursor);
        }
    }
    else
    {
        if (this->document()->findBlock(cursor.selectionEnd()).blockNumber() != this->document()->blockCount()-1)
        {
            QTextCursor cursor2(cursor);
            int selectOffset = 0;
            int selStart = cursor.selectionStart();
            int selEnd = cursor.selectionEnd();
            QString text1;
            QString text2;
            cursor2.setPosition(cursor.selectionEnd());
            int lineCount = this->document()->findBlock(cursor.selectionEnd()).blockNumber();
            lineCount -= this->document()->findBlock(cursor.selectionStart()).blockNumber();
            lineCount++;
            for (int i = 0; i < lineCount; i++)
            {
                cursor2.select(QTextCursor::LineUnderCursor);
                text1 = cursor2.selectedText();
                cursor2.removeSelectedText();
                cursor2.movePosition(QTextCursor::NextBlock);
                cursor2.select(QTextCursor::LineUnderCursor);
                text2 = cursor2.selectedText();
                if (0 == i)
                {
                    selectOffset = text2.length()+1;
                }
                cursor2.removeSelectedText();
                cursor2.insertText(text1);
                cursor2.movePosition(QTextCursor::PreviousBlock);
                cursor2.insertText(text2);
                cursor2.movePosition(QTextCursor::PreviousBlock);
            }
            cursor2.setPosition(selStart + selectOffset);
            cursor2.setPosition(selEnd + selectOffset, QTextCursor::KeepAnchor);
            this->setTextCursor(cursor2);
        }
    }

}