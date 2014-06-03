/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file interfacecfg_syntaxasm.cpp
 */


#include <QtGui>
#include "interfacecfg_syntaxasm.h"
#include "../../guicfg/guicfg.h"


InterfaceCfg_SyntaxAsm::InterfaceCfg_SyntaxAsm(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    this->setFixedWidth(353);
    this->setFixedHeight(200);
    this->show();

    QPalette palette;
    GuiCfg::HighlightOpt highlight;
    highlight = GuiCfg::getInstance().getHighlightOpts(GuiCfg::HighlightLang::PICOBLAZEASM,
                                                       GuiCfg::HighlightRole::COMMENT);

    this->ui.chckICmnt->setChecked(highlight.italic);
    this->ui.chckUCmnt->setChecked(highlight.underline);
    this->ui.chckSCmnt->setChecked(highlight.strike);
    this->ui.chckBCmnt->setChecked(highlight.bold);
    palette = this->ui.btnColorCmnt->palette();
    palette.setColor(QPalette::Button, highlight.color);
    this->ui.btnColorCmnt->setPalette(palette);

    highlight = GuiCfg::getInstance().getHighlightOpts(GuiCfg::HighlightLang::PICOBLAZEASM,
                                                       GuiCfg::HighlightRole::LABEL);

    this->ui.chckILbl->setChecked(highlight.italic);
    this->ui.chckULbl->setChecked(highlight.underline);
    this->ui.chckSLbl->setChecked(highlight.strike);
    this->ui.chckBLbl->setChecked(highlight.bold);
    palette = this->ui.btnColorLbl->palette();
    palette.setColor(QPalette::Button, highlight.color);
    this->ui.btnColorLbl->setPalette(palette);
    
    highlight = GuiCfg::getInstance().getHighlightOpts(GuiCfg::HighlightLang::PICOBLAZEASM,
                                                       GuiCfg::HighlightRole::KEYWORD);

    this->ui.chckIKwrd->setChecked(highlight.italic);
    this->ui.chckUKwrd->setChecked(highlight.underline);
    this->ui.chckSKwrd->setChecked(highlight.strike);
    this->ui.chckBKwrd->setChecked(highlight.bold);
    palette = this->ui.btnColorKwrd->palette();
    palette.setColor(QPalette::Button, highlight.color);
    this->ui.btnColorKwrd->setPalette(palette);
    
    highlight = GuiCfg::getInstance().getHighlightOpts(GuiCfg::HighlightLang::PICOBLAZEASM,
                                                       GuiCfg::HighlightRole::OPERAND);

    this->ui.chckIOpds->setChecked(highlight.italic);
    this->ui.chckUOpds->setChecked(highlight.underline);
    this->ui.chckSOpds->setChecked(highlight.strike);
    this->ui.chckBOpds->setChecked(highlight.bold);
    palette = this->ui.btnColorOpds->palette();
    palette.setColor(QPalette::Button, highlight.color);
    this->ui.btnColorOpds->setPalette(palette);

    highlight = GuiCfg::getInstance().getHighlightOpts(GuiCfg::HighlightLang::PICOBLAZEASM,
                                                       GuiCfg::HighlightRole::OPERATOR);

    this->ui.chckIOpts->setChecked(highlight.italic);
    this->ui.chckUOpts->setChecked(highlight.underline);
    this->ui.chckSOpts->setChecked(highlight.strike);
    this->ui.chckBOpts->setChecked(highlight.bold);
    palette = this->ui.btnColorOpts->palette();
    palette.setColor(QPalette::Button, highlight.color);
    this->ui.btnColorOpts->setPalette(palette);

    highlight = GuiCfg::getInstance().getHighlightOpts(GuiCfg::HighlightLang::PICOBLAZEASM,
                                                       GuiCfg::HighlightRole::QUOTE);

    this->ui.chckIQuote->setChecked(highlight.italic);
    this->ui.chckUQuote->setChecked(highlight.underline);
    this->ui.chckSQuote->setChecked(highlight.strike);
    this->ui.chckBQuote->setChecked(highlight.bold);
    palette = this->ui.btnColorQuote->palette();
    palette.setColor(QPalette::Button, highlight.color);
    this->ui.btnColorQuote->setPalette(palette);

    connect(this->ui.btnColorCmnt, SIGNAL(clicked()), this, SLOT(commentColor()));
    connect(this->ui.btnColorLbl, SIGNAL(clicked()), this, SLOT(labelColor()));
    connect(this->ui.btnColorKwrd, SIGNAL(clicked()), this, SLOT(keywordColor()));
    connect(this->ui.btnColorOpds, SIGNAL(clicked()), this, SLOT(operandColor()));
    connect(this->ui.btnColorOpts, SIGNAL(clicked()), this, SLOT(operatorColor()));
    connect(this->ui.btnColorQuote, SIGNAL(clicked()), this, SLOT(quoteColor()));

    connect(this->ui.chckILbl, SIGNAL(stateChanged(int)), this, SLOT(checkBoxILblChanged(int)));
    connect(this->ui.chckULbl, SIGNAL(stateChanged(int)), this, SLOT(checkBoxULblChanged(int)));
    connect(this->ui.chckSLbl, SIGNAL(stateChanged(int)), this, SLOT(checkBoxSLblChanged(int)));
    connect(this->ui.chckBLbl, SIGNAL(stateChanged(int)), this, SLOT(checkBoxBLblChanged(int)));

    connect(this->ui.chckIKwrd, SIGNAL(stateChanged(int)), this, SLOT(checkBoxIKwrdChanged(int)));
    connect(this->ui.chckUKwrd, SIGNAL(stateChanged(int)), this, SLOT(checkBoxUKwrdChanged(int)));
    connect(this->ui.chckSKwrd, SIGNAL(stateChanged(int)), this, SLOT(checkBoxSKwrdChanged(int)));
    connect(this->ui.chckBKwrd, SIGNAL(stateChanged(int)), this, SLOT(checkBoxBKwrdChanged(int)));

    connect(this->ui.chckIOpds, SIGNAL(stateChanged(int)), this, SLOT(checkBoxIOpdsChanged(int)));
    connect(this->ui.chckUOpds, SIGNAL(stateChanged(int)), this, SLOT(checkBoxUOpdsChanged(int)));
    connect(this->ui.chckSOpds, SIGNAL(stateChanged(int)), this, SLOT(checkBoxSOpdsChanged(int)));
    connect(this->ui.chckBOpds, SIGNAL(stateChanged(int)), this, SLOT(checkBoxBOpdsChanged(int)));

    connect(this->ui.chckIOpts, SIGNAL(stateChanged(int)), this, SLOT(checkBoxIOptsChanged(int)));
    connect(this->ui.chckUOpts, SIGNAL(stateChanged(int)), this, SLOT(checkBoxUOptsChanged(int)));
    connect(this->ui.chckSOpts, SIGNAL(stateChanged(int)), this, SLOT(checkBoxSOptsChanged(int)));
    connect(this->ui.chckBOpts, SIGNAL(stateChanged(int)), this, SLOT(checkBoxBOptsChanged(int)));

    connect(this->ui.chckIQuote, SIGNAL(stateChanged(int)), this, SLOT(checkBoxIQuoteChanged(int)));
    connect(this->ui.chckUQuote, SIGNAL(stateChanged(int)), this, SLOT(checkBoxUQuoteChanged(int)));
    connect(this->ui.chckSQuote, SIGNAL(stateChanged(int)), this, SLOT(checkBoxSQuoteChanged(int)));
    connect(this->ui.chckBQuote, SIGNAL(stateChanged(int)), this, SLOT(checkBoxBQuoteChanged(int)));
}


void InterfaceCfg_SyntaxAsm::commentColor()
{
    this->type = Types::COMMENT;
    QColorDialog dialog;
    //dialog.setOption(QColorDialog::ShowAlphaChannel, false);
    connect(&dialog, SIGNAL(colorSelected(const QColor&)), this, SLOT(colorSelected(const QColor&)));
    dialog.exec();
}


void InterfaceCfg_SyntaxAsm::labelColor()
{
    this->type = Types::LABEL;
    QColorDialog dialog;
    //dialog.setOption(QColorDialog::ShowAlphaChannel, false);
    connect(&dialog, SIGNAL(colorSelected(const QColor&)), this, SLOT(colorSelected(const QColor&)));
    dialog.exec();
}


void InterfaceCfg_SyntaxAsm::keywordColor()
{
    this->type = Types::KEYWORD;
    QColorDialog dialog;
    //dialog.setOption(QColorDialog::ShowAlphaChannel, false);
    connect(&dialog, SIGNAL(colorSelected(const QColor&)), this, SLOT(colorSelected(const QColor&)));
    dialog.exec();
}


void InterfaceCfg_SyntaxAsm::operandColor()
{
    this->type = Types::OPERAND;
    QColorDialog dialog;
    //dialog.setOption(QColorDialog::ShowAlphaChannel, false);
    connect(&dialog, SIGNAL(colorSelected(const QColor&)), this, SLOT(colorSelected(const QColor&)));
    dialog.exec();
}


void InterfaceCfg_SyntaxAsm::operatorColor()
{
    this->type = Types::OPERATOR;
    QColorDialog dialog;
    //dialog.setOption(QColorDialog::ShowAlphaChannel, false);
    connect(&dialog, SIGNAL(colorSelected(const QColor&)), this, SLOT(colorSelected(const QColor&)));
    dialog.exec();
}


void InterfaceCfg_SyntaxAsm::quoteColor()
{
    this->type = Types::QUOTE;
    QColorDialog dialog;
    //dialog.setOption(QColorDialog::ShowAlphaChannel, false);
    connect(&dialog, SIGNAL(colorSelected(const QColor&)), this, SLOT(colorSelected(const QColor&)));
    dialog.exec();
}


void InterfaceCfg_SyntaxAsm::colorSelected(const QColor &color)
{
    switch (this->type)
    {
        case Types::COMMENT:
        {
            QPalette palette = this->ui.btnColorCmnt->palette();
            palette.setColor(QPalette::Button, color);
            this->ui.btnColorCmnt->setPalette(palette);
            emit colorChanged(color, Types::COMMENT);
            break;
        }
        case Types::LABEL:
        {
            QPalette palette = this->ui.btnColorLbl->palette();
            palette.setColor(QPalette::Button, color);
            this->ui.btnColorLbl->setPalette(palette);
            emit colorChanged(color, Types::LABEL);
            break;
        }
        case Types::KEYWORD:
        {
            QPalette palette = this->ui.btnColorKwrd->palette();
            palette.setColor(QPalette::Button, color);
            this->ui.btnColorKwrd->setPalette(palette);
            emit colorChanged(color, Types::KEYWORD);
            break;
        }
        case Types::OPERAND:
        {
            QPalette palette = this->ui.btnColorOpds->palette();
            palette.setColor(QPalette::Button, color);
            this->ui.btnColorOpds->setPalette(palette);
            emit colorChanged(color, Types::OPERAND);
            break;
        }
        case Types::OPERATOR:
        {
            QPalette palette = this->ui.btnColorOpts->palette();
            palette.setColor(QPalette::Button, color);
            this->ui.btnColorOpts->setPalette(palette);
            emit colorChanged(color, Types::OPERATOR);
            break;
        }
        case Types::QUOTE:
        {
            QPalette palette = this->ui.btnColorQuote->palette();
            palette.setColor(QPalette::Button, color);
            this->ui.btnColorQuote->setPalette(palette);
            emit colorChanged(color, Types::QUOTE);
            break;
        }
        default:
        {
            qDebug() << "InterfaceCfg_SyntaxAsm: wrong type";
            break;
        }
    }
}


void InterfaceCfg_SyntaxAsm::checkBoxICmntChanged(int state)
{
    //this->ui.chckICmnt->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxUCmntChanged(int state)
{
    //this->ui.chckUCmnt->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxSCmntChanged(int state)
{
    //this->ui.chckSCmnt->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxBCmntChanged(int state)
{
    //this->ui.chckBCmnt->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxILblChanged(int state)
{
    //this->ui.chckILbl->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxULblChanged(int state)
{
    //this->ui.chckILbl->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxSLblChanged(int state)
{
    //this->ui.chckILbl->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxBLblChanged(int state)
{
    //this->ui.chckILbl->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxIKwrdChanged(int state)
{
    //this->ui.chckIKwrd->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxUKwrdChanged(int state)
{
    //this->ui.chckIKwrd->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxSKwrdChanged(int state)
{
    //this->ui.chckIKwrd->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxBKwrdChanged(int state)
{
    //this->ui.chckIKwrd->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxIOpdsChanged(int state)
{
    //this->ui.chckIOpds->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxUOpdsChanged(int state)
{
    //this->ui.chckIOpds->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxSOpdsChanged(int state)
{
    //this->ui.chckIOpds->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxBOpdsChanged(int state)
{
    //this->ui.chckIOpds->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxIOptsChanged(int state)
{
    //this->ui.chckIOpts->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxUOptsChanged(int state)
{
    //this->ui.chckIOpts->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxSOptsChanged(int state)
{
    //this->ui.chckIOpts->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxBOptsChanged(int state)
{
    //this->ui.chckIOpts->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxIQuoteChanged(int state)
{
    //this->ui.chckIQuote->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxUQuoteChanged(int state)
{
    //this->ui.chckIQuote->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxSQuoteChanged(int state)
{
    //this->ui.chckIQuote->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::checkBoxBQuoteChanged(int state)
{
    //this->ui.chckIQuote->setChecked(state);
}


void InterfaceCfg_SyntaxAsm::save()
{
}