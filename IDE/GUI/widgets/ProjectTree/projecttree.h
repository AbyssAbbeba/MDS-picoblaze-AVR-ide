/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 * @ingroup GUI
 * @file projecttree.h
 */


#ifndef PROJECTTREE_H
#define PROJECTTREE_H


#include <QTreeWidget>
#include <QMenu>
//#include <QString>

class Project;
class ProjectConfigDialog_Core;

/**
 * @brief
 * @ingroup GUI
 * @class ProjectTree
 */
class ProjectTree: public QTreeWidget
{
    Q_OBJECT
    public:
        ProjectTree(QWidget *parent, Project *parentProject);
        ~ProjectTree();
        void setMainFileManual(QString name, QString path);

    private:
        Project *parentProject;
        QWidget *parent;
        QMenu *projectPopup;
        QMenu *filePopup;
        QString lastName;
        QString lastPath;
        QString mainFileName;
        QString mainFilePath;
        QTreeWidgetItem *lastItem;

    private slots:
        void setMainFile();
        void removeFile();
        void config();
        void reloadFiles();

    protected:
        void contextMenuEvent(QContextMenuEvent *event);
};
        


#endif
