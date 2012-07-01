#include <QtGui>
#include "projecttree.h"
#include "../mainform/project.h"

ProjectTree::ProjectTree(QWidget *parent, Project *parentProject)
{
    this->parentProject = parentProject;
    setHeaderHidden(true);
    //setContextMenuPolicy(Qt::ActionsContextMenu);
    projectPopup = new QMenu(this);
    QAction *projectConfigAct = new QAction("Configuration", projectPopup);
    projectPopup->addAction(projectConfigAct);
    filePopup = new QMenu(this);
    QAction *removeFileAct = new QAction("Remove file", filePopup);
    QAction *setMainFileAct = new QAction("Set as main file", filePopup);
    filePopup->addAction(removeFileAct);
    filePopup->addAction(setMainFileAct);
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
            filePopup->popup(event->globalPos());
    }
}
