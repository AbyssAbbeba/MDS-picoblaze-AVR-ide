#ifndef PROJECTCFGDLG_CORE_H
#define PROJECTCFGDLG_CORE_H

#include <QDialog>
#include <QTabWidget>
#include "projectcfg_general.h"
#include "filemgr.h"



class Project;

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

    //private slots:
    //    void bCreate();
    //    void bReject();

    private:
        void freeDialog();
        QTabWidget *tabs;
        QWidget *parent;
        Project *project;
        ProjectCfg_General *generalCfg;
        FileMgr *fileMgr;
};

#endif
