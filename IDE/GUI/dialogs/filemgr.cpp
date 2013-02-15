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



#include "filemgr.h"
#include "../mainform/project.h"
#include <QFileDialog>
#include "errordlg.h"


/**
 * @brief
 * @param
 * @param
 */
FileMgr::FileMgr(QWidget *parentWidget, Project *currProject)
{
    //prepare widget
    this->parent = parentWidget;
    this->project = currProject;
    this->reloadFiles = false;
    fileList = new QListWidget(this);
    deleteBtn = new QPushButton("Delete", this);
    newBtn = new QPushButton("New", this);
    addBtn = new QPushButton("Add", this);
    setMainBtn = new QPushButton("Set Main", this);
    btnLayout = new QVBoxLayout(this);
    btnLayout->addWidget(newBtn);
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(setMainBtn);
    btnLayout->addWidget(deleteBtn);
    btnWidget = new QWidget(this);
    btnWidget->setLayout(btnLayout);
    layout = new QGridLayout(this);
    layout->addWidget(fileList, 0,0,Qt::AlignJustify);
    layout->addWidget(btnWidget, 0,1,Qt::AlignJustify);
    this->setLayout(layout);
    this->adjustSize();
    //load files
    if (this->project->fileNames.size() > 0)
    {
        QString absolutePath = QFileInfo(this->project->prjPath).dir().path();
        for (int i = 0; i < this->project->fileNames.size(); i++)
        {
            QListWidgetItem *newItem = new QListWidgetItem(this->project->fileNames.at(i), fileList);
            newItem->setToolTip(QDir(absolutePath + "/" + this->project->filePaths.at(i)).canonicalPath());
            fileList->addItem(newItem);
        }
    }

    //connect buttons with actions
    connect(newBtn, SIGNAL(clicked()), this, SLOT(newFile()));
    connect(addBtn, SIGNAL(clicked()), this, SLOT(addFile()));
    connect(setMainBtn, SIGNAL(clicked()), this, SLOT(setMainFile()));
    connect(deleteBtn, SIGNAL(clicked()), this, SLOT(deleteFile()));
    //newBtn->setCheckable(false);
}



/**
 * @brief
 */
void FileMgr::deleteFile()
{
    if (fileList->currentItem() != NULL)
    {
        project->removeFile(fileList->currentItem()->toolTip(), fileList->currentItem()->text());
        delete fileList->currentItem();
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
void FileMgr::newFile()
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
            QListWidgetItem *newItem = new QListWidgetItem(path.section('/', -1), fileList);
            newItem->setToolTip(path);
            fileList->addItem(newItem);
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
void FileMgr::addFile()
{
    //dialog window (file search)
    QString path = QFileDialog::getOpenFileName(this, tr("Source File"), "");
    if (path != NULL)
    {
        project->addFile(path, path.section('/', -1));
        QListWidgetItem *newItem = new QListWidgetItem(path.section('/', -1), fileList);
        newItem->setToolTip(path);
        fileList->addItem(newItem);
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
void FileMgr::setMainFile()
{
    if (fileList->currentItem() != NULL)
    {
        project->setMainFile(fileList->currentItem()->toolTip(), fileList->currentItem()->text()); 
        /*if (reloadFiles == false)
        {
            emit reloadTree();
            reloadFiles = true;
        }*/
    }
} 
