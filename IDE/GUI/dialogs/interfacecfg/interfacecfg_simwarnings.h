#ifndef INTERFACECFG_SIMWARNINGS_H
#define INTERFACECFG_SIMWARNINGS_H

#include <QWidget>
#include "ui_interfacecfg_simwarnings.h"


class InterfaceCfg_SimWarnings : public QWidget
{
    Q_OBJECT
    public:
        InterfaceCfg_SimWarnings(QWidget *parent);

    private:
        Ui_InterfaceCfg_SimWarnings ui;

};


#endif