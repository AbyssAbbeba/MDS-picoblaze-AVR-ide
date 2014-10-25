/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup ProjectTree
 * @file projecttree.cpp
 */


#include <QtGui>
#include "projecttree.h"
#include "../../errordialog/errordlg.h"

ProjectTree::ProjectTree(QWidget *parent, bool project)
    : QTreeWidget(parent)
{
    this->parent = parent;
    mainFileName = "";
    mainFilePath = "";
    setHeaderHidden(true);
    projectPopup = new QMenu(this);
    QAction *addFileAct;
    QAction *newFileAct;
    QAction *projectConfigAct;
    QAction *closeProjectAct;
    //if (project == true)
    //{
        newFileAct = new QAction(QIcon(QPixmap(":resources/icons/projNewAdd.png")), "New file", projectPopup);
        addFileAct = new QAction(QIcon(QPixmap(":resources/icons/folder.png")), "Open File", projectPopup);
        projectConfigAct = new QAction(QIcon(QPixmap(":resources/icons/page_white_wrench.png")), "Configuration", projectPopup);
        closeProjectAct = new QAction(QIcon(QPixmap(":resources/icons/breakpoint_disable.png")), "Close project", projectPopup);
        projectPopup->addAction(newFileAct);
        projectPopup->addAction(addFileAct);
        projectPopup->addAction(projectConfigAct);
        projectPopup->addAction(closeProjectAct);
        connect(newFileAct, SIGNAL(triggered()), this, SLOT(newFile()));
        connect(addFileAct, SIGNAL(triggered()), this, SLOT(addFile()));
        connect(projectConfigAct, SIGNAL(triggered()), this, SLOT(config()));
        connect(closeProjectAct, SIGNAL(triggered()), this, SLOT(closeProjectSlot()));
    /*}
    else
    {
        newFileAct = new QAction("New File", projectPopup);
        addFileAct = new QAction("Add File", projectPopup);
        projectPopup->addAction(newFileAct);
        projectPopup->addAction(addFileAct);
        connect(newFileAct, SIGNAL(triggered()), this, SLOT(newFile()));
        connect(addFileAct, SIGNAL(triggered()), this, SLOT(addFile()));
    }*/
    //QAction *projectHWCanvasAct = new QAction("Hardware Canvas", projectPopup);
    //projectPopup->addAction(projectHWCanvasAct);
    QAction *removeFileAct;
    QAction *setMainFileAct;
    filePopup = new QMenu(this);
    if (project == true)
    {
        removeFileAct = new QAction(QIcon(QPixmap(":resources/icons/page_delete.png")), "Remove file", filePopup);
        setMainFileAct = new QAction("Set as main file", filePopup);
        filePopup->addAction(removeFileAct);
        filePopup->addAction(setMainFileAct);
        connect(setMainFileAct, SIGNAL(triggered()), this, SLOT(setMainFile()));
        connect(removeFileAct, SIGNAL(triggered()), this, SLOT(removeFile()));
    }
    else
    {
        removeFileAct = new QAction(QIcon(QPixmap(":resources/icons/page_delete.png")), "Remove file", filePopup);
        filePopup->addAction(removeFileAct);
        connect(removeFileAct, SIGNAL(triggered()), this, SLOT(removeFile()));
    }
    this->setAcceptDrops(true);
}

ProjectTree::~ProjectTree()
{

}

void ProjectTree::contextMenuEvent(QContextMenuEvent *event)
{
   // qDebug() << "ProjectTree: contextMenuEvent()";
    if (this->itemAt(event->pos()) != NULL)
    {
        lastEvent = event;
        //qDebug() << "ProjectTree: emit requestFileCount()";
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
    //qDebug() << "ProjectTree: return contextMenuEvent()";
}


//mozno dat do jedne funkce, signaly jsou sekvencne
void ProjectTree::contextP2(int fileCount)
{
    //qDebug() << "ProjectTree: contextP2()";
    if ( fileCount == 0 || this->itemAt(lastEvent->pos())->childCount() > 0
      || this->itemAt(lastEvent->pos())->toolTip(0) == NULL
       )
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
    //qDebug() << "ProjectTree: return contextP2()";
}


void ProjectTree::setMainFile()
{
    //emit setMainFile(lastPath, lastName);
    //parentProject->setMainFile(lastPath, lastName);
    //clear previous mainfile's background
    if (mainFileName != "" && mainFilePath != "")
    {
        QList<QTreeWidgetItem*> items = this->findItems(mainFileName, Qt::MatchExactly | Qt::MatchRecursive);
        //qDebug() << "Project Tree: previous name: " << lastName;
        //qDebug() << "Project Tree: items found " << items.count();
        for (int i = 0; i < items.count(); i++)
        {
            if (items.at(i)->toolTip(0) == mainFilePath)
            {
                //qDebug() << "Project Tree: color cleared";
                items.at(i)->setBackground(0, QBrush(this->palette().base().color()));
                break;
            }
        }
    }
    //set new mainfile and its background
    QList<QTreeWidgetItem*> items = this->findItems(lastName, Qt::MatchExactly | Qt::MatchRecursive);
    //qDebug() << "Project Tree: last name: " << lastName;
    //qDebug() << "Project Tree: items found " << items.count();
    for (int i = 0; i < items.count(); i++)
    {
        if (items.at(i)->toolTip(0) == lastPath)
        {
            //qDebug() << "Project Tree: mainfile found and colored";
            items.at(i)->setBackground(0, QBrush(QColor(25,25,25,20)));
            mainFileName = lastName;
            mainFilePath = lastPath;
            emit setMainFile(mainFilePath, mainFileName);
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
    //qDebug() << "Project Tree: name: " << name;
    //qDebug() << "Project Tree: path: " << path;
    //qDebug() << "Project Tree: absolute path" << absolutePath;
    //qDebug() << "Project Tree: items found " << items.count();
    for (int i = 0; i < items.count(); i++)
    {
        //qDebug() << "Project Tree: path of" << i << "item is" << items.at(i)->toolTip(0);
        if (items.at(i)->toolTip(0) == absolutePath)
        {
            //qDebug() << "Project Tree: manual color set";
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


void ProjectTree::addFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Source File"), "");
    if (path != NULL)
    {
        emit addFile(path, path.section('/', -1));
        //QTreeWidgetItem *treeProjFile = new QTreeWidgetItem(this->topLevelItem(0));
        //treeProjFile->setText(0, path.section('/', -1));
        //treeProjFile->setData(0, Qt::ToolTipRole, path);
        //fileList->addItem(newItem);
    }
}


void ProjectTree::newFile()
{
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
            //QTextStream fout(&file);
            //fout << wDockManager->getCentralTextEdit()->toPlainText();
            file.close();
            emit addFile(path, path.section('/', -1));
        }

    }
}


void ProjectTree::dropEvent(QDropEvent *e)
{
    qDebug() << "ProjectTree: drop text" << e->mimeData()->text();
    //qDebug() << "ProjectTree: drop data" << e->mimeData()->data();
}


void ProjectTree::closeProjectSlot()
{
    emit closeProject();
}