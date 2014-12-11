/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup DisAsmDialog
 * @file disasmdlg.cpp
 */


#include <QtGui>
#include "disasmdlg.h"
#include "../../../utilities/MCUDataFiles/HexFile.h"
#include "../../../utilities/MCUDataFiles/BinFile.h"
#include "../../../utilities/MCUDataFiles/SrecFile.h"
#include "../../../utilities/MCUDataFiles/XilMemFile.h"
#include "../../../utilities/MCUDataFiles/XilVerilogFile.h"
#include "../../../utilities/MCUDataFiles/XilVHDLFile.h"
#include "../../../disassembler/PicoBlaze/DAsmPicoBlazeKcpsm6.h"
#include "../../../disassembler/PicoBlaze/DAsmPicoBlazeKcpsm3.h"
#include "../../../disassembler/PicoBlaze/DAsmPicoBlazeKcpsm2.h"
#include "../../../disassembler/PicoBlaze/DAsmPicoBlazeKcpsm1.h"
#include "../../../disassembler/PicoBlaze/DAsmPicoBlazeKcpsm1CPLD.h"
#include "../../widgets/Editor/codeedit.h"
#include "../../errordialog/errordlg.h"


/**
 * @brief
 * @param
 * @param
 */
DisAsmDialog::DisAsmDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    this->show();

    connect(this->ui.buttonBox, SIGNAL(accepted()), this, SLOT(create()));
    connect(this->ui.buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(this->ui.btnChange, SIGNAL(clicked()), this, SLOT(setPath()));
}


/**
 * @brief
 */
void DisAsmDialog::setPath()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("(*.hex *.bin *.srec *.mem *.v *.vhd)"));
    if (path != NULL)
    {
        this->ui.lePath->setText(path);
    }
}


/**
 * @brief
 */
void DisAsmDialog::create()
{
    QFile file(this->ui.lePath->text());
    if (false == file.exists())
    {
        error(ERR_OPENFILE);
        return;
    }
    DAsm * disasm = NULL;
    DataFile * dataFile = NULL;
    DAsm::Config config;

    QString type = this->ui.lePath->text().section('.', -1);
    std::string infile = this->ui.lePath->text().toStdString();
    //std::string outfile;
    std::string architecture = this->ui.cbxArchitecture->currentText().toStdString();
    std::string family = this->ui.cbxFamily->currentText().toStdString();

    int memFileBPR = -1;
    XilHDLFile::OPCodeSize opCodeSize = XilHDLFile::OPCodeSize(-1);

    if ( "PicoBlaze" == architecture )
    {
        if ( "kcpsm1cpld" == family )
        {
            disasm = new DAsmPicoBlazeKcpsm1CPLD();
            memFileBPR = 2;
            opCodeSize = XilHDLFile::SIZE_16b;
        }
        else if ( "kcpsm1" == family )
        {
            disasm = new DAsmPicoBlazeKcpsm1();
            memFileBPR = 2;
            opCodeSize = XilHDLFile::SIZE_16b;
        }
        else if ( "kcpsm2" == family )
        {
            disasm = new DAsmPicoBlazeKcpsm2();
            memFileBPR = 3;
            opCodeSize = XilHDLFile::SIZE_18b;
        }
        else if ( "kcpsm3" == family )
        {
            
            disasm = new DAsmPicoBlazeKcpsm3();
            memFileBPR = 3;
            opCodeSize = XilHDLFile::SIZE_18b;
        }
        else if ( "kcpsm6" == family )
        {
            disasm = new DAsmPicoBlazeKcpsm6();
            memFileBPR = 3;
            opCodeSize = XilHDLFile::SIZE_18b;
        }
        else
        {
            accept();
            return;
        }
    }

    if (type == "hex")
    {
        dataFile = new HexFile(infile);
    }
    else if(type == "bin")
    {
        dataFile = new BinFile(infile);

    }
    else if (type == "srec")
    {
        dataFile = new SrecFile(infile);
    }
    else if (type == "mem")
    {
        dataFile = new XilMemFile(infile, memFileBPR);
    }
    else if (type == "v")
    {
        dataFile = new XilVerilogFile(infile, "", "", opCodeSize);
    }
    else if (type == "vhd")
    {
        dataFile = new XilVHDLFile(infile, "", "", opCodeSize);
    }
    else
    {
        accept();
        return;
    }

    if (this->ui.cbxIntend->currentText() == "Tabs")
    {
        config.m_indentation = DAsm::Config::IND_TABS;
    }
    else
    {
        config.m_indentation = DAsm::Config::IND_SPACES;
    }

    if ( this->ui.cbxCase->currentText() == "LF" )
    {
        config.m_eol = DAsm::Config::EOL_LF;
    }
    else if ( this->ui.cbxCase->currentText() == "CR" )
    {
        config.m_eol = DAsm::Config::EOL_CR;
    }
    else
    {
        config.m_eol = DAsm::Config::EOL_CRLF;
    }

    config.m_symbolsToGenerate = DAsm::Config::STG_NONE;
    if (this->ui.chckCode->isChecked() == true)
    {
        config.m_symbolsToGenerate = DAsm::Config::SymbolsToGenerate(config.m_symbolsToGenerate | DAsm::Config::STG_CODE);
    }
    if (this->ui.chckData->isChecked() == true)
    {
        config.m_symbolsToGenerate = DAsm::Config::SymbolsToGenerate(config.m_symbolsToGenerate | DAsm::Config::STG_DATA);
    }
    if (this->ui.chckReg->isChecked() == true)
    {
        config.m_symbolsToGenerate = DAsm::Config::SymbolsToGenerate(config.m_symbolsToGenerate | DAsm::Config::STG_REG);
    }
    if (this->ui.chckPort->isChecked() == true)
    {
        config.m_symbolsToGenerate = DAsm::Config::SymbolsToGenerate(config.m_symbolsToGenerate | DAsm::Config::STG_PORT);
    }
    if (this->ui.chckConst->isChecked() == true)
    {
        config.m_symbolsToGenerate =DAsm::Config::SymbolsToGenerate(config.m_symbolsToGenerate | DAsm::Config::STG_CONST);
    }

    if (this->ui.cbxCase->currentText() == "Lowercase")
    {
        config.m_letterCase = DAsm::Config::LC_LOWERCASE;
    }
    else
    {
        config.m_letterCase = DAsm::Config::LC_UPPERCASE;
    }

    if (this->ui.cbxRadix->currentText() == "Binary")
    {
        config.m_radix = DAsm::Config::R_BIN;
    }
    else if (this->ui.cbxRadix->currentText() == "Octal")
    {
        config.m_radix = DAsm::Config::R_OCT;
    }
    else if (this->ui.cbxRadix->currentText() == "Decimal")
    {
        config.m_radix = DAsm::Config::R_DEC;
    }
    else
    {
        config.m_radix = DAsm::Config::R_HEX;
    }

    bool convertResult;
    config.m_tabSize = this->ui.leTabSize->text().toInt(&convertResult);
    if (convertResult == false)
    {
        config.m_tabSize = 4;
    }
    
    disasm->m_config = config;
    
    bool finalResult = disasm->disassemble(*dataFile);

    if ( true == finalResult )
    {
        //CodeEdit *editor = new CodeEdit(0, false, NULL, NULL, NULL);
        //editor->show();
        //std::vector<std::string> source = disasm->getSourceCode();
        emit output(disasm->getSourceCode());
        /*for (unsigned int i = 0; i < source.size(); i++)
        {
            qDebug() << "source:" << QString::fromStdString(source.at(i));
            editor->getTextEdit()->append(QString::fromStdString(source.at(i)));
        }*/
    }
    else
    {
        //CodeEdit *editor = new CodeEdit(0, false, NULL, NULL, NULL);
        //editor->show();
        //std::vector<std::string> messages = disasm->getMessages();
        emit output(disasm->getMessages());
        /*for (unsigned int i = 0; i < messages.size(); i++)
        {
            qDebug() << "message:" << QString::fromStdString(messages.at(i));
            editor->getTextEdit()->append(QString::fromStdString(messages.at(i)));
        }*/
    }
    
    accept();
}
