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
 * @ingroup Disassembler
 * @file DAsmAdaptable.cxx
 */
// =============================================================================

#include "DAsmAdaptable.h"

// Used for i18n only.
#include <QObject>

bool DAsmAdaptable::disassemble ( const DataFile & source )
{
    unsigned int addr = 0;
    bool result = true;

    int initShift = 0;
    switch ( m_procDef.m_memory.m_program.m_word )
    {
        case AdjSimProcDef::Memory::Program::WORD_1B: initShift = 0;  break;
        case AdjSimProcDef::Memory::Program::WORD_2B: initShift = 8;  break;
        case AdjSimProcDef::Memory::Program::WORD_3B: initShift = 16; break;
    }

    m_lastAddr = 0;
    for ( unsigned int i = 0; i < source.maxSize(); )
    {
        unsigned int code = 0;

        for ( int shift = initShift; shift >= 0; shift -= 8 )
        {
            int byte = -1;
            if ( i < source.maxSize() )
            {
                byte = source [ i++ ];
            }

            if ( -1 == byte )
            {
                if ( initShift == shift )
                {
                    code |= 0x20000000;
                }
                else
                {
                    code |= 0x10000000;
                }
                continue;
            }

            code |= ( byte << ( ( ( AdjSimProcDef::Memory::Program::END_BIG == m_procDef.m_memory.m_program.m_endian )
                                  ? 0 : initShift ) - shift ) );
        }

        if ( 0 == ( 0x20000000 & code ) )
        {
            if ( 0x10000000 & code )
            {
                result = false;
                m_messages.push_back ( QObject::tr("Incomplete OP code at address: %1.").arg(addr).toStdString() );
            }
            else if ( false == phase1(code, addr) )
            {
                m_messages.push_back ( QObject::tr("Invalid OP code at address: %1.").arg(addr).toStdString() );
            }
        }

        addr++;
    }

    phase1Leave();

    addr = 0;
    m_lastAddr = 0;
    for ( unsigned int i = 0; i < source.maxSize(); )
    {
        unsigned int code = 0;
        for ( int shift = initShift; shift >= 0; shift -= 8 )
        {
            int byte = -1;
            if ( i < source.maxSize() )
            {
                byte = source [ i++ ];
            }

            if ( -1 == byte )
            {
                code |= 0x20000000;
                continue;
            }
            code |= ( byte << ( ( ( AdjSimProcDef::Memory::Program::END_BIG == m_procDef.m_memory.m_program.m_endian )
                                  ? 0 : initShift ) - shift ) );
        }

        if ( 0 == ( 0x20000000 & code ) )
        {
            phase2(code, addr);
        }

        addr++;
    }

    return result;
}

bool DAsmAdaptable::phase1 ( unsigned int code,
                             unsigned int addr )
{
    AdjSimProcDef::Instruction instruction;
    if ( false == recognizeInstruction(code, instruction) )
    {
        return false;
    }

    std::vector<int> operands;
    getOperands(code, instruction, operands);

    m_usedCode.insert(addr);

    for ( unsigned int i = 0; i < operands.size(); i++ )
    {
        if ( -1 == operands[i] )
        {
            continue;
        }

        switch ( instruction.m_operands[i].m_type )
        {
            case AdjSimProcDef::Instruction::Operand::T_IMMEDIATE:
                    m_addresses[CONST].insert(operands[i]);
                    break;
            case AdjSimProcDef::Instruction::Operand::T_REG_DIR:
            case AdjSimProcDef::Instruction::Operand::T_REG_INDR:
                    m_addresses[REG].insert(operands[i]);
                    break;
            case AdjSimProcDef::Instruction::Operand::T_DATA_DIR:
            case AdjSimProcDef::Instruction::Operand::T_DATA_INDR:
                    m_addresses[DATA].insert(operands[i]);
                    break;
            case AdjSimProcDef::Instruction::Operand::T_PROGRAM:
                    m_addresses[CODE].insert(operands[i]);
                    break;
            case AdjSimProcDef::Instruction::Operand::T_PORT:
                    m_addresses[PORT].insert(operands[i]);
                    break;
        }
    }

    return true;
}

void DAsmAdaptable::phase2 ( unsigned int code,
                             unsigned int addr )
{
    m_lastAddr++;
    if ( addr != m_lastAddr )
    {
        m_code.push_back(std::string());
        std::string & ln = m_code.back();

        m_lastAddr = addr;

        indent(ln, 16);
        appendStr(ln, "ORG");

        indent(ln, 32);
        appendStr(ln, num2str(addr));
    }

    m_code.push_back(std::string());
    std::string & line = m_code.back();

    if ( m_addresses[CODE].end() != m_addresses[CODE].find(addr) )
    {
        label(line, addr, true);
    }

    AdjSimProcDef::Instruction instruction;
    if ( false == recognizeInstruction(code, instruction) )
    {
        appendStr(line, "DB");
        indent(line, 32);
        appendStr(line, num2str(code));
        return;
    }

    indent(line, 16);

    appendStr(line, instruction.m_mnemonic);
    indent(line, 32);
    
}

void DAsmAdaptable::getOperands ( unsigned int code,
                                  const AdjSimProcDef::Instruction & instruction,
                                  std::vector<int> & operands )
{
    operands.clear();

    for ( const AdjSimProcDef::Instruction::Operand & operand : instruction.m_operands )
    {
        int value = 0;
        int bitNumber = 0;

        if ( true == operand.m_OPCodePermutation.empty() )
        {
            value = -1;
        }
        else
        {
            for ( int perm : operand.m_OPCodePermutation )
            {
                value |= ( ( ( code & ( 1 << perm ) ) >> perm ) << bitNumber++ );
            }
        }

        operands.push_back(value);
    }
}

bool DAsmAdaptable::recognizeInstruction ( unsigned int code,
                                           AdjSimProcDef::Instruction & instruction )
{
    for ( const AdjSimProcDef::Instruction & inst : m_procDef.m_instructionSet )
    {
        bool found = true;
        unsigned int bitNo = 0;
        for ( const AdjSimProcDef::Instruction::OpCodeBit bit : inst.m_opCode )
        {
            if ( AdjSimProcDef::Instruction::OCB_ZERO == bit )
            {
                if ( 0 != ( code & ( 1 << bitNo ) ) )
                {
                    found = false;
                    break;
                }
            }
            else if ( AdjSimProcDef::Instruction::OCB_ONE == bit )
            {
                if ( 0 == ( code & ( 1 << bitNo ) ) )
                {
                    found = false;
                    break;
                }
            }
            bitNo++;
        }

        if ( true == found )
        {
            instruction = inst;
            return true;
        }
    }

    return false;
}

const std::vector<std::string> & DAsmAdaptable::getSourceCode() const
{
    return m_code;
}

const std::vector<std::string> & DAsmAdaptable::getMessages() const
{
    return m_messages;
}

void DAsmAdaptable::clear()
{
    for ( int i = 0; i < AS_MAX_; i++ )
    {
        m_addresses[i].clear();
        m_symbols[i].clear();
    }

    m_lastAddr = 0;
    m_code.clear();
    m_messages.clear();
    m_usedCode.clear();
}


inline void DAsmAdaptable::phase1Leave()
{
    static const struct
    {
        Config::SymbolsToGenerate m_stg;
        AddressSpace m_as;
        const char * m_dir;
        const char * m_prefix;
        bool m_isCode;
    } spec[] = {
        { Config::STG_CODE,  CODE,    "CODE",  "LABEL", true  },
        { Config::STG_DATA,  DATA,    "DATA",  "D",     false },
        { Config::STG_REG,   REG,     "REG",   "R",     false },
        { Config::STG_PORT,  PORT,    "PORT",  "P",     false },
        { Config::STG_CONST, CONST,   "EQU",   "C",     false },
        { Config::STG_NONE,  AS_MAX_, nullptr, nullptr, false }
    };

    for ( int i = 0; Config::STG_NONE != spec[i].m_stg; i++ )
    {
        if ( spec[i].m_stg & m_config.m_symbolsToGenerate )
        {
            unsigned int symbolIndex = 0;
            char symbolIndexStr[16];
            for ( const unsigned int addr : m_addresses[spec[i].m_as] )
            {
                sprintf(symbolIndexStr, "%d", symbolIndex);

                if ( ( false == spec[i].m_isCode ) || ( m_usedCode.end() == m_usedCode.find(addr) ) )
                {
                    m_code.push_back(std::string());
                    std::string & line = m_code.back();

                    appendStr(line, std::string(spec[i].m_prefix) + symbolIndexStr);

                    indent(line, 16);
                    appendStr(line, spec[i].m_dir);

                    indent(line, 32);
                    appendStr(line, num2str(addr));
                }

                m_symbols[spec[i].m_as].insert ( std::pair<unsigned int,std::string>
                                                 ( addr, ( letterCase(spec[i].m_prefix) + symbolIndexStr ) ) );
                symbolIndex++;
            }
        }
    }
}

inline void DAsmAdaptable::appendAddr ( std::string & line,
                                        unsigned int addr,
                                        AddressSpace addrSp,
                                        Config::SymbolsToGenerate stg )
{
    if ( stg & m_config.m_symbolsToGenerate )
    {
        auto it = m_symbols[addrSp].find(addr);
        if ( it != m_symbols[addrSp].end() )
        {
            appendStr ( line, m_symbols[addrSp].find(addr)->second );
            return;
        }
    }

    appendStr ( line, num2str(addr) );
}

void DAsmAdaptable::reg ( std::string & line,
                          unsigned int addr )
{
    appendAddr ( line, addr, REG, Config::STG_REG );
}

void DAsmAdaptable::port ( std::string & line,
                           unsigned int addr )
{
    appendAddr ( line, addr, PORT, Config::STG_PORT );
}

void DAsmAdaptable::data ( std::string & line,
                           unsigned int addr )
{
    appendAddr ( line, addr, DATA, Config::STG_DATA );
}

void DAsmAdaptable::imm ( std::string & line,
                          unsigned int addr )
{
    appendAddr ( line, addr, CONST, Config::STG_CONST );
}

void DAsmAdaptable::label ( std::string & line,
                            unsigned int addr,
                            bool definition )
{
    appendAddr ( line, addr, CODE, Config::STG_CODE );

    if ( true == definition )
    {
        line += ':';
    }
}
