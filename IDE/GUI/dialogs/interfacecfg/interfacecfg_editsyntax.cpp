#include <QtGui>
#include "interfacecfg_editsyntax.h"
#include "interfacecfg_syntaxasm.h"
#include "../../guicfg/guicfg.h"


InterfaceCfg_EditSyntax::InterfaceCfg_EditSyntax(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    InterfaceCfg_SyntaxAsm *syntaxWidget = new InterfaceCfg_SyntaxAsm(this);
    ui.scrollArea->setWidget(syntaxWidget);
    this->show();
}