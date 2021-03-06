/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file projectcfgdlg_core.cpp
 */


#include "projectcfgdlg_core.h"
#include "../project/project.h"
#include <QtGui>
#include "projectcfg_general.h"
#include "projectcfg_memory.h"
#include "projectcfg_filemgr.h"
#include "projectcfg_compiler.h"
#include "projectcfg_templates.h"
#include "projectcfg_comppaths.h"
#include "../cfginterface/cfginterface.h"



/**
 * @brief
 * @param
 * @param
 */
ProjectConfigDialog_Core::ProjectConfigDialog_Core(QWidget *parent, Project *currProject)
    : QDialog(parent)
{
    //qDebug() << "ProjectConfigDialog_Core: ProjectConfigDialog_Core()";
    this->project = currProject;
    this->reloadFiles = false;
    this->setModal(true);
    this->setWindowTitle(tr("Project configuration"));
    this->setWindowIcon(QIcon(QPixmap(":resources/icons/page_white_wrench.png")));

    this->cfgInterface = new CfgInterface(this);
    this->generalCfg = new ProjectCfg_General(cfgInterface, this->project);
    this->memoryCfg = new ProjectCfg_Memory(cfgInterface, this->project);
    this->compilerCfg = new ProjectCfg_Compiler(cfgInterface, this->project);
    this->templatesCfg = new ProjectCfg_Templates(cfgInterface, this->project);
    this->pathsCfg = new ProjectCfg_CompPaths(cfgInterface, this->project);
    this->fileMgr = new ProjectCfg_FileMgr(cfgInterface, this->project);

    this->cfgInterface->addWidget(NULL, "Project", "Project Config");
    this->cfgInterface->addWidget(this->generalCfg, "Options", "Project Options", true);
    this->cfgInterface->addWidget(this->memoryCfg, "Memory", "Memory Options", true);
    this->cfgInterface->addWidget(this->fileMgr, "Files", "Project Files", true);
    this->cfgInterface->addWidget(NULL, "Compiler", "Compiler Config");
    this->cfgInterface->addWidget(this->compilerCfg, "Options", "Compiler Options", true);
    this->cfgInterface->addWidget(this->templatesCfg, "Templates", "Templates Options", true);
    this->cfgInterface->addWidget(this->pathsCfg, "Include Paths", "Compiler Include Paths", true);

    this->cfgInterface->show();
    this->show();
    this->cfgInterface->fixSize();
    this->cfgInterface->expandAll();
    this->setFixedWidth(this->cfgInterface->width());
    this->setFixedHeight(this->cfgInterface->height());
    //this->generalCfg->fixButtonBox();

    connect(this->fileMgr, SIGNAL(reloadTree()), this, SLOT(reload()));
    connect(this->cfgInterface->buttonBox, SIGNAL(accepted()), this, SLOT(ok()));
    connect(this->cfgInterface->buttonBox, SIGNAL(rejected()), this, SLOT(cancel()));
    connect(this->generalCfg,
            SIGNAL(setHWBuildEnabled(bool)),
            this->memoryCfg,
            SLOT(setHWBuildEnabled(bool))
           );
    connect(this->generalCfg,
            SIGNAL(setScratchpadMaximum(int)),
            this->memoryCfg,
            SLOT(setScratchpadMaximum(int))
           );
    connect(this->generalCfg,
            SIGNAL(setProgMemMaximum(int)),
            this->memoryCfg,
            SLOT(setProgMemMaximum(int))
           );
    connect(this->fileMgr,
            SIGNAL(setFiles(QList<QString>, QString)),
            this->compilerCfg,
            SLOT(setFiles(QList<QString>, QString))
           );
    connect(this->compilerCfg,
            SIGNAL(setMainFile(QString)),
            this->fileMgr,
            SLOT(setMainFileByName(QString))
           );
    qDebug() << "set maximum";
    this->memoryCfg->setScratchpadMaximum(this->generalCfg->getScratchpadMaximum(), false);
    this->memoryCfg->setProgMemMaximum(this->generalCfg->getProgMemMaximum(), false);
    this->fileMgr->requestFiles();
    qDebug() << "ProjectConfigDialog_Core: return ProjectConfigDialog_Core()";
}



/**
 * @brief
 * @param
 */
/*void ProjectConfigDialog_Core::closeEvent(QCloseEvent * e)
{
    if (reloadFiles == true)
    {
        emit reloadTree();
    }
}*/


/**
 * @brief
 */
void ProjectConfigDialog_Core::reload()
{
    qDebug() << "ProjectConfigDialog_Core: reload()";
    reloadFiles = true;
}


/**
 * @brief
 */
void ProjectConfigDialog_Core::ok()
{
    int value = this->memoryCfg->save();
    if (value == -1)
    {
        QMessageBox msgBox;
        msgBox.setText("Enter valid Interrupt Vector value");
        msgBox.exec();
        return;
    }
    if (value == -2)
    {
        QMessageBox msgBox;
        msgBox.setText("Enter valid HWBuild value");
        msgBox.exec();
        return;
    }
    this->generalCfg->save();
    this->compilerCfg->save();
    this->pathsCfg->save();
    this->memoryCfg->save();
    if (reloadFiles == true)
    {
        emit reloadTree();
    }
    this->done(1);
}


/**
 * @brief
 */
void ProjectConfigDialog_Core::cancel()
{
    if (reloadFiles == true)
    {
        emit reloadTree();
    }
    this->done(0);
}
