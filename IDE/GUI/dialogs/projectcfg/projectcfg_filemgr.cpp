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
 * @file projectcfg_filemgr.cpp
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
    this->mainFile = "";
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
    connect(ui.btnNew, SIGNAL(clicked()), this, SLOT(newFile()));
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
        if (this->project != NULL)
        {
            project->removeFile(ui.lstFiles->currentItem()->toolTip(), ui.lstFiles->currentItem()->text());
        }
        if (ui.lstFiles->currentItem()->text() == this->mainFile)
        {
            this->mainFile = "";
        }
        delete ui.lstFiles->currentItem();
        if (reloadFiles == false && this->project != NULL)
        {
            emit reloadTree();
            reloadFiles = true;
        }
        QList<QString> files;
        for (int i = 0; i < ui.lstFiles->count(); i++)
        {
            files.append(ui.lstFiles->item(i)->text());
        }
        emit setFiles(files, this->mainFile);
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
            if (this->project != NULL)
            {
                project->addFile(path, path.section('/', -1));
            }
            QListWidgetItem *newItem = new QListWidgetItem(path.section('/', -1), ui.lstFiles);
            newItem->setToolTip(path);
            ui.lstFiles->addItem(newItem);
            if (reloadFiles == false && this->project != NULL)
            {
                emit reloadTree();
                reloadFiles = true;
            }
            QList<QString> files;
            for (int i = 0; i < ui.lstFiles->count(); i++)
            {
                files.append(ui.lstFiles->item(i)->text());
            }
            emit setFiles(files, this->mainFile);
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
        if (this->project != NULL)
        {
            project->addFile(path, path.section('/', -1));
        }
        QListWidgetItem *newItem = new QListWidgetItem(path.section('/', -1), ui.lstFiles);
        newItem->setToolTip(path);
        ui.lstFiles->addItem(newItem);
        if (reloadFiles == false && this->project != NULL)
        {
            emit reloadTree();
            reloadFiles = true;
        }
        QList<QString> files;
        for (int i = 0; i < ui.lstFiles->count(); i++)
        {
            files.append(ui.lstFiles->item(i)->text());
        }
        emit setFiles(files, this->mainFile);
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
        if (this->project != NULL)
        {
            project->setMainFile(ui.lstFiles->currentItem()->toolTip(), ui.lstFiles->currentItem()->text());
        }
        if (reloadFiles == false && this->project != NULL)
        {
            emit reloadTree();
            reloadFiles = true;
        }
        for (int i = 0; i < ui.lstFiles->count(); i++)
        {
            if (this->mainFile == ui.lstFiles->item(i)->text())
            {
                ui.lstFiles->item(i)->setForeground(Qt::black);
            }
        }
        this->ui.lstFiles->currentItem()->setForeground(Qt::blue);
        this->mainFile = ui.lstFiles->currentItem()->text();
        QList<QString> files;
        for (int i = 0; i < ui.lstFiles->count(); i++)
        {
            files.append(ui.lstFiles->item(i)->text());
        }
        emit setFiles(files, this->mainFile);
        /*if (reloadFiles == false)
        {
            emit reloadTree();
            reloadFiles = true;
        }*/
    }
}


QStringList ProjectCfg_FileMgr::getPaths()
{
    QStringList paths;
    for (int i = 0; i < ui.lstFiles->count(); i++)
    {
        paths.append(ui.lstFiles->item(i)->toolTip());
    }
    return paths;
}



void ProjectCfg_FileMgr::setMainFileByName(QString mainFile)
{
    for (int i = 0; i < ui.lstFiles->count(); i++)
    {
        if (this->mainFile == ui.lstFiles->item(i)->text())
        {
            ui.lstFiles->item(i)->setForeground(Qt::black);
        }
    }
    for (int i = 0; i < ui.lstFiles->count(); i++)
    {
        if (mainFile == ui.lstFiles->item(i)->text())
        {
            ui.lstFiles->item(i)->setForeground(Qt::blue);
            this->mainFile = mainFile;
        }
    }
}
