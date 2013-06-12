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



#include "projectcfgdlg_core.h"
#include "../project/project.h"

ProjectConfigDialog_Core::ProjectConfigDialog_Core(QWidget *dialogParent, Project *currProject)
{
    this->parent = dialogParent;
    this->project = currProject;
    this->reloadFiles = false;
    
    this->tabs = new QStackedWidget(this);
    this->generalCfg = new ProjectCfg_General(this, this->project);
    this->compilerCfg = new ProjectCfg_Compiler(this, this->project);
    this->fileMgr = new ProjectCfg_FileMgr(this, this->project);
    this->tabs->addWidget(this->generalCfg);
    this->tabs->addWidget(this->compilerCfg);
    this->tabs->addWidget(this->fileMgr);
    
    this->menuList = new QListWidget(this);
    this->menuList->addItem("General");
    this->menuList->addItem("Compiler");
    this->menuList->addItem("Files");
    
    this->show();
    this->menuList->move(5,10);
    this->menuList->setMaximumHeight(this->tabs->height());
    this->menuList->setMaximumWidth(100);
    this->tabs->move(105,0);
    //this->resize(tabs->width(), tabs->height());
    this->setFixedWidth(this->tabs->width()+this->menuList->width()+10);
    this->setFixedHeight(this->tabs->height());
    this->setModal(true);

    connect(this->fileMgr, SIGNAL(reloadTree()), this, SLOT(reload()));
    connect(this->menuList, SIGNAL(currentRowChanged(int)), this, SLOT(changeWidget(int)));
}


void ProjectConfigDialog_Core::closeEvent(QCloseEvent * e)
{
    if (reloadFiles == true)
    {
        emit reloadTree();
    }
}


void ProjectConfigDialog_Core::reload()
{
    reloadFiles = true;
}


void ProjectConfigDialog_Core::changeWidget(int row)
{
    this->tabs->setCurrentIndex(row);
}
