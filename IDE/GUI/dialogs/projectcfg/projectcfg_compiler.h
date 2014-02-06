#ifndef PROJECTCFG_COMPILER_H
#define PROJECTCFG_COMPILER_H


#include <QWidget>
//#include <QDialogButtonBox>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include "../project/project.h"
#include "ui_projectcfg_compiler.h"


class Project;


/**
 * @brief Project Dialog Config compiler tab
 * @ingroup GUI
 * @class ProjectCfg_Compiler
 */
class ProjectCfg_Compiler : public QWidget
{
    public:
        ProjectCfg_Compiler(QWidget *parentWidget, Project *currProject);
        void save();

    private:
        void load();
      
        Project *project;
        Ui_ProjectCfg_Compiler ui;

};

#endif