/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup CodeEdit
 * @file codeedit.cpp
 */


#include <QtGui>
#include "codeedit.h"
#include "../DockManager/wdockmanager.h"
#include "../../guicfg/guicfg.h"


CodeEdit::CodeEdit(QWidget *parent, bool tabs, QString wName, QString wPath, CodeEdit *parentCodeEdit)
    : QWidget(parent)
{
    //qDebug() << "CodeEdit: CodeEdit()";
    this->parentCodeEdit = parentCodeEdit;
    this->curCodeEdit = NULL;
    //if (this->parentCodeEdit == NULL)
    //{
        //qDebug() << "parentCodeEdit is NULL";
        this->breakpointsLines = new QList<int>();
        this->bookmarksLines = new QList<int>();
    //}
    //else
    //{
        //?
        //qDebug() << "parentCodeEdit is not NULL";
        //this->breakpointsLines = NULL;
        //this->breakpointsLines = new QList<unsigned int>();
    //}
    if (wName == NULL || wPath == NULL || wPath == "untracked")
    {
        //qDebug() << "CodeEdit: untracked";
        if (wPath == "untracked")
        {
            if (wName == "ASM Translator" || wName == "ASM Translator error")
            {
                textEdit = new WTextEdit(this, PICOBLAZEASM);
            }
        }
        else
        {
            textEdit = new WTextEdit(this, PLAIN);
        }
        this->textEdit->setPlainText(" ");
    }
    else
    {
        int index = wName.lastIndexOf(".");
        if (index > 0)
        {
            QString text(wName.right(wName.size() - index));
            if (text == ".h")
            {
                textEdit = new WTextEdit(this, C);
            }
            else if (text == ".cpp" || text == ".cxx" || text == ".cc")
            {
                textEdit = new WTextEdit(this, CPP);
            }
            else if (text == ".c")
            {
                textEdit = new WTextEdit(this, C);
            }
            else if (text == ".asm" || text == ".psm")
            {
                //!!!DO NOT FORGET TO CHECK IF ASM IS AVR OR PIC TYPE!!!
                textEdit = new WTextEdit(this, PICOBLAZEASM);
            }
            else
            {
                textEdit = new WTextEdit(this, PLAIN);
            }
        }
        else
        {
            textEdit = new WTextEdit(this, PLAIN);
        }
    }
    //textEdit->setContextMenuPolicy(Qt::NoContextMenu);
    textEdit->setFont(GuiCfg::getInstance().getEditorFont());
    QFontMetrics fontMetrics(textEdit->font());
    textEdit->setTabStopWidth(GuiCfg::getInstance().getTabWidth() * fontMetrics.width(' '));
    textEdit->setTabToSpaces(GuiCfg::getInstance().getTabToSpaces());
    textEdit->setSpacesInTab(GuiCfg::getInstance().getSpacesInTab());
    lineCount = new WLineCounter(textEdit, true, false, 0, textEdit->font());
    //statusBar = new QStatusBar(this);
    layout = new QGridLayout(this);
    layout->addWidget(lineCount, 0, 0);
    layout->addWidget(textEdit, 0, 1);
    //layout->addWidget(statusBar, 1, 1, 2, 1);
    setLayout(layout);
    name = wName;
    path = wPath;
    parentWidget = parent;
    this->tabs = tabs;
    parentProject = NULL;
    breakpointColor = new QColor(0,255,0);
    //textEdit->setWordWrapMode(QTextOption::NoWrap);
    textEdit->setWordWrapMode(QTextOption::WordWrap);
    textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    //this->makeMenu();
    //this->setFocusPolicy(Qt::StrongFocus);
    //this->textEdit->setFocusPolicy(Qt::NoFocus);
    //this->installEventFilter(this);
    if (wPath != NULL && wPath != "untracked")
    {
        //qDebug() << "CodeEdit: Not untracked";
        QFile file(path);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            this->textEdit->setPlainText(file.readAll());
            file.close();
        }
    }
    changed = false;
    connect(textEdit,
            SIGNAL(focusIn()),
            this,
            SLOT(getFocus())
           );
    connect(textEdit,
            SIGNAL(breakpoint(int)),
            this,
            SLOT(manageBreakpointEmit(int))
           );
    connect(textEdit,
            SIGNAL(breakpointsAddLines(int, int)),
            this,
            SLOT(breakpointsAddLines(int, int))
           );
    connect(textEdit,
            SIGNAL(breakpointsRemoveLines(int, int)),
            this,
            SLOT(breakpointsRemoveLines(int, int))
           );
    connect(textEdit,
            SIGNAL(bookmark(int)),
            this,
            SLOT(manageBookmarkEmit(int))
           );
    connect(textEdit,
            SIGNAL(bookmarksAddLines(int, int)),
            this,
            SLOT(bookmarksAddLines(int, int))
           );
    connect(textEdit,
            SIGNAL(bookmarksRemoveLines(int, int)),
            this,
            SLOT(bookmarksRemoveLines(int, int))
           );
    connect(textEdit,
            SIGNAL(updateLineCounter()),
            this,
            SLOT(updateLineCounter())
           );
    connect(lineCount,
            SIGNAL(breakpoint(int)),
            this,
            SLOT(manageBreakpointEmit(int))
           );
    connect(lineCount,
            SIGNAL(bookmark(int)),
            this,
            SLOT(manageBookmarkEmit(int))
           );
    /*connect(textEdit,
            SIGNAL(blockCountChanged(int)),
            this,
            SLOT(changeHeight())
           );*/
    /*connect(textEdit,
            SIGNAL(textChangedSignal(const QString&, int)),
            this,
            SLOT(updateTextSlotOut(const QString&, int))
           );*/
    /*connect(textEdit,
            SIGNAL(selectionRemovedSignal(int, int)),
            this,
            SLOT(selectionRemovedOut(int, int))
           );*/
    connect(&GuiCfg::getInstance(),
            SIGNAL(editorFontChanged(QFont)),
            this,
            SLOT(changeFont(QFont))
           );
    connect(&GuiCfg::getInstance(),
            SIGNAL(editorFontChanged(QFont)),
            this->lineCount,
            SLOT(changeFont(QFont))
           );
    connect(&GuiCfg::getInstance(),
            SIGNAL(tabWidthChanged(int)),
            this,
            SLOT(changeTabStopWidth(int))
           );
    connect(&GuiCfg::getInstance(),
            SIGNAL(tabToSpacesChanged(bool)),
            this->textEdit,
            SLOT(setTabToSpaces(bool))
           );
    connect(&GuiCfg::getInstance(),
            SIGNAL(spacesInTabChanged(int)),
            this->textEdit,
            SLOT(setSpacesInTab(int))
           );
    //this->connectAct();
    prevBlockCount = this->textEdit->document()->blockCount();
    //this->show();
    this->changeHeight();
    //qDebug() << "CodeEdit: return CodeEdit()";
}



CodeEdit::CodeEdit(QWidget *parent, bool tabs, Project* parentPrj, QString wName, QString wPath, CodeEdit *parentCodeEdit)
    : QWidget(parent)
{
    //qDebug() << "CodeEdit: CodeEdit()2";
    this->parentCodeEdit = parentCodeEdit;
    this->curCodeEdit = NULL;
    //if (this->parentCodeEdit == NULL)
    //{
        //qDebug() << "parentCodeEdit is NULL";
        this->breakpointsLines = new QList<int>();
        this->bookmarksLines = new QList<int>();
    //}
    //else
    //{
        //?
        //qDebug() << "parentCodeEdit is not NULL";
        //this->breakpointsLines = NULL;
        //this->breakpointsLines = new QList<unsigned int>();
    //}
    
    int index = wName.lastIndexOf(".");
    if (index > 0)
    {
        QString text(wName.right(wName.size() - index));
        if (text == ".h")
        {
            textEdit = new WTextEdit(this, C);
        }
        else if (text == ".cpp" || text == ".cxx" || text == ".cc")
        {
            textEdit = new WTextEdit(this, CPP);
        }
        else if (text == ".c")
        {
            textEdit = new WTextEdit(this, C);
        }
        else if (text == ".asm" || text == ".psm")
        {
            //!!!DO NOT FORGET TO CHECK IF ASM IS AVR OR PIC TYPE!!!
            textEdit = new WTextEdit(this, PICOBLAZEASM);
        }
        else
        {
            textEdit = new WTextEdit(this, PLAIN);
        }
    }
    else
    {
        textEdit = new WTextEdit(this, PLAIN);
    }
    //textEdit->setContextMenuPolicy(Qt::NoContextMenu);
    textEdit->setFont(GuiCfg::getInstance().getEditorFont());
    QFontMetrics fontMetrics(textEdit->font());
    textEdit->setTabStopWidth(GuiCfg::getInstance().getTabWidth() * fontMetrics.width(' '));
    textEdit->setTabToSpaces(GuiCfg::getInstance().getTabToSpaces());
    textEdit->setSpacesInTab(GuiCfg::getInstance().getSpacesInTab());
    lineCount = new WLineCounter(textEdit, true, false, 0, textEdit->font());
    //statusBar = new QStatusBar(this);
    layout = new QGridLayout(this);
    layout->addWidget(lineCount, 0, 0);
    layout->addWidget(textEdit, 0, 1);
    //layout->addWidget(statusBar, 1, 0, 2, 1);
    name = wName;
    path = wPath;
    parentWidget = parent;
    this->tabs = tabs;
    parentProject = parentPrj;
    breakpointColor = new QColor(0,255,0);
    textEdit->setWordWrapMode(QTextOption::WordWrap);
    textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    //this->makeMenu();
    //this->setFocusPolicy(Qt::StrongFocus);
    //this->installEventFilter(this);
    if (wPath != NULL)
    {
        QFile file(path);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            this->textEdit->setPlainText(file.readAll());
            file.close();
        }
    }
    changed = false;
    connect(textEdit,
            SIGNAL(focusIn()),
            this,
            SLOT(getFocus())
           );
    connect(textEdit,
            SIGNAL(breakpoint(int)),
            this,
            SLOT(manageBreakpointEmit(int))
           );
    connect(textEdit,
            SIGNAL(breakpointsAddLines(int, int)),
            this,
            SLOT(breakpointsAddLines(int, int))
           );
    connect(textEdit,
            SIGNAL(breakpointsRemoveLines(int, int)),
            this,
            SLOT(breakpointsRemoveLines(int, int))
           );
    connect(textEdit,
            SIGNAL(bookmark(int)),
            this,
            SLOT(manageBookmarkEmit(int))
           );
    connect(textEdit,
            SIGNAL(bookmarksAddLines(int, int)),
            this,
            SLOT(bookmarksAddLines(int, int))
           );
    connect(textEdit,
            SIGNAL(bookmarksRemoveLines(int, int)),
            this,
            SLOT(bookmarksRemoveLines(int, int))
           );
    connect(textEdit,
            SIGNAL(updateLineCounter()),
            this,
            SLOT(updateLineCounter())
           );
    /*connect(textEdit,
            SIGNAL(blockCountChanged(int)),
            this,
            SLOT(changeHeight())
           );*/
    /*connect(textEdit,
            SIGNAL(textChangedSignal(const QString&, int)),
            this,
            SLOT(updateTextSlotOut(const QString&, int))
           );
    connect(textEdit,
            SIGNAL(selectionRemovedSignal(int, int)),
            this,
            SLOT(selectionRemovedOut(int, int))
           );*/
    connect(&GuiCfg::getInstance(),
            SIGNAL(editorFontChanged(QFont)),
            this,
            SLOT(changeFont(QFont))
           );
    connect(&GuiCfg::getInstance(),
            SIGNAL(editorFontChanged(QFont)),
            this->lineCount,
            SLOT(changeFont(QFont))
           );
    connect(&GuiCfg::getInstance(),
            SIGNAL(tabWidthChanged(int)),
            this,
            SLOT(changeTabStopWidth(int))
           );
    connect(&GuiCfg::getInstance(),
            SIGNAL(tabToSpacesChanged(bool)),
            this->textEdit,
            SLOT(setTabToSpaces(bool))
           );
    connect(&GuiCfg::getInstance(),
            SIGNAL(spacesInTabChanged(int)),
            this->textEdit,
            SLOT(setSpacesInTab(int))
           );
    //this->connectAct();
    prevBlockCount = this->textEdit->document()->blockCount();
    this->changeHeight();
    //qDebug() << "CodeEdit: return CodeEdit()2";
}


CodeEdit::~CodeEdit()
{
    if (parentCodeEdit == NULL)
    {
        delete this->breakpointsLines;
        delete this->bookmarksLines;
    }
}




void CodeEdit::connectAct()
{
    //qDebug() << "CodeEdit: connectAct()";
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(setChanged()));
    //qDebug() << "CodeEdit: return connectAct()";
}


//najit efektivnejsi reseni, neco jako signal disable pri zmene a enable pri savu
void CodeEdit::setChanged()
{
    qDebug() << "CodeEdit: setChanged()";
    if (changed == false)
    {
        changed = true;
        if (tabs == true)
        {
            //QString newName("*" + name);
            //qDebug() << "codeedit: emit changed tab status: changed";
            emit changedTabStatus(this->name, this->path, true);
            //((QTabWidget*)parentWidget)->setTabText(((QTabWidget*)parentWidget)->indexOf(this), "*" + name);
        }
        else
        {
            emit changedTabStatus(this->name, this->path, true);
        }
    }
    qDebug() << "CodeEdit: return setChanged()";
}


void CodeEdit::setSaved()
{
    qDebug() << "CodeEdit: setSaved()";
    if (changed == true)
    {
        changed = false;
        if (tabs == true)
        {
            //qDebug() << "codeedit: emit changed tab status: saved";
            emit changedTabStatus(this->name, this->path, false);
            //((QTabWidget*)parentWidget)->setTabText(((QTabWidget*)parentWidget)->indexOf(this), name);
        }
        else
        {
            //qDebug() << "codeedit: emit changed tab status: saved without tabs";
            emit changedTabStatus(this->name, this->path, false);
        }
    }
    /*else
    {
        qDebug() << "codeedit: not changed";
    }*/
    qDebug() << "CodeEdit: return setSaved()";
}


bool CodeEdit::isChanged()
{
    //qDebug() << "CodeEdit: isChanged()";
    //qDebug() << "CodeEdit: return isChanged()";
    return (changed == true);
}

void CodeEdit::setName(QString wName)
{
    //qDebug() << "CodeEdit: setName()";
    name = wName;
    //qDebug() << "CodeEdit: return setName()";
}

void CodeEdit::setPath(QString wPath)
{
    //qDebug() << "CodeEdit: setPath()";
    path = wPath;
    //qDebug() << "CodeEdit: return setPath()";
}


QString CodeEdit::getName()
{
    return name;
}

QString CodeEdit::getPath()
{
    return path;
}


bool CodeEdit::isChild(Project* project)
{
    return (project == parentProject);
}



void CodeEdit::setParentProject(Project* project)
{
    //qDebug() << "CodeEdit: setParentProject()";
    this->parentProject = project;
    //TODO: refresh breakpointLines and bookmarkLines
    //qDebug() << "CodeEdit: return setParentProject()";
}



WTextEdit* CodeEdit::getTextEdit()
{
    return textEdit;
}


void CodeEdit::splitHorizontal()
{
    //qDebug() << "CodeEdit: splitHorizontal()";
    //qDebug() << "Code Edit: split signal - horizontal";
    emit splitSignal(Qt::Horizontal, 0);
    //qDebug() << "CodeEdit: return splitHorizontal()";
}

void CodeEdit::splitVertical()
{
    //qDebug() << "CodeEdit: splitVertical()";
    //qDebug() << "Code Edit: split signal - vertical";
    emit splitSignal(Qt::Vertical, 0);
    //qDebug() << "CodeEdit: return splitVertical()";
}


/*void CodeEdit::selectionRemovedOut(int posStart, int posEnd)
{
    emit updateRemoveSelection(posStart, posEnd, this);
    emit updateAnalysers(this);
    if (prevBlockCount != this->textEdit->document()->blockCount())
    {
        this->changeHeight();
        prevBlockCount = this->textEdit->document()->blockCount();
    }
}


void CodeEdit::selectionRemovedIn(int posStart, int posEnd, CodeEdit *editor)
{
    if ( this != editor )
    {
        QTextCursor textCursor = this->textEdit->textCursor();
        int prevPos = textCursor.position();
        textCursor.setPosition(posStart);
        textCursor.setPosition(posEnd, QTextCursor::KeepAnchor);
        textCursor.removeSelectedText();

        textCursor.setPosition(prevPos);
        if ( NULL == parentCodeEdit )
        {
            //qDebug() << "CodeEdit: updateTextSlotIn parent";
            emit updateRemoveSelection(posStart, posEnd, editor);
        }
        else if ( this->textEdit->document()->blockCount() != prevBlockCount )
        {
            this->changeHeight();
            prevBlockCount = this->textEdit->document()->blockCount();
        }
        prevBlockCount = this->textEdit->document()->blockCount();
    }
}


void CodeEdit::updateTextSlotOut(const QString& text, int pos)
{
    qDebug() << "CodeEdit: updateTextSlotOut()";
    emit updateText(text, pos, this);
    emit updateAnalysers(this);
    if (prevBlockCount != this->textEdit->document()->blockCount())
    {
        this->changeHeight();
        prevBlockCount = this->textEdit->document()->blockCount();
    }
    qDebug() << "CodeEdit: return updateTextSlotOut()";
}

void CodeEdit::updateTextSlotIn(const QString& textIn, int pos, CodeEdit *editor)
{
    qDebug() << "CodeEdit: updateTextSlotIn()";
    //qDebug() << "Code Edit: update";
    if ( this != editor )
    {
        QTextCursor textCursor = this->textEdit->textCursor();
        int prevPos = textCursor.position();
        textCursor.setPosition(pos);
        if (textIn.length() == 1)
        {
            //printable 32 - 126 ASCII or space or tab
            if ( textIn[0].isPrint() || textIn[0].isSpace() )
            {
                textCursor.insertText(textIn);
            }
            //backspace
            else if ( textIn[0].toAscii() == 8 )
            {
                textCursor.deletePreviousChar();
            }
            //delete
            else if ( textIn[0].toAscii() == 127 )
            {
                textCursor.deleteChar();
            }
        }
        else
        {
            textCursor.insertText(textIn);
        }
        textCursor.setPosition(prevPos);
        if ( NULL == parentCodeEdit )
        {
            //qDebug() << "CodeEdit: updateTextSlotIn parent";
            emit updateText(textIn, pos, editor);
        }
        else if ( this->textEdit->document()->blockCount() != prevBlockCount )
        {
            this->changeHeight();
            prevBlockCount = this->textEdit->document()->blockCount();
        }
        prevBlockCount = this->textEdit->document()->blockCount();
    }
    qDebug() << "CodeEdit: return updateTextSlotIn()";
}*/


void CodeEdit::loadCodeEdit(CodeEdit* editor)
{
    //qDebug() << "CodeEdit: loadCodeEditor()";
    //disconnect(textEdit, SIGNAL(textChanged()), 0, 0);
    //disconnect(this, SIGNAL(updateText(const QString&, int, CodeEdit*)), 0, 0);
    //disconnect(this, SIGNAL(updateRemoveSelection(int, int, CodeEdit*)), 0, 0);
    //disconnect(this, SIGNAL(), 0, 0);
    //disconnect(this, SIGNAL(bookmarkListAdd(QString,int)), 0, 0);
    //disconnect(this, SIGNAL(bookmarkListRemove(QString,int)), 0, 0);
    //disconnect(this, SIGNAL(breakpointListAdd(QString,int)), 0, 0);
    //disconnect(this, SIGNAL(breakpointListRemove(QString,int)), 0, 0);
    //this->breakpointList.clear();
    //this->bookmarkList.clear();
    /*if (editor->getTextEdit()->toPlainText().isEmpty() == false)
    {
        this->textEdit->setPlainText(editor->getTextEdit()->toPlainText());
    }
    else
    {
        this->textEdit->setPlainText(" ");
    }*/
    //qDebug() << "is changed?" << editor->isChanged();
    if (curCodeEdit != NULL)
    {
        if (true == this->changed)
        {
            curCodeEdit->setChanged();
        }
        else
        {
            curCodeEdit->setSaved();
        }
        curCodeEdit->setScrollValue(this->textEdit->verticalScrollBar()->value());
        curCodeEdit->setCursorValue(this->textEdit->textCursor());
    }
    
    bool prevChanged = editor->isChanged();
    this->setName(editor->getName());
    this->setPath(editor->getPath());
    this->textEdit->deleteHighlighter();
    this->textEdit->setDocument(editor->getTextEdit()->document());
    //this->show();
    //this->update();
    this->breakpointsLines = editor->getBreakpointsLines();
    this->bookmarksLines = editor->getBookmarksLines();
    if (this->breakpointsLines == NULL)
    {
        qDebug() << "CodeEdit: breakpointsLines == NULL";
    }
    if (this->bookmarksLines == NULL)
    {
        qDebug() << "CodeEdit: bookmarskLines == NULL";
    }
    this->lineCount->getWidget()->setBreakpointList(this->breakpointsLines);
    this->lineCount->getWidget()->setBookmarkList(this->bookmarksLines);
    /*if (name != NULL)
    {
        int index = this->name.lastIndexOf(".");
        if (index > 0)
        {
            QString text(this->name.right(this->name.size() - index));
            if (text == ".h")
            {
                this->textEdit->reloadHighlighter(C);
            }
            else if (text == ".cpp" || text == ".cxx" || text == ".cc")
            {
                this->textEdit->reloadHighlighter(CPP);
            }
            else if (text == ".c")
            {
                this->textEdit->reloadHighlighter(C);
            }
            else if (text == ".asm")
            {
                //!!!DO NOT FORGET TO CHECK IF ASM IS AVR OR PIC TYPE!!!
                //qDebug() << "CodeEdit: asm";
                this->textEdit->reloadHighlighter(PICOBLAZEASM);
            }
            else
            {
                this->textEdit->reloadHighlighter(PLAIN);
            }
        }
        else
        {
                this->textEdit->reloadHighlighter(PLAIN);
        }
    }*/
    emit CodeEditChanged(editor);
    this->changeHeight();
    this->changed = prevChanged;
    if (false == this->changed)
    {
        emit changedTabStatus(this->name, this->path, false);
    }
    curCodeEdit = editor;
    if (false == curCodeEdit->getCursorValue().isNull())
    {
        this->textEdit->setTextCursor(curCodeEdit->getCursorValue());
    }
    this->textEdit->verticalScrollBar()->setValue(curCodeEdit->getScrollValue());
    /*if (true == editor->isChanged())
    {
        //qDebug() << "CodeEdit: loadcodeedit is changed";
        this->changed = true;
    }
    else
    {
        //qDebug() << "CodeEdit: loadcodeedit is saved";
        this->changed = false;
    }*/
    //qDebug() << "CodeEdit: return loadCodeEditor()";
}


QWidget* CodeEdit::getParent()
{
    return parentWidget;
}



void CodeEdit::getFocus()
{
    //qDebug() << "CodeEdit: getFocus()";
    if (parentWidget != NULL)
    {
        ((BaseEditor*)parentWidget)->focusIn();
    }
    //qDebug() << "CodeEdit: return getFocus()";
}

void CodeEdit::manageBreakpointEmit(int line)
{
    //qDebug() << "CodeEdit: manageBreakpointEmit()";
    /*bool prevChanged = this->changed;
    if (prevChanged == true)
    {
        qDebug() << "CodeEdit: already changed";
    }
    if (false == textEdit->isLineHighlighted(line, breakpointColor))
    {
        textEdit->highlightLine(line, breakpointColor);
        emit breakpointEmit(this->path, line);
    }
    else
    {
        textEdit->highlightLine(line, NULL);
    }
    if (false == prevChanged)
    {
        this->setSaved();
    }*/
    emit breakpointEmit(this->path, line);
    //qDebug() << "CodeEdit: return manageBreakpointEmit()";
}

void CodeEdit::manageBookmarkEmit(int line)
{
    //qDebug() << "CodeEdit: manageBookmarkEmit()";
    emit bookmarkEmit(this->path, line);
    //qDebug() << "CodeEdit: return manageBookmarkEmit()";
}

/*QList<int> CodeEdit::getBreakpointList()
{
    return breakpointList;
}*/

/*QList<int> CodeEdit::getBookmarkList()
{
    return bookmarkList;
}*/


CodeEdit* CodeEdit::getParentCodeEdit()
{
    if (this->parentCodeEdit == NULL)
    {
        return this;
    }
    else
    {
        return this->parentCodeEdit;
    }
}


void CodeEdit::setParentCodeEdit(CodeEdit *parentCodeEdit)
{
    //qDebug() << "CodeEdit: setParentCodeEdit()";
    this->parentCodeEdit = parentCodeEdit;
    //qDebug() << "CodeEdit: return setParentCodeEdit()";
}


void CodeEdit::changeHeight()
{
    //qDebug() << "CodeEdit: changeHeight()";
    this->lineCount->getWidget()->changeHeight();
    //this->lineCount->getWidget()->update();
    //qDebug() << "CodeEdit: return changeHeight()";
}


void CodeEdit::updateLineCounter()
{
    this->lineCount->getWidget()->update();
}


void CodeEdit::changeFont(QFont font)
{
    this->textEdit->setFont(font);
}


Project* CodeEdit::getParentProject()
{
    return this->parentProject;
}


void CodeEdit::addBreakpointLine(int line)
{
    this->breakpointsLines->append(line);
    this->lineCount->getWidget()->update();
}


void CodeEdit::removeBreakpointLine(int line)
{
    this->breakpointsLines->removeAll(line);
    this->lineCount->getWidget()->update();
}


QList<int>* CodeEdit::getBreakpointsLines()
{
    return this->breakpointsLines;
}


void CodeEdit::addBookmarkLine(int line)
{
    this->bookmarksLines->append(line);
    this->lineCount->getWidget()->update();
}


void CodeEdit::removeBookmarkLine(int line)
{
    this->bookmarksLines->removeAll(line);
    this->lineCount->getWidget()->update();
}


QList<int>* CodeEdit::getBookmarksLines()
{
    return this->bookmarksLines;
}


void CodeEdit::breakpointsAddLines(int line, int linesAdded)
{
    qDebug() << "CodeEdit: breakpointsAddLines";
    emit breakpointsAddLines(this->path, line, linesAdded);
}


void CodeEdit::breakpointsRemoveLines(int line, int linesRemoved)
{
    emit breakpointsRemoveLines(this->path, line, linesRemoved);
}


void CodeEdit::moveBreakpointsLines(int line, int linesChanged, bool added)
{
    bool update = false;
    if (true == added)
    {
        for (int i = 0; i < this->breakpointsLines->count(); i++)
        {
            if (this->breakpointsLines->at(i) > line)
            {
                (*(this->breakpointsLines))[i] += linesChanged;
                update = true;
            }
        }
    }
    else
    {
        QList<int>::iterator i;
        for (i = this->breakpointsLines->begin(); i != this->breakpointsLines->end(); i++)
        {
            if ( *i >= line
            && *i < line + linesChanged
            )
            {
                *i = -1;
                update = true;
            }
            else
            {
                if (*i >= line + linesChanged)
                {
                    *i -= linesChanged;
                    update = true;
                }
            }
        }
        this->breakpointsLines->removeAll(-1);
        /*if (this->breakpointsLines->contains(line))
        {
            this->breakpointsLines->removeAll(line);
            update = true;
        }*/
        /*
        for (int i = 0; i < this->breakpointsLines->count(); i++)
        {
            if ( this->breakpointsLines->at(i) >= line
              && this->breakpointsLines->at(i) <= line + linesChanged;
               )
            {
                this->breakpointsLines->removeAll(line);
                update = true;
            }
            else
            {
                if (this->breakpointsLines->at(i) > line + linesChanged)
                {
                    (*(this->breakpointsLines))[i] -= linesChanged;
                    update = true;
                }
            }
        }*/
    }
    if (true == update)
    {
        this->lineCount->getWidget()->update();
    }
}


void CodeEdit::bookmarksAddLines(int line, int linesAdded)
{
    qDebug() << "CodeEdit: bookmarksAddLines";
    emit bookmarksAddLines(this->path, line, linesAdded);
}


void CodeEdit::bookmarksRemoveLines(int line, int linesRemoved)
{
    emit bookmarksRemoveLines(this->path, line, linesRemoved);
}


void CodeEdit::moveBookmarksLines(int line, int linesChanged, bool added)
{
    bool update = false;
    if (true == added)
    {
        for (int i = 0; i < this->bookmarksLines->count(); i++)
        {
            if (this->bookmarksLines->at(i) > line)
            {
                (*(this->bookmarksLines))[i] += linesChanged;
                update = true;
            }
        }
    }
    else
    {
        QList<int>::iterator i;
        for (i = this->bookmarksLines->begin(); i != this->bookmarksLines->end(); i++)
        {
            if ( *i >= line
            && *i < line + linesChanged
            )
            {
                *i = -1;
                update = true;
            }
            else
            {
                if (*i >= line + linesChanged)
                {
                    *i -= linesChanged;
                    update = true;
                }
            }
        }
        this->bookmarksLines->removeAll(-1);
    }
    if (true == update)
    {
        this->lineCount->getWidget()->update();
    }
}


void CodeEdit::changeTabStopWidth(int width)
{
    QFontMetrics fontMetrics(textEdit->font());
    textEdit->setTabStopWidth(width * fontMetrics.width(' '));
}


void CodeEdit::setBreakpointsLines(QList<unsigned int> breakpoints)
{
    //qDebug() << "CodeEdit: setBreakpointsLines(unsigned int)";
    /*if (breakpoints == NULL)
    {
        qDebug() << "CodeEdit: breakpoints NULL";
    }*/
    //qDebug() << "breakpoints count" << breakpoints.count();
    //if (breakpoints != NULL && breakpointsLines != NULL)
    if (breakpointsLines != NULL)
    {
        this->breakpointsLines->clear();
        for (int i = 0; i < breakpoints.count(); i++)
        {
            //qDebug() << "CodeEdit: appending breakpoint";
            this->breakpointsLines->append((int)breakpoints.at(i));
        }
        //qDebug() << "CodeEdit: setBreakpoints - updateWidget()";
        this->lineCount->getWidget()->update();
    }
}


void CodeEdit::setBreakpointsLines(QList<int> breakpoints)
{
    if (breakpointsLines != NULL)
    {
        this->breakpointsLines->clear();
        for (int i = 0; i < breakpoints.count(); i++)
        {
            this->breakpointsLines->append(breakpoints.at(i));
        }
        this->lineCount->getWidget()->update();
    }
}


void CodeEdit::setBookmarksLines(QList<unsigned int> bookmarks)
{
    if (this->bookmarksLines != NULL)
    {
        this->bookmarksLines->clear();
        for (int i = 0; i < bookmarks.count(); i++)
        {
            this->bookmarksLines->append((int)bookmarks.at(i));
        }
        this->lineCount->getWidget()->update();
    } 
}


void CodeEdit::setBookmarksLines(QList<int> bookmarks)
{
    if (this->bookmarksLines != NULL)
    {
        this->bookmarksLines->clear();
        for (int i = 0; i < bookmarks.count(); i++)
        {
            this->bookmarksLines->append(bookmarks.at(i));
        }
        this->lineCount->getWidget()->update();
    }
}


int CodeEdit::getScrollValue()
{
    return this->curScrollValue;
}


void CodeEdit::setScrollValue(int value)
{
    this->curScrollValue = value;
}


QTextCursor CodeEdit::getCursorValue()
{
    return this->curCursorPos;
}


void CodeEdit::setCursorValue(QTextCursor value)
{
    this->curCursorPos = value;
}