#ifndef INTERFACECFGDLG_CORE_H
#define INTERFACECFGDLG_CORE_H


#include <QDialog>
#include "../cfginterface/cfginterface.h"
#include "interfacecfg_editfonts.h"
#include "interfacecfg_editgeneral.h"
#include "interfacecfg_editsyntax.h"
#include "interfacecfg_idegeneral.h"
#include "interfacecfg_simothers.h"
#include "interfacecfg_simwarnings.h"


class InterfaceCfgDlg_Core : public QDialog
{
    Q_OBJECT
    public:
        InterfaceCfgDlg_Core(QWidget *parent);
        
    private:
        void load();
        
        CfgInterface *cfgInterface;
        InterfaceCfg_EditFonts *editFonts;
        InterfaceCfg_EditGeneral *editGeneral;
        InterfaceCfg_EditSyntax *editSyntax;
        InterfaceCfg_IDEGeneral *ideGeneral;
        InterfaceCfg_SimOthers *simOthers;
        InterfaceCfg_SimWarnings *simWarnings;

    private slots:
        void ok();
        void cancel();
};


#endif