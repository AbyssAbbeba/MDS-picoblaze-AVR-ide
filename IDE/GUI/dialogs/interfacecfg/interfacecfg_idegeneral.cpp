/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup InterfaceCfg_IDEGeneral
 * @file interfacecfg_idegeneral.cpp
 */


#include <QtGui>
#include "interfacecfg_idegeneral.h"
#include "../../guicfg/guicfg.h"


InterfaceCfg_IDEGeneral::InterfaceCfg_IDEGeneral(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    this->ui.chckSession->setChecked(GuiCfg::getInstance().getSessionRestoration());
    
    this->show();
}

void InterfaceCfg_IDEGeneral::save()
{
    GuiCfg::getInstance().setSessionRestoration(this->ui.chckSession->isChecked());
}