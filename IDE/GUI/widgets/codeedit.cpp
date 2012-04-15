#include <QtGui>
#include "codeedit.h"
#include "wdockmanager.h"


CodeEdit::CodeEdit(QTabWidget *parent, QString wName, QString wPath)
    : QWidget(parent)
{
    textEdit = new QPlainTextEdit(this);
    lineCount = new WLineCounter((QTextEdit*)textEdit, false, this->height());
    layout = new QGridLayout(this);
    layout->addWidget(lineCount, 0, 0);
    layout->addWidget(textEdit, 0, 1);
    setLayout(layout);
    name = wName;
    path = wPath;
    changed = false;
    parentWidget = parent;
    parentProject = NULL;
    textEdit->setWordWrapMode(QTextOption::NoWrap);
    textEdit->setFont(QFont ("Andale Mono", 11));
    //setWordWrapMode(QTextOption::WordWrap);
}



CodeEdit::CodeEdit(QTabWidget *parent, Project* parentPrj, QString wName, QString wPath)
    : QWidget(parent)
{
    textEdit = new QPlainTextEdit(this);
    lineCount = new WLineCounter((QTextEdit*)textEdit, false, this->height());
    layout = new QGridLayout(this);
    layout->addWidget(lineCount, 0, 0);
    layout->addWidget(textEdit, 0, 1);
    setLayout(layout);
    name = wName;
    path = wPath;
    changed = false;
    parentWidget = parent;
    parentProject = parentPrj;
    textEdit->setWordWrapMode(QTextOption::NoWrap);
    textEdit->setFont(QFont ("Andale Mono", 11));
}




void CodeEdit::connectAct()
{
    connect(this, SIGNAL(textEdit->textChanged()), this, SLOT(setChanged()));
}


//najit efektivnejsi reseni, neco jako signal disable pri zmene a enable pri savu
void CodeEdit::setChanged()
{
    if (changed == false)
    {
        changed = true;
        parentWidget->setTabText(parentWidget->indexOf(this), "*" + name);
    }
}


void CodeEdit::setSaved()
{
    //if (changed == true)
    changed = false;
    parentWidget->setTabText(parentWidget->indexOf(this), name);
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



QPlainTextEdit *CodeEdit::getTextEdit()
{
    return textEdit;
}
