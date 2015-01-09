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
 * @file projectcfg_templates.cpp
 */


#include <QtGui>
#include "projectcfg_templates.h"


ProjectCfg_Templates::ProjectCfg_Templates(QWidget *parent, Project *currProject)
    : QWidget(parent)
{
    ui.setupUi(this);
    this->project = currProject;
    QPixmap pm_btnChange(":/resources//icons//drive_magnify.png");
    QIcon icon_btnChange(pm_btnChange);
    ui.btnVHDL->setIcon(icon_btnChange);
    ui.btnVerilog->setIcon(icon_btnChange);

    if (this->project != NULL)
    {
        this->load();
    }
    else
    {
        ui.leVHDL->setDisabled(true);
        ui.btnVHDL->setDisabled(true);
        ui.chckVHDL->setChecked(true);
        ui.leVerilog->setDisabled(true);
        ui.btnVHDL->setDisabled(true);
        ui.chckVerilog->setChecked(true);
    }

    
    connect(ui.chckVHDL,
            SIGNAL(stateChanged(int)),
            this,
            SLOT(checkedVHDL(int))
           );

    connect(ui.chckVerilog,
            SIGNAL(stateChanged(int)),
            this,
            SLOT(checkedVerilog(int))
           );
    connect(ui.btnVHDL,
            SIGNAL(clicked()),
            this,
            SLOT(setPathVHDL())
           );
    connect(ui.btnVerilog,
            SIGNAL(clicked()),
            this,
            SLOT(setPathVerilog())
           );
}


void ProjectCfg_Templates::load()
{
    if (true == this->project->defaultVerilog)
    {
        ui.chckVerilog->setChecked(true);
        ui.leVerilog->setDisabled(true);
        ui.leVerilog->setText("");
    }
    else
    {
        ui.chckVerilog->setChecked(false);
        ui.leVerilog->setEnabled(true);
        ui.leVerilog->setText(this->project->templateVerilog);
    }
    if (true == this->project->defaultVHDL)
    {
        ui.chckVHDL->setChecked(true);
        ui.leVHDL->setDisabled(true);
        ui.leVHDL->setText("");
    }
    else
    {
        ui.chckVHDL->setChecked(false);
        ui.leVHDL->setEnabled(true);
        ui.leVHDL->setText(this->project->templateVHDL);
    }
}


void ProjectCfg_Templates::save()
{
    this->project->setTemplates(ui.chckVerilog->isChecked(),
                                ui.leVerilog->text(),
                                ui.chckVHDL->isChecked(),
                                ui.leVHDL->text()
                               );
}


void ProjectCfg_Templates::checkedVHDL(int checked)
{
    if (Qt::Unchecked == checked)
    {
        ui.leVHDL->setEnabled(true);
        ui.btnVHDL->setEnabled(true);
    }
    else
    {
        ui.leVHDL->setDisabled(true);
        ui.btnVHDL->setDisabled(true);
    }
}


void ProjectCfg_Templates::checkedVerilog(int checked)
{
    if (Qt::Unchecked == checked)
    {
        ui.leVerilog->setEnabled(true);
        ui.btnVerilog->setEnabled(true);
    }
    else
    {
        ui.leVerilog->setDisabled(true);
        ui.btnVerilog->setDisabled(true);
    }
}


void ProjectCfg_Templates::setPathVHDL()
{
    QString path;
    if (NULL == this->project)
    {
        path = QFileDialog::getOpenFileName(this, tr("Template File"), QString(), QString("VHDL File (*.vhd)"), 0);
    }
    else if (this->project->prjPath != "untracked")
    {
        path = QFileDialog::getOpenFileName(this, tr("Template File"), QDir(this->project->prjPath.section('/',0, -2)).absolutePath(), QString("VHDL File (*.vhd)"), 0);
    }
    else
    {
        path = QFileDialog::getOpenFileName(this, tr("Template File"), QString(), QString("VHDL File (*.vhd)"), 0);
    }
    if (NULL != path)
    {
        if (ui.leVHDL->isEnabled() == false)
        {
            ui.leVHDL->setEnabled(true);
            ui.chckVHDL->setChecked(false);
        }
        ui.leVHDL->setText(path);
    }
}


void ProjectCfg_Templates::setPathVerilog()
{
    QString path;
    if (NULL == this->project)
    {
        path = QFileDialog::getOpenFileName(this, tr("Template File"), QString(), QString("Verilog File (*.v)"), 0);
    }
    else if (this->project->prjPath != "untracked")
    {
        path = QFileDialog::getOpenFileName(this, tr("Template File"), QDir(this->project->prjPath.section('/',0, -2)).absolutePath(), QString("Verilog File (*.v)"), 0);
    }
    else
    {
        path = QFileDialog::getOpenFileName(this, tr("Template File"), QString(), QString("Verilog File (*.v)"), 0);
    }
    if (NULL != path)
    {
        if (ui.leVerilog->isEnabled() == false)
        {
            ui.leVerilog->setEnabled(true);
            ui.chckVerilog->setChecked(false);
        }
        ui.leVerilog->setText(path);
    }
}