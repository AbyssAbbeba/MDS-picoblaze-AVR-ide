/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2014
 *
 */



#include <QtGui>
#include "projectcfg_memory.h"


/**
 * @brief
 * @param
 * @param
 */
ProjectCfg_Memory::ProjectCfg_Memory(QWidget *parent, Project *currProject)
    : QWidget(parent)
{
    ui.setupUi(this);

    this->project = currProject;

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

    ui.sldScratchpad->setValue(ui.sldScratchpad->maximum());
    ui.sldProgMem->setValue(ui.sldProgMem->maximum());

    this->show();

    if (this->project != NULL)
    {
        this->load();
    }
}


int ProjectCfg_Memory::getIntVector()
{
    bool ok;
    int value = ui.leIntVector->text().toInt(&ok, 16);
    if (ok != true || value > ui.lblProgMem->text().toInt())
    {
        return -1;
    }
    return value;
}


int ProjectCfg_Memory::getHWBuild()
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


int ProjectCfg_Memory::getScratchpadSize()
{
    return ui.lblScratchpad->text().toInt();
}


int ProjectCfg_Memory::getProgMemSize()
{
    return ui.lblProgMem->text().toInt();
}


void ProjectCfg_Memory::setHWBuildEnabled(bool enabled)
{
    if (true == enabled)
    {
        ui.lblHWBuild->setEnabled(true);
        ui.leHWBuild->setEnabled(true);
    }
    else
    {
        ui.lblHWBuild->setDisabled(true);
        ui.leHWBuild->setDisabled(true);
    }
}


void ProjectCfg_Memory::setScratchpadMaximum(int maximum)
{
    if (maximum > 0)
    {
        ui.sldScratchpad->setEnabled(true);
        ui.lblScratchpad->setEnabled(true);
        ui.sldScratchpad->setMaximum(maximum);
        ui.sldScratchpad->setValue(maximum);
        //qDebug() << "ProjectCfg_Memory: maximum" << maximum;
    }
    else
    {
        ui.sldScratchpad->setDisabled(true);
        ui.lblScratchpad->setDisabled(true);
    }
}


void ProjectCfg_Memory::setProgMemMaximum(int maximum)
{
    ui.sldProgMem->setMaximum(maximum);
    ui.sldProgMem->setValue(maximum);
}


void ProjectCfg_Memory::sliderScratchpadUpdate(int value)
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


void ProjectCfg_Memory::sliderProgMemUpdate(int value)
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


void ProjectCfg_Memory::load()
{
    ui.leIntVector->setText(QString::number(project->intVector, 16));
    if (project->hwBuild > -1)
    {
        ui.lblHWBuild->setEnabled(true);
        ui.leHWBuild->setEnabled(true);
        ui.leHWBuild->setText(QString::number(project->hwBuild, 16));
    }
    else
    {
        ui.lblHWBuild->setDisabled(true);
        ui.leHWBuild->setDisabled(true);
    }
    ui.sldProgMem->setValue(project->progMemSize);
    ui.sldScratchpad->setValue(project->scratchpadSize);
}


int ProjectCfg_Memory::save()
{
    if (this->getHWBuild() == -2)
    {
        return -2;
    }
    if (this->getIntVector() == -1)
    {
        return -1;
    }
    this->project->setIntVector(this->getIntVector());
    this->project->setHWBuild(this->getHWBuild());
    this->project->setScratchpad(ui.lblScratchpad->text().toInt());
    this->project->setProgMem(ui.lblProgMem->text().toInt());
    return 0;
}