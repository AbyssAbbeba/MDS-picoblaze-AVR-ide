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
 * @file AdjSimProcDef.cxx
 */
// =============================================================================

#include "AdjSimProcDef.h"

std::ostream & operator << ( std::ostream & out,
                             const AdjSimProcDef & definition )
{
    out << "Name: \"" << definition.m_name << '"' << std::endl;
    out << "Interrupt Vector: " << definition.m_interruptVector << std::endl;

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

    out << "OP code: 0b";
    for ( int i = ( definition.m_opCode.size() - 1 ); i >= 0 ; i-- )
    {
        switch ( definition.m_opCode[i] )
        {
            case AdjSimProcDef::Instruction::OCB_ZERO:      out << '0';
            case AdjSimProcDef::Instruction::OCB_ONE:       out << '1';
            case AdjSimProcDef::Instruction::OCB_DONT_CARE: out << '-';
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
        if ( false == definition.m_operands[i].m_OPCodePermutation.empty() )
        {
            out << "        Permutation: ";
            for ( int j = ( definition.m_operands[i].m_OPCodePermutation.size() - 1 ); j >= 0; j-- )
            {
                out << definition.m_operands[i].m_OPCodePermutation[j];
                if ( 0 != i )
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
