// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup AdjSimProcDef
 * @file AdjSimProcDefGui.cxx
 */
// =============================================================================

#include "AdjSimProcDefGui.h"

#include "AdjSimProcDef.h"
#include "AdjSimProcDefParser.h"
#include "AdjSimProcDefGenerator.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>

#include <QDebug>

AdjSimProcDefGui::AdjSimProcDefGui ( QWidget * parent )
                                   : QWidget(parent)
{
    m_modified = false;

    setupUi(this);
    setupConnections();
}

const QString & AdjSimProcDefGui::getFileName() const
{
    return m_fileName;
}

inline void AdjSimProcDefGui::setupConnections()
{
    // Processor Name & Description
    connect ( lineEditProcessorName,    SIGNAL(textEdited(const QString &)), SLOT(setModified(const QString &)) );
    connect ( textEditDescription,      SIGNAL(textChanged()),               SLOT(setModified()) );
    // Program Memory
    connect ( spinBoxProgramSize,       SIGNAL(valueChanged(int)),           SLOT(setModified(int)) );
    connect ( spinBoxProgramWordSize,   SIGNAL(valueChanged(int)),           SLOT(setModified(int)) );
    connect ( comboBoxProgramEndian,    SIGNAL(currentIndexChanged(int)),    SLOT(setModified(int)) );
    // Data Memory
    connect ( checkBoxDataMemory,       SIGNAL(stateChanged(int)),           SLOT(disenaDataMem(int)) );
    connect ( checkBoxDataMemory,       SIGNAL(stateChanged(int)),           SLOT(setModified(int)) );
    connect ( spinBoxDataMemorySize,    SIGNAL(valueChanged(int)),           SLOT(setModified(int)) );
    // Register File
    connect ( checkBoxRegisterFile,     SIGNAL(stateChanged(int)),           SLOT(disenaRegFile(int)) );
    connect ( checkBoxRegisterFile,     SIGNAL(stateChanged(int)),           SLOT(setModified(int)) );
    connect ( spinBoxRegisterFileSize,  SIGNAL(valueChanged(int)),           SLOT(setModified(int)) );
    connect ( spinBoxRegisterFileBanks, SIGNAL(valueChanged(int)),           SLOT(setModified(int)) );
    // Stack
    connect ( radioButtonStackMode0,    SIGNAL(toggled(bool)),               SLOT(setModified(bool)) );
    connect ( radioButtonStackMode1,    SIGNAL(toggled(bool)),               SLOT(setModified(bool)) );
    connect ( radioButtonStackMode2,    SIGNAL(toggled(bool)),               SLOT(setModified(bool)) );
    connect ( spinBoxStackDSize,        SIGNAL(valueChanged(int)),           SLOT(setModified(int)) );
}

void AdjSimProcDefGui::openFile ( const QString & fileName )
{
    static const long long int MAX_SIZE = 102400;
    char data [ MAX_SIZE ];

    QFile file(fileName, this);
    if ( ( false == file.open(QIODevice::ReadOnly) ) || ( -1LL == file.read(data, MAX_SIZE) ) )
    {
        QMessageBox::critical ( this,
                                tr("Read error"),
                                tr("Unable to read the specified file."),
                                QMessageBox::Ok,
                                QMessageBox::Ok );
        return;
    }

    AdjSimProcDefParser parser(data);

    if ( false == parser.isValid() )
    {
        QMessageBox::critical ( this,
                                tr("Data error"),
                                tr("The specified file is not compatible with this program."),
                                QMessageBox::Ok,
                                QMessageBox::Ok );
        return;
    }

    setFileName(fileName);

    const AdjSimProcDef & procDef = parser.data();

    lineEditProcessorName->setText(QString::fromStdString(procDef.m_name));
    textEditDescription->setPlainText(QString::fromStdString(procDef.m_description));
}

void AdjSimProcDefGui::saveFile ( const QString & fileName )
{
    AdjSimProcDef procDef;

    // Processor Name & Description
    procDef.m_name = lineEditProcessorName->text().toStdString();
    procDef.m_description = textEditDescription->toPlainText().toStdString();

    // Program memory
    procDef.m_memory.m_program.m_size = (unsigned int) spinBoxProgramSize->value();
    int wordSize = spinBoxProgramWordSize->value();
    if ( wordSize <= 8 )
    {
        procDef.m_memory.m_program.m_word = AdjSimProcDef::Memory::Program::WORD_1B;
    }
    else if ( wordSize <= 16 )
    {
        procDef.m_memory.m_program.m_word = AdjSimProcDef::Memory::Program::WORD_2B;
    }
    else
    {
        procDef.m_memory.m_program.m_word = AdjSimProcDef::Memory::Program::WORD_3B;
    }
    if ( 0 == comboBoxProgramEndian->currentIndex() )
    {
        procDef.m_memory.m_program.m_endian = AdjSimProcDef::Memory::Program::END_BIG;
    }
    else
    {
        procDef.m_memory.m_program.m_endian = AdjSimProcDef::Memory::Program::END_LITTLE;
    }

    // Data memory
    if ( Qt::Checked == checkBoxDataMemory->checkState() )
    {
        procDef.m_memory.m_data.m_size = (unsigned int) spinBoxDataMemorySize->value();
    }
    else
    {
        procDef.m_memory.m_data.m_size = 0;
    }

    // Register File
    if ( Qt::Checked == checkBoxRegisterFile->checkState() )
    {
        procDef.m_memory.m_register.m_size = (unsigned int) spinBoxRegisterFileSize->value();
        procDef.m_memory.m_register.m_banks = (unsigned int) spinBoxRegisterFileBanks->value();
    }
    else
    {
        procDef.m_memory.m_register.m_size = 0;
        procDef.m_memory.m_register.m_banks = 0;
    }

// /// Location where the stacked values are stored.
// struct Content
// {
// Space m_space; ///< Memory space.
// int m_offset;  ///< This value is added to the stack pointer value before accessing stack memory.
// } m_content;
//
// /// Location designated for the stack pointer.
// struct Pointer
// {
// Space m_space;   ///< Memory space.
// int m_address;   ///< Address in the designated memory space.
// int m_maxSize; ///< Maximum allowed stack pointer value, -1 means no limit.
// bool m_indirect; ///< If true, "m_address" is threated as indirect address.
// } m_pointer;

    if ( true == radioButtonStackMode0->isChecked() )
    {
        procDef.m_stack.m_size = 0;
        procDef.m_stack.m_useDesignatedStack = true;
    }
    else if ( true == radioButtonStackMode1->isChecked() )
    {
        procDef.m_stack.m_size = (unsigned int) spinBoxStackDSize->value();
        procDef.m_stack.m_useDesignatedStack = true;
    }
    else
    {
        procDef.m_stack.m_size = 0;
        procDef.m_stack.m_useDesignatedStack = false;

//         procDef.m_stack.m_simpleStack.m_operation = AdjSimProcDef::Stack::SimpleStack::OP_PREINC
//         procDef.m_stack.m_simpleStack.m_operation = AdjSimProcDef::Stack::SimpleStack::OP_POSTINC
//         procDef.m_stack.m_simpleStack.m_operation = AdjSimProcDef::Stack::SimpleStack::OP_PREDEC
//         procDef.m_stack.m_simpleStack.m_operation = AdjSimProcDef::Stack::SimpleStack::OP_POSTDEC
//
//         procDef.m_stack.m_simpleStack.m_content.m_space = AdjSimProcDef::Stack::SimpleStack::SP_REG
//         procDef.m_stack.m_simpleStack.m_content.m_space = AdjSimProcDef::Stack::SimpleStack::SP_DATA
//         procDef.m_stack.m_simpleStack.m_content.m_offset
//         procDef.m_stack.m_simpleStack.m_pointer.m_space
//         procDef.m_stack.m_simpleStack.m_pointer.m_address
//         procDef.m_stack.m_simpleStack.m_pointer.m_maxSize
//         procDef.m_stack.m_simpleStack.m_pointer.m_indirect
    }



    QFile file(fileName, this);
    AdjSimProcDefGenerator generator(procDef);
    if ( ( false == file.open(QIODevice::WriteOnly) ) || ( -1LL == file.write(generator.data().c_str()) ) )
    {
        QMessageBox::critical ( this,
                                tr("Write error"),
                                tr("Unable to write to the specified file."),
                                QMessageBox::Ok,
                                QMessageBox::Ok );
        return;
    }

    setFileName(fileName);
}

void AdjSimProcDefGui::setFileName ( const QString & fileName )
{
    m_fileName = fileName;
    if ( true == m_fileName.isEmpty() )
    {
        labelFileName->setText(tr("<untitled>"));
    }
    else
    {
        labelFileName->setText(m_fileName);
    }

    clearModified();
}

void AdjSimProcDefGui::clearModified()
{
    if ( true == m_modified )
    {
        setWindowTitle(windowTitle().remove(0, tr("[modified] ").size()));
    }
    m_modified = false;
}

void AdjSimProcDefGui::setModified ( int )
{
    setModified();
}

void AdjSimProcDefGui::setModified ( bool )
{
    setModified();
}

void AdjSimProcDefGui::setModified ( const QString & )
{
    setModified();
}

void AdjSimProcDefGui::setModified()
{
    if ( false == m_modified )
    {
        setWindowTitle(tr("[modified] ") + windowTitle());
    }
    m_modified = true;
}

void AdjSimProcDefGui::disenaRegFile ( int checkState )
{
    spinBoxRegisterFileSize->setEnabled ( Qt::Checked == checkState );
    spinBoxRegisterFileBanks->setEnabled ( Qt::Checked == checkState );
}

void AdjSimProcDefGui::disenaDataMem ( int checkState )
{
    spinBoxDataMemorySize->setEnabled ( Qt::Checked == checkState );
}

void AdjSimProcDefGui::on_pushButtonOpenFile_clicked()
{
    if ( true == m_modified )
    {
        if ( QMessageBox::Yes ==
             QMessageBox::question ( this,
                                     tr("Save file"),
                                     tr("Current processor definition has been modified, do you want to save it?"),
                                     QMessageBox::Yes | QMessageBox::No,
                                     QMessageBox::Yes )
           )
        {
            if ( false == on_pushButtonSaveAs_clicked() )
            {
                return;
            }
        }
    }

    QString directory;
    if ( true == m_fileName.isEmpty() )
    {
        directory = QDir::homePath();
    }
    else
    {
        directory = m_fileName;
    }

    QFileDialog fileDialog ( this,
                             tr("Select processor definition file"),
                             directory,
                             tr("MDS processor definition files (*.procdef);;All files (*)") );

    if ( QDialog::Accepted == fileDialog.exec() )
    {
        openFile(fileDialog.selectedFiles()[0]);
    }
}

bool AdjSimProcDefGui::on_pushButtonSaveAs_clicked()
{
    QString directory;
    if ( true == m_fileName.isEmpty() )
    {
        directory = QDir::homePath() + QDir::separator() + tr("untitled");
    }
    else
    {
        directory = m_fileName;
    }

    QFileDialog fileDialog ( this,
                             tr("Save processor definition file"),
                             directory,
                             tr("MDS processor definition files (*.procdef);;All files (*)") );

    fileDialog.setAcceptMode(QFileDialog::AcceptSave);

    if ( QDialog::Accepted == fileDialog.exec() )
    {
        saveFile(fileDialog.selectedFiles()[0]);
        return true;
    }

    return false;
}
