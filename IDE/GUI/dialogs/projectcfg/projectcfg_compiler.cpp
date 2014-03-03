#include <QtGui>
#include "projectcfg_compiler.h"



/**
 * @brief
 * @param
 * @param
 */
ProjectCfg_Compiler::ProjectCfg_Compiler(QWidget *parentWidget, Project *currProject)
    : QWidget(parentWidget)
{
    this->project = currProject;

    ui.setupUi(this);

    if (this->project != NULL)
    {
        this->load();
    }
    else
    {
    }
}


/**
 * @brief
 */
void ProjectCfg_Compiler::load()
{
    this->ui.chckSymbolTbl->setChecked(project->compileOpt.at(0));
    this->ui.chckMacroTbl->setChecked(project->compileOpt.at(1));
    this->ui.chckDbgFile->setChecked(project->compileOpt.at(2));
    this->ui.chckCodeTree->setChecked(project->compileOpt.at(3));
    this->ui.chckLstFile->setChecked(project->compileOpt.at(4));
    this->ui.chckHexFile->setChecked(project->compileOpt.at(5));
    this->ui.chckBinFile->setChecked(project->compileOpt.at(6));
    this->ui.chckSRecFile->setChecked(project->compileOpt.at(7));
    this->ui.chckMemFile->setChecked(project->compileOpt.at(8));
    this->ui.chckRawHexFile->setChecked(project->compileOpt.at(9));
    this->ui.chckVerilogFile->setChecked(project->compileOpt.at(10));
    this->ui.chckVHDLFile->setChecked(project->compileOpt.at(11));
    if (project->prjPath == "untracked" && project->prjName == "untracked")
    {
        this->ui.leMain->setDisabled(true);
        this->ui.chckMain->setDisabled(true);
    }
    else
    {
        this->ui.leMain->setText(project->mainFileName);
        this->ui.chckMain->setChecked(project->useMainFile);
    }
}


/**
 * @brief
 */
void ProjectCfg_Compiler::save()
{
    QList<bool> opt;
    opt.append(this->ui.chckSymbolTbl->isChecked());
    opt.append(this->ui.chckMacroTbl->isChecked());
    opt.append(this->ui.chckDbgFile->isChecked());
    opt.append(this->ui.chckCodeTree->isChecked());
    opt.append(this->ui.chckLstFile->isChecked());
    opt.append(this->ui.chckHexFile->isChecked());
    opt.append(this->ui.chckBinFile->isChecked());
    opt.append(this->ui.chckSRecFile->isChecked());
    opt.append(this->ui.chckMemFile->isChecked());
    opt.append(this->ui.chckRawHexFile->isChecked());
    opt.append(this->ui.chckVerilogFile->isChecked());
    opt.append(this->ui.chckVHDLFile->isChecked());
    
    this->project->setCompileOpt(opt);
    this->project->setUseMainFile(this->ui.chckMain->isChecked());
}


/**
 * @brief
 */
QList<bool> ProjectCfg_Compiler::getOpt()
{
    QList<bool> opt;
    opt.append(this->ui.chckSymbolTbl->isChecked());
    opt.append(this->ui.chckMacroTbl->isChecked());
    opt.append(this->ui.chckDbgFile->isChecked());
    opt.append(this->ui.chckCodeTree->isChecked());
    opt.append(this->ui.chckLstFile->isChecked());
    opt.append(this->ui.chckHexFile->isChecked());
    opt.append(this->ui.chckBinFile->isChecked());
    opt.append(this->ui.chckSRecFile->isChecked());

    return opt;
}