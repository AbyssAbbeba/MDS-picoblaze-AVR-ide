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
//#include <QDialogButtonBox>
#include <QWidget>
#include <QStringList>
#include "../project/project.h"
#include "ui_projectcfg_general.h"


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
        void save();

    signals:
        void setScratchpadMaximum(int value);
        void setProgMemMaximum(int value);
        void setHWBuildEnabled(bool enabled);

    private slots:
        void familyChanged(const QString &text);
        
    private:
        void load();
        
        Project *project;
        Ui_ProjectCfg_General ui;
        QStringList notes;
};



#endif
