/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file interfacecfg_simwarnings.h
 */


#ifndef INTERFACECFG_SIMWARNINGS_H
#define INTERFACECFG_SIMWARNINGS_H

#include <QWidget>
#include "ui_interfacecfg_simwarnings.h"


class InterfaceCfg_SimWarnings : public QWidget
{
    Q_OBJECT
    public:
        InterfaceCfg_SimWarnings(QWidget *parent);
        void save();

    private:
        Ui_InterfaceCfg_SimWarnings ui;

};


#endif