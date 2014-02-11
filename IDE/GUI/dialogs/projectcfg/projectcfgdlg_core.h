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
 * @file projectcfgdlg_core.h
 */


#ifndef PROJECTCFGDLG_CORE_H
#define PROJECTCFGDLG_CORE_H


#include <QDialog>
#include "projectcfg_general.h"
#include "projectcfg_memory.h"
#include "projectcfg_filemgr.h"
#include "projectcfg_compiler.h"
#include "projectcfg_comppaths.h"
#include "../cfginterface/cfginterface.h"



class Project;
//class FileMgr;

/**
 * @brief Project Dialog Config
 * @ingroup GUI
 * @class ProjectConfigDialog_Core
 * @details Used in opened project configuration
 */
class ProjectConfigDialog_Core : public QDialog
{
    Q_OBJECT
    //Q_INTERFACES(CfgInterface)
    public:
        ProjectConfigDialog_Core(QWidget *parent, Project *currProject);

        bool reloadFiles;

    signals:
        void reloadTree();

    private:
        //void freeDialog();

        //QWidget *parent;
        Project *project;
        CfgInterface *cfgInterface;
        ProjectCfg_General *generalCfg;
        ProjectCfg_Memory *memoryCfg;
        ProjectCfg_FileMgr *fileMgr;
        ProjectCfg_Compiler *compilerCfg;
        ProjectCfg_CompPaths *pathsCfg;

    private slots:
        void reload();
        void ok();
        void cancel();

    protected:
        virtual void closeEvent(QCloseEvent *e);
};

#endif
