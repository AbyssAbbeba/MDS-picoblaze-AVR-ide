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
 * @file interfacecfg_externalapps.h
 */


#ifndef INTERFACECFG_EXTERNALAPPS_H
#define INTERFACECFG_EXTERNALAPPS_H

#include <QWidget>
#include "ui_interfacecfg_externalapps.h"


class InterfaceCfg_ExternalApps : public QWidget
{
    Q_OBJECT
    public:
        InterfaceCfg_ExternalApps(QWidget *parent);
        void save();

    private slots:
        void setPath1();
        void setPath2();
        void setPath3();

    private:
        Ui_InterfaceCfg_ExternalApps ui;

};


#endif