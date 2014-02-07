#ifndef PROJECTCFG_COMPPATHS_H
#define PROJECTCFG_COMPPATHS_H


#include <QWidget>
#include "ui_projectcfg_comppaths.h"

class Project;


class ProjectCfg_CompPaths : public QWidget
{
    Q_OBJECT
    public:
       ProjectCfg_CompPaths(QWidget *parentWidget, Project *currProject);
       void save();
       QStringList getPaths();
       
    private slots:
        void newPath();
        void editPath();
        void deletePath();
        
    private:
        void load();
        
        Project *project;
        Ui_ProjectCfg_CompPaths ui;
};




#endif