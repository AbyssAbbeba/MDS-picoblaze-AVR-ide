// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup Kcpsm3Asm
 * @file AsmKcpsm3SymbolTable.cxx
 */
// =============================================================================

#include "AsmKcpsm3SymbolTable.h"

void AsmKcpsm3SymbolTable::addSymbol ( CompilerExpr * const name,
                                       StatementTypes::StatementType type,
                                       CompilerExpr * const value )
{
    using namespace StatementTypes;

    SymbolType symbolType;

    switch ( type )
    {
        case ASMKCPSM3_DIR_EQU: symbolType = STYPE_NUMBER; break;
        case ASMKCPSM3_LABEL:   symbolType = STYPE_LABEL;  break;
        default:
            symbolType = STYPE_INVALID;
            break;
    }

    m_table.push_back ( Symbol ( name->m_lValue.m_data.m_symbol, symbolType, value ) );
}

unsigned int AsmKcpsm3SymbolTable::resolveExpr ( CompilerExpr * expr,
                                                 int bitsMax )
{
//     if ( -1 != bitsMax )
//     {
//         unsigned int mask = 0;
//         for ( int i = 0; i < bitsMax; i++ )
//         {
//             mask |= ( 1 << i );
//         }
//     }

    return 123;
}

std::ostream & operator << ( std::ostream & out,
                             const AsmKcpsm3SymbolTable * symbolTable )
{
    for ( std::vector<AsmKcpsm3SymbolTable::Symbol>::const_iterator i = symbolTable->m_table.begin();
          i != symbolTable->m_table.end();
          i++ )
    {
        out << i->m_name;
        out << " | ";
        out << i->m_type;
        out << " = ";
        out << i->m_value;
        out << std::endl;
    }

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const AsmKcpsm3SymbolTable::SymbolType symbolType )
{
    switch ( symbolType )
    {
        case AsmKcpsm3SymbolTable::STYPE_INVALID: out << "STYPE_INVALID"; break;
        case AsmKcpsm3SymbolTable::STYPE_NUMBER:  out << "STYPE_NUMBER";  break;
        case AsmKcpsm3SymbolTable::STYPE_LABEL:   out << "STYPE_LABEL";   break;
    }
    return out;
}
