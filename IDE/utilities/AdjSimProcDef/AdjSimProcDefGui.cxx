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

// Standard header files.
#include <set>
#include <memory>

// Qt header files.
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QIntValidator>
#include <QTreeWidgetItem>
#include <QRegExpValidator>

// Macros
#define SHOW_HIDE_OP_CODE_BIT(BIT, SIZE)        \
    if ( SIZE < BIT )                           \
    {                                           \
        comboBoxInstOpCode ## BIT -> hide();    \
        labelInstOpCode ## BIT -> hide();       \
    }                                           \
    else                                        \
    {                                           \
        comboBoxInstOpCode ## BIT -> show();    \
        labelInstOpCode ## BIT -> show();       \
    }

// Initialize static constant attributes.
const AdjSimProcDef::Instruction AdjSimProcDefGui::m_emptyInstruction;

AdjSimProcDefGui::AdjSimProcDefGui ( QWidget * parent )
                                   : QWidget(parent)
{
    m_idCounter = 0;
    m_lock = false;

    setupUi(this);
    setupConnections();
    setupValidators();
}

AdjSimProcDefGui::~AdjSimProcDefGui()
{
    for ( auto i : m_instructions )
    {
        delete i.second;
    }
}

const QString & AdjSimProcDefGui::getFileName() const
{
    return m_fileName;
}

void AdjSimProcDefGui::openFile ( const QString & fileName )
{
    static const long long int MAX_SIZE = 102400;
    std::unique_ptr<char[]> data ( new char [ MAX_SIZE ] );
    ssize_t len;

    QFile file(fileName, this);
    if (
           ( false == file.open ( QIODevice::ReadOnly ) )
               ||
           ( -1LL == ( len = (ssize_t) ( file.read(data.get(), MAX_SIZE) ) ) )
       )
    {
        QMessageBox::critical ( this,
                                tr("Read error"),
                                tr("Unable to read the specified file."),
                                QMessageBox::Ok,
                                QMessageBox::Ok );
        return;
    }

    std::unique_ptr<AdjSimProcDefParser> parser ( new AdjSimProcDefParser ( std::string(data.get(), len) ) );

    if ( false == parser.get()->isValid() )
    {
        QMessageBox::critical ( this,
                                tr("Data error"),
                                tr("The specified file is not compatible with this program."),
                                QMessageBox::Ok,
                                QMessageBox::Ok );
        return;
    }

    // Clear instruction set
    for ( auto i : m_instructions )
    {
        delete i.second;
    }
    m_idCounter = 0;
    m_instructions.clear();
    treeWidgetInstructions->clear();

    const AdjSimProcDef & procDef = parser.get()->data();

    // Processor Name, Description, and Fail jump
    lineEditProcessorName->setText(QString::fromStdString(procDef.m_name));
    textEditDescription->setPlainText(QString::fromStdString(procDef.m_description));
    lineEditFailJump->setText(QString::fromStdString(procDef.m_failjmp));

    // Program memory
    spinBoxProgramSize->setValue(procDef.m_memory.m_program.m_size);
    spinBoxProgramWordSize->setValue(procDef.m_memory.m_program.m_wordSize);
    comboBoxProgramEndian->setCurrentIndex ( int(procDef.m_memory.m_program.m_endian) );

    // Data memory
    spinBoxDataMemorySize->setValue(procDef.m_memory.m_data.m_size);
    checkBoxDataMemory->setCheckState ( ( 0 == procDef.m_memory.m_data.m_size ) ? Qt::Unchecked : Qt::Checked );

    // Register File
    spinBoxRegisterFileSize->setValue(procDef.m_memory.m_register.m_size);
    spinBoxRegisterFileBanks->setValue(procDef.m_memory.m_register.m_banks);
    checkBoxRegisterFile->setCheckState ( ( 0 == procDef.m_memory.m_register.m_size ) ? Qt::Unchecked : Qt::Checked );

    // Stack
    if ( true == procDef.m_stack.m_useDesignatedStack )
    {
        if ( 0 == procDef.m_stack.m_size )
        {
            radioButtonStackMode0->setChecked(true);
        }
        else
        {
            radioButtonStackMode1->setChecked(true);
        }
        spinBoxStackDSize->setValue(procDef.m_stack.m_size);
    }
    else
    {
        radioButtonStackMode2->setChecked(true);
        spinBoxStackDSize->setValue(0);
        comboBoxStackOperation->setCurrentIndex(int(procDef.m_stack.m_simpleStack.m_operation));
        spinBoxStackContentOffset->setValue(procDef.m_stack.m_simpleStack.m_content.m_offset);
        comboBoxStackContentSpace->setCurrentIndex(int(procDef.m_stack.m_simpleStack.m_content.m_space));
        comboBoxStackPointerSpace->setCurrentIndex(int(procDef.m_stack.m_simpleStack.m_pointer.m_space));
        spinBoxStackPointerAddress->setValue(procDef.m_stack.m_simpleStack.m_pointer.m_address);
        spinBoxStackPointerMax->setValue(procDef.m_stack.m_simpleStack.m_pointer.m_maxSize);
        checkBoxStackIndirectPtr->setCheckState ( ( true == procDef.m_stack.m_simpleStack.m_pointer.m_indirect )
                                                   ? Qt::Checked : Qt::Unchecked );
    }

    // Instruction set
    for ( const AdjSimProcDef::Instruction & inst : procDef.m_instructionSet )
    {
        AdjSimProcDef::Instruction * newInst = new AdjSimProcDef::Instruction(inst);

        for ( unsigned int i = newInst->m_opCode.size(); i < 24; i++ )
        {
            newInst->m_opCode.push_back(AdjSimProcDef::Instruction::OCB_DONT_CARE);
        }
        newInst->m_operands.resize(3);
        for ( unsigned int i = 0; i < newInst->m_operands.size(); i++ )
        {
            newInst->m_operands[i].m_size = 16;
            for ( unsigned int j = newInst->m_operands[i].m_OPCodePermutation.size();
                  j < (unsigned int) (newInst->m_operands[i].m_size);
                  j++ )
            {
                newInst->m_operands[i].m_OPCodePermutation.push_back(-1);
            }
        }

        QString opCode = "0b";
        for ( int i = ( newInst->m_opCode.size() - 1); i >= 0; i-- )
        {
            switch ( newInst->m_opCode[i] )
            {
                case AdjSimProcDef::Instruction::OCB_ZERO:
                    opCode += '0';
                    break;
                case AdjSimProcDef::Instruction::OCB_ONE:
                    opCode += '1';
                    break;
                case AdjSimProcDef::Instruction::OCB_DONT_CARE:
                    opCode += '-';
                    break;
            }
        }

        QTreeWidgetItem * item = new QTreeWidgetItem ( treeWidgetInstructions,
                                                       QStringList ( { QString("%1").arg(m_idCounter++),
                                                                       QString::fromStdString(newInst->m_mnemonic),
                                                                       opCode } ) );

        m_instructions.insert({item, newInst});
    }

    // Available flags
    checkBoxHasFlagZero->setCheckState(procDef.m_flags.m_hasZero ? Qt::Checked : Qt::Unchecked);
    checkBoxHasFlagCarry->setCheckState(procDef.m_flags.m_hasCarry ? Qt::Checked : Qt::Unchecked);
    checkBoxHasFlagOverflow->setCheckState(procDef.m_flags.m_hasOverflow ? Qt::Checked : Qt::Unchecked);
    checkBoxHasFlagNegative->setCheckState(procDef.m_flags.m_hasNegative ? Qt::Checked : Qt::Unchecked);
    checkBoxHasFlagHalfCarry->setCheckState(procDef.m_flags.m_hasHalfCarry ? Qt::Checked : Qt::Unchecked);
    checkBoxHasFlagParity->setCheckState(procDef.m_flags.m_hasParity ? Qt::Checked : Qt::Unchecked);
    // Ports
    checkBoxPortsSupported->setCheckState ( ( ( 0 == procDef.m_ports.m_addrWidth ) ||
                                              ( 0 == procDef.m_ports.m_dataWidth ) ) ? Qt::Unchecked : Qt::Checked );
    spinBoxPortsAddressWidth->setValue(procDef.m_ports.m_addrWidth);
    spinBoxPortsDataWidth->setValue(procDef.m_ports.m_dataWidth);
    // Interrupts
    checkBoxHasInterrupts->setCheckState(procDef.m_hasInterrupts ? Qt::Checked : Qt::Unchecked);
    spinBoxInterruptVector->setValue(procDef.m_interruptVector);
    checkBoxInterruptBackupFlags->setCheckState(procDef.m_flags.m_backupWhenInterrupted ? Qt::Checked : Qt::Unchecked);
    checkBoxInterruptAutoDisable->setCheckState(procDef.m_flags.m_autoDisableInterrupts ? Qt::Checked : Qt::Unchecked);
    checkBoxInterruptAutoEnable->setCheckState(procDef.m_flags.m_autoEnableInterrupts ? Qt::Checked : Qt::Unchecked);

    setFileName(fileName);
}

void AdjSimProcDefGui::saveFile ( const QString & fileName )
{
    AdjSimProcDef procDef;

    // Processor Name, Description, and Fail jump
    procDef.m_name = lineEditProcessorName->text().toUpper().toStdString();
    procDef.m_description = textEditDescription->toPlainText().toStdString();
    procDef.m_failjmp = lineEditFailJump->text().toUpper().toStdString();

    // Program memory
    procDef.m_memory.m_program.m_size = (unsigned int) spinBoxProgramSize->value();
    int wordSize = spinBoxProgramWordSize->value();
    procDef.m_memory.m_program.m_wordSize = wordSize;
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
    }

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


    // Instruction set
    std::map<int, int> idMap;
    for ( const auto node : m_instructions )
    {
        idMap.insert ( { node.first->text(0).toInt(),
                         int(procDef.m_instructionSet.size()) } );

        procDef.m_instructionSet.push_back(*(node.second));

        unsigned int opCodeSize = 0;
        for ( unsigned int bitNumber = 0; bitNumber < node.second->m_opCode.size(); bitNumber++ )
        {
            if ( AdjSimProcDef::Instruction::OCB_DONT_CARE != node.second->m_opCode[bitNumber] )
            {
                opCodeSize = bitNumber + 1;
            }
        }
        procDef.m_instructionSet.back().m_opCode.resize(opCodeSize);

        std::vector<AdjSimProcDef::Instruction::Operand> & operands = procDef.m_instructionSet.back().m_operands;
        for ( unsigned int oprNumber = 0; oprNumber < operands.size(); oprNumber++ )
        {
            unsigned int operandSize = 0;
            const std::vector<int> & operand = operands[oprNumber].m_OPCodePermutation;
            for ( unsigned int bitNumber = 0; bitNumber < operand.size(); bitNumber++ )
            {
                if ( -1 != operand[bitNumber] )
                {
                    operandSize = bitNumber + 1;
                }
            }
            operands[oprNumber].m_size = operandSize;
            operands[oprNumber].m_OPCodePermutation.resize(operandSize);
        }
        for ( int i = ( int(operands.size()) - 1 ); i >= 0; i-- )
        {
            if ( ( 0 == operands[i].m_size ) && ( -1 == operands[i].m_fixedValue ) )
            {
                operands.erase(operands.begin() + i);
            }
        }
    }
    for ( auto inst : procDef.m_instructionSet )
    {
        if ( -1 != inst.m_next.m_positive )
        {
            inst.m_next.m_positive = idMap[inst.m_next.m_positive];
        }

        if ( -1 != inst.m_next.m_negative )
        {
            inst.m_next.m_negative = idMap[inst.m_next.m_negative];
        }
    }

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

int AdjSimProcDefGui::checkIDs()
{
    std::set<int> ids;

    for ( const auto node : m_instructions )
    {
        ids.insert(node.first->text(0).toInt());
    }

    for ( const auto node : m_instructions )
    {
        if ( -1 != node.second->m_next.m_positive )
        {
            if ( ids.cend() == ids.find(node.second->m_next.m_positive) )
            {
                return node.first->text(0).toInt();
            }
        }
        if ( -1 != node.second->m_next.m_positive )
        {
            if ( ids.cend() == ids.find(node.second->m_next.m_positive) )
            {
                return node.first->text(0).toInt();
            }
        }
    }

    return -1;
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

void AdjSimProcDefGui::closeEvent ( QCloseEvent * event )
{
    maybeSave() ? event->accept() : event->ignore();
}

void AdjSimProcDefGui::clearModified()
{
    setWindowModified(false);
}

void AdjSimProcDefGui::setModified()
{
    setWindowModified(true);
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
    groupBoxStackD->setEnabled(radioButtonStackMode1->isChecked());
    groupBoxStackS->setEnabled(radioButtonStackMode2->isChecked());
}

void AdjSimProcDefGui::disenaPorts ( int checkState )
{
    spinBoxPortsAddressWidth -> setEnabled ( Qt::Checked == checkState );
    spinBoxPortsDataWidth    -> setEnabled ( Qt::Checked == checkState );
}

void AdjSimProcDefGui::disenaInterrupts ( int checkState )
{
    spinBoxInterruptVector       -> setEnabled ( Qt::Checked == checkState );
    checkBoxInterruptBackupFlags -> setEnabled ( Qt::Checked == checkState );
    checkBoxInterruptAutoDisable -> setEnabled ( Qt::Checked == checkState );
    checkBoxInterruptAutoEnable  -> setEnabled ( Qt::Checked == checkState );
}

void AdjSimProcDefGui::showHideOPCodeBits ( int size )
{
    --size;

    SHOW_HIDE_OP_CODE_BIT(23, size)
    SHOW_HIDE_OP_CODE_BIT(22, size)
    SHOW_HIDE_OP_CODE_BIT(21, size)
    SHOW_HIDE_OP_CODE_BIT(20, size)
    SHOW_HIDE_OP_CODE_BIT(19, size)
    SHOW_HIDE_OP_CODE_BIT(18, size)
    SHOW_HIDE_OP_CODE_BIT(17, size)
    SHOW_HIDE_OP_CODE_BIT(16, size)
    SHOW_HIDE_OP_CODE_BIT(15, size)
    SHOW_HIDE_OP_CODE_BIT(14, size)
    SHOW_HIDE_OP_CODE_BIT(13, size)
    SHOW_HIDE_OP_CODE_BIT(12, size)
    SHOW_HIDE_OP_CODE_BIT(11, size)
    SHOW_HIDE_OP_CODE_BIT(10, size)
    SHOW_HIDE_OP_CODE_BIT( 9, size)
    SHOW_HIDE_OP_CODE_BIT( 8, size)
    SHOW_HIDE_OP_CODE_BIT( 7, size)
    SHOW_HIDE_OP_CODE_BIT( 6, size)
    SHOW_HIDE_OP_CODE_BIT( 5, size)
    SHOW_HIDE_OP_CODE_BIT( 4, size)
    SHOW_HIDE_OP_CODE_BIT( 3, size)
    SHOW_HIDE_OP_CODE_BIT( 2, size)
    SHOW_HIDE_OP_CODE_BIT( 1, size)
    SHOW_HIDE_OP_CODE_BIT( 0, size)
}

bool AdjSimProcDefGui::maybeSave()
{
    if ( false == isWindowModified() )
    {
        return true;
    }

    switch ( QMessageBox::warning ( this,
                                    tr ( "Application" ),
                                    tr ( "The document has been modified.\n"
                                         "Do you want to save your changes?" ),
                                    QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                    QMessageBox::Save )
           )
    {
        case QMessageBox::Save:
            return on_pushButtonSaveAs_clicked();
        case QMessageBox::Discard:
            return true;
        default:
            return false;
    }
}

void AdjSimProcDefGui::on_pushButtonOpenFile_clicked()
{
    if ( false == maybeSave() )
    {
        return;
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
    if ( true == lineEditProcessorName->text().isEmpty() )
    {
        QMessageBox::critical ( this,
                                tr ( "Processor name?" ),
                                tr ( "You have to specify name of the processor."),
                                QMessageBox::Ok,
                                QMessageBox::Ok );
        return false;
    }

    int mismatchID = checkIDs();
    if ( -1 != mismatchID )
    {
        QMessageBox::critical ( this,
                                tr ( "ID mismatch" ),
                                tr ( "This processor definition is not valid.\nInstruction with ID %1 links to "
                                     "nonexistent instruction." ) . arg(mismatchID),
                                QMessageBox::Ok,
                                QMessageBox::Ok );
        return false;
    }

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
                                                   QStringList ( { QString("%1").arg(m_idCounter++),
                                                                   tr("<undefined>"),
                                                                   tr("<undefined>") } ) );

    m_instructions.insert({item, new AdjSimProcDef::Instruction()});
    treeWidgetInstructions->setCurrentItem(item);
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
    m_lock = true;

    const AdjSimProcDef::Instruction * inst;
    bool instructionSelected = ( false == treeWidgetInstructions->selectedItems().isEmpty() );

    pushButtonRemoveInstruction->setEnabled(instructionSelected);
    toolBoxInstructions->setEnabled(instructionSelected);

    if ( true == instructionSelected )
    {
        inst = m_instructions[treeWidgetInstructions->selectedItems().front()];
        showHideOPCodeBits(spinBoxProgramWordSize->value());
    }
    else
    {
        inst = & m_emptyInstruction;
        showHideOPCodeBits(24);
    }

    if ( true == inst->m_mnemonic.empty() )
    {
        lineEditInstMnemonic -> setText ( tr("<undefined>") );
    }
    else
    {
        lineEditInstMnemonic -> setText ( QString::fromStdString(inst->m_mnemonic) );
    }

    comboBoxInstOperation -> setCurrentIndex ( (int) (inst -> m_operation) );

    comboBoxInstOpCode0  -> setCurrentIndex ( (int) (inst -> m_opCode[0]) );
    comboBoxInstOpCode1  -> setCurrentIndex ( (int) (inst -> m_opCode[1]) );
    comboBoxInstOpCode2  -> setCurrentIndex ( (int) (inst -> m_opCode[2]) );
    comboBoxInstOpCode3  -> setCurrentIndex ( (int) (inst -> m_opCode[3]) );
    comboBoxInstOpCode4  -> setCurrentIndex ( (int) (inst -> m_opCode[4]) );
    comboBoxInstOpCode5  -> setCurrentIndex ( (int) (inst -> m_opCode[5]) );
    comboBoxInstOpCode6  -> setCurrentIndex ( (int) (inst -> m_opCode[6]) );
    comboBoxInstOpCode7  -> setCurrentIndex ( (int) (inst -> m_opCode[7]) );
    comboBoxInstOpCode8  -> setCurrentIndex ( (int) (inst -> m_opCode[8]) );
    comboBoxInstOpCode9  -> setCurrentIndex ( (int) (inst -> m_opCode[9]) );
    comboBoxInstOpCode10 -> setCurrentIndex ( (int) (inst -> m_opCode[10]) );
    comboBoxInstOpCode11 -> setCurrentIndex ( (int) (inst -> m_opCode[11]) );
    comboBoxInstOpCode12 -> setCurrentIndex ( (int) (inst -> m_opCode[12]) );
    comboBoxInstOpCode13 -> setCurrentIndex ( (int) (inst -> m_opCode[13]) );
    comboBoxInstOpCode14 -> setCurrentIndex ( (int) (inst -> m_opCode[14]) );
    comboBoxInstOpCode15 -> setCurrentIndex ( (int) (inst -> m_opCode[15]) );
    comboBoxInstOpCode16 -> setCurrentIndex ( (int) (inst -> m_opCode[16]) );
    comboBoxInstOpCode17 -> setCurrentIndex ( (int) (inst -> m_opCode[17]) );
    comboBoxInstOpCode18 -> setCurrentIndex ( (int) (inst -> m_opCode[18]) );
    comboBoxInstOpCode19 -> setCurrentIndex ( (int) (inst -> m_opCode[19]) );
    comboBoxInstOpCode20 -> setCurrentIndex ( (int) (inst -> m_opCode[20]) );
    comboBoxInstOpCode21 -> setCurrentIndex ( (int) (inst -> m_opCode[21]) );
    comboBoxInstOpCode22 -> setCurrentIndex ( (int) (inst -> m_opCode[22]) );
    comboBoxInstOpCode23 -> setCurrentIndex ( (int) (inst -> m_opCode[23]) );

    if ( -1 == inst->m_operands[0].m_fixedValue )
    {
        tabWidgetInstOpr0->setCurrentIndex(0);
        lineEditInstOpr0FixedValue->clear();
    }
    else
    {
        tabWidgetInstOpr0->setCurrentIndex(1);
        lineEditInstOpr0FixedValue->setText(QString("%1").arg(inst->m_operands[0].m_fixedValue));
    }
    comboBoxInstOpr0Order->setCurrentIndex((int)(inst->m_operands[0].m_number));
    comboBoxInstOpr0Addressing->setCurrentIndex((int)(inst->m_operands[0].m_type));
    spinBoxInstOpr0Bit0->setValue(inst->m_operands[0].m_OPCodePermutation[0]);
    spinBoxInstOpr0Bit1->setValue(inst->m_operands[0].m_OPCodePermutation[1]);
    spinBoxInstOpr0Bit2->setValue(inst->m_operands[0].m_OPCodePermutation[2]);
    spinBoxInstOpr0Bit3->setValue(inst->m_operands[0].m_OPCodePermutation[3]);
    spinBoxInstOpr0Bit4->setValue(inst->m_operands[0].m_OPCodePermutation[4]);
    spinBoxInstOpr0Bit5->setValue(inst->m_operands[0].m_OPCodePermutation[5]);
    spinBoxInstOpr0Bit6->setValue(inst->m_operands[0].m_OPCodePermutation[6]);
    spinBoxInstOpr0Bit7->setValue(inst->m_operands[0].m_OPCodePermutation[7]);
    spinBoxInstOpr0Bit8->setValue(inst->m_operands[0].m_OPCodePermutation[8]);
    spinBoxInstOpr0Bit9->setValue(inst->m_operands[0].m_OPCodePermutation[9]);
    spinBoxInstOpr0Bit10->setValue(inst->m_operands[0].m_OPCodePermutation[10]);
    spinBoxInstOpr0Bit11->setValue(inst->m_operands[0].m_OPCodePermutation[11]);
    spinBoxInstOpr0Bit12->setValue(inst->m_operands[0].m_OPCodePermutation[12]);
    spinBoxInstOpr0Bit13->setValue(inst->m_operands[0].m_OPCodePermutation[13]);
    spinBoxInstOpr0Bit14->setValue(inst->m_operands[0].m_OPCodePermutation[14]);
    spinBoxInstOpr0Bit15->setValue(inst->m_operands[0].m_OPCodePermutation[15]);

    if ( -1 == inst->m_operands[1].m_fixedValue )
    {
        tabWidgetInstOpr1->setCurrentIndex(0);
        lineEditInstOpr1FixedValue->clear();
    }
    else
    {
        tabWidgetInstOpr1->setCurrentIndex(1);
        lineEditInstOpr1FixedValue->setText(QString("%1").arg(inst->m_operands[1].m_fixedValue));
    }
    comboBoxInstOpr1Order->setCurrentIndex((int)(inst->m_operands[1].m_number));
    comboBoxInstOpr1Addressing->setCurrentIndex((int)(inst->m_operands[1].m_type));
    spinBoxInstOpr1Bit0->setValue(inst->m_operands[1].m_OPCodePermutation[0]);
    spinBoxInstOpr1Bit1->setValue(inst->m_operands[1].m_OPCodePermutation[1]);
    spinBoxInstOpr1Bit2->setValue(inst->m_operands[1].m_OPCodePermutation[2]);
    spinBoxInstOpr1Bit3->setValue(inst->m_operands[1].m_OPCodePermutation[3]);
    spinBoxInstOpr1Bit4->setValue(inst->m_operands[1].m_OPCodePermutation[4]);
    spinBoxInstOpr1Bit5->setValue(inst->m_operands[1].m_OPCodePermutation[5]);
    spinBoxInstOpr1Bit6->setValue(inst->m_operands[1].m_OPCodePermutation[6]);
    spinBoxInstOpr1Bit7->setValue(inst->m_operands[1].m_OPCodePermutation[7]);
    spinBoxInstOpr1Bit8->setValue(inst->m_operands[1].m_OPCodePermutation[8]);
    spinBoxInstOpr1Bit9->setValue(inst->m_operands[1].m_OPCodePermutation[9]);
    spinBoxInstOpr1Bit10->setValue(inst->m_operands[1].m_OPCodePermutation[10]);
    spinBoxInstOpr1Bit11->setValue(inst->m_operands[1].m_OPCodePermutation[11]);
    spinBoxInstOpr1Bit12->setValue(inst->m_operands[1].m_OPCodePermutation[12]);
    spinBoxInstOpr1Bit13->setValue(inst->m_operands[1].m_OPCodePermutation[13]);
    spinBoxInstOpr1Bit14->setValue(inst->m_operands[1].m_OPCodePermutation[14]);
    spinBoxInstOpr1Bit15->setValue(inst->m_operands[1].m_OPCodePermutation[15]);

    if ( -1 == inst->m_operands[2].m_fixedValue )
    {
        tabWidgetInstOpr2->setCurrentIndex(0);
        lineEditInstOpr2FixedValue->clear();
    }
    else
    {
        tabWidgetInstOpr2->setCurrentIndex(1);
        lineEditInstOpr2FixedValue->setText(QString("%1").arg(inst->m_operands[2].m_fixedValue));
    }
    comboBoxInstOpr2Order->setCurrentIndex((int)(inst->m_operands[2].m_number));
    comboBoxInstOpr2Addressing->setCurrentIndex((int)(inst->m_operands[2].m_type));
    spinBoxInstOpr2Bit0->setValue(inst->m_operands[2].m_OPCodePermutation[0]);
    spinBoxInstOpr2Bit1->setValue(inst->m_operands[2].m_OPCodePermutation[1]);
    spinBoxInstOpr2Bit2->setValue(inst->m_operands[2].m_OPCodePermutation[2]);
    spinBoxInstOpr2Bit3->setValue(inst->m_operands[2].m_OPCodePermutation[3]);
    spinBoxInstOpr2Bit4->setValue(inst->m_operands[2].m_OPCodePermutation[4]);
    spinBoxInstOpr2Bit5->setValue(inst->m_operands[2].m_OPCodePermutation[5]);
    spinBoxInstOpr2Bit6->setValue(inst->m_operands[2].m_OPCodePermutation[6]);
    spinBoxInstOpr2Bit7->setValue(inst->m_operands[2].m_OPCodePermutation[7]);
    spinBoxInstOpr2Bit8->setValue(inst->m_operands[2].m_OPCodePermutation[8]);
    spinBoxInstOpr2Bit9->setValue(inst->m_operands[2].m_OPCodePermutation[9]);
    spinBoxInstOpr2Bit10->setValue(inst->m_operands[2].m_OPCodePermutation[10]);
    spinBoxInstOpr2Bit11->setValue(inst->m_operands[2].m_OPCodePermutation[11]);
    spinBoxInstOpr2Bit12->setValue(inst->m_operands[2].m_OPCodePermutation[12]);
    spinBoxInstOpr2Bit13->setValue(inst->m_operands[2].m_OPCodePermutation[13]);
    spinBoxInstOpr2Bit14->setValue(inst->m_operands[2].m_OPCodePermutation[14]);
    spinBoxInstOpr2Bit15->setValue(inst->m_operands[2].m_OPCodePermutation[15]);

    comboBoxInstFlagZero->setCurrentIndex(int(inst->m_flags[0]));
    comboBoxInstFlagCarry->setCurrentIndex(int(inst->m_flags[1]));
    comboBoxInstFlagOverflow->setCurrentIndex(int(inst->m_flags[2]));
    comboBoxInstFlagNegative->setCurrentIndex(int(inst->m_flags[3]));
    comboBoxInstFlagHalfCarry->setCurrentIndex(int(inst->m_flags[4]));
    comboBoxInstFlagIE->setCurrentIndex(int(inst->m_flags[5]));
    comboBoxInstFlagParity->setCurrentIndex(int(inst->m_flags[6]));
    comboBoxInstFlagFlag->setCurrentIndex(int(inst->m_flags[7]));
    comboBoxInstParity->setCurrentIndex(int(inst->m_operParameters.m_parity));
    comboBoxInstResult->setCurrentIndex(int(inst->m_operParameters.m_result));
    checkBoxIgnoreCarry->setCheckState(inst->m_operParameters.m_ignoreCarryFlag ? Qt::Checked : Qt::Unchecked);
    comboBoxInstConditionType->setCurrentIndex(int(inst->m_condition.m_type));
    comboBoxInstConditionFlag->setCurrentIndex(int(inst->m_condition.m_flag));
    spinBoxInstTimePositive->setValue(inst->m_time.m_positive);
    spinBoxInstTimeNegative->setValue(inst->m_time.m_negative);
    lineEditInstNextPositive->setText(QString("%1").arg(inst->m_next.m_positive));
    lineEditInstNextNegative->setText(QString("%1").arg(inst->m_next.m_negative));

    m_lock = false;
}

void AdjSimProcDefGui::instModified()
{
    if ( ( true == m_lock ) || ( true == treeWidgetInstructions->selectedItems().isEmpty() ) )
    {
        return;
    }

    QTreeWidgetItem * item = treeWidgetInstructions->selectedItems().front();
    auto iter = m_instructions.find(item);
    AdjSimProcDef::Instruction * inst = iter->second;

    inst->m_mnemonic = lineEditInstMnemonic->text().toUpper().toStdString();
    inst->m_operation = (AdjSimProcDef::Instruction::Operation) (comboBoxInstOperation->currentIndex());

    inst->m_opCode[0]  = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode0 ->currentIndex());
    inst->m_opCode[1]  = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode1 ->currentIndex());
    inst->m_opCode[2]  = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode2 ->currentIndex());
    inst->m_opCode[3]  = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode3 ->currentIndex());
    inst->m_opCode[4]  = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode4 ->currentIndex());
    inst->m_opCode[5]  = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode5 ->currentIndex());
    inst->m_opCode[6]  = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode6 ->currentIndex());
    inst->m_opCode[7]  = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode7 ->currentIndex());
    inst->m_opCode[8]  = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode8 ->currentIndex());
    inst->m_opCode[9]  = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode9 ->currentIndex());
    inst->m_opCode[10] = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode10->currentIndex());
    inst->m_opCode[11] = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode11->currentIndex());
    inst->m_opCode[12] = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode12->currentIndex());
    inst->m_opCode[13] = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode13->currentIndex());
    inst->m_opCode[14] = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode14->currentIndex());
    inst->m_opCode[15] = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode15->currentIndex());
    inst->m_opCode[16] = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode16->currentIndex());
    inst->m_opCode[17] = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode17->currentIndex());
    inst->m_opCode[18] = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode18->currentIndex());
    inst->m_opCode[19] = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode19->currentIndex());
    inst->m_opCode[20] = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode20->currentIndex());
    inst->m_opCode[21] = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode21->currentIndex());
    inst->m_opCode[22] = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode22->currentIndex());
    inst->m_opCode[23] = (AdjSimProcDef::Instruction::OpCodeBit) (comboBoxInstOpCode23->currentIndex());

    if ( 0 == tabWidgetInstOpr0->currentIndex() )
    {
        inst->m_operands[0].m_size = 16;
        inst->m_operands[0].m_number = (AdjSimProcDef::Instruction::Operand::Number)
                                       (comboBoxInstOpr0Order->currentIndex());
        inst->m_operands[0].m_type = (AdjSimProcDef::Instruction::Operand::Type)
                                     (comboBoxInstOpr0Addressing->currentIndex());
        inst->m_operands[0].m_fixedValue = -1;
        inst->m_operands[0].m_OPCodePermutation[0] = spinBoxInstOpr0Bit0->value();
        inst->m_operands[0].m_OPCodePermutation[1] = spinBoxInstOpr0Bit1->value();
        inst->m_operands[0].m_OPCodePermutation[2] = spinBoxInstOpr0Bit2->value();
        inst->m_operands[0].m_OPCodePermutation[3] = spinBoxInstOpr0Bit3->value();
        inst->m_operands[0].m_OPCodePermutation[4] = spinBoxInstOpr0Bit4->value();
        inst->m_operands[0].m_OPCodePermutation[5] = spinBoxInstOpr0Bit5->value();
        inst->m_operands[0].m_OPCodePermutation[6] = spinBoxInstOpr0Bit6->value();
        inst->m_operands[0].m_OPCodePermutation[7] = spinBoxInstOpr0Bit7->value();
        inst->m_operands[0].m_OPCodePermutation[8] = spinBoxInstOpr0Bit8->value();
        inst->m_operands[0].m_OPCodePermutation[9] = spinBoxInstOpr0Bit9->value();
        inst->m_operands[0].m_OPCodePermutation[10] = spinBoxInstOpr0Bit10->value();
        inst->m_operands[0].m_OPCodePermutation[11] = spinBoxInstOpr0Bit11->value();
        inst->m_operands[0].m_OPCodePermutation[12] = spinBoxInstOpr0Bit12->value();
        inst->m_operands[0].m_OPCodePermutation[13] = spinBoxInstOpr0Bit13->value();
        inst->m_operands[0].m_OPCodePermutation[14] = spinBoxInstOpr0Bit14->value();
        inst->m_operands[0].m_OPCodePermutation[15] = spinBoxInstOpr0Bit15->value();
    }
    else
    {
        inst->m_operands[0].m_size = 0;
        inst->m_operands[0].m_OPCodePermutation.clear();
        inst->m_operands[0].m_number = AdjSimProcDef::Instruction::Operand::N_HIDDEN;
        inst->m_operands[0].m_type = (AdjSimProcDef::Instruction::Operand::Type)
                                     (comboBoxInstOpr0Addressing->currentIndex());
        inst->m_operands[0].m_fixedValue = lineEditInstOpr0FixedValue->text().toInt();
    }

    if ( 0 == tabWidgetInstOpr1->currentIndex() )
    {
        inst->m_operands[1].m_size = 16;
        inst->m_operands[1].m_number = (AdjSimProcDef::Instruction::Operand::Number)
                                       (comboBoxInstOpr1Order->currentIndex());
        inst->m_operands[1].m_type = (AdjSimProcDef::Instruction::Operand::Type)
                                     (comboBoxInstOpr1Addressing->currentIndex());
        inst->m_operands[1].m_fixedValue = -1;
        inst->m_operands[1].m_OPCodePermutation[0] = spinBoxInstOpr1Bit0->value();
        inst->m_operands[1].m_OPCodePermutation[1] = spinBoxInstOpr1Bit1->value();
        inst->m_operands[1].m_OPCodePermutation[2] = spinBoxInstOpr1Bit2->value();
        inst->m_operands[1].m_OPCodePermutation[3] = spinBoxInstOpr1Bit3->value();
        inst->m_operands[1].m_OPCodePermutation[4] = spinBoxInstOpr1Bit4->value();
        inst->m_operands[1].m_OPCodePermutation[5] = spinBoxInstOpr1Bit5->value();
        inst->m_operands[1].m_OPCodePermutation[6] = spinBoxInstOpr1Bit6->value();
        inst->m_operands[1].m_OPCodePermutation[7] = spinBoxInstOpr1Bit7->value();
        inst->m_operands[1].m_OPCodePermutation[8] = spinBoxInstOpr1Bit8->value();
        inst->m_operands[1].m_OPCodePermutation[9] = spinBoxInstOpr1Bit9->value();
        inst->m_operands[1].m_OPCodePermutation[10] = spinBoxInstOpr1Bit10->value();
        inst->m_operands[1].m_OPCodePermutation[11] = spinBoxInstOpr1Bit11->value();
        inst->m_operands[1].m_OPCodePermutation[12] = spinBoxInstOpr1Bit12->value();
        inst->m_operands[1].m_OPCodePermutation[13] = spinBoxInstOpr1Bit13->value();
        inst->m_operands[1].m_OPCodePermutation[14] = spinBoxInstOpr1Bit14->value();
        inst->m_operands[1].m_OPCodePermutation[15] = spinBoxInstOpr1Bit15->value();
    }
    else
    {
        inst->m_operands[1].m_size = 0;
        inst->m_operands[1].m_OPCodePermutation.clear();
        inst->m_operands[1].m_number = AdjSimProcDef::Instruction::Operand::N_HIDDEN;
        inst->m_operands[1].m_type = (AdjSimProcDef::Instruction::Operand::Type)
                                     (comboBoxInstOpr0Addressing->currentIndex());
        inst->m_operands[1].m_fixedValue = lineEditInstOpr1FixedValue->text().toInt();
    }

    if ( 0 == tabWidgetInstOpr2->currentIndex() )
    {
        inst->m_operands[2].m_size = 16;
        inst->m_operands[2].m_number = (AdjSimProcDef::Instruction::Operand::Number)
                                       (comboBoxInstOpr2Order->currentIndex());
        inst->m_operands[2].m_type = (AdjSimProcDef::Instruction::Operand::Type)
                                     (comboBoxInstOpr2Addressing->currentIndex());
        inst->m_operands[2].m_fixedValue = -1;
        inst->m_operands[2].m_OPCodePermutation[0] = spinBoxInstOpr2Bit0->value();
        inst->m_operands[2].m_OPCodePermutation[1] = spinBoxInstOpr2Bit1->value();
        inst->m_operands[2].m_OPCodePermutation[2] = spinBoxInstOpr2Bit2->value();
        inst->m_operands[2].m_OPCodePermutation[3] = spinBoxInstOpr2Bit3->value();
        inst->m_operands[2].m_OPCodePermutation[4] = spinBoxInstOpr2Bit4->value();
        inst->m_operands[2].m_OPCodePermutation[5] = spinBoxInstOpr2Bit5->value();
        inst->m_operands[2].m_OPCodePermutation[6] = spinBoxInstOpr2Bit6->value();
        inst->m_operands[2].m_OPCodePermutation[7] = spinBoxInstOpr2Bit7->value();
        inst->m_operands[2].m_OPCodePermutation[8] = spinBoxInstOpr2Bit8->value();
        inst->m_operands[2].m_OPCodePermutation[9] = spinBoxInstOpr2Bit9->value();
        inst->m_operands[2].m_OPCodePermutation[10] = spinBoxInstOpr2Bit10->value();
        inst->m_operands[2].m_OPCodePermutation[11] = spinBoxInstOpr2Bit11->value();
        inst->m_operands[2].m_OPCodePermutation[12] = spinBoxInstOpr2Bit12->value();
        inst->m_operands[2].m_OPCodePermutation[13] = spinBoxInstOpr2Bit13->value();
        inst->m_operands[2].m_OPCodePermutation[14] = spinBoxInstOpr2Bit14->value();
        inst->m_operands[2].m_OPCodePermutation[15] = spinBoxInstOpr2Bit15->value();
    }
    else
    {
        inst->m_operands[2].m_size = 0;
        inst->m_operands[2].m_OPCodePermutation.clear();
        inst->m_operands[2].m_number = AdjSimProcDef::Instruction::Operand::N_HIDDEN;
        inst->m_operands[2].m_type = (AdjSimProcDef::Instruction::Operand::Type)
                                     (comboBoxInstOpr0Addressing->currentIndex());
        inst->m_operands[2].m_fixedValue = lineEditInstOpr2FixedValue->text().toInt();
    }

    inst->m_flags[0] = (AdjSimProcDef::Instruction::FlagEffect) (comboBoxInstFlagZero->currentIndex());
    inst->m_flags[1] = (AdjSimProcDef::Instruction::FlagEffect) (comboBoxInstFlagCarry->currentIndex());
    inst->m_flags[2] = (AdjSimProcDef::Instruction::FlagEffect) (comboBoxInstFlagOverflow->currentIndex());
    inst->m_flags[3] = (AdjSimProcDef::Instruction::FlagEffect) (comboBoxInstFlagNegative->currentIndex());
    inst->m_flags[4] = (AdjSimProcDef::Instruction::FlagEffect) (comboBoxInstFlagHalfCarry->currentIndex());
    inst->m_flags[5] = (AdjSimProcDef::Instruction::FlagEffect) (comboBoxInstFlagIE->currentIndex());
    inst->m_flags[6] = (AdjSimProcDef::Instruction::FlagEffect) (comboBoxInstFlagParity->currentIndex());
    inst->m_flags[7] = (AdjSimProcDef::Instruction::FlagEffect) (comboBoxInstFlagFlag->currentIndex());

    inst->m_operParameters.m_parity = (AdjSimProcDef::Instruction::OperParameters::Parity)
                                      comboBoxInstParity->currentIndex();
    inst->m_operParameters.m_result = (AdjSimProcDef::Instruction::OperParameters::Result)
                                      comboBoxInstResult->currentIndex();
    inst->m_operParameters.m_ignoreCarryFlag = ( Qt::Checked == checkBoxIgnoreCarry->checkState() );

    inst->m_condition.m_type = (AdjSimProcDef::Instruction::Condition::Type) comboBoxInstConditionType->currentIndex();
    inst->m_condition.m_flag = (AdjSimProcDef::Instruction::ProcessorFlag) comboBoxInstConditionFlag->currentIndex();

    inst->m_time.m_positive = (unsigned int) spinBoxInstTimePositive->value();
    inst->m_time.m_negative = (unsigned int) spinBoxInstTimeNegative->value();

    if ( true == lineEditInstNextPositive->text().isEmpty() )
    {
        inst->m_next.m_positive = -1;
    }
    else
    {
        inst->m_next.m_positive = lineEditInstNextPositive->text().toInt();
    }

    if ( true == lineEditInstNextNegative->text().isEmpty() )
    {
        inst->m_next.m_negative = -1;
    }
    else
    {
        inst->m_next.m_negative = lineEditInstNextNegative->text().toInt();
    }

    QString opCode = "0b";
    for ( int i = ( inst->m_opCode.size() - 1); i >= 0; i-- )
    {
        switch ( inst->m_opCode[i] )
        {
            case AdjSimProcDef::Instruction::OCB_ZERO:
                opCode += '0';
                break;
            case AdjSimProcDef::Instruction::OCB_ONE:
                opCode += '1';
                break;
            case AdjSimProcDef::Instruction::OCB_DONT_CARE:
                opCode += '-';
                break;
        }
    }

    item->setText(1, QString::fromStdString(inst->m_mnemonic));
    item->setText(2, opCode);
}

inline void AdjSimProcDefGui::setupConnections()
{
    // Processor Name, Description, and Fail jump
    connect ( lineEditProcessorName,        SIGNAL(textEdited(const QString &)), SLOT(setModified()) );
    connect ( textEditDescription,          SIGNAL(textChanged()),               SLOT(setModified()) );
    connect ( lineEditFailJump,             SIGNAL(textEdited(const QString &)), SLOT(setModified()) );
    // Program Memory
    connect ( spinBoxProgramSize,           SIGNAL(valueChanged(int)),           SLOT(setModified()) );
    connect ( spinBoxProgramWordSize,       SIGNAL(valueChanged(int)),           SLOT(setModified()) );
    connect ( spinBoxProgramWordSize,       SIGNAL(valueChanged(int)),           SLOT(showHideOPCodeBits(int)) );
    connect ( comboBoxProgramEndian,        SIGNAL(currentIndexChanged(int)),    SLOT(setModified()) );
    // Data Memory
    connect ( checkBoxDataMemory,           SIGNAL(stateChanged(int)),           SLOT(disenaDataMem(int)) );
    connect ( checkBoxDataMemory,           SIGNAL(stateChanged(int)),           SLOT(setModified()) );
    connect ( spinBoxDataMemorySize,        SIGNAL(valueChanged(int)),           SLOT(setModified()) );
    // Register File
    connect ( checkBoxRegisterFile,         SIGNAL(stateChanged(int)),           SLOT(disenaRegFile(int)) );
    connect ( checkBoxRegisterFile,         SIGNAL(stateChanged(int)),           SLOT(setModified()) );
    connect ( spinBoxRegisterFileSize,      SIGNAL(valueChanged(int)),           SLOT(setModified()) );
    connect ( spinBoxRegisterFileBanks,     SIGNAL(valueChanged(int)),           SLOT(setModified()) );
    // Stack
    connect ( radioButtonStackMode0,        SIGNAL(toggled(bool)),               SLOT(disenaStack(bool)) );
    connect ( radioButtonStackMode1,        SIGNAL(toggled(bool)),               SLOT(disenaStack(bool)) );
    connect ( radioButtonStackMode2,        SIGNAL(toggled(bool)),               SLOT(disenaStack(bool)) );
    connect ( radioButtonStackMode0,        SIGNAL(toggled(bool)),               SLOT(setModified()) );
    connect ( radioButtonStackMode1,        SIGNAL(toggled(bool)),               SLOT(setModified()) );
    connect ( radioButtonStackMode2,        SIGNAL(toggled(bool)),               SLOT(setModified()) );
    connect ( spinBoxStackDSize,            SIGNAL(valueChanged(int)),           SLOT(setModified()) );
    // Available Flags
    connect ( checkBoxHasFlagZero,          SIGNAL(stateChanged(int)),           SLOT(setModified()) );
    connect ( checkBoxHasFlagCarry,         SIGNAL(stateChanged(int)),           SLOT(setModified()) );
    connect ( checkBoxHasFlagOverflow,      SIGNAL(stateChanged(int)),           SLOT(setModified()) );
    connect ( checkBoxHasFlagNegative,      SIGNAL(stateChanged(int)),           SLOT(setModified()) );
    connect ( checkBoxHasFlagHalfCarry,     SIGNAL(stateChanged(int)),           SLOT(setModified()) );
    connect ( checkBoxHasFlagParity,        SIGNAL(stateChanged(int)),           SLOT(setModified()) );
    // Ports
    connect ( checkBoxPortsSupported,       SIGNAL(stateChanged(int)),           SLOT(disenaPorts(int)) );
    connect ( checkBoxPortsSupported,       SIGNAL(stateChanged(int)),           SLOT(setModified()) );
    connect ( spinBoxPortsAddressWidth,     SIGNAL(valueChanged(int)),           SLOT(setModified()) );
    connect ( spinBoxPortsDataWidth,        SIGNAL(valueChanged(int)),           SLOT(setModified()) );
    // Interrupts
    connect ( checkBoxHasInterrupts,        SIGNAL(stateChanged(int)),           SLOT(disenaInterrupts(int)) );
    connect ( checkBoxHasInterrupts,        SIGNAL(stateChanged(int)),           SLOT(setModified()) );
    connect ( spinBoxInterruptVector,       SIGNAL(valueChanged(int)),           SLOT(setModified()) );
    connect ( checkBoxInterruptBackupFlags, SIGNAL(stateChanged(int)),           SLOT(setModified()) );
    connect ( checkBoxInterruptAutoDisable, SIGNAL(stateChanged(int)),           SLOT(setModified()) );
    connect ( checkBoxInterruptAutoEnable,  SIGNAL(stateChanged(int)),           SLOT(setModified()) );

    /*
     * Instruction Set
     */
    connect ( lineEditInstMnemonic,         SIGNAL(textEdited(const QString &)), SLOT(instModified()) );
    connect ( comboBoxInstOperation,        SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode0,          SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode1,          SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode2,          SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode3,          SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode4,          SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode5,          SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode6,          SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode7,          SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode8,          SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode9,          SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode10,         SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode11,         SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode12,         SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode13,         SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode14,         SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode15,         SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode16,         SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode17,         SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode18,         SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode19,         SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode20,         SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode21,         SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode22,         SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpCode23,         SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    // -
    connect ( tabWidgetInstOpr0,            SIGNAL(currentChanged(int)),         SLOT(instModified()) );
    connect ( comboBoxInstOpr0Order,        SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpr0Addressing,   SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( spinBoxInstOpr0Bit0,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr0Bit1,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr0Bit2,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr0Bit3,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr0Bit4,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr0Bit5,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr0Bit6,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr0Bit7,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr0Bit8,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr0Bit9,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr0Bit10,         SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr0Bit11,         SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr0Bit12,         SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr0Bit13,         SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr0Bit14,         SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr0Bit15,         SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( lineEditInstOpr0FixedValue,   SIGNAL(textEdited(const QString &)), SLOT(instModified()) );
    // -
    connect ( tabWidgetInstOpr1,            SIGNAL(currentChanged(int)),         SLOT(instModified()) );
    connect ( comboBoxInstOpr1Order,        SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpr1Addressing,   SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( spinBoxInstOpr1Bit0,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr1Bit1,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr1Bit2,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr1Bit3,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr1Bit4,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr1Bit5,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr1Bit6,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr1Bit7,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr1Bit8,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr1Bit9,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr1Bit10,         SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr1Bit11,         SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr1Bit12,         SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr1Bit13,         SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr1Bit14,         SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr1Bit15,         SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( lineEditInstOpr1FixedValue,   SIGNAL(textEdited(const QString &)), SLOT(instModified()) );
    // -
    connect ( tabWidgetInstOpr2,            SIGNAL(currentChanged(int)),         SLOT(instModified()) );
    connect ( comboBoxInstOpr2Order,        SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstOpr2Addressing,   SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( spinBoxInstOpr2Bit0,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr2Bit1,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr2Bit2,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr2Bit3,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr2Bit4,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr2Bit5,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr2Bit6,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr2Bit7,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr2Bit8,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr2Bit9,          SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr2Bit10,         SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr2Bit11,         SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr2Bit12,         SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr2Bit13,         SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr2Bit14,         SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstOpr2Bit15,         SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( lineEditInstOpr2FixedValue,   SIGNAL(textEdited(const QString &)), SLOT(instModified()) );
    // -
    connect ( comboBoxInstFlagZero,         SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstFlagCarry,        SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstFlagOverflow,     SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstFlagNegative,     SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstFlagHalfCarry,    SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstFlagIE,           SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstFlagParity,       SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstFlagFlag,         SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstParity,           SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstResult,           SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( checkBoxIgnoreCarry,          SIGNAL(stateChanged(int)),           SLOT(instModified()) );
    connect ( comboBoxInstConditionType,    SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( comboBoxInstConditionFlag,    SIGNAL(currentIndexChanged(int)),    SLOT(instModified()) );
    connect ( spinBoxInstTimePositive,      SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( spinBoxInstTimeNegative,      SIGNAL(valueChanged(int)),           SLOT(instModified()) );
    connect ( lineEditInstNextPositive,     SIGNAL(textEdited(const QString &)), SLOT(instModified()) );
    connect ( lineEditInstNextNegative,     SIGNAL(textEdited(const QString &)), SLOT(instModified()) );
}

inline void AdjSimProcDefGui::setupValidators()
{
    QRegExpValidator * procName = new QRegExpValidator(QRegExp("[_A-Za-z0-9][_A-Za-z0-9 ]*"), this);
    QRegExpValidator * mnemonic = new QRegExpValidator(QRegExp("[_A-Za-z][_A-Za-z0-9]*"), this);
    QIntValidator * number = new QIntValidator(-1, 0xffff, this);

    lineEditProcessorName->setValidator(procName);
    lineEditInstMnemonic->setValidator(mnemonic);
    lineEditInstOpr0FixedValue->setValidator(number);
    lineEditInstOpr1FixedValue->setValidator(number);
    lineEditInstOpr2FixedValue->setValidator(number);
    lineEditInstNextPositive->setValidator(number);
    lineEditInstNextNegative->setValidator(number);
}
