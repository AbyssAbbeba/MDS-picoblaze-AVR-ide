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


    //ui.teNotes->append("Name:\t\t" + ui.cmbFamily->currentText());
    //ui.teNotes->append("IO port address count:\t" + notes.at(0));
    //ui.teNotes->append("Register count:\t" + notes.at(1));
    //ui.teNotes->append("Scratchpad:\t\t" + notes.at(2));
    //ui.teNotes->append("Callstack:\t\t" + notes.at(3));
    //ui.teNotes->append("Program memory:\t" + notes.at(4));

    connect(ui.cmbFamily,
            SIGNAL(currentIndexChanged(const QString&)),
            this,
            SLOT(familyChanged(const QString&))
           );
    connect(ui.sldScratchpad,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(sliderScratchpadUpdate(int))
           );
    connect(ui.sldProgMem,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(sliderProgMemUpdate(int))
           );

    //ui.cmbFamily->setCurrentIndex(4);
    //ui.sldScratchpad->setValue(ui.sldScratchpad->maximum());
    //ui.sldProgMem->setValue(ui.sldProgMem->maximum());

    this->load();
    this->show();
}


int ProjectCfg_General::getIntVector()
{
    bool ok;
    int value = ui.leIntVector->text().toInt(&ok, 16);
    if (ok != true || value > ui.lblProgMem->text().toInt())
    {
        return -1;
    }
    return value;
}


int ProjectCfg_General::getHWBuild()
{
    bool ok;
    int value = ui.leHWBuild->text().toInt(&ok, 16);
    if (ui.leHWBuild->isEnabled() == false)
    {
        return -1;
    }
    if (ok != true)
    {
        return -2;
    }
    return value;
}


void ProjectCfg_General::familyChanged(const QString &text)
{
    if ("kcpsm6" == text)
    {
        ui.lblHWBuild->setEnabled(true);
        ui.leHWBuild->setEnabled(true);
    }
    else
    {
        ui.lblHWBuild->setDisabled(true);
        ui.leHWBuild->setDisabled(true);
    }

    int index = ui.cmbFamily->findText(text);
    ui.teNotes->clear();
    ui.teNotes->append("IO port address count:\t" + notes.at(5*index));
    ui.teNotes->append("Register count:\t" + notes.at(5*index+1));
    ui.teNotes->append("Callstack size:\t" + notes.at(5*index+3));

    if (notes.at(5*index+2) != "-")
    {
        ui.sldScratchpad->setEnabled(true);
        ui.lblScratchpad->setEnabled(true);
        ui.sldScratchpad->setMaximum(log2(notes.at(5*index+2).toInt()));
        ui.sldScratchpad->setValue(ui.sldScratchpad->maximum());
    }
    else
    {
        ui.sldScratchpad->setDisabled(true);
        ui.lblScratchpad->setDisabled(true);
    }

    ui.sldProgMem->setMaximum(log2(notes.at(5*index+4).toInt()));
    ui.sldProgMem->setValue(ui.sldProgMem->maximum());
}


void ProjectCfg_General::sliderScratchpadUpdate(int value)
{
    if (value > -1)
    {
        ui.lblScratchpad->setText(QString::number(qPow(2,value)));
    }
    else
    {
        ui.lblScratchpad->setText("0");
    }
}


void ProjectCfg_General::sliderProgMemUpdate(int value)
{
    if (value > -1)
    {
        ui.lblProgMem->setText(QString::number(qPow(2,value)));
    }
    else
    {
        ui.lblProgMem->setText("0");
    }
}


void ProjectCfg_General::load()
{
    ui.leName->setText(this->project->prjName);
    ui.leIntVector->setText(QString::number(project->intVector, 16));
    if (project->hwBuild > -1)
    {
        ui.leHWBuild->setText(QString::number(project->hwBuild, 16));
    }
    ui.cmbFamily->setCurrentIndex(ui.cmbFamily->findText(project->family));
    ui.sldScratchpad->setValue(project->progMemSize);
    ui.sldScratchpad->setValue(project->scratchpadSize);
}


bool ProjectCfg_General::save()
{
    if (this->getHWBuild() == -2)
    {
        return -2;
    }
    if (this->getIntVector() == -1)
    {
        return -1;
    }
    this->project->setName(ui.leName->text());
    this->project->setFamily(ui.cmbFamily->currentText());
    this->project->setIntVector(this->getIntVector());
    this->project->setHWBuild(this->getHWBuild());
    this->project->setScratchpad(ui.lblScratchpad->text().toInt());
    this->project->setProgMem(ui.lblProgMem->text().toInt());
    return 0;
}