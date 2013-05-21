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
#include "AsmKcpsm3Macros.h"

// Standard headers.
#include <cstdio>
#include <fstream>

AsmKcpsm3SymbolTable::Symbol::Symbol ( const CompilerExpr * value,
                                       const CompilerBase::SourceLocation * location,
                                       SymbolType type,
                                       int finalValue,
                                       bool redefinable )
{
    m_type = type;
    m_used = false;
    m_constant = !redefinable;
    m_finalValue = finalValue;

    if ( NULL != location )
    {
        m_location = *location;
    }

    if ( NULL != value )
    {
        m_value = value->copyChainLink();
    }
    else
    {
        m_value = new CompilerExpr();
    }
}

AsmKcpsm3SymbolTable::Symbol::~Symbol()
{
    if ( NULL != m_value )
    {
// TODO: for some reason this line has to be commented, otherwise the program crashes; but anyway having this line 
// commented has to cause a memory leak so it has to be resolved somehow...
//         delete m_value;
    }
}

int AsmKcpsm3SymbolTable::addSymbol ( const std::string & name,
                                      const CompilerExpr * value,
                                      const CompilerBase::SourceLocation * location,
                                      const SymbolType type,
                                      bool resolve,
                                      bool redefinable )
{
    int finalValue = -1;

    if ( false == isDefined(name, type) )
    {
        if ( true == resolve )
        {
            finalValue = (int) resolveExpr(value);
        }

        m_table.insert ( std::pair<std::string,Symbol>(name, Symbol(value, location, type, finalValue, redefinable)) );
    }
    else
    {
        if ( NULL == location )
        {
            m_compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
                                                QObject::tr ( "symbol already defined: " ).toStdString()
                                                            + "\"" + name + "\"" );
        }
        else
        {
            m_compilerCore -> compilerMessage ( *location,
                                                CompilerBase::MT_ERROR,
                                                QObject::tr ( "symbol already defined: " ).toStdString()
                                                            + "\"" + name + "\"" );
        }
    }

    return finalValue;
}

void AsmKcpsm3SymbolTable::removeSymbol ( const std::string & name,
                                          const CompilerBase::SourceLocation & location,
                                          const SymbolType type )
{
    for ( std::multimap<std::string,Symbol>::iterator it = m_table.find(name);
          it != m_table.end();
          it++ )
    {
        if ( STYPE_UNSPECIFIED == type || type == it->second.m_type )
        {
            m_table.erase(it);
            return;
        }
    }

    m_compilerCore -> compilerMessage ( location,
                                        CompilerBase::MT_ERROR,
                                        QObject::tr("symbol not defined: ").toStdString() + "\"" + name + "\"");
}

bool AsmKcpsm3SymbolTable::isDefined ( const std::string & name,
                                       const SymbolType type ) const
{
    for ( std::multimap<std::string,Symbol>::const_iterator it = m_table.find(name);
          it != m_table.cend();
          it++ )
    {
        if ( STYPE_UNSPECIFIED == type || type == it->second.m_type )
        {
            return true;
        }
    }

    return false;
}

int AsmKcpsm3SymbolTable::assignValue ( const std::string & name,
                                        const CompilerExpr * value,
                                        const CompilerBase::SourceLocation * location,
                                        const SymbolType type,
                                        bool resolve )
{
    int finalValue = -1;

    for ( std::multimap<std::string,Symbol>::iterator it = m_table.find(name);
          it != m_table.end();
          it++ )
    {
        if ( STYPE_UNSPECIFIED == type || type == it->second.m_type )
        {
            if ( NULL != it->second.m_value )
            {
                delete it->second.m_value;
            }

            it->second.m_finalValue = -1;
            it->second.m_value = value->copyChainLink();

            if ( true == resolve )
            {
                finalValue = (int) resolveExpr(value, -1, location);
                it->second.m_finalValue = finalValue;
            }

            break;
        }
    }

    return finalValue;
}

const CompilerExpr * AsmKcpsm3SymbolTable::getValue ( const std::string & name,
                                                      const SymbolType type )
{
    for ( std::multimap<std::string,Symbol>::iterator it = m_table.find(name);
          it != m_table.end();
          it++ )
    {
        if ( STYPE_UNSPECIFIED == type || type == it->second.m_type )
        {
            it->second.m_used = true;
            return it->second.m_value;
        }
    }

    return NULL;
}

int AsmKcpsm3SymbolTable::getExprValue ( ExprValSide side,
                                         const CompilerExpr * expr )
{
    const CompilerExpr::Value * value;

    if ( LEFT == side )
    {
        value = & ( expr->m_lValue );
    }
    else
    {
        value = & ( expr->m_rValue );
    }

    switch ( value->m_type )
    {
        case CompilerExpr::Value::TYPE_EMPTY:
        {
            m_compilerCore -> compilerMessage ( expr->m_location,
                                                CompilerBase::MT_ERROR,
                                                QObject::tr("undefined value").toStdString());
            break;
        }
        case CompilerExpr::Value::TYPE_INT:
        {
            return value->m_data.m_integer;
        }
        case CompilerExpr::Value::TYPE_REAL:
        {
            m_compilerCore -> compilerMessage ( expr->m_location,
                                                CompilerBase::MT_ERROR,
                                                QObject::tr("real numbers are not supported in assembler").toStdString());
            break;
        }
        case CompilerExpr::Value::TYPE_EXPR:
        {
            return computeExpr(value->m_data.m_expr);
        }
        case CompilerExpr::Value::TYPE_SYMBOL:
        {
            const CompilerExpr * symbolValue = getValue(value->m_data.m_symbol);
            if ( NULL == symbolValue )
            {
                m_compilerCore -> compilerMessage ( expr->m_location,
                                                    CompilerBase::MT_ERROR,
                                                    QObject::tr("undefined symbol: ").toStdString() + "\"" + value->m_data.m_symbol + "\"");
                break;
            }
            else
            {
                return computeExpr(symbolValue);
            }
        }
        case CompilerExpr::Value::TYPE_ARRAY:
        {
            m_compilerCore -> compilerMessage ( expr->m_location,
                                                CompilerBase::MT_ERROR,
                                                QObject::tr("this value is not valid inside an expression").toStdString());
            break;
        }
    }

    return 1;
}

int AsmKcpsm3SymbolTable::computeExpr ( const CompilerExpr * expr )
{
    switch ( expr->m_operator )
    {
        case CompilerExpr::OPER_NONE:
            return getExprValue(LEFT, expr);
        case CompilerExpr::OPER_ADD:
            return getExprValue(LEFT, expr) + getExprValue(RIGHT, expr);
        case CompilerExpr::OPER_SUB:
            return getExprValue(LEFT, expr) - getExprValue(RIGHT, expr);
        case CompilerExpr::OPER_MULT:
            return getExprValue(LEFT, expr) * getExprValue(RIGHT, expr);
        case CompilerExpr::OPER_DIV:
        {
            int right = getExprValue(RIGHT, expr);
            if ( 0 == right )
            {
                m_compilerCore -> compilerMessage ( expr->m_location,
                                                    CompilerBase::MT_ERROR,
                                                    QObject::tr("division by zero").toStdString());
                return 1;
            }
            return getExprValue(LEFT, expr) / right;
        }
        case CompilerExpr::OPER_LOW:
            return ( 0xff & getExprValue(LEFT, expr) );
        case CompilerExpr::OPER_HIGH:
            return ( ( 0xff00 & getExprValue(LEFT, expr) ) >> 8 );
        case CompilerExpr::OPER_CMPL:
            return ~getExprValue(LEFT, expr);
        case CompilerExpr::OPER_NOT:
            return ( 0 == getExprValue(LEFT, expr) ) ? 1 : 0;
        case CompilerExpr::OPER_MOD:
        {
            int right = getExprValue(RIGHT, expr);
            if ( 0 == right )
            {
                m_compilerCore -> compilerMessage ( expr->m_location,
                                                    CompilerBase::MT_ERROR,
                                                    QObject::tr("division by zero").toStdString());
                return 1;
            }
            return getExprValue(LEFT, expr) % right;
        }
        case CompilerExpr::OPER_LOR:
            return ( getExprValue(LEFT, expr) || getExprValue(RIGHT, expr) ) ? 1 : 0;
        case CompilerExpr::OPER_LAND:
            return ( getExprValue(LEFT, expr) && getExprValue(RIGHT, expr) ) ? 1 : 0;
        case CompilerExpr::OPER_BOR:
            return getExprValue(LEFT, expr) | getExprValue(RIGHT, expr);
        case CompilerExpr::OPER_BXOR:
            return getExprValue(LEFT, expr) ^ getExprValue(RIGHT, expr);
        case CompilerExpr::OPER_BAND:
            return getExprValue(LEFT, expr) & getExprValue(RIGHT, expr);
        case CompilerExpr::OPER_EQ:
            return ( getExprValue(LEFT, expr) == getExprValue(RIGHT, expr) ) ? 1 : 0;
        case CompilerExpr::OPER_NE:
            return ( getExprValue(LEFT, expr) != getExprValue(RIGHT, expr) ) ? 1 : 0;
        case CompilerExpr::OPER_LT:
            return ( getExprValue(LEFT, expr) < getExprValue(RIGHT, expr) ) ? 1 : 0;
        case CompilerExpr::OPER_LE:
            return ( getExprValue(LEFT, expr) <= getExprValue(RIGHT, expr) ) ? 1 : 0;
        case CompilerExpr::OPER_GE:
            return ( getExprValue(LEFT, expr) >= getExprValue(RIGHT, expr) ) ? 1 : 0;
        case CompilerExpr::OPER_GT:
            return ( getExprValue(LEFT, expr) > getExprValue(RIGHT, expr) ) ? 1 : 0;
        case CompilerExpr::OPER_SHR:
            return getExprValue(LEFT, expr) >> getExprValue(RIGHT, expr);
        case CompilerExpr::OPER_SHL:
            return getExprValue(LEFT, expr) << getExprValue(RIGHT, expr);
        default:
            // Unsupported operator.
            break;
    }

    m_compilerCore -> compilerMessage ( expr->m_location,
                                        CompilerBase::MT_ERROR,
                                        QObject::tr("unable to resolve this expression").toStdString());
    return 1;
}

unsigned int AsmKcpsm3SymbolTable::resolveExpr ( const CompilerExpr * expr,
                                                 int bitsMax,
                                                 const CompilerBase::SourceLocation * origLocation )
{
    int resultOrig = computeExpr(expr);
    unsigned int result = (unsigned int) resultOrig;
    const CompilerBase::SourceLocation * location = origLocation;

    if ( NULL == location )
    {
        location = &(expr->location());
    }

    if ( -1 != bitsMax )
    {
        unsigned int mask = 0;
        for ( int i = 0; i < bitsMax; i++ )
        {
            mask |= ( 1 << i );
        }

        if ( resultOrig < 0 )
        {
            result &= mask;

            // Check whether it's still a negative number.
            if ( 0 == (result & ( 1 << (bitsMax-1))) )
            {
                m_compilerCore -> compilerMessage ( *location,
                                                    CompilerBase::MT_WARNING,
                                                    QObject::tr("sign overflow. Result is negative number lower that the lowest negative number representable in two's complement arithmetic by the given number of bits (%1 bits in this case) ").arg(bitsMax).toStdString());
            }
            else
            {
                m_compilerCore -> compilerMessage ( *location,
                                                    CompilerBase::MT_REMARK,
                                                    QObject::tr("result is negative number: %1, this will represented as %2-bit number in two's complement arithmetic which makes it: %3").arg(resultOrig).arg(bitsMax).arg(result).toStdString());
            }
        }
        else if ( ( result & mask ) != result )
        {
            m_compilerCore -> compilerMessage ( *location,
                                                CompilerBase::MT_WARNING,
                                                QObject::tr("value out of range: %1, allowed range is [0,%2] (trimmed to %3 bits) which makes it %4").arg(result).arg(mask).arg(bitsMax).arg(result&mask).toStdString());
            result &= mask;
        }
    }

    return result;
}

unsigned int AsmKcpsm3SymbolTable::substitute ( const std::string & origSymbol,
                                                const CompilerExpr * newSymbol,
                                                CompilerExpr * expr )
{
    unsigned int result = 0;

    if ( NULL == expr )
    {
        return result;
    }

    for ( ; NULL != expr; expr = expr->m_next )
    {
        switch ( expr->m_lValue.m_type )
        {
            case CompilerExpr::Value::TYPE_EXPR:
            {
                result += substitute(origSymbol, newSymbol, expr->m_lValue.m_data.m_expr);
            }
            case CompilerExpr::Value::TYPE_SYMBOL:
            {
                if ( origSymbol == expr->m_lValue.m_data.m_symbol )
                {
                    if ( CompilerExpr::OPER_NONE == newSymbol->oper() )
                    {
                        expr->m_lValue = newSymbol->lVal().makeCopy();
                    }
                    else
                    {
                        expr->m_lValue = CompilerExpr::Value(newSymbol->copyChainLink());
                    }
                    result++;
                }
            }
            default:
                break;
        }
        switch ( expr->m_rValue.m_type )
        {
            case CompilerExpr::Value::TYPE_EXPR:
            {
                result += substitute(origSymbol, newSymbol, expr->m_rValue.m_data.m_expr);
            }
            case CompilerExpr::Value::TYPE_SYMBOL:
            {
                if ( origSymbol == expr->m_rValue.m_data.m_symbol )
                {
                    if ( CompilerExpr::OPER_NONE == newSymbol->oper() )
                    {
                        expr->m_rValue = newSymbol->lVal().makeCopy();
                    }
                    else
                    {
                        expr->m_rValue = CompilerExpr::Value(newSymbol->copyChainLink());
                    }
                    result++;
                }
            }
            default:
                break;
        }
    }

    return result;
}

void AsmKcpsm3SymbolTable::output()
{
    if ( true == m_opts->m_symbolTable.empty() )
    {
        return;
    }

    if ( true == m_opts->m_makeBackupFiles )
    {
        rename(m_opts->m_symbolTable.c_str(), (m_opts->m_symbolTable + "~").c_str());
    }

    std::ofstream file ( m_opts->m_symbolTable, ( std::fstream::out | std::fstream::trunc ) );

    if ( false == file.is_open() )
    {
        m_compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
                                            QObject::tr("Unable to open ").toStdString() + "\"" + m_opts->m_symbolTable  + "\"" );
        return;
    }

    file << this;

    if ( true == file.bad() )
    {
        m_compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
                                            QObject::tr("Unable to write to ").toStdString() + "\"" + m_opts->m_symbolTable  + "\"" );
        return;
    }
}

void AsmKcpsm3SymbolTable::clear()
{
    m_table.clear();
}

void AsmKcpsm3SymbolTable::printSymLocation ( std::ostream & out,
                                              const CompilerBase::SourceLocation & location ) const
{
    if ( false == location.isSet() )
    {
        return;
    }

    out << m_compilerCore->locationToStr(location);
}

std::ostream & operator << ( std::ostream & out,
                             const AsmKcpsm3SymbolTable * symbolTable )
{
    for ( std::multimap<std::string,AsmKcpsm3SymbolTable::Symbol>::const_iterator symbol = symbolTable->m_table.cbegin();
          symbol != symbolTable->m_table.cend();
          symbol++ )
    {
        out << symbol->first;
        out << " ";
        for ( unsigned int i = symbol->first.size(); i < 35; i++ )
        {
            if ( i % 2 )
            {
                out << " ";
            }
            else
            {
                out << ".";
            }
        }

        switch ( symbol->second.m_type )
        {
            case AsmKcpsm3SymbolTable::STYPE_UNSPECIFIED: out << "        "; break;
            case AsmKcpsm3SymbolTable::STYPE_NUMBER:      out << " NUMBER "; break;
            case AsmKcpsm3SymbolTable::STYPE_REGISTER:    out << " REG.   "; break;
            case AsmKcpsm3SymbolTable::STYPE_LABEL:       out << " LABEL  "; break;
            case AsmKcpsm3SymbolTable::STYPE_PORT:        out << " PORT   "; break;
            case AsmKcpsm3SymbolTable::STYPE_EXPRESSION:  out << " EXPR.  "; break;
        }

        if ( -1 == symbol->second.m_finalValue )
        {
            out << "       ";
        }
        else
        {
            char finalValue[8];
            sprintf(finalValue, "0x%04X ", symbol->second.m_finalValue);
            out << finalValue;
        }

        if ( true == symbol->second.m_used )
        {
            out << "USED     ";
        }
        else
        {
            out << "NOT USED ";
        }

        if ( true == symbol->second.m_constant )
        {
            out << "CONSTANT    ";
        }
        else
        {
            out << "REDEFINABLE ";
        }

        if ( AsmKcpsm3Macros::MANGLE_PREFIX == symbol->first[0] )
        {
            out << "LOCAL";
        }
        else
        {
            symbolTable->printSymLocation(out, symbol->second.m_location);
        }

        out << std::endl;
    }

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const AsmKcpsm3SymbolTable::SymbolType symbolType )
{
    switch ( symbolType )
    {
        case AsmKcpsm3SymbolTable::STYPE_UNSPECIFIED: out << "STYPE_UNSPECIFIED"; break;
        case AsmKcpsm3SymbolTable::STYPE_NUMBER:      out << "STYPE_NUMBER";      break;
        case AsmKcpsm3SymbolTable::STYPE_REGISTER:    out << "STYPE_REGISTER";    break;
        case AsmKcpsm3SymbolTable::STYPE_LABEL:       out << "STYPE_LABEL";       break;
        case AsmKcpsm3SymbolTable::STYPE_PORT:        out << "STYPE_PORT";        break;
        case AsmKcpsm3SymbolTable::STYPE_EXPRESSION:  out << "STYPE_EXPRESSION";  break;
    }

    return out;
}
