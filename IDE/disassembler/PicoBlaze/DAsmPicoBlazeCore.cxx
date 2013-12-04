// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup DisasmPicoBlaze
 * @file DAsmPicoBlazeCore.cxx
 */
// =============================================================================

#include "DAsmPicoBlazeCore.h"

// Used for i18n only.
#include <QObject>

bool DAsmPicoBlazeCore::disassemble ( const DataFile & source )
{
    bool result = true;
    unsigned int addr = 0;

    m_lastAddr = 0;
    for ( unsigned int i = 0; i < source.maxSize(); )
    {
        unsigned int code = 0;
        for ( int shift = 16; shift >= 0; shift -= 8 )
        {
            int byte = -1;
            if ( i < source.maxSize() )
            {
                byte = source [ i++ ];
            }

            if ( -1 == byte )
            {
                if ( 16 == shift )
                {
                    code |= 0x200000;
                }
                else
                {
                    code |= 0x100000;
                }
                continue;
            }

            code |= ( byte << shift );
        }

        if ( 0 == ( 0x200000 & code ) )
        {
            if ( 0x100000 & code )
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
        for ( int shift = 16; shift >= 0; shift -= 8 )
        {
            int byte = -1;
            if ( i < source.maxSize() )
            {
                byte = source [ i++ ];
            }

            if ( -1 == byte )
            {
                code |= 0x200000;
                continue;
            }
            code |= ( byte << shift );
        }

        if ( 0 == ( 0x200000 & code ) )
        {
            phase2(code, addr);
        }

        addr++;
    }

    return result;
}

const std::vector<std::string> & DAsmPicoBlazeCore::getSourceCode() const
{
    return m_code;
}

const std::vector<std::string> & DAsmPicoBlazeCore::getMessages() const
{
    return m_messages;
}

void DAsmPicoBlazeCore::clear()
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


inline void DAsmPicoBlazeCore::phase1Leave()
{
    static const struct
    {
        Config::SymbolsToGenerate m_stg;
        AddressSpace m_as;
        const char * m_dir;
        const char * m_prefix;
        bool m_isCode;
    } spec[] = {
        { Config::STG_CODE,  CODE,    "CODE", "LABEL", true  },
        { Config::STG_DATA,  DATA,    "DATA", "D",     false },
        { Config::STG_REG,   REG,     "REG",  "R",     false },
        { Config::STG_PORT,  PORT,    "PORT", "P",     false },
        { Config::STG_CONST, CONST,   "EQU",  "C",     false },
        { Config::STG_NONE,  AS_MAX_, NULL,   NULL,    false }
    };

    for ( int i = 0; Config::STG_NONE != spec[i].m_stg; i++ )
    {
        if ( spec[i].m_stg & m_config.m_symbolsToGenerate )
        {
            unsigned int symbolIndex = 0;
            char symbolIndexStr[16];
            for ( std::set<unsigned int>::iterator it = m_addresses[spec[i].m_as].begin();
                  it != m_addresses[spec[i].m_as].end();
                  it++ )
            {
                sprintf(symbolIndexStr, "%d", symbolIndex);

                if ( ( false == spec[i].m_isCode ) || ( m_usedCode.end() == m_usedCode.find(*it) ) )
                {
                    m_code.push_back(std::string());
                    std::string & line = m_code.back();

                    appendStr(line, std::string(spec[i].m_prefix) + symbolIndexStr);

                    indent(line, 16);
                    appendStr(line, spec[i].m_dir);

                    indent(line, 32);
                    appendStr(line, num2str(*it));
                }

                m_symbols[spec[i].m_as].insert ( std::pair<unsigned int,std::string>
                                                 ( *it, ( letterCase(spec[i].m_prefix) + symbolIndexStr ) ) );
                symbolIndex++;
            }
        }
    }
}

inline void DAsmPicoBlazeCore::appendAddr ( std::string & line,
                                            unsigned int addr,
                                            AddressSpace addrSp,
                                            Config::SymbolsToGenerate stg )
{
    if ( stg & m_config.m_symbolsToGenerate )
    {
        std::map<unsigned int,std::string>::const_iterator it = m_symbols[addrSp].find(addr);
        if ( it != m_symbols[addrSp].end() )
        {
            appendStr ( line, m_symbols[addrSp].find(addr)->second );
            return;
        }
    }

    appendStr ( line, num2str(addr) );
}

void DAsmPicoBlazeCore::reg ( std::string & line,
                              unsigned int addr )
{
    appendAddr ( line, addr, REG, Config::STG_REG );
}

void DAsmPicoBlazeCore::port ( std::string & line,
                               unsigned int addr )
{
    appendAddr ( line, addr, PORT, Config::STG_PORT );
}

void DAsmPicoBlazeCore::data ( std::string & line,
                               unsigned int addr )
{
    appendAddr ( line, addr, DATA, Config::STG_DATA );
}

void DAsmPicoBlazeCore::imm ( std::string & line,
                              unsigned int addr )
{
    appendAddr ( line, addr, CONST, Config::STG_CONST );
}

void DAsmPicoBlazeCore::label ( std::string & line,
                                unsigned int addr,
                                bool definition )
{
    appendAddr ( line, addr, CODE, Config::STG_CODE );

    if ( true == definition )
    {
        line += ':';
    }
}
