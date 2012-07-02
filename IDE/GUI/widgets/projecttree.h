#ifndef PROJECTTREE_H
#define PROJECTTREE_H


#include <QTreeWidget>
#include <QMenu>
//#include <QString>

class Project;

class ProjectTree: public QTreeWidget
{
    Q_OBJECT
    public:
        ProjectTree(QWidget *parent, Project *parentProject);
        ~ProjectTree();

    private:
        Project *parentProject;
        QMenu *projectPopup;
        QMenu *filePopup;
        QString lastName;
        QString lastPath;
        QTreeWidgetItem *lastItem;

    private slots:
        void setMainFile();
        void removeFile();

    protected:
        void contextMenuEvent(QContextMenuEvent *event);
};
        


#endif
