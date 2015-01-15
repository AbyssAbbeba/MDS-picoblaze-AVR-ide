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
 * @file translatordlgr.cpp
 */


#include <QtGui>
#include "translatordlg.h"
#include "../../../utilities/AsmTranslator/AsmTranslator.h"
#include "../../../utilities/AsmTranslator/AsmTranslatorConfig.h"
#include "../../errordialog/errordlg.h"
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
    this->setFixedSize(this->size());

    connect(this->ui.btnBox, SIGNAL(accepted()), this, SLOT(create()));
    connect(this->ui.btnBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(this->ui.btnChange, SIGNAL(clicked()), this, SLOT(setPath()));
    connect(this->ui.cmbIndent,
            SIGNAL(currentIndexChanged(const QString &)),
            this,
            SLOT(tabsChanged(const QString &)));
}


/**
 * @brief
 */
void TranslatorDlg::setPath()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), m_path, tr("(*.psm *.asm)"));
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
    if (true == ui.lePath->text().isEmpty())
    {
        return;
    }
    QFile file(this->ui.lePath->text());
    if (false == file.exists())
    {
        error(ERR_OPENFILE);
        return;
    }
    AsmTranslator translator;

    std::ifstream input(this->ui.lePath->text().toStdString());
    std::ostringstream outputStream;
    translator.m_config.m_letterCase[0] = (AsmTranslatorConfig::LetterCase) (this->ui.cmbCaseSymbol->currentIndex());
    translator.m_config.m_letterCase[1] = (AsmTranslatorConfig::LetterCase) (this->ui.cmbCaseDir->currentIndex());
    translator.m_config.m_letterCase[2] = (AsmTranslatorConfig::LetterCase) (this->ui.cmbCaseInst->currentIndex());
    translator.m_config.m_eol = (AsmTranslatorConfig::EndOfLine) (this->ui.cmbEOL->currentIndex());
    translator.m_config.m_indentation = (AsmTranslatorConfig::Indentation) this->ui.cmbIndent->currentIndex();
    translator.m_config.m_tabSize = this->ui.spinTabs->value();
    translator.m_config.m_shortInstructions = this->ui.chckShortInst->isChecked();
    bool finalResult = translator.translate((AsmTranslator::Variant)(this->ui.cmbType->currentIndex()),
                                            outputStream,
                                            input);

    if ( true == finalResult )
    {
        std::vector<std::string> outputVector;
        outputVector.push_back(outputStream.str());
        emit output(outputVector);
    }
    else
    {
        //connect in mainform
        emit outputError(translator.getMessages());
    }
    
    accept();
}


void TranslatorDlg::tabsChanged(const QString &text)
{
    if (text == "Tabs")
    {
        this->ui.spinTabs->setEnabled(true);
        this->ui.lblTabs->setEnabled(true);
    }
    else
    {
        this->ui.spinTabs->setEnabled(false);
        this->ui.lblTabs->setEnabled(false);
    }
}


void TranslatorDlg::setPath(QString path)
{
    m_path = path;
}