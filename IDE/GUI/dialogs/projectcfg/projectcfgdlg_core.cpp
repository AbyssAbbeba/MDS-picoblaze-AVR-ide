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



/**
 * @brief
 * @param
 * @param
 */
ProjectConfigDialog_Core::ProjectConfigDialog_Core(QWidget *parent, Project *currProject)
    : QDialog(parent)
{
    qDebug() << "ProjectConfigDialog_Core: ProjectConfigDialog_Core()";
    this->project = currProject;
    this->reloadFiles = false;
    this->setModal(true);
    this->setWindowTitle("Project Config");

    this->cfgInterface = new CfgInterface(this);
    this->generalCfg = new ProjectCfg_General(cfgInterface, this->project);
    this->compilerCfg = new ProjectCfg_Compiler(cfgInterface, this->project);
    this->pathsCfg = new ProjectCfg_CompPaths(cfgInterface, this->project);
    this->fileMgr = new ProjectCfg_FileMgr(cfgInterface, this->project);

    this->cfgInterface->addWidget(NULL, "Project", "Project Config");
    this->cfgInterface->addWidget(this->generalCfg, "Options", "Project Options", true);
    this->cfgInterface->addWidget(this->fileMgr, "Files", "Project Files", true);
    this->cfgInterface->addWidget(NULL, "Compiler", "Compiler Config");
    this->cfgInterface->addWidget(this->compilerCfg, "Options", "Compiler Options", true);
    this->cfgInterface->addWidget(this->pathsCfg, "Include Paths", "Compiler Include Paths", true);
    

    this->cfgInterface->show();
    this->show();
    this->cfgInterface->fixSize();
    this->setFixedWidth(this->cfgInterface->width());
    this->setFixedHeight(this->cfgInterface->height());
    //this->generalCfg->fixButtonBox();
    
    connect(this->fileMgr, SIGNAL(reloadTree()), this, SLOT(reload()));
    connect(this->cfgInterface->buttonBox, SIGNAL(accepted()), this, SLOT(ok()));
    connect(this->cfgInterface->buttonBox, SIGNAL(rejected()), this, SLOT(cancel()));
    qDebug() << "ProjectConfigDialog_Core: return ProjectConfigDialog_Core()";
}



/**
 * @brief
 * @param
 */
void ProjectConfigDialog_Core::closeEvent(QCloseEvent * e)
{
    if (reloadFiles == true)
    {
        emit reloadTree();
    }
}


/**
 * @brief
 */
void ProjectConfigDialog_Core::reload()
{
    reloadFiles = true;
}


/**
 * @brief
 */
void ProjectConfigDialog_Core::ok()
{
    this->compilerCfg->save();
    this->pathsCfg->save();
    this->done(1);
}


/**
 * @brief
 */
void ProjectConfigDialog_Core::cancel()
{
    this->done(0);
}
