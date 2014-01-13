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
 * @file DAsm.cxx
 */
// =============================================================================

#include "DAsm.h"

DAsm::~DAsm()
{
}

DAsm::Config::Config()
{
    m_symbolsToGenerate = SymbolsToGenerate ( STG_CODE | STG_DATA | STG_REG | STG_PORT | STG_CONST );

    m_tabSize     = 8;
    m_eol         = EOF_LF;
    m_indentation = IND_SPACES;
    m_letterCase  = LC_LOWERCASE;
    m_radix       = R_HEX;
}

std::ostream & operator << ( std::ostream & out,
                             const DAsm & dAsm )
{
    const char * eol = "\n";

    switch ( dAsm.m_config.m_eol )
    {
        case DAsm::Config::EOF_LF:   eol = "\n";   break;
        case DAsm::Config::EOF_CR:   eol = "\r";   break;
        case DAsm::Config::EOF_CRLF: eol = "\r\n"; break;
    }

    for ( const auto & src : dAsm.getSourceCode() )
    {
        out << src << eol;
    }

    return out;
}
