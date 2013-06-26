/**
 * @brief C++ file for projectcfg_general class
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 *
 */



#include "projectcfg_general.h"
#include "../project/project.h"

ProjectCfg_General::ProjectCfg_General(QWidget *parentWidget, Project *currProject)
    : QWidget(parentWidget)
{
    //this->parent = parentWidget;
    this->project = currProject;
    //layout = new QVBoxLayout(this);
    labelName = new QLabel(this);
    projName = new QLineEdit(this);
    //buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    labelName->setText("Project name");
    projName->setText(this->project->prjName);
    projName->setReadOnly(true);
    projName->setFrame(false);
    this->show();
    
    labelName->move(5, 10);
    projName->move(5, 30);
    //projName->move(this->width() - buttonBox->width()-5, this->height() - buttonBox->height()-5);
    //layout->addWidget(labelName);
    //layout->addWidget(projName);
    //layout->addWidget(buttonBox);
    //this->setLayout(layout);
    //this->adjustSize();
}


/*void ProjectCfg_General::fixButtonBox()
{
    this->buttonBox->move(this->width() - buttonBox->width()-5, this->height() - buttonBox->height()-5);
}*/
