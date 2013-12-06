#ifndef INTERFACECFG_SIMOTHERS_H
#define INTERFACECFG_SIMOTHERS_H

#include <QWidget>
#include "ui_interfacecfg_simothers.h"


class InterfaceCfg_SimOthers : public QWidget
{
    Q_OBJECT
    public:
        InterfaceCfg_SimOthers(QWidget *parent);

    private:
        Ui_InterfaceCfg_SimOthers ui;

};


#endif