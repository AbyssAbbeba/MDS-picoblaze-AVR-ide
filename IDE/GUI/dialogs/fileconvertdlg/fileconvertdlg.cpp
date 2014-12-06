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

    setOptIn(this->ui.cmbTypeIn->currentIndex());
    setOptOut(this->ui.cmbTypeOut->currentIndex());
}

void FileConvertDlg::setPathIn()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), "", "(*.hex *.ihex *.bin *.srec *.mem *.v *.vhd *.rawhex)");
    if (path != NULL)
    {
        this->ui.lePathIn->setText(path);
    }
}

void FileConvertDlg::setPathOut()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), "", "(*.hex *.ihex *.bin *.srec *.mem *.v *.vhd *.rawhex)");
    if (path != NULL)
    {
        this->ui.lePathOut->setText(path);
    }
}

void FileConvertDlg::setPathTemplate()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), "", "(*.v *.vhd)");
    if (path != NULL)
    {
        this->ui.leTemplate->setText(path);
    }
}

void FileConvertDlg::create()
{
    DataFile * input;
    DataFile * output;

    try
    {
        switch (this->ui.cmbTypeIn->currentIndex())
        {
            case 0: // HEX
                input = new RawHexDumpFile((RawHexDumpFile::OPCodeSize)(this->ui.cmbOpcodeIn->currentText().toInt()));
                break;
            case 1: // MEM
                input = new XilMemFile(this->ui.cmbBPRIn->currentText().toInt());
                break;
            case 2: // VHDL
                input = new XilVHDLFile("", "", (XilHDLFile::OPCodeSize)(this->ui.cmbOpcodeIn->currentText().toInt()));
                break;
            case 3: // Verilog
                input = new XilVerilogFile("", "", (XilHDLFile::OPCodeSize)(this->ui.cmbOpcodeIn->currentText().toInt()));
                break;
            case 4: // Intel 8 HEX
                input = new HexFile();
                break;
            case 5: // S-Rec
                input = new SrecFile();
                break;
            case 6: // Raw binary
                input = new BinFile();
                break;
            default:
                return;
        }

        switch (this->ui.cmbTypeOut->currentIndex())
        {
            case 0: // HEX
                input = new RawHexDumpFile ( (RawHexDumpFile::OPCodeSize)(this->ui.cmbOpcodeOut->currentText().toInt()),
                                             ui.spinBoxOutputOverallSize->value() * ( (this->ui.cmbOpcodeOut->currentText().toInt()) == 16 ? 2 : 3 ) );
                break;
            case 1: // MEM
                output = new XilMemFile(this->ui.cmbBPROut->currentText().toInt());
                break;
            case 2: // VHDL
                output = new XilVHDLFile ( this->ui.leTemplateName->text().toStdString(),
                                           this->ui.leTemplate->text().toStdString(),
                                           (XilHDLFile::OPCodeSize)(this->ui.cmbOpcodeOut->currentText().toInt()) );
                break;
            case 3: // Verilog
                output = new XilVerilogFile ( this->ui.leTemplateName->text().toStdString(),
                                              this->ui.leTemplate->text().toStdString(),
                                              (XilHDLFile::OPCodeSize)(this->ui.cmbOpcodeOut->currentText().toInt()) );
                break;
            case 4: // Intel 8 HEX
                output = new HexFile();
                break;
            case 5: // S-Rec
                output = new SrecFile();
                break;
            case 6: // Raw binary
                output = new BinFile();
                break;
            default:
                return;
        }

        input->clearAndLoad(this->ui.lePathIn->text().toStdString());
        output->setData(input->getData(), input->maxSize());
        output->save(this->ui.lePathOut->text().toStdString());

    }
    catch ( const DataFileException & e )
    {
        // TODO: display some error message.
    }

    delete input;
    delete output;

    accept();
}

void FileConvertDlg::setOptIn(int index)
{
    ui.lblBPRIn->setEnabled(false);
    ui.cmbBPRIn->setEnabled(false);
    ui.lblOpcodeIn->setEnabled(false);
    ui.cmbOpcodeIn->setEnabled(false);

    switch (index)
    {
        case 0: // HEX
            break;

        case 1: // MEM
            ui.lblBPRIn->setEnabled(true);
            ui.cmbBPRIn->setEnabled(true);
            break;

        case 2: // VHDL
        case 3: // Verilog
            ui.lblOpcodeIn->setEnabled(true);
            ui.cmbOpcodeIn->setEnabled(true);
            break;

        case 4: // Intel 8 HEX
        case 5: // S-Rec
        case 6: // Raw binary
            break;

        default:
            qDebug() << "FileConvertDlg: in type error occured";
            break;
    }
}

void FileConvertDlg::setOptOut(int index)
{
    ui.lblBPROut->setEnabled(false);
    ui.cmbBPROut->setEnabled(false);
    ui.lblOpcodeOut->setEnabled(false);
    ui.cmbOpcodeOut->setEnabled(false);
    ui.lblTemplateName->setEnabled(false);
    ui.leTemplateName->setEnabled(false);
    ui.lblTemplate->setEnabled(false);
    ui.leTemplate->setEnabled(false);
    ui.btnTemplate->setEnabled(false);
    ui.spinBoxOutputOverallSize->setEnabled(false);
    ui.labelspinBoxOutputOverallSize->setEnabled(false);

    switch (index)
    {
        case 0: // HEX
            ui.lblBPROut->setEnabled(true);
            ui.cmbBPROut->setEnabled(true);
            ui.spinBoxOutputOverallSize->setEnabled(true);
            ui.labelspinBoxOutputOverallSize->setEnabled(true);
            break;

        case 1: // MEM
            ui.lblBPROut->setEnabled(true);
            ui.cmbBPROut->setEnabled(true);
            break;

        case 2: // VHDL
        case 3: // Verilog
            ui.lblOpcodeOut->setEnabled(true);
            ui.cmbOpcodeOut->setEnabled(true);
            ui.lblTemplateName->setEnabled(true);
            ui.leTemplateName->setEnabled(true);
            ui.lblTemplate->setEnabled(true);
            ui.leTemplate->setEnabled(true);
            ui.btnTemplate->setEnabled(true);
            break;

        case 4: // Intel 8 HEX
        case 5: // S-Rec
        case 6: // Raw binary
            break;

        default:

            qDebug() << "FileConvertDlg: out type error occured";
            break;
    }
}
