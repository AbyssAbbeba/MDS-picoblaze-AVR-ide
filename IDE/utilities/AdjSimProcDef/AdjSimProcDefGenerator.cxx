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
 * @file AdjSimProcDefGenerator.cxx
 */
// =============================================================================

#include "AdjSimProcDefGenerator.h"

#include <cstdio>

const char * AdjSimProcDefGenerator::FILE_HEADER = "MDS processor definition file";
const char * AdjSimProcDefGenerator::FILE_VERSION = "1.1";

inline void writeInt ( std::string & target,
                       char * buffer,
                       unsigned int value )
{
    sprintf ( buffer, "%x", value );
    target += buffer;
    target += '\b';
}

inline void writeInt ( std::string & target,
                       char * buffer,
                       int value )
{
    sprintf ( buffer, "%x", value );
    target += buffer;
    target += '\b';
}

inline void writeBool ( std::string & target,
                        bool value )
{
    target += ( value ? '1' : '0' );
    target += '\b';
}

AdjSimProcDefGenerator::AdjSimProcDefGenerator ( const AdjSimProcDef & def )
{
    char buffer[16];

    // Header and Version
    m_data += FILE_HEADER;
    m_data += '\b';
    m_data += FILE_VERSION;
    m_data += '\b';

    // Processor Name, Description, and Fail jump
    m_data += def.m_name;
    m_data += '\b';
    m_data += def.m_description;
    m_data += '\b';
    m_data += def.m_failjmp;
    m_data += '\b';

    // Interrupt Vector
    writeBool ( m_data, def.m_hasInterrupts );
    writeInt ( m_data, buffer, def.m_interruptVector );

    // Ports
    writeInt ( m_data, buffer, def.m_ports.m_addrWidth );
    writeInt ( m_data, buffer, def.m_ports.m_dataWidth );

    // Memory
    writeInt ( m_data, buffer, def.m_memory.m_register.m_size );
    writeInt ( m_data, buffer, def.m_memory.m_register.m_banks );
    writeInt ( m_data, buffer, def.m_memory.m_data.m_size );
    writeInt ( m_data, buffer, def.m_memory.m_program.m_size );
    writeInt ( m_data, buffer, def.m_memory.m_program.m_wordSize );
    writeInt ( m_data, buffer, def.m_memory.m_program.m_word );
    writeInt ( m_data, buffer, def.m_memory.m_program.m_endian );

    // Stack
    writeInt ( m_data, buffer, def.m_stack.m_size );
    writeBool ( m_data, def.m_stack.m_useDesignatedStack );
    writeInt ( m_data, buffer, def.m_stack.m_simpleStack.m_operation );
    writeInt ( m_data, buffer, def.m_stack.m_simpleStack.m_content.m_space );
    writeInt ( m_data, buffer, def.m_stack.m_simpleStack.m_content.m_offset );
    writeInt ( m_data, buffer, def.m_stack.m_simpleStack.m_pointer.m_space );
    writeInt ( m_data, buffer, def.m_stack.m_simpleStack.m_pointer.m_address );
    writeInt ( m_data, buffer, def.m_stack.m_simpleStack.m_pointer.m_maxSize );
    writeBool ( m_data, def.m_stack.m_simpleStack.m_pointer.m_indirect );

    // Flags
    writeBool ( m_data, def.m_flags.m_backupWhenInterrupted );
    writeBool ( m_data, def.m_flags.m_autoDisableInterrupts );
    writeBool ( m_data, def.m_flags.m_autoEnableInterrupts );
    writeBool ( m_data, def.m_flags.m_hasZero );
    writeBool ( m_data, def.m_flags.m_hasCarry );
    writeBool ( m_data, def.m_flags.m_hasOverflow );
    writeBool ( m_data, def.m_flags.m_hasNegative );
    writeBool ( m_data, def.m_flags.m_hasHalfCarry );
    writeBool ( m_data, def.m_flags.m_hasParity );

    // Instruction Set
    for ( const AdjSimProcDef::Instruction & instruction : def.m_instructionSet )
    {
        // <MARK: (re-)start>
        writeBool ( m_data, true );

        // Next
        writeInt ( m_data, buffer, instruction.m_next.m_positive );
        writeInt ( m_data, buffer, instruction.m_next.m_negative );

        // Time
        writeInt ( m_data, buffer, instruction.m_time.m_positive );
        writeInt ( m_data, buffer, instruction.m_time.m_negative );

        // Flag Effect
        for ( int i = 0; i < AdjSimProcDef::Instruction::FLAG__MAX__; i++ )
        {
            writeInt ( m_data, buffer, instruction.m_flags[i] );
        }

        // Condition
        writeInt ( m_data, buffer, instruction.m_condition.m_type );
        writeInt ( m_data, buffer, instruction.m_condition.m_flag );

        // Operation
        writeInt ( m_data, buffer, instruction.m_operation );

        // Operation Parameters
        writeInt ( m_data, buffer, instruction.m_operParameters.m_result );
        writeInt ( m_data, buffer, instruction.m_operParameters.m_parity );
        writeBool ( m_data, instruction.m_operParameters.m_ignoreCarryFlag );
        writeBool ( m_data, instruction.m_operParameters.m_ignoreZeroFlag );

        // Mnemonic
        m_data += instruction.m_mnemonic;
        m_data += '\b';

        // OP Code Bits
        writeInt ( m_data, buffer, (unsigned int) instruction.m_opCode.size() );
        for ( AdjSimProcDef::Instruction::OpCodeBit bit : instruction.m_opCode )
        {
            writeInt ( m_data, buffer, bit );
        }

        // Operands
        writeInt ( m_data, buffer, (unsigned int) instruction.m_operands.size() );
        for ( const AdjSimProcDef::Instruction::Operand & operand : instruction.m_operands )
        {
            writeInt ( m_data, buffer, operand.m_type );
            writeInt ( m_data, buffer, operand.m_number );
            writeInt ( m_data, buffer, operand.m_size );
            writeInt ( m_data, buffer, operand.m_fixedValue );

            writeInt ( m_data, buffer, (unsigned int) operand.m_OPCodePermutation.size() );
            for ( int perm : operand.m_OPCodePermutation )
            {
                writeInt ( m_data, buffer, perm );
            }
        }
    }

    // <MARK: end>
    writeBool ( m_data, false );
}
