#ifndef INTERFACECFG_EDITSYNTAX_H
#define INTERFACECFG_EDITSYNTAX_H

#include <QWidget>
#include "ui_interfacecfg_editsyntax.h"


class InterfaceCfg_EditSyntax : public QWidget
{
    Q_OBJECT
    public:
        InterfaceCfg_EditSyntax(QWidget *parent);

    private:
        Ui_InterfaceCfg_EditSyntax ui;

};


#endif