/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 *
 */



#include <QtGui>
#include "translatordlg.h"
#include "../../../utilities/AsmTranslator/AsmTranslator.h"
#include <string>
#include <cstdio>
#include <fstream>
#include <sstream>


/**
 * @brief
 * @param
 * @param
 */
TranslatorDlg::TranslatorDlg(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    this->show();

    connect(this->ui.btnBox, SIGNAL(accepted()), this, SLOT(create()));
    connect(this->ui.btnBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(this->ui.btnChange, SIGNAL(clicked()), this, SLOT(setPath()));
}


/**
 * @brief
 */
void TranslatorDlg::setPath()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("(*.psm *.asm)"));
    if (path != NULL)
    {
        this->ui.lePath->setText(path);
    }
}


/**
 * @brief
 */
void TranslatorDlg::create()
{
    AsmTranslator translator;

    std::ifstream input(this->ui.lePath->text().toStdString());
    std::ostringstream outputStream;
    bool finalResult = translator.translate((AsmTranslator::Variant)(this->ui.cmbType->currentIndex()), outputStream, input);
    
    if ( true == finalResult )
    {
        std::vector<std::string> outputVector;
        outputVector.push_back(outputStream.str());
        emit output(outputVector);
    }
    else
    {
        emit output(translator.getMessages());
    }
    
    accept();
}
