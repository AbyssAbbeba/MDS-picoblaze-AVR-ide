#include "projectcfgdlg_core.h"
#include "../mainform/project.h"

ProjectConfigDialog_Core::ProjectConfigDialog_Core(QWidget *dialogParent, Project *currProject)
{
    this->parent = dialogParent;
    this->project = currProject;
    this->reloadFiles = false;
    this->tabs = new QTabWidget(this);
    this->tabs->setTabsClosable(false);
    this->tabs->setMovable(false);
    this->generalCfg = new ProjectCfg_General(this, this->project);
    this->fileMgr = new FileMgr(this, this->project);
    this->tabs->addTab(this->generalCfg, "General");
    this->tabs->addTab(this->fileMgr, "Files");
    this->show();
    //this->resize(tabs->width(), tabs->height());
    this->setFixedWidth(this->tabs->width());
    this->setFixedHeight(this->tabs->height());
    this->setModal(true);
}


void ProjectConfigDialog_Core::closeEvent(QCloseEvent * e)
{
    emit reloadTree();
}
