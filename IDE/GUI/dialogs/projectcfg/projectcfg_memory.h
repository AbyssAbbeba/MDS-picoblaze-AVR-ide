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
 * @file projectcfg_memory.h
 */




#ifndef PROJECTCFG_MEMORY_H
#define PROJECTCFG_MEMORY_H

#include "../project/project.h"
#include "ui_projectcfg_memory.h"



/**
 * @brief Project Dialog Memory tab
 * @ingroup GUI
 * @class ProjectDialog
 * @details Used in creating new project
 */
class ProjectCfg_Memory : public QWidget
{
    Q_OBJECT
    public:
        ProjectCfg_Memory(QWidget *parent, Project *currProject);
        int getIntVector();
        int getHWBuild();
        int getScratchpadSize();
        int getProgMemSize();
        int save();

    public slots:
        void setHWBuildEnabled(bool enabled);
        void setScratchpadMaximum(int maximum);
        void setProgMemMaximum(int maximum);

    private slots:
        void sliderScratchpadUpdate(int value);
        void sliderProgMemUpdate(int value);

    private:
        void load();
        
        Ui_Projectcfg_Memory ui;
        Project *project;

};

#endif