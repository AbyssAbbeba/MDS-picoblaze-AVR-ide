/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup InterfaceCfg_EditSyntax
 * @file interfacecfg_editsyntax.cpp
 */


#include <QtGui>
#include "interfacecfg_editsyntax.h"
#include "../../guicfg/guicfg.h"


InterfaceCfg_EditSyntax::InterfaceCfg_EditSyntax(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    InterfaceCfg_SyntaxAsm *syntaxWidget = new InterfaceCfg_SyntaxAsm(this);
    this->ui.scrollArea->setWidget(syntaxWidget);
    this->ui.chckHighlight->setChecked(GuiCfg::getInstance().getHighlightEnabled());
    this->ui.scrollArea->setEnabled(GuiCfg::getInstance().getHighlightEnabled());
    this->ui.frameExample->setEnabled(GuiCfg::getInstance().getHighlightEnabled());
    this->show();

    connect(syntaxWidget, SIGNAL(colorChanged(const QColor&, InterfaceCfg_SyntaxAsm::Types)), this, SLOT(colorChanged(const QColor&, InterfaceCfg_SyntaxAsm::Types)));
    connect(syntaxWidget, SIGNAL(italicChanged(bool, InterfaceCfg_SyntaxAsm::Types)), this, SLOT(italicChanged(bool, InterfaceCfg_SyntaxAsm::Types)));
    connect(syntaxWidget, SIGNAL(underlineChanged(bool, InterfaceCfg_SyntaxAsm::Types)), this, SLOT(underlineChanged(bool, InterfaceCfg_SyntaxAsm::Types)));
    connect(syntaxWidget, SIGNAL(strikeChanged(bool, InterfaceCfg_SyntaxAsm::Types)), this, SLOT(strikeChanged(bool, InterfaceCfg_SyntaxAsm::Types)));
    connect(syntaxWidget, SIGNAL(boldChanged(bool, InterfaceCfg_SyntaxAsm::Types)), this, SLOT(boldChanged(bool, InterfaceCfg_SyntaxAsm::Types)));
    connect(this->ui.chckHighlight, SIGNAL(stateChanged(int)), this, SLOT(checkBoxChanged(int)));
}



void InterfaceCfg_EditSyntax::colorChanged(const QColor &color, InterfaceCfg_SyntaxAsm::Types type)
{
}


void InterfaceCfg_EditSyntax::italicChanged(bool enabled, InterfaceCfg_SyntaxAsm::Types type)
{
}


void InterfaceCfg_EditSyntax::underlineChanged(bool enabled, InterfaceCfg_SyntaxAsm::Types type)
{
}


void InterfaceCfg_EditSyntax::strikeChanged(bool enabled, InterfaceCfg_SyntaxAsm::Types type)
{
}


void InterfaceCfg_EditSyntax::boldChanged(bool enabled, InterfaceCfg_SyntaxAsm::Types type)
{
}


void InterfaceCfg_EditSyntax::save()
{
}


void InterfaceCfg_EditSyntax::checkBoxChanged(int state)
{
    this->ui.scrollArea->setEnabled(state);
    this->ui.frameExample->setEnabled(state);
}