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
 * @file interfacecgfdlg_core.h
 */


#ifndef INTERFACECFGDLG_CORE_H
#define INTERFACECFGDLG_CORE_H


#include <QDialog>



class CfgInterface;
class InterfaceCfg_EditFonts;
class InterfaceCfg_EditGeneral;
class InterfaceCfg_EditSyntax;
class InterfaceCfg_IDEGeneral;
class InterfaceCfg_SimOthers;
class InterfaceCfg_SimWarnings;


class InterfaceCfgDlg_Core : public QDialog
{
    Q_OBJECT
    public:
        InterfaceCfgDlg_Core(QWidget *parent);
        
    private:
        //void save();
        
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