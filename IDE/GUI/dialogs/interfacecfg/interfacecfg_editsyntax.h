#ifndef INTERFACECFG_EDITSYNTAX_H
#define INTERFACECFG_EDITSYNTAX_H

#include <QWidget>
#include <QColor>
#include "interfacecfg_syntaxasm.h"
#include "ui_interfacecfg_editsyntax.h"


class InterfaceCfg_EditSyntax : public QWidget
{
    Q_OBJECT
    public:
        InterfaceCfg_EditSyntax(QWidget *parent);
        void save();

    private:
        Ui_InterfaceCfg_EditSyntax ui;

    private slots:
        void colorChanged(const QColor &color, InterfaceCfg_SyntaxAsm::Types type);
        void italicChanged(bool enabled, InterfaceCfg_SyntaxAsm::Types type);
        void underlineChanged(bool enabled, InterfaceCfg_SyntaxAsm::Types type);
        void strikeChanged(bool enabled, InterfaceCfg_SyntaxAsm::Types type);
        void boldChanged(bool enabled, InterfaceCfg_SyntaxAsm::Types type);
        void checkBoxChanged(int state);

};


#endif