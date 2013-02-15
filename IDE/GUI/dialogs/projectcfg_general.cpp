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
#include "../mainform/project.h"

ProjectCfg_General::ProjectCfg_General(QWidget *parentWidget, Project *currProject)
{
    this->parent = parentWidget;
    this->project = currProject;
    layout = new QVBoxLayout(this);
    projName = new QLineEdit(this);
    labelName = new QLabel(this);
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    labelName->setText("Project name");
    layout->addWidget(labelName);
    layout->addWidget(projName);
    layout->addWidget(buttonBox);
    this->setLayout(layout);
    this->adjustSize();
}
