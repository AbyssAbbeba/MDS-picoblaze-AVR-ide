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
#include <QTabWidget>
#include "projectcfg_general.h"
#include "filemgr.h"



class Project;
//class FileMgr;

/**
 * @brief
 * @ingroup GUI
 * @class ProjectConfigDialog_Core
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

        QTabWidget *tabs;
        QWidget *parent;
        Project *project;
        ProjectCfg_General *generalCfg;
        FileMgr *fileMgr;

    private slots:
        void reload();

    protected:
        virtual void closeEvent(QCloseEvent *e);
};

#endif
