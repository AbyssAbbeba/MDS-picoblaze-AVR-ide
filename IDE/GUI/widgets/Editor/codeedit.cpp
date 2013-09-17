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
#include "codeedit.h"
#include "wdockmanager.h"


CodeEdit::CodeEdit(QWidget *parent, bool tabs, QString wName, QString wPath, CodeEdit *parentCodeEdit)
    : QWidget(parent)
{
    qDebug() << "CodeEdit: CodeEdit()";
    this->parentCodeEdit = parentCodeEdit;
    if (this->parentCodeEdit == NULL)
    {
        qDebug() << "PARENT CODE EDIT: NULL";
    }
    if (wName == NULL || wPath == NULL)
    {
        textEdit = new WTextEdit(this, PLAIN);
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
            else if (text == ".asm")
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
    textEdit->setContextMenuPolicy(Qt::NoContextMenu);
    textEdit->setFont(QFont ("Andale Mono", 11));
    QFontMetrics fontMetrics(textEdit->font());
    textEdit->setTabStopWidth(4*fontMetrics.width(' '));
    lineCount = new WLineCounter(textEdit, false, false, 0, textEdit->font());
    layout = new QGridLayout(this);
    layout->addWidget(lineCount, 0, 0);
    layout->addWidget(textEdit, 0, 1);
    setLayout(layout);
    name = wName;
    path = wPath;
    changed = false;
    parentWidget = parent;
    this->tabs = tabs;
    parentProject = NULL;
    breakpointColor = new QColor(0,255,0);
    //textEdit->setWordWrapMode(QTextOption::NoWrap);
    textEdit->setWordWrapMode(QTextOption::WordWrap);
    textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    this->makeMenu();
    //this->setFocusPolicy(Qt::StrongFocus);
    //this->textEdit->setFocusPolicy(Qt::NoFocus);
    //this->installEventFilter(this);
    if (wPath != NULL && wPath != "untracked")
    {
        QFile file(path);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            this->textEdit->setPlainText(file.readAll());
            file.close();
        }
    }
    connect(textEdit, SIGNAL(focusIn()), this, SLOT(getFocus()));
    connect(textEdit, SIGNAL(breakpoint(int)), this, SLOT(manageBreakpointEmit(int)));
    connect(textEdit, SIGNAL(bookmark(int)), this, SLOT(manageBookmarkEmit(int)));
    connect(textEdit, SIGNAL(textChangedSignal(const QString&, int)), this, SLOT(updateTextSlotOut(const QString&, int)));
    //this->connectAct();
    prevBlockCount = this->textEdit->document()->blockCount();
    qDebug() << "CodeEdit: return CodeEdit()";
}



CodeEdit::CodeEdit(QWidget *parent, bool tabs, Project* parentPrj, QString wName, QString wPath, CodeEdit *parentCodeEdit)
    : QWidget(parent)
{
    qDebug() << "CodeEdit: CodeEdit()";
    this->parentCodeEdit = parentCodeEdit;
    if (this->parentCodeEdit == NULL) //&& parentCodeEdit == NULL)
    {
        qDebug() << "PARENT CODE EDIT: NULL";
    }
    
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
        else if (text == ".asm")
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
    textEdit->setContextMenuPolicy(Qt::NoContextMenu);
    textEdit->setFont(QFont ("Andale Mono", 11));
    lineCount = new WLineCounter(textEdit, false, false, 0, textEdit->font());
    layout = new QGridLayout(this);
    layout->addWidget(lineCount, 0, 0);
    layout->addWidget(textEdit, 0, 1);
    setLayout(layout);
    name = wName;
    path = wPath;
    changed = false;
    parentWidget = parent;
    this->tabs = tabs;
    parentProject = parentPrj;
    breakpointColor = new QColor(0,255,0);
    textEdit->setWordWrapMode(QTextOption::WordWrap);
    textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    this->makeMenu();
    this->setFocusPolicy(Qt::StrongFocus);
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
    connect(textEdit, SIGNAL(focusIn()), this, SLOT(getFocus()));
    connect(textEdit, SIGNAL(breakpoint(int)), this, SLOT(manageBreakpointEmit(int)));
    connect(textEdit, SIGNAL(bookmark(int)), this, SLOT(manageBookmarkEmit(int)));
    connect(textEdit, SIGNAL(textChangedSignal(const QString&, int)), this, SLOT(updateTextSlotOut(const QString&, int)));
    //this->connectAct();
    prevBlockCount = this->textEdit->document()->blockCount();
    qDebug() << "CodeEdit: return CodeEdit()";
}


CodeEdit::~CodeEdit()
{
    if (parentCodeEdit == NULL)
    {
        qDebug() << "DELETING PARENT CODE EDIT!";
    }
}


void CodeEdit::makeMenu()
{
    qDebug() << "CodeEdit: makeMenu()";
    editorPopup = new QMenu(this);
    QAction *splitHorizontalAct = new QAction("Split horizontal", editorPopup);
    QAction *splitVerticalAct = new QAction("Split vertical", editorPopup);
    editorPopup->addAction(splitHorizontalAct);
    editorPopup->addAction(splitVerticalAct);
    connect(splitHorizontalAct, SIGNAL(triggered()), this, SLOT(splitHorizontal()));
    connect(splitVerticalAct, SIGNAL(triggered()), this, SLOT(splitVertical()));
    qDebug() << "CodeEdit: return makeMenu()";
}




void CodeEdit::connectAct()
{
    qDebug() << "CodeEdit: connectAct()";
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(setChanged()));
    qDebug() << "CodeEdit: return connectAct()";
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
    qDebug() << "CodeEdit: isChanged()";
    qDebug() << "CodeEdit: return isChanged()";
    return (changed == true);
}

void CodeEdit::setName(QString wName)
{
    qDebug() << "CodeEdit: setName()";
    name = wName;
    qDebug() << "CodeEdit: return setName()";
}

void CodeEdit::setPath(QString wPath)
{
    qDebug() << "CodeEdit: setPath()";
    path = wPath;
    qDebug() << "CodeEdit: return setPath()";
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
    return (project==parentProject);
}



void CodeEdit::setParentProject(Project* project)
{
    qDebug() << "CodeEdit: setParentProject()";
    parentProject=project;
    qDebug() << "CodeEdit: return setParentProject()";
}



WTextEdit* CodeEdit::getTextEdit()
{
    return textEdit;
}


void CodeEdit::splitHorizontal()
{
    qDebug() << "CodeEdit: splitHorizontal()";
    //qDebug() << "Code Edit: split signal - horizontal";
    emit splitSignal(Qt::Horizontal, 0);
    qDebug() << "CodeEdit: return splitHorizontal()";
}

void CodeEdit::splitVertical()
{
    qDebug() << "CodeEdit: splitVertical()";
    //qDebug() << "Code Edit: split signal - vertical";
    emit splitSignal(Qt::Vertical, 0);
    qDebug() << "CodeEdit: return splitVertical()";
}


void CodeEdit::contextMenuEvent(QContextMenuEvent *event)
{
    //if (target == textEdit)
        editorPopup->popup(event->globalPos());
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
            qDebug() << "CodeEdit: updateTextSlotIn parent";
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
}


void CodeEdit::loadCodeEdit(CodeEdit* editor)
{
    qDebug() << "CodeEdit: loadCodeEditor()";
    //disconnect(textEdit, SIGNAL(textChanged()), 0, 0);
    disconnect(this, SIGNAL(updateText(const QString&, int, CodeEdit*)), 0, 0);
    //disconnect(this, SIGNAL(), 0, 0);
    disconnect(this, SIGNAL(bookmarkListAdd(int)), 0, 0);
    disconnect(this, SIGNAL(bookmarkListRemove(int)), 0, 0);
    disconnect(this, SIGNAL(breakpointListAdd(int)), 0, 0);
    disconnect(this, SIGNAL(breakpointListRemove(int)), 0, 0);
    this->breakpointList.clear();
    this->bookmarkList.clear();
    if (editor->getTextEdit()->toPlainText() != "")
    {
        this->textEdit->setText(editor->getTextEdit()->toPlainText());
    }
    this->setName(editor->getName());
    this->setPath(editor->getPath());
    if (name != NULL)
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
    }
    emit CodeEditChanged(editor);
    this->changeHeight();
    qDebug() << "CodeEdit: return loadCodeEditor()";
}


QWidget* CodeEdit::getParent()
{
    return parentWidget;
}



void CodeEdit::getFocus()
{
    qDebug() << "CodeEdit: getFocus()";
    if (parentWidget != NULL)
    {
        ((BaseEditor*)parentWidget)->focusIn();
    }
    qDebug() << "CodeEdit: return getFocus()";
}

void CodeEdit::manageBreakpointEmit(int line)
{
    qDebug() << "CodeEdit: manageBreakpointEmit()";
    int index;
    index = breakpointList.indexOf(line);
    if (index == -1)
    {
        textEdit->highlightLine(line, breakpointColor);
        breakpointList.append(line);
        emit breakpointListAdd(line);
    }
    else
    {
        textEdit->highlightLine(line, NULL);
        breakpointList.removeAt(index);
        emit breakpointListRemove(line);
    }
    qDebug() << "CodeEdit: return manageBreakpointEmit()";
}

void CodeEdit::manageBookmarkEmit(int line)
{
    qDebug() << "CodeEdit: manageBookmarkEmit()";
    int index;
    index = bookmarkList.indexOf(line);
    if (index == -1)
    {
        bookmarkList.append(line);
        emit bookmarkListAdd(line);
    }
    else
    {
        bookmarkList.removeAt(index);
        emit bookmarkListRemove(line);
    }
    qDebug() << "CodeEdit: return manageBookmarkEmit()";
}

QList<int> CodeEdit::getBreakpointList()
{
    return breakpointList;
}

QList<int> CodeEdit::getBookmarkList()
{
    return bookmarkList;
}


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
    qDebug() << "CodeEdit: setParentCodeEdit()";
    this->parentCodeEdit = parentCodeEdit;
    qDebug() << "CodeEdit: return setParentCodeEdit()";
}


void CodeEdit::changeHeight()
{
    qDebug() << "CodeEdit: changeHeight()";
    this->lineCount->getWidget()->changeHeight();
    //this->lineCount->getWidget()->update();
    qDebug() << "CodeEdit: return changeHeight()";
}