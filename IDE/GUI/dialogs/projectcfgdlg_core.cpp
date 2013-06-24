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

ProjectConfigDialog_Core::ProjectConfigDialog_Core(QWidget *parent, Project *currProject)
    : QDialog(parent)
{
    qDebug() << "ProjectConfigDialog_Core: ProjectConfigDialog_Core()";
    this->project = currProject;
    this->reloadFiles = false;
    //this->setModal(true);

    this->cfgInterface = new CfgInterface(this);
    this->generalCfg = new ProjectCfg_General(cfgInterface, this->project);
    this->compilerCfg = new ProjectCfg_Compiler(cfgInterface, this->project);
    this->pathsCfg = new ProjectCfg_CompPaths(cfgInterface, this->project);
    this->fileMgr = new ProjectCfg_FileMgr(cfgInterface, this->project);
    
    this->cfgInterface->addWidget(this->generalCfg, "General");
    this->cfgInterface->addWidget(this->compilerCfg, "Compiler");
    this->cfgInterface->addWidget(this->pathsCfg, "Include Paths", true);
    this->cfgInterface->addWidget(this->fileMgr, "Files");
    

    this->cfgInterface->show();
    this->show();
    this->cfgInterface->fixSize();
    this->setFixedWidth(this->cfgInterface->width());
    this->setFixedHeight(this->cfgInterface->height());
    
    connect(this->fileMgr, SIGNAL(reloadTree()), this, SLOT(reload()));
    qDebug() << "ProjectConfigDialog_Core: return ProjectConfigDialog_Core()";
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


/*void ProjectConfigDialog_Core::changeWidget(QTreeWidgetItem *curr, QTreeWidgetItem *prev)
{
    this->tabs->setCurrentIndex(curr->type());
}*/
