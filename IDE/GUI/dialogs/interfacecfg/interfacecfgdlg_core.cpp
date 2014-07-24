/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup InterfaceCfgDlg_Core
 * @file interfacecfgdlg_core.cpp
 */


#include <QtGui>
#include "interfacecfgdlg_core.h"
#include "../../guicfg/guicfg.h"


InterfaceCfgDlg_Core::InterfaceCfgDlg_Core(QWidget *parent)
    : QDialog(parent)
{
    this->setModal(true);
    this->setWindowTitle("Interface Config");
    this->cfgInterface = new CfgInterface(this);

    this->ideGeneral = new InterfaceCfg_IDEGeneral(this->cfgInterface);
    this->editGeneral = new InterfaceCfg_EditGeneral(this->cfgInterface);
    //this->editSyntax = new InterfaceCfg_EditSyntax(this->cfgInterface);
    this->editFonts = new InterfaceCfg_EditFonts(this->cfgInterface);
    this->simWarnings = new InterfaceCfg_SimWarnings(this->cfgInterface);
    //this->simOthers = new InterfaceCfg_SimOthers(this->cfgInterface);

    this->cfgInterface->addWidget(NULL, "IDE", "IDE Config");
    this->cfgInterface->addWidget(this->ideGeneral, "General", "IDE General Options", true);
    //this->cfgInterface->addWidget(NULL, "Shortcuts", "IDE Shortcuts", true);
    this->cfgInterface->addWidget(NULL, "Editor", "Editor Config");
    this->cfgInterface->addWidget(this->editGeneral, "General", "Editor General Options", true);
    //this->cfgInterface->addWidget(this->editSyntax, "Highlighter", "Editor Syntax Highlight", true);
    this->cfgInterface->addWidget(this->editFonts, "Fonts", "Editor Fonts", true);
    this->cfgInterface->addWidget(NULL, "Simulator", "Simulator Config");
    this->cfgInterface->addWidget(this->simWarnings, "Warnings", "Simulator Warnings", true);
    //this->cfgInterface->addWidget(this->simOthers, "Others", "Simulator Others", true);

    this->cfgInterface->show();
    this->show();
    this->cfgInterface->fixSize();
    this->setFixedWidth(this->cfgInterface->width());
    this->setFixedHeight(this->cfgInterface->height());
    this->cfgInterface->expandAll();

    connect(this->cfgInterface->buttonBox, SIGNAL(accepted()), this, SLOT(ok()));
    connect(this->cfgInterface->buttonBox, SIGNAL(rejected()), this, SLOT(cancel()));
}


/**
 * @brief
 */
void InterfaceCfgDlg_Core::ok()
{
    this->editFonts->save();
    this->editGeneral->save();
    //this->editSyntax->save();
    this->ideGeneral->save();
    this->simWarnings->save();
    //this->simOthers->save();
    GuiCfg::getInstance().saveConfig();
    this->done(1);
}


/**
 * @brief
 */
void InterfaceCfgDlg_Core::cancel()
{
    this->done(0);
}


/*void InterfaceCfgDlg_Core::load()
{
    //GuiCfg::getInstance();
}*/
