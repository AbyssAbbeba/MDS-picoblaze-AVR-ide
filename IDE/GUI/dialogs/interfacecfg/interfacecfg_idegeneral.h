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
 * @file interfacecfg_idegeneral.h
 */


#ifndef INTERFACECFG_IDEGENERAL_H
#define INTERFACECFG_IDEGENERAL_H

#include <QWidget>
#include "ui_interfacecfg_idegeneral.h"


class InterfaceCfg_IDEGeneral : public QWidget
{
    Q_OBJECT
    public:
        InterfaceCfg_IDEGeneral(QWidget *parent);
        void save();

    private:
        Ui_InterfaceCfg_IDEGeneral ui;

};


#endif