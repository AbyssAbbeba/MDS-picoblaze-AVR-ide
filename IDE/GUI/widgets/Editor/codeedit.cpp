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
    this->parentCodeEdit = parentCodeEdit;
    if (this->parentCodeEdit == NULL && parentCodeEdit == NULL)
        qDebug() << "PARENT CODE EDIT: NULL";
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
                textEdit = new WTextEdit(this, AVRASM);
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
    lineCount = new WLineCounter(textEdit, false, false, 20);
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
    //textEdit->setWordWrapMode(QTextOption::NoWrap);
    textEdit->setWordWrapMode(QTextOption::WordWrap);
    textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    textEdit->setFont(QFont ("Andale Mono", 11));
    this->makeMenu();
    //this->setFocusPolicy(Qt::StrongFocus);
    //this->textEdit->setFocusPolicy(Qt::NoFocus);
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
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(updateTextSlotOut()));
    this->connectAct();
}



CodeEdit::CodeEdit(QWidget *parent, bool tabs, Project* parentPrj, QString wName, QString wPath, CodeEdit *parentCodeEdit)
    : QWidget(parent)
{
    this->parentCodeEdit = parentCodeEdit;
    if (this->parentCodeEdit == NULL && parentCodeEdit == NULL)
        qDebug() << "PARENT CODE EDIT: NULL";
    
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
            textEdit = new WTextEdit(this, AVRASM);
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
    lineCount = new WLineCounter(textEdit, false, false, 20);
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
    textEdit->setWordWrapMode(QTextOption::WordWrap);
    textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    textEdit->setFont(QFont ("Andale Mono", 11));
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
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(updateTextSlotOut()));
    this->connectAct();
}


CodeEdit::~CodeEdit()
{
    if (parentCodeEdit == NULL)
        qDebug() << "DELETING PARENT CODE EDIT!";
}


void CodeEdit::makeMenu()
{
    editorPopup = new QMenu(this);
    QAction *splitHorizontalAct = new QAction("Split horizontal", editorPopup);
    QAction *splitVerticalAct = new QAction("Split vertical", editorPopup);
    editorPopup->addAction(splitHorizontalAct);
    editorPopup->addAction(splitVerticalAct);
    connect(splitHorizontalAct, SIGNAL(triggered()), this, SLOT(splitHorizontal()));
    connect(splitVerticalAct, SIGNAL(triggered()), this, SLOT(splitVertical()));
}




void CodeEdit::connectAct()
{
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(setChanged()));
}


//najit efektivnejsi reseni, neco jako signal disable pri zmene a enable pri savu
void CodeEdit::setChanged()
{
    if (changed == false)
    {
        changed = true;
        if (tabs == true)
        {
            //QString newName("*" + name);
            qDebug() << "codeedit: emit changed tab status: changed";
            emit changedTabStatus(this->name, this->path, true);
            //((QTabWidget*)parentWidget)->setTabText(((QTabWidget*)parentWidget)->indexOf(this), "*" + name);
        }
    }
    lineCount->getWidget()->update();
}


void CodeEdit::setSaved()
{
    if (changed == true)
    {
        changed = false;
        if (tabs == true)
        {
            qDebug() << "codeedit: emit changed tab status: saved";
            emit changedTabStatus(this->name, this->path, false);
            //((QTabWidget*)parentWidget)->setTabText(((QTabWidget*)parentWidget)->indexOf(this), name);
        }
        else
        {
            qDebug() << "codeedit: emit changed tab status: saved without tabs";
            emit changedTabStatus(this->name, this->path, false);
        }
    }
    else
    {
        qDebug() << "codeedit: not changed";
    }
}


bool CodeEdit::isChanged()
{
    return (changed == true);
}

void CodeEdit::setName(QString wName)
{
    name = wName;
}

void CodeEdit::setPath(QString wPath)
{
    path = wPath;
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
    parentProject=project;
}



WTextEdit* CodeEdit::getTextEdit()
{
    return textEdit;
}


void CodeEdit::splitHorizontal()
{
    qDebug() << "Code Edit: split signal - horizontal";
    emit splitSignal(Qt::Horizontal, 0);
}

void CodeEdit::splitVertical()
{
    qDebug() << "Code Edit: split signal - vertical";
    emit splitSignal(Qt::Vertical, 0);
}


void CodeEdit::contextMenuEvent(QContextMenuEvent *event)
{
    //if (target == textEdit)
        editorPopup->popup(event->globalPos());
}


void CodeEdit::updateTextSlotOut()
{
    emit updateText(this->textEdit->toPlainText());
    emit updateAnalysers(this);
}

void CodeEdit::updateTextSlotIn(const QString& textIn)
{
    //qDebug() << "Code Edit: update";
    if (textIn.compare(this->textEdit->toPlainText()) != 0)
       this->textEdit->setText(textIn);
}


void CodeEdit::loadCodeEdit(CodeEdit* editor)
{
    qDebug() << "Code Edit: load Code Editor";
    //disconnect(textEdit, SIGNAL(textChanged()), 0, 0);
    disconnect(this, SIGNAL(updateText(const QString&)), 0, 0);
    disconnect(this, SIGNAL(bookmarkListAdd(int)), 0, 0);
    disconnect(this, SIGNAL(bookmarkListRemove(int)), 0, 0);
    disconnect(this, SIGNAL(breakpointListAdd(int)), 0, 0);
    disconnect(this, SIGNAL(breakpointListRemove(int)), 0, 0);
    this->breakpointList.clear();
    this->bookmarkList.clear();
    this->textEdit->setText(editor->getTextEdit()->toPlainText());
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
                this->textEdit->reloadHighlighter(AVRASM);
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
}


QWidget* CodeEdit::getParent()
{
    return parentWidget;
}



void CodeEdit::getFocus()
{
    ((BaseEditor*)parentWidget)->focusIn();
}

void CodeEdit::manageBreakpointEmit(int line)
{
    int index;
    index = breakpointList.indexOf(line);
    textEdit->highlightLine(line);
    if (index == -1)
    {
        breakpointList.append(line);
        emit breakpointListAdd(line);
    }
    else
    {
        breakpointList.removeAt(index);
        emit breakpointListRemove(line);
    }
}

void CodeEdit::manageBookmarkEmit(int line)
{
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
        return this;
    else
        return this->parentCodeEdit;
}


void CodeEdit::setParentCodeEdit(CodeEdit *parentCodeEdit)
{
    this->parentCodeEdit = parentCodeEdit;
}