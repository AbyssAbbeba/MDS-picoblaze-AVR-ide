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
    connect(ui.btnColorLbl, SIGNAL(clicked()), this, SLOT(commentColor()));
    connect(ui.btnColorKwrd, SIGNAL(clicked()), this, SLOT(commentColor()));
    connect(ui.btnColorOpds, SIGNAL(clicked()), this, SLOT(commentColor()));
    connect(ui.btnColorOpts, SIGNAL(clicked()), this, SLOT(commentColor()));
    connect(ui.btnColorQuote, SIGNAL(clicked()), this, SLOT(commentColor()));
}


void InterfaceCfg_SyntaxAsm::commentColor()
{
    this->type = 0;
    QColorDialog dialog;
    //dialog.setOption(QColorDialog::ShowAlphaChannel, false);
    connect(&dialog, SIGNAL(colorSelected(const QColor&)), this, SLOT(colorSelected(const QColor&)));
    dialog.exec();
}


void InterfaceCfg_SyntaxAsm::labelColor()
{
    this->type = 1;
    QColorDialog dialog;
    //dialog.setOption(QColorDialog::ShowAlphaChannel, false);
    connect(&dialog, SIGNAL(colorSelected(const QColor&)), this, SLOT(colorSelected(const QColor&)));
    dialog.exec();
}


void InterfaceCfg_SyntaxAsm::keywordColor()
{
    this->type = 2;
    QColorDialog dialog;
    //dialog.setOption(QColorDialog::ShowAlphaChannel, false);
    connect(&dialog, SIGNAL(colorSelected(const QColor&)), this, SLOT(colorSelected(const QColor&)));
    dialog.exec();
}


void InterfaceCfg_SyntaxAsm::operandColor()
{
    this->type = 3;
    QColorDialog dialog;
    //dialog.setOption(QColorDialog::ShowAlphaChannel, false);
    connect(&dialog, SIGNAL(colorSelected(const QColor&)), this, SLOT(const colorSelected(QColor&)));
    dialog.exec();
}


void InterfaceCfg_SyntaxAsm::operatorColor()
{
    this->type = 4;
    QColorDialog dialog;
    //dialog.setOption(QColorDialog::ShowAlphaChannel, false);
    connect(&dialog, SIGNAL(colorSelected(const QColor&)), this, SLOT(const colorSelected(QColor&)));
    dialog.exec();
}


void InterfaceCfg_SyntaxAsm::quoteColor()
{
    this->type = 5;
    QColorDialog dialog;
    //dialog.setOption(QColorDialog::ShowAlphaChannel, false);
    connect(&dialog, SIGNAL(colorSelected(const QColor&)), this, SLOT(const colorSelected(QColor&)));
    dialog.exec();
}


void InterfaceCfg_SyntaxAsm::colorSelected(const QColor &color)
{
    switch (this->type)
    {
        case 0:
        {
            QPalette palette = this->ui.btnColorCmnt->palette();
            palette.setColor(QPalette::Button, color);
            this->ui.btnColorCmnt->setPalette(palette);
            break;
        }
        case 1:
        {
            QPalette palette = this->ui.btnColorLbl->palette();
            palette.setColor(QPalette::Button, color);
            this->ui.btnColorLbl->setPalette(palette);
            break;
        }
        case 2:
        {
            QPalette palette = this->ui.btnColorKwrd->palette();
            palette.setColor(QPalette::Button, color);
            this->ui.btnColorKwrd->setPalette(palette);
            break;
        }
        case 3:
        {
            QPalette palette = this->ui.btnColorOpds->palette();
            palette.setColor(QPalette::Button, color);
            this->ui.btnColorOpds->setPalette(palette);
            break;
        }
        case 4:
        {
            QPalette palette = this->ui.btnColorOpts->palette();
            palette.setColor(QPalette::Button, color);
            this->ui.btnColorOpts->setPalette(palette);
            break;
        }
        case 5:
        {
            QPalette palette = this->ui.btnColorQuote->palette();
            palette.setColor(QPalette::Button, color);
            this->ui.btnColorQuote->setPalette(palette);
            break;
        }
        default:
        {
            qDebug() << "InterfaceCfg_SyntaxAsm: wrong type";
            break;
        }
    }
}