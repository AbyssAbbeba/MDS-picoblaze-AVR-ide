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
    this->lblSymbolTbl->move(25,50);
    this->lblMacroTbl->move(25,70);
    this->lblDbgFile->move(25,90);
    this->lblCodeTree->move(25,110);
    /*this->lblLstFile->move(25,110);
    this->lblHexFile->move(25,130);
    this->lblBinFile->move(25,150);
    this->lblSRecFile->move(25,170);*/
    this->lblLstFile->move(155,50);
    this->lblHexFile->move(155,70);
    this->lblBinFile->move(155,90);
    this->lblSRecFile->move(155, 110);

    this->leMain->move(5, 20);

    this->chckSymbolTbl->move(5,50);
    this->chckMacroTbl->move(5,70);
    this->chckDbgFile->move(5,90);
    this->chckCodeTree->move(5,110);
    /*this->chckLstFile->move(5,110);
    this->chckHexFile->move(5,130);
    this->chckBinFile->move(5,150);
    this->chckSRecFile->move(5,170);*/
    this->chckLstFile->move(135,50);
    this->chckHexFile->move(135,70);
    this->chckBinFile->move(135,90);
    this->chckSRecFile->move(135,110);

    this->load();
}


/**
 * @brief
 */
void ProjectCfg_Compiler::load()
{
    qDebug() << "ProjectCfg_Compiler: load()";
    this->chckSymbolTbl->setChecked(project->compileOpt.at(0));
    this->chckMacroTbl->setChecked(project->compileOpt.at(1));
    this->chckDbgFile->setChecked(project->compileOpt.at(2));
    this->chckCodeTree->setChecked(project->compileOpt.at(3));
    this->chckLstFile->setChecked(project->compileOpt.at(4));
    this->chckHexFile->setChecked(project->compileOpt.at(5));
    this->chckBinFile->setChecked(project->compileOpt.at(6));
    this->chckSRecFile->setChecked(project->compileOpt.at(7));
    this->leMain->setText(project->mainFileName);
    
    /*int w = leMain->fontMetrics().boundingRect(leMain->text()).width();
    if (w > 200)
    {
    }
    else
    {*/
        this->leMain->setMinimumWidth(200);
        this->leMain->setMaximumWidth(200);
    //}
    qDebug() << "ProjectCfg_Compiler: return load()";
}


/**
 * @brief
 */
void ProjectCfg_Compiler::save()
{
    QList<bool> opt;
    opt.append(this->chckSymbolTbl->isChecked());
    opt.append(this->chckMacroTbl->isChecked());
    opt.append(this->chckDbgFile->isChecked());
    opt.append(this->chckCodeTree->isChecked());
    opt.append(this->chckLstFile->isChecked());
    opt.append(this->chckHexFile->isChecked());
    opt.append(this->chckBinFile->isChecked());
    opt.append(this->chckSRecFile->isChecked());
    
    this->project->setCompileOpt(opt);
}