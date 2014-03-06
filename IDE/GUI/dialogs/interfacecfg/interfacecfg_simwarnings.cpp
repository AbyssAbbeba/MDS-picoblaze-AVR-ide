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
}


void InterfaceCfg_SimWarnings::save()
{
}