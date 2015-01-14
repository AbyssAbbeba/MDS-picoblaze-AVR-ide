// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014, 2015 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup AdjSimProcDef
 * @file AdjSimProcDef.cxx
 */
// =============================================================================

#include "AdjSimProcDef.h"

AdjSimProcDef::Instruction::Instruction()
{
    m_next.m_negative = -1;
    m_next.m_positive = -1;
    m_time.m_positive = 1;
    m_time.m_negative = 1;
    for ( int i = 0; i < FLAG__MAX__; i++ )
    {
        m_flags[i] = FE_DO_NOT_CHANGE;
    }
    m_condition.m_flag = FLAG_ZERO;
    m_condition.m_type = Condition::T_UNCONDITIONAL;
    m_operation = OP_NONE;
    m_operParameters.m_result = OperParameters::RES_DISCARD;
    m_operParameters.m_parity = OperParameters::P_EVEN;
    m_operParameters.m_ignoreCarryFlag = true;
    m_operParameters.m_ignoreZeroFlag = true;
    m_opCode.assign(24, OCB_DONT_CARE);
    m_operands.resize(3);
    m_operands[0].m_number = Operand::N_HIDDEN/*N_FIRST*/;
    m_operands[1].m_number = Operand::N_HIDDEN/*N_SECOND*/;
    m_operands[2].m_number = Operand::N_HIDDEN/*N_THIRD*/;
}

AdjSimProcDef::Instruction::Operand::Operand()
{
    m_type = T_IMMEDIATE;
    m_number = N_HIDDEN;

    m_size = 16;
    m_fixedValue = -1;
    m_OPCodePermutation.assign(16, -1);
}

std::ostream & operator << ( std::ostream & out,
                             const AdjSimProcDef & definition )
{
    out << "Name: \"" << definition.m_name << '"' << std::endl;
    out << "Description: \"" << definition.m_description << '"' << std::endl;
    out << "Fail jump instruction: \"" << definition.m_failjmp << '"' << std::endl;
    out << "Supports interrupt(s): " << definition.m_hasInterrupts << std::endl;
    if ( true == definition.m_hasInterrupts )
    {
        out << "Interrupt Vector: " << definition.m_interruptVector << std::endl;
    }

    out << "== Ports ==" << std::endl;
    out << definition.m_ports << std::endl;

    out << "== Memory ==" << std::endl;
    out << definition.m_memory << std::endl;

    out << "== Stack ==" << std::endl;
    out << definition.m_stack << std::endl;

    out << "== Flags ==" << std::endl;
    out << definition.m_flags << std::endl;

    out << "== Instruction Set ==" << std::endl;
    unsigned int number = 0;
    for ( const AdjSimProcDef::Instruction & i : definition.m_instructionSet )
    {
        out << "Instruction " << number++ << std::endl;
        out << i << std::endl;
    }

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const AdjSimProcDef::Ports & definition )
{
    out << "Address Width: " << definition.m_addrWidth << std::endl;
    out << "Data Width: " << definition.m_dataWidth << std::endl;
    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const AdjSimProcDef::Memory & definition )
{
    out << "Register File Size: " << definition.m_register.m_size << std::endl;
    out << "Register File Banks: " << definition.m_register.m_banks << std::endl;

    out << "Data Memory Size: " << definition.m_data.m_size << std::endl;

    out << "Program Size: " << definition.m_program.m_size << std::endl;
    out << "Program Word Size: " << definition.m_program.m_wordSize << std::endl;
    out << "Program Word Width: ";
    switch ( definition.m_program.m_word  )
    {
        case AdjSimProcDef::Memory::Program::WORD_1B: out << "1B"; break;
        case AdjSimProcDef::Memory::Program::WORD_2B: out << "2B"; break;
        case AdjSimProcDef::Memory::Program::WORD_3B: out << "3B"; break;
    }
    out << std::endl;
    out << "Program Word Endian: ";
    switch ( definition.m_program.m_endian )
    {
        case AdjSimProcDef::Memory::Program::END_BIG: out << "BIG"; break;
        case AdjSimProcDef::Memory::Program::END_LITTLE: out << "LITTLE"; break;
    }
    out << std::endl;

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const AdjSimProcDef::Stack::SimpleStack::Space & definition )
{
    switch ( definition )
    {
        case AdjSimProcDef::Stack::SimpleStack::SP_REG:  out << "REG";  break;
        case AdjSimProcDef::Stack::SimpleStack::SP_DATA: out << "DATA"; break;
    }

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const AdjSimProcDef::Stack & definition )
{
    out << std::boolalpha;

    out << "Size: " << definition.m_size << std::endl;
    out << "Use Designated Stack: " << definition.m_useDesignatedStack << std::endl;

    if ( false == definition.m_useDesignatedStack )
    {
        out << "Simple Stack Operation: ";
        switch ( definition.m_simpleStack.m_operation )
        {
            case AdjSimProcDef::Stack::SimpleStack::OP_PREINC:
                out << "Increment SP before PUSH, decrement after POP.";
                break;
            case AdjSimProcDef::Stack::SimpleStack::OP_POSTINC:
                out << "Increment SP after PUSH, decrement before POP.";
                break;
            case AdjSimProcDef::Stack::SimpleStack::OP_PREDEC:
                out << "Decrement SP before PUSH, increment after POP.";
                break;
            case AdjSimProcDef::Stack::SimpleStack::OP_POSTDEC:
                out << "Decrement SP after PUSH, increment before POP.";
                break;
        }
        out << std::endl;

        out << "Simple Stack Content: ( offset=" << definition.m_simpleStack.m_content.m_offset
            << ", space=" << definition.m_simpleStack.m_content.m_space << " )" << std::endl;

        out << "Simple Stack Pointer: ( space=" << definition.m_simpleStack.m_pointer.m_space
            << ", address=" << definition.m_simpleStack.m_pointer.m_address
            << ", maxSize=" << definition.m_simpleStack.m_pointer.m_maxSize
            << ", indirect=" << definition.m_simpleStack.m_pointer.m_indirect << " )" << std::endl;
    }

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const AdjSimProcDef::Flags & definition )
{
    out << std::boolalpha;

    out << "Has Zero: " << definition.m_hasZero << std::endl;
    out << "Has Carry: " << definition.m_hasCarry << std::endl;
    out << "Has Overflow: " << definition.m_hasOverflow << std::endl;
    out << "Has Negative: " << definition.m_hasNegative << std::endl;
    out << "Has Half: " << definition.m_hasHalfCarry << std::endl;
    out << "Has Parity: " << definition.m_hasParity << std::endl;
    out << "Backup When Interrupted: " << definition.m_backupWhenInterrupted << std::endl;
    out << "Auto Disable Interrupts: " << definition.m_autoDisableInterrupts << std::endl;
    out << "Auto Enable Interrupts: " << definition.m_autoEnableInterrupts << std::endl;

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const AdjSimProcDef::Instruction & definition )
{
    out << "Mnemonic: \"" << definition.m_mnemonic << "\"" << std::endl;
    out << "Operation: " << definition.m_operation << std::endl;
    out << "Time: POS=" << definition.m_time.m_positive << " NEG=" << definition.m_time.m_negative << std::endl;
    out << "Next OP: POS=" << definition.m_next.m_positive << " NEG=" << definition.m_next.m_negative << std::endl;
    out << "Condition: " << definition.m_condition.m_type;
    if ( AdjSimProcDef::Instruction::Condition::T_UNCONDITIONAL != definition.m_condition.m_type )
    {
        out << " for flag " << definition.m_condition.m_flag;
    }
    out << std::endl;

    out << "Flag effect:" << std::endl;
    for ( int i = 0; i < AdjSimProcDef::Instruction::FLAG__MAX__; i++ )
    {
        out << "    " << AdjSimProcDef::Instruction::ProcessorFlag(i) << " - ";
        switch ( definition.m_flags[i] )
        {
            case AdjSimProcDef::Instruction::FE_DO_NOT_CHANGE:    out << "do not change";     break;
            case AdjSimProcDef::Instruction::FE_ALWAYS_SET:       out << "always set";        break;
            case AdjSimProcDef::Instruction::FE_ALWAYS_CLEAR:     out << "always clear";      break;
            case AdjSimProcDef::Instruction::FE_DEPENS_ON_RESULT: out << "depends on result"; break;
        }
        out << std::endl;
    }

    out << "Operation parameters:" << std::endl;
    out << "    Result: ";
    switch ( definition.m_operParameters.m_result )
    {
        case AdjSimProcDef::Instruction::OperParameters::RES_STORE_IN_1ST: out << "store in 1st operand"; break;
        case AdjSimProcDef::Instruction::OperParameters::RES_STORE_IN_3RD: out << "store in 3rd operand"; break;
        case AdjSimProcDef::Instruction::OperParameters::RES_DISCARD:      out << "discard result";       break;
    }
    out << std::endl;
    out << "    Parity: ";
    switch ( definition.m_operParameters.m_parity )
    {
        case AdjSimProcDef::Instruction::OperParameters::P_EVEN: out << "even"; break;
        case AdjSimProcDef::Instruction::OperParameters::P_ODD:  out << "odd";  break;
    }
    out << std::endl;
    out << "    Ignore Carry flag: " << definition.m_operParameters.m_ignoreCarryFlag << std::endl;
    out << "    Ignore Zero flag: " << definition.m_operParameters.m_ignoreZeroFlag << std::endl;

    out << "OP code: 0b";
    for ( int i = ( definition.m_opCode.size() - 1 ); i >= 0 ; i-- )
    {
        switch ( definition.m_opCode[i] )
        {
            case AdjSimProcDef::Instruction::OCB_ZERO:      out << '0'; break;
            case AdjSimProcDef::Instruction::OCB_ONE:       out << '1'; break;
            case AdjSimProcDef::Instruction::OCB_DONT_CARE: out << '-'; break;
        }
    }
    out << std::endl;

    out << "Operands:" << std::endl;
    for ( unsigned int i = 0; i < definition.m_operands.size(); i++ )
    {
        out << "    Operand " << i << std::endl;
        out << "        Type: ";
        switch ( definition.m_operands[i].m_type )
        {
            case AdjSimProcDef::Instruction::Operand::T_IMMEDIATE: out << "IMMEDIATE"; break;
            case AdjSimProcDef::Instruction::Operand::T_REG_DIR:   out << "REG DIR";   break;
            case AdjSimProcDef::Instruction::Operand::T_REG_INDR:  out << "REG INDR";  break;
            case AdjSimProcDef::Instruction::Operand::T_DATA_DIR:  out << "DATA DIR";  break;
            case AdjSimProcDef::Instruction::Operand::T_DATA_INDR: out << "DATA INDR"; break;
            case AdjSimProcDef::Instruction::Operand::T_PROGRAM:   out << "PROGRAM";   break;
            case AdjSimProcDef::Instruction::Operand::T_PORT:      out << "PORT";      break;
        }
        out << std::endl;
        out << "        Size: " << definition.m_operands[i].m_size << std::endl;
        out << "        Fixed Value: " << definition.m_operands[i].m_fixedValue << std::endl;
        out << "        Order/Number: ";
        switch ( definition.m_operands[i].m_number )
        {
            case AdjSimProcDef::Instruction::Operand::N_FIRST:
                out << "1st";
                break;
            case AdjSimProcDef::Instruction::Operand::N_SECOND:
                out << "2nd";
                break;
            case AdjSimProcDef::Instruction::Operand::N_THIRD:
                out << "3rd";
                break;
            case AdjSimProcDef::Instruction::Operand::N_HIDDEN:
                out << "hidden";
                break;
        }
        out << std::endl;

        if ( false == definition.m_operands[i].m_OPCodePermutation.empty() )
        {
            out << "        Permutation: ";
            for ( int j = ( definition.m_operands[i].m_OPCodePermutation.size() - 1 ); j >= 0; j-- )
            {
                out << definition.m_operands[i].m_OPCodePermutation[j];
                if ( 0 != j )
                {
                    out << ", ";
                }
            }
            out << std::endl;
        }
    }

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const AdjSimProcDef::Instruction::ProcessorFlag & flag )
{
    switch ( flag )
    {
        case AdjSimProcDef::Instruction::FLAG_ZERO:        out << "ZERO";        break;
        case AdjSimProcDef::Instruction::FLAG_CARRY:       out << "CARRY";       break;
        case AdjSimProcDef::Instruction::FLAG_OVERFLOW:    out << "OVERFLOW";    break;
        case AdjSimProcDef::Instruction::FLAG_NEGATIVE:    out << "NEGATIVE";    break;
        case AdjSimProcDef::Instruction::FLAG_HALF_CARRY:  out << "HALF_CARRY";  break;
        case AdjSimProcDef::Instruction::FLAG_INTR_ENABLE: out << "INTR_ENABLE"; break;
        case AdjSimProcDef::Instruction::FLAG_PARITY:      out << "PARITY";      break;
        case AdjSimProcDef::Instruction::FLAG_FLAG:        out << "FLAG";        break;
        case AdjSimProcDef::Instruction::FLAG__MAX__:      out << "<invalid>";   break;
    }

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const AdjSimProcDef::Instruction::Operation operation )
{
    switch ( operation )
    {
        case AdjSimProcDef::Instruction::OP_NONE:          out << "NONE";                  break;
        case AdjSimProcDef::Instruction::OP_ABS_JUMP:      out << "ABS JUMP";              break;
        case AdjSimProcDef::Instruction::OP_ABS_CALL:      out << "ABS CALL";              break;
        case AdjSimProcDef::Instruction::OP_REL_JUMP:      out << "REL CALL";              break;
        case AdjSimProcDef::Instruction::OP_REL_CALL:      out << "REL CALL";              break;
        case AdjSimProcDef::Instruction::OP_OFS_JUMP:      out << "OFS CALL";              break;
        case AdjSimProcDef::Instruction::OP_OFS_CALL:      out << "OFS CALL";              break;
        case AdjSimProcDef::Instruction::OP_IDX_JUMP:      out << "IDX CALL";              break;
        case AdjSimProcDef::Instruction::OP_IDX_CALL:      out << "IDX CALL";              break;
        case AdjSimProcDef::Instruction::OP_RETURN:        out << "RETURN";                break;
        case AdjSimProcDef::Instruction::OP_ISR_RETURN:    out << "ISR RETURN";            break;
        case AdjSimProcDef::Instruction::OP_SET_BANK:      out << "SET BANK";              break;
        case AdjSimProcDef::Instruction::OP_MOVE:          out << "MOVE";                  break;
        case AdjSimProcDef::Instruction::OP_CB_MOVE:       out << "CROSS BANK MOVE";       break;
        case AdjSimProcDef::Instruction::OP_MOVE_BIT:      out << "MOVE BIT";              break;
        case AdjSimProcDef::Instruction::OP_CB_MOVE_BIT:   out << "CROSS BANK MOVE BIT";   break;
        case AdjSimProcDef::Instruction::OP_SWAP:          out << "SWAP";                  break;
        case AdjSimProcDef::Instruction::OP_CB_SWAP:       out << "CROSS BANK SWAP";       break;
        case AdjSimProcDef::Instruction::OP_SWAP_BIT:      out << "SWAP BIT";              break;
        case AdjSimProcDef::Instruction::OP_CB_SWAP_BIT:   out << "CROSS BANK SWAPBIT";    break;
        case AdjSimProcDef::Instruction::OP_CPL:           out << "CPL";                   break;
        case AdjSimProcDef::Instruction::OP_BIT_TEST:      out << "BIT TEST";              break;
        case AdjSimProcDef::Instruction::OP_ADD:           out << "ADD";                   break;
        case AdjSimProcDef::Instruction::OP_SUB:           out << "SUB";                   break;
        case AdjSimProcDef::Instruction::OP_AND:           out << "AND";                   break;
        case AdjSimProcDef::Instruction::OP_OR:            out << "OR";                    break;
        case AdjSimProcDef::Instruction::OP_XOR:           out << "XOR";                   break;
        case AdjSimProcDef::Instruction::OP_SHIFT_LEFT_0:  out << "SHIFT LEFT 0";          break;
        case AdjSimProcDef::Instruction::OP_SHIFT_RIGHT_0: out << "SHIFT RIGHT 0";         break;
        case AdjSimProcDef::Instruction::OP_SHIFT_LEFT_1:  out << "SHIFT LEFT 1";          break;
        case AdjSimProcDef::Instruction::OP_SHIFT_RIGHT_1: out << "SHIFT RIGHT 1";         break;
        case AdjSimProcDef::Instruction::OP_SHIFT_LEFT_R:  out << "SHIFT LEFT R";          break;
        case AdjSimProcDef::Instruction::OP_SHIFT_RIGHT_R: out << "SHIFT RIGHT R";         break;
        case AdjSimProcDef::Instruction::OP_SHIFT_LEFT_C:  out << "SHIFT LEFT C";          break;
        case AdjSimProcDef::Instruction::OP_SHIFT_RIGHT_C: out << "SHIFT RIGHT C";         break;
        case AdjSimProcDef::Instruction::OP_ROTATE_LEFT:   out << "ROTATE LEFT";           break;
        case AdjSimProcDef::Instruction::OP_ROTATE_RIGHT:  out << "ROTATE RIGHT";          break;

        default:
            out << "<ERROR:Unknown!>";
            break;
    }

    return out;
}