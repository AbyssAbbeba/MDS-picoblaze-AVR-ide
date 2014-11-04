/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file projecttree.h
 */


#ifndef PROJECTTREE_H
#define PROJECTTREE_H


#include <QTreeWidget>
#include <QString>

class Project;
class QMenu;
class QTreeWidgetItem;
class QContextMenuEvent;
//class ProjectConfigDialog_Core;

/**
 * @brief
 * @ingroup GUI
 * @class ProjectTree
 */
class ProjectTree: public QTreeWidget
{
    Q_OBJECT
    public:
        ProjectTree(QWidget *parent, bool project);
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
        QContextMenuEvent *lastEvent;

    private slots:
        void newFile();
        void addFile();
        void setMainFile();
        void removeFile();
        void config();
        void closeProjectSlot();
        //void reloadFiles();
        void contextP2(int fileCount);

    signals:
        void requestFileCount();
        void startProjectCfgDlgCore();
        void setMainFile(QString path, QString name);
        void removeFile(QString path, QString name);
        void addFile(QString path, QString name);
        void closeProject();

    protected:
        void contextMenuEvent(QContextMenuEvent *event);
        void dropEvent(QDropEvent *e);
};
        


#endif
