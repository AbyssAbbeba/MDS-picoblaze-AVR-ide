#include <QtGui>
#include "projectcfg_compiler.h"



ProjectCfg_Compiler::ProjectCfg_Compiler(QWidget *parentWidget, Project *currProject)
    : QWidget(parentWidget)
{
    this->project = currProject;


    //this-> buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    
    this->lblMain = new QLabel("Main File", this);
    this->lblSymbolTbl = new QLabel("Symbol Table", this);
    this->lblMacroTbl = new QLabel("Macro Table", this);
    this->lblDbgFile = new QLabel("Debug File", this);
    this->lblCodeTree = new QLabel("Code Tree", this);
    this->lblLstFile = new QLabel("List File", this);
    this->lblHexFile = new QLabel("Hex File", this);
    this->lblBinFile = new QLabel("Bin File", this);
    this->lblSRecFile = new QLabel("SRec File", this);

    this->leMain = new QLineEdit(this);
    this->leMain->setText(this->project->mainFileName);
    this->leMain->setMaximumWidth(70);
    this->leMain->setReadOnly(true);
    this->leMain->setFrame(false);
    
    this->chckSymbolTbl = new QCheckBox(this);
    this->chckMacroTbl = new QCheckBox(this);
    this->chckDbgFile = new QCheckBox(this);
    this->chckCodeTree = new QCheckBox(this);
    this->chckLstFile = new QCheckBox(this);
    this->chckHexFile = new QCheckBox(this);
    this->chckBinFile = new QCheckBox(this);
    this->chckSRecFile = new QCheckBox(this);

    this->lblMain->move(5,5);
    this->lblSymbolTbl->move(25,30);
    this->lblMacroTbl->move(25,50);
    this->lblDbgFile->move(25,70);
    this->lblCodeTree->move(25,90);
    this->lblLstFile->move(25,110);
    this->lblHexFile->move(25,130);
    this->lblBinFile->move(25,150);
    this->lblSRecFile->move(25,170);

    this->leMain->move(75, 5);

    this->chckSymbolTbl->move(5,30);
    this->chckMacroTbl->move(5,50);
    this->chckDbgFile->move(5,70);
    this->chckCodeTree->move(5,90);
    this->chckLstFile->move(5,110);
    this->chckHexFile->move(5,130);
    this->chckBinFile->move(5,150);
    this->chckSRecFile->move(5,170);
}


void ProjectCfg_Compiler::load()
{
    this->chckSymbolTbl->setChecked(project->compileOpt.at(0));
    this->chckMacroTbl->setChecked(project->compileOpt.at(1));
    this->chckDbgFile->setChecked(project->compileOpt.at(2));
    this->chckCodeTree->setChecked(project->compileOpt.at(3));
    this->chckLstFile->setChecked(project->compileOpt.at(4));
    this->chckHexFile->setChecked(project->compileOpt.at(5));
    this->chckBinFile->setChecked(project->compileOpt.at(6));
    this->chckSRecFile->setChecked(project->compileOpt.at(7));
}


void ProjectCfg_Compiler::save()
{
    QList<bool> opt;
    opt[0] = this->chckSymbolTbl->isChecked();
    opt[1] = this->chckMacroTbl->isChecked();
    opt[2] = this->chckDbgFile->isChecked();
    opt[3] = this->chckCodeTree->isChecked();
    opt[4] = this->chckLstFile->isChecked();
    opt[5] = this->chckHexFile->isChecked();
    opt[6] = this->chckBinFile->isChecked();
    opt[7] = this->chckSRecFile->isChecked();
    
    this->project->setCompileOpt(opt);
}