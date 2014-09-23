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
    m_idCounter = 0;
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
    connect ( lineEditProcessorName,    SIGNAL(textEdited(const QString &)), SLOT(setModified()) );
    connect ( textEditDescription,      SIGNAL(textChanged()),               SLOT(setModified()) );
    // Program Memory
    connect ( spinBoxProgramSize,       SIGNAL(valueChanged(int)),           SLOT(setModified()) );
    connect ( spinBoxProgramWordSize,   SIGNAL(valueChanged(int)),           SLOT(setModified()) );
    connect ( comboBoxProgramEndian,    SIGNAL(currentIndexChanged(int)),    SLOT(setModified()) );
    // Data Memory
    connect ( checkBoxDataMemory,       SIGNAL(stateChanged(int)),           SLOT(disenaDataMem(int)) );
    connect ( checkBoxDataMemory,       SIGNAL(stateChanged(int)),           SLOT(setModified()) );
    connect ( spinBoxDataMemorySize,    SIGNAL(valueChanged(int)),           SLOT(setModified()) );
    // Register File
    connect ( checkBoxRegisterFile,     SIGNAL(stateChanged(int)),           SLOT(disenaRegFile(int)) );
    connect ( checkBoxRegisterFile,     SIGNAL(stateChanged(int)),           SLOT(setModified()) );
    connect ( spinBoxRegisterFileSize,  SIGNAL(valueChanged(int)),           SLOT(setModified()) );
    connect ( spinBoxRegisterFileBanks, SIGNAL(valueChanged(int)),           SLOT(setModified()) );
    // Stack
    connect ( radioButtonStackMode0,    SIGNAL(toggled(bool)),               SLOT(disenaStack(bool)) );
    connect ( radioButtonStackMode1,    SIGNAL(toggled(bool)),               SLOT(disenaStack(bool)) );
    connect ( radioButtonStackMode2,    SIGNAL(toggled(bool)),               SLOT(disenaStack(bool)) );
    connect ( radioButtonStackMode0,    SIGNAL(toggled(bool)),               SLOT(setModified()) );
    connect ( radioButtonStackMode1,    SIGNAL(toggled(bool)),               SLOT(setModified()) );
    connect ( radioButtonStackMode2,    SIGNAL(toggled(bool)),               SLOT(setModified()) );
    connect ( spinBoxStackDSize,        SIGNAL(valueChanged(int)),           SLOT(setModified()) );
    // Available Flags
    connect ( checkBoxHasFlagZero,      SIGNAL(stateChanged(int)),           SLOT(setModified()) );
    connect ( checkBoxHasFlagCarry,     SIGNAL(stateChanged(int)),           SLOT(setModified()) );
    connect ( checkBoxHasFlagOverflow,  SIGNAL(stateChanged(int)),           SLOT(setModified()) );
    connect ( checkBoxHasFlagNegative,  SIGNAL(stateChanged(int)),           SLOT(setModified()) );
    connect ( checkBoxHasFlagHalfCarry, SIGNAL(stateChanged(int)),           SLOT(setModified()) );
    connect ( checkBoxHasFlagParity,    SIGNAL(stateChanged(int)),           SLOT(setModified()) );
    // Ports
    connect ( checkBoxPortsSupported,   SIGNAL(stateChanged(int)),           SLOT(disenaPorts(int)) );
    connect ( checkBoxPortsSupported,   SIGNAL(stateChanged(int)),           SLOT(setModified()) );
    connect ( spinBoxPortsAddressWidth, SIGNAL(valueChanged(int)),           SLOT(setModified()) );
    connect ( spinBoxPortsDataWidth,    SIGNAL(valueChanged(int)),           SLOT(setModified()) );
    // Interrupts
    connect ( checkBoxHasInterrupts,    SIGNAL(stateChanged(int)),           SLOT(disenaInterrupts(int)) );
    connect ( checkBoxHasInterrupts,    SIGNAL(stateChanged(int)),           SLOT(setModified()) );
    connect ( spinBoxInterruptVector,   SIGNAL(valueChanged(int)),           SLOT(setModified()) );
    connect ( checkBoxInterruptBackupFlags, SIGNAL(stateChanged(int)),       SLOT(setModified()) );
    connect ( checkBoxInterruptAutoDisable, SIGNAL(stateChanged(int)),       SLOT(setModified()) );
    connect ( checkBoxInterruptAutoEnable,  SIGNAL(stateChanged(int)),       SLOT(setModified()) );
    // Instruction Set
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

    m_idCounter = 0;
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

    // Stack
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

        switch ( comboBoxStackOperation->currentIndex() )
        {
            case 0:
                procDef.m_stack.m_simpleStack.m_operation = AdjSimProcDef::Stack::SimpleStack::OP_PREINC;
                break;
            case 1:
                procDef.m_stack.m_simpleStack.m_operation = AdjSimProcDef::Stack::SimpleStack::OP_POSTINC;
                break;
            case 2:
                procDef.m_stack.m_simpleStack.m_operation = AdjSimProcDef::Stack::SimpleStack::OP_PREDEC;
                break;
            case 3:
                procDef.m_stack.m_simpleStack.m_operation = AdjSimProcDef::Stack::SimpleStack::OP_POSTDEC;
                break;
        }

        procDef.m_stack.m_simpleStack.m_content.m_offset = (unsigned int) spinBoxStackContentOffset->value();

        switch ( comboBoxStackContentSpace->currentIndex() )
        {
            case 0:
                procDef.m_stack.m_simpleStack.m_content.m_space = AdjSimProcDef::Stack::SimpleStack::SP_REG;
                break;
            case 1:
                procDef.m_stack.m_simpleStack.m_content.m_space = AdjSimProcDef::Stack::SimpleStack::SP_DATA;
                break;
        }

        switch ( comboBoxStackPointerSpace->currentIndex() )
        {
            case 0:
                procDef.m_stack.m_simpleStack.m_pointer.m_space = AdjSimProcDef::Stack::SimpleStack::SP_REG;
                break;
            case 1:
                procDef.m_stack.m_simpleStack.m_pointer.m_space = AdjSimProcDef::Stack::SimpleStack::SP_DATA;
                break;
        }

        procDef.m_stack.m_simpleStack.m_pointer.m_address = (unsigned int) spinBoxStackPointerAddress->value();
        procDef.m_stack.m_simpleStack.m_pointer.m_maxSize = (unsigned int) spinBoxStackPointerMax->value();

        procDef.m_stack.m_simpleStack.m_pointer.m_indirect = ( Qt::Checked == checkBoxStackIndirectPtr->checkState() );
    }

    /*
     * Instruction set
     */




    // Available flags
    procDef.m_flags.m_hasZero      = ( Qt::Checked == checkBoxHasFlagZero       -> checkState() );
    procDef.m_flags.m_hasCarry     = ( Qt::Checked == checkBoxHasFlagCarry      -> checkState() );
    procDef.m_flags.m_hasOverflow  = ( Qt::Checked == checkBoxHasFlagOverflow   -> checkState() );
    procDef.m_flags.m_hasNegative  = ( Qt::Checked == checkBoxHasFlagNegative   -> checkState() );
    procDef.m_flags.m_hasHalfCarry = ( Qt::Checked == checkBoxHasFlagHalfCarry  -> checkState() );
    procDef.m_flags.m_hasParity    = ( Qt::Checked == checkBoxHasFlagParity     -> checkState() );
    // Ports
    if ( Qt::Checked == checkBoxPortsSupported->checkState() )
    {
        procDef.m_ports.m_addrWidth = (unsigned int) spinBoxPortsAddressWidth->value();
        procDef.m_ports.m_dataWidth = (unsigned int) spinBoxPortsDataWidth->value();
    }
    else
    {
        procDef.m_ports.m_addrWidth = 0;
        procDef.m_ports.m_dataWidth = 0;
    }
    // Interrupts
    procDef.m_hasInterrupts = ( Qt::Checked == checkBoxHasInterrupts->checkState() );
    procDef.m_interruptVector = (unsigned int) spinBoxInterruptVector->value();
    procDef.m_flags.m_backupWhenInterrupted = ( Qt::Checked == checkBoxInterruptBackupFlags->checkState() );
    procDef.m_flags.m_autoDisableInterrupts = ( Qt::Checked == checkBoxInterruptAutoDisable->checkState() );
    procDef.m_flags.m_autoEnableInterrupts =  ( Qt::Checked == checkBoxInterruptAutoEnable->checkState() );

    // Save the generated processor definition into a file.
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

void AdjSimProcDefGui::disenaStack ( bool )
{
    bool enableDesignated = radioButtonStackMode1->isChecked();
    bool enableSimple = radioButtonStackMode2->isChecked();

    spinBoxStackDSize->setEnabled(enableDesignated);

    comboBoxStackOperation->setEnabled(enableSimple);
    spinBoxStackContentOffset->setEnabled(enableSimple);
    comboBoxStackContentSpace->setEnabled(enableSimple);
    comboBoxStackPointerSpace->setEnabled(enableSimple);
    spinBoxStackPointerAddress->setEnabled(enableSimple);
    spinBoxStackPointerMax->setEnabled(enableSimple);
    checkBoxStackIndirectPtr->setEnabled(enableSimple);
}

void AdjSimProcDefGui::disenaPorts ( int checkState )
{
    spinBoxPortsAddressWidth->setEnabled ( Qt::Checked == checkState );
    spinBoxPortsDataWidth->setEnabled ( Qt::Checked == checkState );
}

void AdjSimProcDefGui::disenaInterrupts ( int checkState )
{
    spinBoxInterruptVector->setEnabled ( Qt::Checked == checkState );
    checkBoxInterruptBackupFlags->setEnabled ( Qt::Checked == checkState );
    checkBoxInterruptAutoDisable->setEnabled ( Qt::Checked == checkState );
    checkBoxInterruptAutoEnable->setEnabled ( Qt::Checked == checkState );
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

void AdjSimProcDefGui::on_pushButtonNewInstruction_clicked()
{
    setModified();

    QTreeWidgetItem * item = new QTreeWidgetItem ( treeWidgetInstructions,
                                                   QStringList ( {QString("%1").arg(m_idCounter++),"--","--"} ) );
    treeWidgetInstructions->setCurrentItem(item);

    m_instructions.insert(std::make_pair(item, new AdjSimProcDef::Instruction()));
}

void AdjSimProcDefGui::on_pushButtonRemoveInstruction_clicked()
{
    setModified();

    QTreeWidgetItem * item = treeWidgetInstructions->selectedItems().front();
    auto iter = m_instructions.find(item);
    if ( m_instructions.cend() != iter )
    {
        delete iter->second;
        m_instructions.erase(iter);
    }
    delete item;
}

void AdjSimProcDefGui::on_treeWidgetInstructions_itemSelectionChanged()
{
    bool instructionSelected = ( false == treeWidgetInstructions->selectedItems().isEmpty() );

    pushButtonRemoveInstruction->setEnabled(instructionSelected);
    toolBoxInstructions->setEnabled(instructionSelected);

/*
    lineEditInstMnemonic
    comboBoxInstOperation

    comboBoxInstFlagZero
    comboBoxInstFlagCarry
    comboBoxInstFlagOverflow
    comboBoxInstFlagNegative
    comboBoxInstFlagHalfCarry
    comboBoxInstFlagIE
    comboBoxInstFlagParity
    comboBoxInstFlagFlag
    comboBoxInstParity
    comboBoxInstResult
    checkBoxIgnoreCarry
    comboBoxInstConditionType
    comboBoxInstConditionFlag
    spinBoxInstTimePositive
    spinBoxInstTimeNegative
    lineEditInstNextPositive
    lineEditInstNextNegative
*/
}
