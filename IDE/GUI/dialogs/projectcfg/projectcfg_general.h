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
        int getScratchpadMaximum();
        int getProgMemMaximum();
        //bool getHWBuildEnabled();

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
