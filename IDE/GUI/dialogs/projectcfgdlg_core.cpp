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
    this->tabs = new QTabWidget(this);
    this->tabs->setTabsClosable(false);
    this->tabs->setMovable(false);
    this->generalCfg = new ProjectCfg_General(this, this->project);
    this->fileMgr = new FileMgr(this, this->project);
    this->tabs->addTab(this->generalCfg, "General");
    this->tabs->addTab(this->fileMgr, "Files");
    this->show();
    //this->resize(tabs->width(), tabs->height());
    this->setFixedWidth(this->tabs->width());
    this->setFixedHeight(this->tabs->height());
    this->setModal(true);

    connect(fileMgr, SIGNAL(reloadTree()), this, SLOT(reload()));
}


void ProjectConfigDialog_Core::closeEvent(QCloseEvent * e)
{
    if (reloadFiles == true)
        emit reloadTree();
}


void ProjectConfigDialog_Core::reload()
{
    reloadFiles = true;
}
