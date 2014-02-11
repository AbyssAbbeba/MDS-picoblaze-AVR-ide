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
 * @file projectdlg.h
 */




#ifndef PROJECTDLG_H
#define PROJECTDLG_H

#include <QWidget>
#include <QDialogButtonBox>
#include <QDialog>
#include "../../project/project.h"
#include "projectdlg_general.h"
#include "../projectcfg/projectcfg_compiler.h"
#include "../projectcfg/projectcfg_comppaths.h"
#include "../projectcfg/projectcfg_filemgr.h"
#include "../projectcfg/projectcfg_memory.h"



/**
 * @brief Project Dialog
 * @ingroup GUI
 * @class ProjectDialog
 * @details Used in creating new project
 */
class ProjectDialog : public QDialog
{
    Q_OBJECT
    public:
        ProjectDialog(QWidget *parent, ProjectMan *dialogProjectMan);

    private slots:
        void bCreate();
        void bReject();

    private:
        QDialogButtonBox *buttonBox;
        ProjectMan *projectMan;
        Projectdlg_General *prjdlg_general;
        ProjectCfg_Memory *prjdlg_memory;
        ProjectCfg_Compiler *prjdlg_compiler;
        ProjectCfg_CompPaths *prjdlg_comppaths;
        ProjectCfg_FileMgr *prjdlg_filemgr;
        
};

#endif
