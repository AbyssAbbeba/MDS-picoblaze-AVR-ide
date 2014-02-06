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
}


/**
 * @brief
 */
void ProjectCfg_Compiler::load()
{
    qDebug() << "ProjectCfg_Compiler: load()";
    this->ui.chckSymbolTbl->setChecked(project->compileOpt.at(0));
    this->ui.chckMacroTbl->setChecked(project->compileOpt.at(1));
    this->ui.chckDbgFile->setChecked(project->compileOpt.at(2));
    this->ui.chckCodeTree->setChecked(project->compileOpt.at(3));
    this->ui.chckLstFile->setChecked(project->compileOpt.at(4));
    this->ui.chckHexFile->setChecked(project->compileOpt.at(5));
    this->ui.chckBinFile->setChecked(project->compileOpt.at(6));
    this->ui.chckSRecFile->setChecked(project->compileOpt.at(7));
    this->ui.leMain->setText(project->mainFileName);
    qDebug() << "ProjectCfg_Compiler: return load()";
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
    
    this->project->setCompileOpt(opt);
}