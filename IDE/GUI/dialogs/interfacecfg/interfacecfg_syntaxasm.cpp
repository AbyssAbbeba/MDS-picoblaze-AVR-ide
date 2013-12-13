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

    connect(ui.btnColorCmnt, SIGNAL(clicked()), this, SLOT(commentColor()));
    connect(ui.btnColorLbl, SIGNAL(clicked()), this, SLOT(labelColor()));
    connect(ui.btnColorKwrd, SIGNAL(clicked()), this, SLOT(keywordColor()));
    connect(ui.btnColorOpds, SIGNAL(clicked()), this, SLOT(operandColor()));
    connect(ui.btnColorOpts, SIGNAL(clicked()), this, SLOT(operatorColor()));
    connect(ui.btnColorQuote, SIGNAL(clicked()), this, SLOT(quoteColor()));
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