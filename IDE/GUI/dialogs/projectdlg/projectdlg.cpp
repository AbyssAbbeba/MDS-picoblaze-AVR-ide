/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup ProjectDialog
 * @file projectdlg.cpp
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
    this->prjdlg_memory = new ProjectCfg_Memory(this, NULL);
    this->prjdlg_compiler = new ProjectCfg_Compiler(this, NULL);
    this->prjdlg_templates = new ProjectCfg_Templates(this, NULL);
    this->prjdlg_comppaths = new ProjectCfg_CompPaths(this, NULL);
    this->prjdlg_filemgr = new ProjectCfg_FileMgr(this, NULL);
    int height = prjdlg_general->height() + tabWidget->height();
    int width = prjdlg_comppaths->width();// + tabWidget->width();
    tabWidget->addTab(prjdlg_general, "General");
    tabWidget->addTab(prjdlg_memory, "Memory");
    tabWidget->addTab(prjdlg_compiler, "Compiler");
    tabWidget->addTab(prjdlg_templates, "Templates");
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
    connect(this->prjdlg_general,
            SIGNAL(setHWBuildEnabled(bool)),
            this->prjdlg_memory,
            SLOT(setHWBuildEnabled(bool))
           );
    connect(this->prjdlg_general,
            SIGNAL(setScratchpadMaximum(int)),
            this->prjdlg_memory,
            SLOT(setScratchpadMaximum(int))
           );
    connect(this->prjdlg_general,
            SIGNAL(setProgMemMaximum(int)),
            this->prjdlg_memory,
            SLOT(setProgMemMaximum(int))
           );
    connect(this->prjdlg_filemgr,
            SIGNAL(setFiles(QList<QString>, QString)),
            this->prjdlg_compiler,
            SLOT(setFiles(QList<QString>, QString))
           );
    connect(this->prjdlg_compiler,
            SIGNAL(setMainFile(QString)),
            this->prjdlg_filemgr,
            SLOT(setMainFileByName(QString))
           );
    this->prjdlg_memory->setHWBuildEnabled(true);
    this->prjdlg_memory->setScratchpadMaximum(log2(256));
    this->prjdlg_memory->setProgMemMaximum(log2(4096));

}


/**
 * @brief
 */
void ProjectDialog::bCreate()
{
    if (this->prjdlg_memory->getIntVector() == -1)
    {
        QMessageBox msgBox;
        msgBox.setText("Enter valid Interrupt Vector value");
        msgBox.exec();
        return;
    }
    if (this->prjdlg_memory->getHWBuild() == -2)
    {
        QMessageBox msgBox;
        msgBox.setText("Enter valid HWBuild value");
        msgBox.exec();
        return;
    }
    //vytvoreni projektu
    if (true == this->prjdlg_general->getPath().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("Enter valid project path");
        msgBox.exec();
        return;
    }
    if (true == this->prjdlg_general->getName().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("Enter valid project name");
        msgBox.exec();
        return;
    }
    QDir prjDir(this->prjdlg_general->getPath());
    if (false == prjDir.exists())
    {
        prjDir.mkpath(".");
    }
    QFile file(this->prjdlg_general->getPath() + "/" + this->prjdlg_general->getName() + ".mds-project");
    if (true == file.exists())
    {
        QString text = "File " + file.fileName() + " already exists. Do you want to rewrite the file anyway?";
        if (QMessageBox::Yes != QMessageBox::question(this, "File Exists", text, QMessageBox::Yes|QMessageBox::No))
        {
            return;
        }
    }
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("Error opening file");
        msgBox.exec();
    }
    else
    {
        //nacteni projektu do manageru otevrenych projektu
        projectMan->addProject(this->prjdlg_general->getName(),
                               this->prjdlg_general->getPath() + "/" + this->prjdlg_general->getName() + ".mds-project",
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

        projectMan->getActive()->setIntVector(this->prjdlg_memory->getIntVector());
        projectMan->getActive()->setHWBuild(this->prjdlg_memory->getHWBuild());
        projectMan->getActive()->setScratchpad(this->prjdlg_memory->getScratchpadSize());
        projectMan->getActive()->setProgMem(this->prjdlg_memory->getProgMemSize());
        projectMan->getActive()->setAsmType(prjdlg_general->getAsmType());
        projectMan->getActive()->saveProject();

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
