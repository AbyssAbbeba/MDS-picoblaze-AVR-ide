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



#include <QtGui>
#include "projectdlg.h"


/**
 * @brief
 * @param
 * @param
 */
ProjectDialog::ProjectDialog(QWidget *parent, ProjectMan *dialogProjectMan)
    : QDialog(parent)
{   
    projectMan = dialogProjectMan;

    QTabWidget *tabWidget = new QTabWidget(this);
    this->prjdlg_general = new Projectdlg_General(this);
    this->prjdlg_compiler = new ProjectCfg_Compiler(this, NULL);
    this->prjdlg_comppaths = new ProjectCfg_CompPaths(this, NULL);
    this->prjdlg_filemgr = new ProjectCfg_FileMgr(this, NULL);
    int height = prjdlg_general->height() + tabWidget->height();
    int width = prjdlg_comppaths->width();// + tabWidget->width();
    tabWidget->addTab(prjdlg_general, "General");
    tabWidget->addTab(prjdlg_compiler, "Compiler");
    tabWidget->addTab(prjdlg_comppaths, "Paths");
    tabWidget->addTab(prjdlg_filemgr, "Files");
    
    this->buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    this->show();
    tabWidget->setFixedHeight(height);
    tabWidget->setFixedWidth(width);
    
    this->setFixedWidth(tabWidget->width());
    this->setFixedHeight(tabWidget->height() + this->buttonBox->height() + 10);
    this->buttonBox->move(this->width() - this->buttonBox->width() - 5, this->height() - this->buttonBox->height() - 5);

    
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(bCreate()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}


/**
 * @brief
 */
void ProjectDialog::bCreate()
{
    if (this->prjdlg_general->getIntVector() == -1)
    {
        QMessageBox msgBox;
        msgBox.setText("Enter valid Interrupt Vector value");
        msgBox.exec();
        return;
    }
    if (this->prjdlg_general->getHWBuild() == -2)
    {
        QMessageBox msgBox;
        msgBox.setText("Enter valid HWBuild value");
        msgBox.exec();
        return;
    }
    //vytvoreni projektu
    QFile file(this->prjdlg_general->getPath() + "/" + this->prjdlg_general->getName() + ".mmp");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox msgBox;
        if (this->prjdlg_general->getName().isEmpty() == true)
        {
            msgBox.setText("Enter valid project name");
        }
        else
        {
            msgBox.setText("Error opening file");
        }
        msgBox.exec();
    }
    else
    {
        //nacteni projektu do manageru otevrenych projektu
        projectMan->addProject(this->prjdlg_general->getName(),
                               this->prjdlg_general->getPath() + "/" + this->prjdlg_general->getName() + ".mmp",
                               this->prjdlg_general->getFamily(),
                               LANG_ASM,
                               &file);
        file.close();

        
        projectMan->getActive()->setCompileOpt(this->prjdlg_compiler->getOpt());
        projectMan->getActive()->setCompileIncPaths(this->prjdlg_comppaths->getPaths());
        
        QStringList paths = this->prjdlg_filemgr->getPaths();
        for (int i = 0; i < paths.count(); i++)
        {
            projectMan->getActive()->addFile(paths.at(i), paths.at(i).section('/', -1));
        }

        projectMan->getActive()->setIntVector(this->prjdlg_general->getIntVector());
        projectMan->getActive()->setHWBuild(this->prjdlg_general->getHWBuild());
        projectMan->getActive()->setScratchpad(this->prjdlg_general->getScratchpadSize());
        projectMan->getActive()->setProgMem(this->prjdlg_general->getProgMemSize());

        accept();
    }
}


/**
 * @brief
 */
void ProjectDialog::bReject()
{
    reject();
}
