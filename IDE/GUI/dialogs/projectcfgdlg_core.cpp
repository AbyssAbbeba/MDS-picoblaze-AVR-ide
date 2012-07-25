#include "projectcfgdlg_core.h"
#include "../mainform/project.h"

ProjectConfigDialog_Core::ProjectConfigDialog_Core(QWidget *dialogParent, Project *currProject)
{
    this->parent = dialogParent;
    this->project = currProject;
    tabs = new QTabWidget(this);
    tabs->setTabsClosable(false);
    tabs->setMovable(false);
    generalCfg = new ProjectCfg_General(this, this->project);
    fileMgr = new FileMgr(this, this->project);
    tabs->addTab(generalCfg, "General");
    tabs->addTab(fileMgr, "Files");

    this->resize(generalCfg->size());
}
