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
 * @file projectcfgdlg_core.h
 */


#ifndef PROJECTCFGDLG_CORE_H
#define PROJECTCFGDLG_CORE_H


#include <QDialog>



class Project;
class CfgInterface;
class ProjectCfg_General;
class ProjectCfg_Memory;
class ProjectCfg_FileMgr;
class ProjectCfg_Compiler;
class ProjectCfg_Templates;
class ProjectCfg_CompPaths;
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
        ProjectCfg_Templates *templatesCfg;
        ProjectCfg_CompPaths *pathsCfg;

    private slots:
        void reload();
        void ok();
        void cancel();

    //protected:
    //    virtual void closeEvent(QCloseEvent *e);
};

#endif
