/**
 * @brief Header for ProjectCfg_General class
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 * @ingroup GUI
 * @file projectcfg_general.h
 */


#ifndef PROJECTCFG_GENERAL_H
#define PROJECTCFG_GENERAL_H

//#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QLabel>
#include "../project/project.h"


class Project;


/**
 * @brief Project Dialog Config general tab
 * @ingroup GUI
 * @class ProjectCfg_General
 */
class ProjectCfg_General : public QWidget
{
    Q_OBJECT
    public:
        ProjectCfg_General(QWidget *parentWidget, Project *currProject);
    private:
        Project *project;
        QWidget *parent;
        //QVBoxLayout *layout;
        QDialogButtonBox *buttonBox;
        QLineEdit *projName;
        QLabel *labelName;
};



#endif
