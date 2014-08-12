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
    for ( const AdjSimProcDef::Instruction & i : definition.m_instructionSet )
    {
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
    out << "Has Half: " << definition.m_hasHalf << std::endl;
    out << "Has Parity: " << definition.m_hasParity << std::endl;
    out << "Backup When Interruped: " << definition.m_backupWhenInterruped << std::endl;
    out << "Auto Disable Interrputs: " << definition.m_autoDisableInterrputs << std::endl;
    out << "Auto Enable Interrputs: " << definition.m_autoEnableInterrputs << std::endl;

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const AdjSimProcDef::Instruction & definition )
{

}
