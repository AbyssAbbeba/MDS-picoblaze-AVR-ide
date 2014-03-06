/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file projectcfg_general.cpp
 */


#include <QtGui>
#include <math.h>
#include "projectcfg_general.h"
#include "../project/project.h"



/**
 * @brief
 * @param
 * @param
 */
ProjectCfg_General::ProjectCfg_General(QWidget *parentWidget, Project *currProject)
    : QWidget(parentWidget)
{
    this->project = currProject;
    ui.setupUi(this);

    if (this->project->prjPath == "untracked" && this->project->prjName == "untracked")
    {
        ui.leName->setReadOnly(true);
    }

    notes.append("256");
    notes.append("8");
    notes.append("-");
    notes.append("4");
    notes.append("256");

    notes.append("256");
    notes.append("16");
    notes.append("-");
    notes.append("15");
    notes.append("256");

    notes.append("256");
    notes.append("32");
    notes.append("-");
    notes.append("31");
    notes.append("1024");

    notes.append("256");
    notes.append("16");
    notes.append("64");
    notes.append("31");
    notes.append("1024");

    notes.append("256");
    notes.append("2x16");
    notes.append("256");
    notes.append("31");
    notes.append("4096");

    connect(ui.cmbFamily,
            SIGNAL(currentIndexChanged(const QString&)),
            this,
            SLOT(familyChanged(const QString&))
           );

    //ui.cmbFamily->setCurrentIndex(4);
    //ui.sldScratchpad->setValue(ui.sldScratchpad->maximum());
    //ui.sldProgMem->setValue(ui.sldProgMem->maximum());

    this->load();
    this->show();
}



void ProjectCfg_General::familyChanged(const QString &text)
{
    if ("kcpsm6" == text)
    {
        emit setHWBuildEnabled(true);
    }
    else
    {
        emit setHWBuildEnabled(false);
    }

    int index = ui.cmbFamily->findText(text);
    ui.teNotes->clear();
    ui.teNotes->append("IO port address count:\t" + notes.at(5*index));
    ui.teNotes->append("Register count:\t" + notes.at(5*index+1));
    ui.teNotes->append("Callstack size:\t" + notes.at(5*index+3));

    if (notes.at(5*index+2) != "-")
    {
        emit setScratchpadMaximum(log2(notes.at(5*index+2).toInt()));
    }
    else
    {
        emit setScratchpadMaximum(-1);
    }

   emit setProgMemMaximum(log2(notes.at(5*index+4).toInt()));
}


void ProjectCfg_General::load()
{
    ui.leName->setText(this->project->prjName);
    ui.cmbFamily->setCurrentIndex(ui.cmbFamily->findText(project->family));
}


void ProjectCfg_General::save()
{
    this->project->setName(ui.leName->text());
    this->project->setFamily(ui.cmbFamily->currentText());
}


int ProjectCfg_General::getScratchpadMaximum()
{
    return log2(notes.at(5*ui.cmbFamily->currentIndex()+2).toInt());
}


int ProjectCfg_General::getProgMemMaximum()
{
    return log2(notes.at(5*ui.cmbFamily->currentIndex()+4).toInt());
}


/*bool ProjectCfg_General::getHWBuildEnabled()
{
    return (ui.cmbFamily->currentText() == "kcpsm6");
}*/