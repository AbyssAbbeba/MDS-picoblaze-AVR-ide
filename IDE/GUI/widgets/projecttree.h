#ifndef PROJECTTREE_H
#define PROJECTTREE_H


#include <QTreeWidget>
#include <QMenu>

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
    protected:
        void contextMenuEvent(QContextMenuEvent *event);
};
        


#endif
