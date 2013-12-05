#ifndef INTERFACECFG_EDITFONTS_H
#define INTERFACECFG_EDITFONTS_H

#include <QWidget>
#include "ui_interfacecfg_editfonts.h"


class InterfaceCfg_EditFonts : public QWidget
{
    Q_OBJECT
    public:
        InterfaceCfg_EditFonts(QWidget *parent);

    private:
        Ui_InterfaceCfg_EditFonts ui;

};


#endif