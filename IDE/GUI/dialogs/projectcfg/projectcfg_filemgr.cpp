/**
 * @brief C++ file for file manager class
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 *
 */



#include "projectcfg_filemgr.h"
#include "../project/project.h"
#include <QFileDialog>
#include "../errordialog/errordlg.h"


/**
 * @brief
 * @param
 * @param
 */
ProjectCfg_FileMgr::ProjectCfg_FileMgr(QWidget *parentWidget, Project *currProject)
    : QWidget(parentWidget)
{
    //prepare widget
    //this->parent = parentWidget;
    this->project = currProject;
    this->reloadFiles = false;
    ui.setupUi(this);
    //load files
    if (this->project != NULL)
    {
        if (this->project->fileNames.size() > 0)
        {
            QString absolutePath = QFileInfo(this->project->prjPath).dir().path();
            for (int i = 0; i < this->project->fileNames.size(); i++)
            {
                QListWidgetItem *newItem = new QListWidgetItem(this->project->fileNames.at(i), ui.lstFiles);
                newItem->setToolTip(QDir(absolutePath + "/" + this->project->filePaths.at(i)).canonicalPath());
                ui.lstFiles->addItem(newItem);
            }
        }
    }
    //connect buttons with actions
    connect(this->ui.btnNew, SIGNAL(clicked()), this, SLOT(newFile()));
    connect(ui.btnAdd, SIGNAL(clicked()), this, SLOT(addFile()));
    connect(ui.btnSetMain, SIGNAL(clicked()), this, SLOT(setMainFile()));
    connect(ui.btnDelete, SIGNAL(clicked()), this, SLOT(deleteFile()));
    //ui.btnNew->setCheckable(false);
}



/**
 * @brief
 */
void ProjectCfg_FileMgr::deleteFile()
{
    if (ui.lstFiles->currentItem() != NULL)
    {
        project->removeFile(ui.lstFiles->currentItem()->toolTip(), ui.lstFiles->currentItem()->text());
        delete ui.lstFiles->currentItem();
        if (reloadFiles == false)
        {
            emit reloadTree();
            reloadFiles = true;
        }
    }
}


/**
 * @brief
 */
void ProjectCfg_FileMgr::newFile()
{
    //modal dialog window
    QString path = QFileDialog::getSaveFileName(this, tr("Source File"), QString(), QString(), 0, QFileDialog::DontUseNativeDialog);
    if (path != NULL)
    {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            error(ERR_OPENFILE);
        }
        else
        {
            file.close();
            project->addFile(path, path.section('/', -1));
            QListWidgetItem *newItem = new QListWidgetItem(path.section('/', -1), ui.lstFiles);
            newItem->setToolTip(path);
            ui.lstFiles->addItem(newItem);
            if (reloadFiles == false)
            {
                emit reloadTree();
                reloadFiles = true;
            }
        }
    }
}


/**
 * @brief
 */
void ProjectCfg_FileMgr::addFile()
{
    //dialog window (file search)
    QString path = QFileDialog::getOpenFileName(this, tr("Source File"), "");
    if (path != NULL)
    {
        project->addFile(path, path.section('/', -1));
        QListWidgetItem *newItem = new QListWidgetItem(path.section('/', -1), ui.lstFiles);
        newItem->setToolTip(path);
        ui.lstFiles->addItem(newItem);
        /*if (reloadFiles == false)
        {
            emit reloadTree();
            reloadFiles = true;
        }*/
    }
}


/**
 * @brief
 */
void ProjectCfg_FileMgr::setMainFile()
{
    if (ui.lstFiles->currentItem() != NULL)
    {
        project->setMainFile(ui.lstFiles->currentItem()->toolTip(), ui.lstFiles->currentItem()->text());
        /*if (reloadFiles == false)
        {
            emit reloadTree();
            reloadFiles = true;
        }*/
    }
} 
