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
 * @file AdjSimProcDefParser.cxx
 */
// =============================================================================

#include "AdjSimProcDefParser.h"
#include "AdjSimProcDefGenerator.h"

#include <cstdlib>
#include <stdexcept>

inline void readString ( const std::string & def,
                         std::string & token,
                         size_t & pos )
{
    size_t posOrig = pos;
    pos = def.find('\n', posOrig);

    if ( std::string::npos == pos )
    {
        throw std::runtime_error("Definition ends prematurely.");
    }
    else
    {
        token = def.substr(posOrig, pos - posOrig);
        pos++;
    }
}

inline void readUInt ( const std::string & def,
                       unsigned int & value,
                       size_t & pos )
{
    size_t posOrig = pos;
    pos = def.find('\n', posOrig);

    if ( std::string::npos == pos )
    {
        throw std::runtime_error("Definition ends prematurely.");
    }
    else
    {
        std::string token = def.substr(posOrig, pos - posOrig);
        value = (unsigned int) strtoul(token.c_str(), NULL, 16);
        pos++;
    }
}

inline void readInt ( const std::string & def,
                      int & value,
                      size_t & pos )
{
    size_t posOrig = pos;
    pos = def.find('\n', posOrig);

    if ( std::string::npos == pos )
    {
        throw std::runtime_error("Definition ends prematurely.");
    }
    else
    {
        std::string token = def.substr(posOrig, pos - posOrig);
        value = (int) strtoul(token.c_str(), NULL, 16);
        pos++;
    }
}

inline void readBool ( const std::string & def,
                       bool & value,
                       size_t & pos )
{
    size_t posOrig = pos;
    pos = def.find('\n', posOrig);

    if ( std::string::npos == pos )
    {
        throw std::runtime_error("Definition ends prematurely.");
    }
    else
    {
        std::string token = def.substr(posOrig, pos - posOrig);
        if ( 0 == token.compare("0") )
        {
            value = false;
        }
        else
        {
            value = true;
        }
        pos++;
    }
}

AdjSimProcDefParser::AdjSimProcDefParser ( const std::string & def )
{
    size_t pos = 0;
    int value;
    unsigned int size;
    std::string token;
    bool mark;

    m_valid = true;

    try
    {
        // Header and Version
        readString(def, token, pos);
        if ( 0 != token.compare(AdjSimProcDefGenerator::FILE_HEADER) )
        {
            throw std::runtime_error("Unknown file format.");
        }
        readString(def, token, pos);
        if ( 0 != token.compare(AdjSimProcDefGenerator::FILE_VERSION) )
        {
            throw std::runtime_error("Incompatible file version.");
        }

        // Processor Name & Description
        readString(def, m_data.m_name, pos);
        readString(def, m_data.m_description, pos);

        // Interrupt Vector
        readBool(def, m_data.m_hasInterrupts, pos);
        readUInt(def, m_data.m_interruptVector, pos);

        // Ports
        readUInt(def, m_data.m_ports.m_addrWidth, pos);
        readUInt(def, m_data.m_ports.m_dataWidth, pos);

        // Memory
        readUInt(def, m_data.m_memory.m_register.m_size, pos);
        readUInt(def, m_data.m_memory.m_register.m_banks, pos);
        readUInt(def, m_data.m_memory.m_data.m_size, pos);
        readUInt(def, m_data.m_memory.m_program.m_size, pos);
        readInt(def, (int&) m_data.m_memory.m_program.m_word, pos);
        readInt(def, (int&) m_data.m_memory.m_program.m_endian, pos);

        // Stack
        readUInt(def, m_data.m_stack.m_size, pos);
        readBool(def, m_data.m_stack.m_useDesignatedStack, pos);
        readInt(def, (int&) m_data.m_stack.m_simpleStack.m_operation, pos);
        readInt(def, (int&) m_data.m_stack.m_simpleStack.m_content.m_space, pos);
        readInt(def, m_data.m_stack.m_simpleStack.m_content.m_offset, pos);
        readInt(def, (int&) m_data.m_stack.m_simpleStack.m_pointer.m_space, pos);
        readInt(def, m_data.m_stack.m_simpleStack.m_pointer.m_address, pos);
        readInt(def, m_data.m_stack.m_simpleStack.m_pointer.m_maxSize, pos);
        readBool(def, m_data.m_stack.m_simpleStack.m_pointer.m_indirect, pos);

        // Flags
        readBool(def, m_data.m_flags.m_backupWhenInterrupted, pos);
        readBool(def, m_data.m_flags.m_autoDisableInterrupts, pos);
        readBool(def, m_data.m_flags.m_autoEnableInterrupts, pos);
        readBool(def, m_data.m_flags.m_hasZero, pos);
        readBool(def, m_data.m_flags.m_hasCarry, pos);
        readBool(def, m_data.m_flags.m_hasOverflow, pos);
        readBool(def, m_data.m_flags.m_hasNegative, pos);
        readBool(def, m_data.m_flags.m_hasHalfCarry, pos);
        readBool(def, m_data.m_flags.m_hasParity, pos);

        m_data.m_instructionSet.clear();
        while ( true )
        {
            readBool(def, mark, pos);
            if ( false == mark )
            {
                break;
            }

            AdjSimProcDef::Instruction instruction;

            // Next
            readInt(def, instruction.m_next.m_positive, pos);
            readInt(def, instruction.m_next.m_negative, pos);

            // Time
            readUInt(def, instruction.m_time.m_positive, pos);
            readUInt(def, instruction.m_time.m_negative, pos);

            // Flag Effect
            for ( int i = 0; i < AdjSimProcDef::Instruction::FLAG__MAX__; i++ )
            {
                readInt(def, (int&) instruction.m_flags[i], pos );
            }

            // Condition
            readInt(def, (int&) instruction.m_condition.m_type, pos );
            readInt(def, (int&) instruction.m_condition.m_flag, pos );

            // Operation
            readInt(def, (int&) instruction.m_operation, pos );

            // Operation Parameters
            readInt(def, (int&) instruction.m_operParameters.m_result, pos );
            readInt(def, (int&) instruction.m_operParameters.m_parity, pos );
            readBool(def, instruction.m_operParameters.m_ignoreCarryFlag, pos);

            // Mnemonic
            readString(def, instruction.m_mnemonic, pos);

            // OP Code Bits
            readUInt(def, size, pos );
            instruction.m_opCode.clear();
            for ( unsigned int i = 0; i < size; i++ )
            {
                readInt(def, value, pos );
                instruction.m_opCode.push_back(AdjSimProcDef::Instruction::OpCodeBit(value));
            }

            // Operands
            readUInt(def, size, pos );
            instruction.m_operands.clear();
            for ( unsigned int i = 0; i < size; i++ )
            {
                AdjSimProcDef::Instruction::Operand operand;

                readInt(def, (int&) operand.m_type, pos );
                readInt(def, operand.m_size, pos );
                readInt(def, operand.m_fixedValue, pos );

                readUInt(def, size, pos );
                for ( unsigned int i = 0; i < size; i++ )
                {
                    readInt(def, value, pos );
                    operand.m_OPCodePermutation.push_back(value);
                }

                instruction.m_operands.push_back(operand);
            }

            m_data.m_instructionSet.push_back(instruction);
        }
    }
    catch ( const std::runtime_error & e )
    {
        m_valid = false;
    }
}
