#ifndef INTERFACECFG_SYNTAXASM_H
#define INTERFACECFG_SYNTAXASM_H

#include <QWidget>
#include "ui_interfacecfg_syntaxasm.h"


class InterfaceCfg_SyntaxAsm : public QWidget
{
    Q_OBJECT
    public:
        InterfaceCfg_SyntaxAsm(QWidget *parent);

    private:
        Ui_InterfaceCfg_SyntaxAsm ui;

};


#endif