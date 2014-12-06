/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup  FileConvertDlg
 * @file fileconvertdlg.cpp
 */


#include <QtGui>
#include "fileconvertdlg.h"
#include "../../../utilities/MCUDataFiles/HexFile.h"
#include "../../../utilities/MCUDataFiles/BinFile.h"
#include "../../../utilities/MCUDataFiles/SrecFile.h"
#include "../../../utilities/MCUDataFiles/XilMemFile.h"
#include "../../../utilities/MCUDataFiles/XilVerilogFile.h"
#include "../../../utilities/MCUDataFiles/XilVHDLFile.h"
#include "../../../utilities/MCUDataFiles/RawHexDumpFile.h"
#include "../../../utilities/MCUDataFiles/DataFile.h"


/**
 * @brief
 * @param
 * @param
 */
FileConvertDlg::FileConvertDlg(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    this->show();

    connect(this->ui.btnBox, SIGNAL(accepted()), this, SLOT(create()));
    connect(this->ui.btnBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(this->ui.btnChangeIn, SIGNAL(clicked()), this, SLOT(setPathIn()));
    connect(this->ui.btnChangeOut, SIGNAL(clicked()), this, SLOT(setPathOut()));
    connect(this->ui.btnTemplate, SIGNAL(clicked()), this, SLOT(setPathTemplate()));
    connect(this->ui.cmbTypeIn, SIGNAL(currentIndexChanged(int)), this, SLOT(setOptIn(int)));
    connect(this->ui.cmbTypeOut, SIGNAL(currentIndexChanged(int)), this, SLOT(setOptOut(int)));
}


/**
 * @brief
 */
void FileConvertDlg::setPathIn()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), "", "(*.hex *.bin *.srec *.mem *.v *.vhd)");
    if (path != NULL)
    {
        this->ui.lePathIn->setText(path);
    }
}


/**
 * @brief
 */
void FileConvertDlg::setPathOut()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), "", "(*.hex *.bin *.srec *.mem *.v *.vhd)");
    if (path != NULL)
    {
        this->ui.lePathOut->setText(path);
    }
}


/**
 * @brief
 */
void FileConvertDlg::setPathTemplate()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), "", "(*.v *.vhd)");
    if (path != NULL)
    {
        this->ui.leTemplate->setText(path);
    }
}


/**
 * @brief
 */
void FileConvertDlg::create()
{
    //bool finalResult;
    DataFile * input;
    DataFile * output;

    try
    {
        switch (this->ui.cmbTypeIn->currentIndex())
        {
            case 0:
                input = new RawHexDumpFile((RawHexDumpFile::OPCodeSize)(this->ui.cmbOpcodeIn->currentText().toInt()));
                break;
            case 1:
                input = new XilMemFile(this->ui.cmbBPRIn->currentText().toInt());
                break;
            case 2:
                input = new XilVHDLFile("", "", (XilHDLFile::OPCodeSize)(this->ui.cmbOpcodeIn->currentText().toInt()));
                break;
            case 3:
                input = new XilVerilogFile("", "", (XilHDLFile::OPCodeSize)(this->ui.cmbOpcodeIn->currentText().toInt()));
                break;
            case 4:
                input = new HexFile();
                break;
            case 5:
                input = new SrecFile();
                break;
            case 6:
                input = new BinFile();
                break;
        }

        input->clearAndLoad(this->ui.lePathIn->text().toStdString());

        switch (this->ui.cmbTypeOut->currentIndex())
        {
            case 0:
            {
                output = new HexFile();
                break;
            }
            case 1:
            {
                output = new BinFile();
                break;
            }
            case 2:
            {
                output = new SrecFile();
                break;
            }
            case 3:
            {
                output = new XilMemFile(this->ui.cmbBPROut->currentText().toInt());
                break;
            }
            case 4:
            {
                output = new XilVerilogFile(this->ui.leTemplateName->text().toStdString(), this->ui.leTemplate->text().toStdString(),
                                            (XilHDLFile::OPCodeSize)(this->ui.cmbOpcodeOut->currentText().toInt()));
                break;
            }
            case 5:
            {
                output = new XilVHDLFile(this->ui.leTemplateName->text().toStdString(), this->ui.leTemplate->text().toStdString(),
                                        (XilHDLFile::OPCodeSize)(this->ui.cmbOpcodeOut->currentText().toInt()));
                break;
            }
        }

        output->setData(input->getData(), input->maxSize());
        output->save(this->ui.lePathOut->text().toStdString());

    }
    catch ( const DataFileException & e )
    {
    }

    delete input;
    delete output;

    accept();
}


/**
 * @brief
 */
void FileConvertDlg::setOptIn(int index)
{
    switch (index)
    {
        case 0:
        {
            this->ui.lblBPRIn->setEnabled(false);
            this->ui.cmbBPRIn->setEnabled(false);
            this->ui.lblOpcodeIn->setEnabled(false);
            this->ui.cmbOpcodeIn->setEnabled(false);
            break;
        }
        case 1:
        {
            this->ui.lblBPRIn->setEnabled(false);
            this->ui.cmbBPRIn->setEnabled(false);
            this->ui.lblOpcodeIn->setEnabled(false);
            this->ui.cmbOpcodeIn->setEnabled(false);
            break;
        }
        case 2:
        {
            this->ui.lblBPRIn->setEnabled(false);
            this->ui.cmbBPRIn->setEnabled(false);
            this->ui.lblOpcodeIn->setEnabled(false);
            this->ui.cmbOpcodeIn->setEnabled(false);
            break;
        }
        case 3:
        {
            this->ui.lblBPRIn->setEnabled(true);
            this->ui.cmbBPRIn->setEnabled(true);
            this->ui.lblOpcodeIn->setEnabled(false);
            this->ui.cmbOpcodeIn->setEnabled(false);
            break;
        }
        case 4:
        {
            this->ui.lblBPRIn->setEnabled(false);
            this->ui.cmbBPRIn->setEnabled(false);
            this->ui.lblOpcodeIn->setEnabled(true);
            this->ui.cmbOpcodeIn->setEnabled(true);
            break;
        }
        case 5:
        {
            this->ui.lblBPRIn->setEnabled(false);
            this->ui.cmbBPRIn->setEnabled(false);
            this->ui.lblOpcodeIn->setEnabled(true);
            this->ui.cmbOpcodeIn->setEnabled(true);
            break;
        }
        default:
        {
            qDebug() << "FileConvertDlg: in type error occured";
            break;
        }
    }
}


/**
 * @brief
 */
void FileConvertDlg::setOptOut(int index)
{
    switch (index)
    {
        case 0:
        {
            this->ui.lblBPROut->setEnabled(false);
            this->ui.cmbBPROut->setEnabled(false);
            this->ui.lblOpcodeOut->setEnabled(false);
            this->ui.cmbOpcodeOut->setEnabled(false);
            this->ui.lblTemplateName->setEnabled(false);
            this->ui.leTemplateName->setEnabled(false);
            this->ui.lblTemplate->setEnabled(false);
            this->ui.leTemplate->setEnabled(false);
            this->ui.btnTemplate->setEnabled(false);
            break;
        }
        case 1:
        {
            this->ui.lblBPROut->setEnabled(false);
            this->ui.cmbBPROut->setEnabled(false);
            this->ui.lblOpcodeOut->setEnabled(false);
            this->ui.cmbOpcodeOut->setEnabled(false);
            this->ui.lblTemplateName->setEnabled(false);
            this->ui.leTemplateName->setEnabled(false);
            this->ui.lblTemplate->setEnabled(false);
            this->ui.leTemplate->setEnabled(false);
            this->ui.btnTemplate->setEnabled(false);
            break;
        }
        case 2:
        {
            this->ui.lblBPROut->setEnabled(false);
            this->ui.cmbBPROut->setEnabled(false);
            this->ui.lblOpcodeOut->setEnabled(false);
            this->ui.cmbOpcodeOut->setEnabled(false);
            this->ui.lblTemplateName->setEnabled(false);
            this->ui.leTemplateName->setEnabled(false);
            this->ui.lblTemplate->setEnabled(false);
            this->ui.leTemplate->setEnabled(false);
            this->ui.btnTemplate->setEnabled(false);
            break;
        }
        case 3:
        {
            this->ui.lblBPROut->setEnabled(true);
            this->ui.cmbBPROut->setEnabled(true);
            this->ui.lblOpcodeOut->setEnabled(false);
            this->ui.cmbOpcodeOut->setEnabled(false);
            this->ui.lblTemplateName->setEnabled(false);
            this->ui.leTemplateName->setEnabled(false);
            this->ui.lblTemplate->setEnabled(false);
            this->ui.leTemplate->setEnabled(false);
            this->ui.btnTemplate->setEnabled(false);
            break;
        }
        case 4:
        {
            this->ui.lblBPROut->setEnabled(false);
            this->ui.cmbBPROut->setEnabled(false);
            this->ui.lblOpcodeOut->setEnabled(true);
            this->ui.cmbOpcodeOut->setEnabled(true);
            this->ui.lblTemplateName->setEnabled(true);
            this->ui.leTemplateName->setEnabled(true);
            this->ui.lblTemplate->setEnabled(true);
            this->ui.leTemplate->setEnabled(true);
            this->ui.btnTemplate->setEnabled(true);
            break;
        }
        case 5:
        {
            this->ui.lblBPROut->setEnabled(false);
            this->ui.cmbBPROut->setEnabled(false);
            this->ui.lblOpcodeOut->setEnabled(true);
            this->ui.cmbOpcodeOut->setEnabled(true);
            this->ui.lblTemplateName->setEnabled(true);
            this->ui.leTemplateName->setEnabled(true);
            this->ui.lblTemplate->setEnabled(true);
            this->ui.leTemplate->setEnabled(true);
            this->ui.btnTemplate->setEnabled(true);
            break;
        }
        default:
        {
            qDebug() << "FileConvertDlg: out type error occured";
            break;
        }
    }
}
