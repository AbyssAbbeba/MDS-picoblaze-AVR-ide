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
 * @file interfacecfg_simothers.h
 */


#ifndef INTERFACECFG_SIMOTHERS_H
#define INTERFACECFG_SIMOTHERS_H

#include <QWidget>
#include "ui_interfacecfg_simothers.h"


class InterfaceCfg_SimOthers : public QWidget
{
    Q_OBJECT
    public:
        InterfaceCfg_SimOthers(QWidget *parent);
        void save();

    private:
        Ui_InterfaceCfg_SimOthers ui;

};


#endif