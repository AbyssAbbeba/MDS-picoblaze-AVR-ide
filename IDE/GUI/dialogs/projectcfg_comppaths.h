#ifndef PROJECTCFG_COMPPATHS_H
#define PROJECTCFG_COMPPATHS_H


#include <QWidget>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>

class Project;


class ProjectCfg_CompPaths : public QWidget
{
    Q_OBJECT
    public:
       ProjectCfg_CompPaths(QWidget *parentWidget, Project *currProject);
       void save();
       
    private slots:
        void New();
        void Edit();
        void Delete();
        
    private:
        void load();
        
        Project *project;
        QPushButton *newBtn;
        QPushButton *editBtn;
        QPushButton *deleteBtn;
        QListWidget *fileList;
        QGridLayout *layout;
        QVBoxLayout *btnLayout;
        QWidget *btnWidget;
};




#endif