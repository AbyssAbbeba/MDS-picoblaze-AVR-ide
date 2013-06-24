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
//#include "../project/project.h"
//#include "../dialogs/projectcfgdlg_core.h"

ProjectTree::ProjectTree(QWidget *parent)
    : QTreeWidget(parent)
{
    //this->parentProject = parentProject;
    this->parent = parent;
    mainFileName = "";
    mainFilePath = "";
    setHeaderHidden(true);
    projectPopup = new QMenu(this);
    QAction *projectConfigAct = new QAction("Configuration", projectPopup);
    //QAction *projectHWCanvasAct = new QAction("Hardware Canvas", projectPopup);
    projectPopup->addAction(projectConfigAct);
    //projectPopup->addAction(projectHWCanvasAct);
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
    qDebug() << "ProjectTree: contextMenuEvent()";
    if (this->itemAt(event->pos()) != NULL)
    {
        lastEvent = event;
        qDebug() << "ProjectTree: emit requestFileCount()";
        emit requestFileCount();
        /*if (parentProject->fileCount == 0 || this->itemAt(event->pos())->childCount() > 0)
        {
            projectPopup->popup(event->globalPos());
        }
        else
        {
            lastName = this->itemAt(event->pos())->text(0);
            lastPath = this->itemAt(event->pos())->toolTip(0);
            lastItem = this->itemAt(event->pos());
            filePopup->popup(event->globalPos());
        }*/
    }
    qDebug() << "ProjectTree: return contextMenuEvent()";
}


//mozno dat do jedne funkce, signaly jsou sekvencne
void ProjectTree::contextP2(int fileCount)
{
    qDebug() << "ProjectTree: contextP2()";
    if (fileCount == 0 || this->itemAt(lastEvent->pos())->childCount() > 0)
    {
        projectPopup->popup(lastEvent->globalPos());
    }
    else
    {
        lastName = this->itemAt(lastEvent->pos())->text(0);
        lastPath = this->itemAt(lastEvent->pos())->toolTip(0);
        lastItem = this->itemAt(lastEvent->pos());
        filePopup->popup(lastEvent->globalPos());
    }
    qDebug() << "ProjectTree: return contextP2()";
}


void ProjectTree::setMainFile()
{
    emit setMainFile(lastPath, lastName);
    //parentProject->setMainFile(lastPath, lastName);
    //clear previous mainfile's background
    if (mainFileName != "" && mainFilePath != "")
    {
        QList<QTreeWidgetItem*> items = this->findItems(mainFileName, Qt::MatchExactly | Qt::MatchRecursive);
        qDebug() << "Project Tree: previous name: " << lastName;
        qDebug() << "Project Tree: items found " << items.count();
        for (int i = 0; i < items.count(); i++)
        {
            if (items.at(i)->toolTip(0) == mainFilePath)
            {
                qDebug() << "Project Tree: color cleared";
                items.at(i)->setBackground(0, QBrush(this->palette().base().color()));
                break;
            }
        }
    }
    //set new mainfile and its background
    QList<QTreeWidgetItem*> items = this->findItems(lastName, Qt::MatchExactly | Qt::MatchRecursive);
    qDebug() << "Project Tree: last name: " << lastName;
    qDebug() << "Project Tree: items found " << items.count();
    for (int i = 0; i < items.count(); i++)
    {
        if (items.at(i)->toolTip(0) == lastPath)
        {
            qDebug() << "Project Tree: mainfile found and colored";
            items.at(i)->setBackground(0, QBrush(QColor(25,25,25,20)));
            mainFileName = lastName;
            mainFilePath = lastPath;
            break;
        }
    }
}


void ProjectTree::setMainFileManual(QString name, QString path)
{
    //set new mainfile and its background
    QDir projectDir = QFileInfo(this->topLevelItem(0)->toolTip(0)).dir();
    QString absolutePath = projectDir.path();
    absolutePath = QDir(absolutePath + "/" + path).canonicalPath();
    QList<QTreeWidgetItem*> items = this->findItems(name, Qt::MatchExactly | Qt::MatchRecursive);
    qDebug() << "Project Tree: name: " << name;
    qDebug() << "Project Tree: path: " << path;
    qDebug() << "Project Tree: absolute path" << absolutePath;
    qDebug() << "Project Tree: items found " << items.count();
    for (int i = 0; i < items.count(); i++)
    {
        qDebug() << "Project Tree: path of" << i << "item is" << items.at(i)->toolTip(0);
        if (items.at(i)->toolTip(0) == absolutePath)
        {
            qDebug() << "Project Tree: manual color set";
            items.at(i)->setBackground(0, QBrush(QColor(25,25,25,20)));
            mainFileName = name;
            mainFilePath = absolutePath;
            break;
        }
    }
}


void ProjectTree::removeFile()
{
    delete lastItem;
    emit removeFile(lastPath, lastName);
    //parentProject->removeFile(lastPath, lastName);
}


void ProjectTree::config()
{
    emit startProjectCfgDlgCore();
    //ProjectConfigDialog_Core *cfgdlg = new ProjectConfigDialog_Core(this, parentProject);
    //cfgdlg->show();
    //connect(cfgdlg, SIGNAL(reloadTree()), this, SLOT(reloadFiles()));
}


/*void ProjectTree::reloadFiles()
{
    qDebug() << "reloading\n";
}*/


/*void ProjectTree::addFile()
{
    delete lastItem;
    parentProject->removeFile(lastPath, lastName);
}*/
