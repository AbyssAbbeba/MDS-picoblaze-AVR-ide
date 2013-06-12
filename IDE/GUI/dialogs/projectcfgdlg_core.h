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
#include <QStackedWidget>
#include <QListWidget>
#include "projectcfg_general.h"
#include "projectcfg_filemgr.h"
#include "projectcfg_compiler.h"



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
    public:
        ProjectConfigDialog_Core(QWidget *dialogParent, Project *currProject);

        bool reloadFiles;

    signals:
        void reloadTree();

    private:
        void freeDialog();

        QListWidget *menuList;
        QStackedWidget *tabs;
        QWidget *parent;
        Project *project;
        ProjectCfg_General *generalCfg;
        ProjectCfg_FileMgr *fileMgr;
        ProjectCfg_Compiler *compilerCfg;

    private slots:
        void reload();
        void changeWidget(int row);

    protected:
        virtual void closeEvent(QCloseEvent *e);
};

#endif
