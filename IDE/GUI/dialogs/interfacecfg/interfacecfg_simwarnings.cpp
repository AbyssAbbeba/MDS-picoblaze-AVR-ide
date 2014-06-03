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
 * @file interfacecfg_simwarnings.cpp
 */


#include <QtGui>
#include "interfacecfg_simwarnings.h"
#include "../../guicfg/guicfg.h"


InterfaceCfg_SimWarnings::InterfaceCfg_SimWarnings(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    this->show();

    GuiCfg::WarningsOpt options = GuiCfg::getInstance().getWarningsOpt();

    ui.chckRdNonexist->setChecked(options.memReadNonexist);
    ui.chckWrNonexist->setChecked(options.memWriteNonexist);
    ui.chckRdUnimpl->setChecked(options.memReadUnimplement);
    ui.chckWrUnimpl->setChecked(options.memWriteUnimplement);
    ui.chckRdAccess->setChecked(options.memReadAccess);
    ui.chckWrAccess->setChecked(options.memWriteAccess);
    ui.chckRdUndef->setChecked(options.memReadUndef);
    ui.chckRdDefault->setChecked(options.memReadDefault);
    ui.chckRdWrOnly->setChecked(options.memReadWriteOnly);
    ui.chckWrRdOnly->setChecked(options.memWriteReadOnly);
    ui.chckRdPtWrOnly->setChecked(options.memReadPartWriteOnly);
    ui.chckWrPtRdOnly->setChecked(options.memWritePartReadOnly);
    ui.chckRdReserved->setChecked(options.memReadReserved);
    ui.chckWrReserved->setChecked(options.memWriteReserved);

    ui.chckStackOver->setChecked(options.stackOverflow);
    ui.chckStackUnder->setChecked(options.stackUnderflow);

    ui.chckOpcode->setChecked(options.cpuOpcode);
    ui.chckJmp->setChecked(options.cpuJump);               
    ui.chckCall->setChecked(options.cpuCall);
    ui.chckIRQ->setChecked(options.cpuIRQ);
    ui.chckRet->setChecked(options.cpuRet);
    ui.chckReti->setChecked(options.cpuReti);
    ui.chckOpset->setChecked(options.cpuOpset);
    ui.chckInstUnsup->setChecked(options.cpuInsUnsupported);
    ui.chckInsIgnored->setChecked(options.cpuInsIgnored);
    ui.chckPcOver->setChecked(options.cpuPcOverflow);
    ui.chckPcUnder->setChecked(options.cpuPcUnderflow);       
}


void InterfaceCfg_SimWarnings::save()
{
    GuiCfg::WarningsOpt options = GuiCfg::getInstance().getWarningsOpt();
    
    options.memReadNonexist = ui.chckRdNonexist->isChecked();
    options.memWriteNonexist = ui.chckWrNonexist->isChecked();
    options.memReadUnimplement = ui.chckRdUnimpl->isChecked();
    options.memWriteUnimplement = ui.chckWrUnimpl->isChecked();
    options.memReadAccess = ui.chckRdAccess->isChecked();
    options.memWriteAccess = ui.chckWrAccess->isChecked();
    options.memReadUndef = ui.chckRdUndef->isChecked();
    options.memReadDefault = ui.chckRdDefault->isChecked();
    options.memReadWriteOnly = ui.chckRdWrOnly->isChecked();
    options.memWriteReadOnly = ui.chckWrRdOnly->isChecked();
    options.memReadPartWriteOnly = ui.chckRdPtWrOnly->isChecked();
    options.memWritePartReadOnly = ui.chckWrPtRdOnly->isChecked();
    options.memReadReserved = ui.chckRdReserved->isChecked();
    options.memWriteReserved = ui.chckWrReserved->isChecked();

    options.stackOverflow = ui.chckStackOver->isChecked();
    options.stackUnderflow = ui.chckStackUnder->isChecked();

    options.cpuOpcode = ui.chckOpcode->isChecked();
    options.cpuJump = ui.chckJmp->isChecked();
    options.cpuCall = ui.chckCall->isChecked();
    options.cpuIRQ = ui.chckIRQ->isChecked();
    options.cpuRet = ui.chckRet->isChecked();
    options.cpuReti = ui.chckReti->isChecked();
    options.cpuOpset = ui.chckOpset->isChecked();
    options.cpuInsUnsupported = ui.chckInstUnsup->isChecked();
    options.cpuInsIgnored = ui.chckInsIgnored->isChecked();
    options.cpuPcOverflow = ui.chckPcOver->isChecked();
    options.cpuPcUnderflow = ui.chckPcUnder->isChecked();
    
    GuiCfg::getInstance().setWarningsOpt(options);
}