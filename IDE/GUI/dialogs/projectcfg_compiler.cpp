#include <QtGui>
#include "projectcfg_compiler.h"



ProjectCfg_Compiler::ProjectCfg_Compiler(QWidget *parentWidget, Project *currProject)
    : QWidget(parentWidget)
{
    this->project = currProject;
    
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