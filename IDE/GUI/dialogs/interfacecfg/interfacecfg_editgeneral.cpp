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
    this->ui.spinSpaces->setEnabled(this->ui.chckSpaces->isChecked());
    this->ui.spinSpaces->setValue(GuiCfg::getInstance().getSpacesInTabs());
    this->ui.cmbEncoding->setCurrentIndex(this->ui.cmbEncoding->findText(GuiCfg::getInstance().getEncoding()));
    this->ui.cmbEOL->setCurrentIndex(this->ui.cmbEOL->findText(GuiCfg::getInstance().getEOL()));
    
    this->connect(this->ui.chckSpaces, SIGNAL(stateChanged(int)), this, SLOT(checkBoxChanged(int)));
}


void InterfaceCfg_EditGeneral::checkBoxChanged(int state)
{
    this->ui.spinSpaces->setEnabled(state);
}

void InterfaceCfg_EditGeneral::save()
{
    GuiCfg::getInstance().setTabWidth(this->ui.spinTabWidth->value());
    GuiCfg::getInstance().setTabToSpaces(this->ui.chckSpaces->isChecked());
    GuiCfg::getInstance().setSpacesInTab(this->ui.spinSpaces->value());
    GuiCfg::getInstance().setEncoding(this->ui.cmbEncoding->currentText());
    GuiCfg::getInstance().setEOL(this->ui.cmbEOL->currentText());
}