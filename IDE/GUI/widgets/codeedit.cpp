#include <QtGui>
#include "codeedit.h"
#include "wdockmanager.h"


CodeEdit::CodeEdit(QTabWidget *parent, QString wName, QString wPath)
    : QPlainTextEdit(parent)
{
    name = wName;
    path = wPath;
    changed = false;
    parentWidget = parent;
    parentProject = NULL;
    setWordWrapMode(QTextOption::NoWrap);
    setFont(QFont ("Andale Mono", 11));
    //setWordWrapMode(QTextOption::WordWrap);
}



CodeEdit::CodeEdit(QTabWidget *parent, Project* parentPrj, QString wName, QString wPath)
    : QPlainTextEdit(parent)
{
    name = wName;
    path = wPath;
    changed = false;
    parentWidget = parent;
    parentProject = parentPrj;
}




void CodeEdit::connectAct()
{
    connect(this, SIGNAL(textChanged()), this, SLOT(setChanged()));
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
