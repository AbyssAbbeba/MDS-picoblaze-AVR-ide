/**
 * @brief C++ file for file manager class
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 *
 */



#include "projectcfg_comppaths.h"
#include "../project/project.h"
#include <QFileDialog>
#include "../errordialog/errordlg.h"


/**
 * @brief
 * @param
 * @param
 */
ProjectCfg_CompPaths::ProjectCfg_CompPaths(QWidget *parentWidget, Project *currProject)
    : QWidget(parentWidget)
{
    this->project = currProject;
    fileList = new QListWidget(this);
    deleteBtn = new QPushButton("Delete", this);
    newBtn = new QPushButton("New", this);
    editBtn = new QPushButton("Edit", this);
    btnLayout = new QVBoxLayout(this);
    btnLayout->addWidget(newBtn);
    btnLayout->addWidget(editBtn);
    btnLayout->addWidget(deleteBtn);
    btnWidget = new QWidget(this);
    btnWidget->setLayout(btnLayout);
    layout = new QGridLayout(this);
    layout->addWidget(fileList, 0,0,Qt::AlignJustify);
    layout->addWidget(btnWidget, 0,1,Qt::AlignJustify);
    this->setLayout(layout);
    this->adjustSize();

    connect(this->newBtn, SIGNAL(clicked()), this, SLOT(New()));
    connect(this->editBtn, SIGNAL(clicked()), this, SLOT(Edit()));
    connect(this->deleteBtn, SIGNAL(clicked()), this, SLOT(Delete()));
    //connect(this, SIGNAL(savePaths(QList<QString>)), this->project, SLOT(setCompileIncPaths(QList<QString>)));
}


void ProjectCfg_CompPaths::New()
{
    
}


void ProjectCfg_CompPaths::Edit()
{

}


void ProjectCfg_CompPaths::Delete()
{

}


void ProjectCfg_CompPaths::load()
{
    for (int i = 0; i < this->project->compileIncPaths.count(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem(this->project->compileIncPaths.at(i), this->fileList);
    }
}


void ProjectCfg_CompPaths::save()
{
    QStringList paths;
    for (int i = 0; i < this->fileList->count(); i++)
    {
        paths.append(this->fileList->item(i)->text());
    }
    this->project->setCompileIncPaths(paths);
}