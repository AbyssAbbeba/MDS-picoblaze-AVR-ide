#ifndef INTERFACECFG_EDITGENERAL_H
#define INTERFACECFG_EDITGENERAL_H

#include <QWidget>
#include "ui_interfacecfg_editgeneral.h"


class InterfaceCfg_EditGeneral : public QWidget
{
    Q_OBJECT
    public:
        InterfaceCfg_EditGeneral(QWidget *parent);
        void save();

    private:
        Ui_InterfaceCfg_EditGeneral ui;

    private slots:
        void checkBoxChanged(int state);

};


#endif