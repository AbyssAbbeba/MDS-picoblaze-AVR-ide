#include <QtGui>
#include "interfacecfg_editgeneral.h"
#include "../../guicfg/guicfg.h"


InterfaceCfg_EditGeneral::InterfaceCfg_EditGeneral(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    this->show();

    this->ui.spinTabWidth->setValue(GuiCfg::getInstance().getTabWidth());
    this->ui.chckSpaces->setChecked(GuiCfg::getInstance().getTabToSpaces());
    this->ui.spinSpaces->setValue(GuiCfg::getInstance().getSpacesInTabs());
    this->ui.cmbEncoding->setCurrentIndex(this->ui.cmbEncoding->findText(GuiCfg::getInstance().getEncoding()));
    this->ui.cmbEOL->setCurrentIndex(this->ui.cmbEOL->findText(GuiCfg::getInstance().getEOL()));
}