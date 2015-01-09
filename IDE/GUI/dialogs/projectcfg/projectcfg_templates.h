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
 * @file projectcfg_templates.h
 */


#ifndef PROJECTCFG_TEMPLATES
#define PROJECTCFG_TEMPLATES

#include "../project/project.h"
#include "ui_projectcfg_templates.h"


class ProjectCfg_Templates : public QWidget
{
    Q_OBJECT 
    public:
        ProjectCfg_Templates(QWidget *parent, Project *currProject);
        void save();

    private slots:
        void checkedVHDL(int checked);
        void checkedVerilog(int checked);
        void setPathVHDL();
        void setPathVerilog();

    private:
        void load();
        
        Ui_ProjectCfg_Templates ui;
        Project *project;
};


#endif