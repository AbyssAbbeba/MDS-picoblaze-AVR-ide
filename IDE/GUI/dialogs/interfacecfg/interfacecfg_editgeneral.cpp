/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup InterfaceCfg_EditGeneral
 * @file interfacecfg_editgeneral.cpp
 */


#include <QtGui>
#include "interfacecfg_editgeneral.h"
#include "../../guicfg/guicfg.h"


InterfaceCfg_EditGeneral::InterfaceCfg_EditGeneral(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    this->show();

    QList<QByteArray> codecs = QTextCodec::availableCodecs();
    foreach (QByteArray array, codecs)
    {
        this->ui.cmbEncoding->addItem(QString(array));
    }


    this->ui.spinTabWidth->setValue(GuiCfg::getInstance().getTabWidth());
    this->ui.chckSpaces->setChecked(GuiCfg::getInstance().getTabToSpaces());
    if (true == this->ui.chckSpaces->isChecked())
    {
        this->ui.lblSpaces->setEnabled(true);
        this->ui.spinSpaces->setEnabled(true);
    }
    else
    {
        this->ui.lblSpaces->setDisabled(true);
        this->ui.spinSpaces->setDisabled(true);
    }
    this->ui.spinSpaces->setValue(GuiCfg::getInstance().getSpacesInTab());
    this->ui.cmbEncoding->setCurrentIndex(this->ui.cmbEncoding->findText(GuiCfg::getInstance().getEncoding()));
    this->ui.cmbEOL->setCurrentIndex(this->ui.cmbEOL->findText(GuiCfg::getInstance().getEOL()));
    
    this->connect(this->ui.chckSpaces, SIGNAL(stateChanged(int)), this, SLOT(checkBoxChanged(int)));
}


void InterfaceCfg_EditGeneral::checkBoxChanged(int state)
{
    this->ui.lblSpaces->setEnabled(state);
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