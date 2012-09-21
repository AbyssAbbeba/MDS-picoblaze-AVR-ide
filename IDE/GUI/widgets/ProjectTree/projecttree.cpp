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
#include "projecttree.h"
#include "../mainform/project.h"
#include "../dialogs/projectcfgdlg_core.h"

ProjectTree::ProjectTree(QWidget *parent, Project *parentProject)
{
    this->parentProject = parentProject;
    this->parent = parent;
    setHeaderHidden(true);
    projectPopup = new QMenu(this);
    QAction *projectConfigAct = new QAction("Configuration", projectPopup);
    projectPopup->addAction(projectConfigAct);
    filePopup = new QMenu(this);
    QAction *removeFileAct = new QAction("Remove file", filePopup);
    QAction *setMainFileAct = new QAction("Set as main file", filePopup);
    filePopup->addAction(removeFileAct);
    filePopup->addAction(setMainFileAct);
    connect(setMainFileAct, SIGNAL(triggered()), this, SLOT(setMainFile()));
    connect(removeFileAct, SIGNAL(triggered()), this, SLOT(removeFile()));
    connect(projectConfigAct, SIGNAL(triggered()), this, SLOT(config()));
}

ProjectTree::~ProjectTree()
{

}

void ProjectTree::contextMenuEvent(QContextMenuEvent *event)
{
    if (this->itemAt(event->pos()) != NULL)
    {
        if (parentProject->fileCount == 0 || this->itemAt(event->pos())->childCount() > 0)
            projectPopup->popup(event->globalPos());
        else
        {
            lastName = this->itemAt(event->pos())->text(0);
            lastPath = this->itemAt(event->pos())->toolTip(0);
            lastItem = this->itemAt(event->pos());
            filePopup->popup(event->globalPos());
        }
    }
}

void ProjectTree::setMainFile()
{
    parentProject->setMainFile(lastPath, lastName);
}


void ProjectTree::removeFile()
{
    delete lastItem;
    parentProject->removeFile(lastPath, lastName);
}


void ProjectTree::config()
{
    ProjectConfigDialog_Core *cfgdlg = new ProjectConfigDialog_Core(this, parentProject);
    cfgdlg->show();
    connect(cfgdlg, SIGNAL(reloadTree()), this, SLOT(reloadFiles()));
}


void ProjectTree::reloadFiles()
{
    qDebug() << "reloading\n";
}


/*void ProjectTree::addFile()
{
    delete lastItem;
    parentProject->removeFile(lastPath, lastName);
}*/
