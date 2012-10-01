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


CodeEdit::CodeEdit(QWidget *parent, bool tabs, QString wName, QString wPath)
    : QWidget(parent)
{
    textEdit = new QTextEdit(this);
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
    textEdit->setWordWrapMode(QTextOption::NoWrap);
    //setWordWrapMode(QTextOption::WordWrap);
    textEdit->setFont(QFont ("Andale Mono", 11));
    this->makeMenu();
    this->setFocusPolicy(Qt::StrongFocus);
    this->textEdit->setFocusPolicy(Qt::NoFocus);
    this->installEventFilter(this);
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        this->textEdit->setPlainText(file.readAll());
        file.close();
    }
    //this->connectAct();
}



CodeEdit::CodeEdit(QWidget *parent, bool tabs, Project* parentPrj, QString wName, QString wPath)
    : QWidget(parent)
{
    textEdit = new QTextEdit(this);
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
    textEdit->setWordWrapMode(QTextOption::NoWrap);
    textEdit->setFont(QFont ("Andale Mono", 11));
    this->makeMenu();
    this->setFocusPolicy(Qt::StrongFocus);
    this->installEventFilter(this);
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        this->textEdit->setPlainText(file.readAll());
        file.close();
    }
    //this->connectAct();
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
            QString newName("*" + name);
            emit changedTabName(this, newName);
            //((QTabWidget*)parentWidget)->setTabText(((QTabWidget*)parentWidget)->indexOf(this), "*" + name);
        }
    }
    lineCount->getWidget()->update();
}


void CodeEdit::setSaved()
{
    //if (changed == true)
    changed = false;
    if (tabs == true)
        emit changedTabName(this, name);
        //((QTabWidget*)parentWidget)->setTabText(((QTabWidget*)parentWidget)->indexOf(this), name);
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



QTextEdit* CodeEdit::getTextEdit()
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
}

void CodeEdit::updateTextSlotIn(const QString& textIn)
{
    qDebug() << "Code Edit: update";
    if (textIn.compare(this->textEdit->toPlainText()) != 0)
       this->textEdit->setText(textIn);
}


void CodeEdit::loadCodeEdit(CodeEdit* editor)
{
    qDebug() << "Code Edit: load Code Editor" << editor->getTextEdit()->toPlainText();
    disconnect(this, SIGNAL(textChanged()), 0, 0);
    disconnect(this, SIGNAL(updateText(const QString&)), 0, 0);
    this->textEdit->setText(editor->getTextEdit()->toPlainText());
    emit CodeEditChanged(editor);
}


QWidget* CodeEdit::getParent()
{
    return parentWidget;
}


bool CodeEdit::eventFilter(QObject *target, QEvent *event)
{
    if (event->type() == QEvent::FocusIn)
    {
        if (target == this || target == textEdit)
        {
            ((BaseEditor*)parentWidget)->focusIn();
            textEdit->setFocus(Qt::MouseFocusReason);
            return true;
        }
    }
    return QWidget::eventFilter(target, event);
}
