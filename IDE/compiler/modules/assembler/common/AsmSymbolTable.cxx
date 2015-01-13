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
 * @ingroup Assembler
 * @file AsmSymbolTable.cxx
 */
// =============================================================================

#include "AsmSymbolTable.h"
#include "AsmMacros.h"

// MDS build config.
#include "mds.h"

// Standard headers.
#include <cstdio>
#include <cstring>
#include <fstream>

const std::map<std::string, AsmSymbolTable::PredefinedSymbolID> AsmSymbolTable::PREDEFINED_SYMBOLS =
{
    { "__MDS_VERSION__", PRE_DEF_MDS_VERSION }
};

// 16b: MSB > |MAJOR|MINOR|MINOR|PATCH| < LSB
const CompilerExpr AsmSymbolTable::S_MDS_VERSION =
    CompilerExpr ( CompilerValue ( ( ( PRODUCT_VERSION_MAJOR & 0x0f ) << 12 ) |
                                   ( ( PRODUCT_VERSION_MINOR & 0xff ) <<  4 ) |
                                   ( ( PRODUCT_VERSION_PATCH & 0x0f ) <<  0 ) ) );

AsmSymbolTable::Symbol::Symbol ( const CompilerExpr * value,
                                 const CompilerSourceLocation * location,
                                 SymbolType type,
                                 int finalValue,
                                 bool redefinable )
{
    m_type = type;
    m_masked = false;
    m_used = false;
    m_constant = !redefinable;
    m_finalValue = finalValue;

    if ( nullptr != location )
    {
        m_location = *location;
    }

    if ( nullptr != value )
    {
        m_value = value->copyChainLink();
    }
    else
    {
        m_value = new CompilerExpr();
    }
}

AsmSymbolTable::Symbol::Symbol ( const AsmSymbolTable::Symbol & obj )
{

    m_type       = obj.m_type;
    m_masked     = obj.m_masked;
    m_used       = obj.m_used;
    m_value      = obj.m_value;
    m_constant   = obj.m_constant;
    m_location   = obj.m_location;
    m_finalValue = obj.m_finalValue;
}

AsmSymbolTable::~AsmSymbolTable()
{
    clear();
}

int AsmSymbolTable::addSymbol ( const std::string & name,
                                const CompilerExpr * value,
                                const CompilerSourceLocation * location,
                                const SymbolType type,
                                bool resolve,
                                bool redefinable )
{
    int finalValue = -1;

    if ( false == isDefined(name) )
    {
        if ( true == resolve )
        {
            finalValue = (int) resolveExpr(value);
            CompilerExpr finalValueExpr(finalValue, *location);
            m_table.insert ( std::pair<std::string,Symbol> ( name,
                                                             Symbol ( &finalValueExpr,
                                                                      location,
                                                                      type,
                                                                      finalValue,
                                                                      redefinable ) ) );
        }
        else
        {
            m_table.insert ( std::pair<std::string,Symbol> ( name,
                                                             Symbol ( value,
                                                                      location,
                                                                      type,
                                                                      finalValue,
                                                                      redefinable ) ) );
        }
    }
    else
    {
        if ( nullptr == location )
        {
            m_compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                                CompilerBase::MT_ERROR,
                                                QObject::tr ( "symbol already defined: " ).toStdString()
                                                            + '"' + name + '"' );
        }
        else
        {
            m_compilerCore -> semanticMessage ( *location,
                                                CompilerBase::MT_ERROR,
                                                QObject::tr ( "symbol already defined: " ).toStdString()
                                                            + '"' + name + '"' );
        }
    }

    return finalValue;
}

AsmSymbolTable::SymbolType AsmSymbolTable::getType ( const std::string & name )
{
    if ( PREDEFINED_SYMBOLS.cend() != PREDEFINED_SYMBOLS.find(name) )
    {
        return STYPE_NUMBER;
    }

    const auto it = m_table.find(name);
    if ( m_table.cend() != it )
    {
        return it->second.m_type;
    }
    else
    {
        return STYPE_UNSPECIFIED;
    }
}

void AsmSymbolTable::maskNonLabels()
{
    for ( auto & symbol : m_table )
    {
        if ( ( STYPE_LABEL != symbol.second.m_type ) && ( STYPE_EXPRESSION != symbol.second.m_type ) )
        {
            symbol.second.m_masked = true;
        }
    }
}

AsmSymbolTable::SymbolType AsmSymbolTable::getType ( const CompilerExpr * expr )
{
    if ( CompilerExpr::OPER_NONE != expr->oper() )
    {
        // If there is an operator, it's clear that we are not dealing with a single value thus it's an expression.
        return STYPE_EXPRESSION;
    }
    else
    {
        switch ( expr->lVal().m_type )
        {
            case CompilerValue::TYPE_INT:    return STYPE_NUMBER;
            case CompilerValue::TYPE_REAL:   return STYPE_NUMBER;
            case CompilerValue::TYPE_EMPTY:  return STYPE_UNSPECIFIED;
            case CompilerValue::TYPE_ARRAY:  return STYPE_UNSPECIFIED;
            case CompilerValue::TYPE_EXPR:   return getType(expr->lVal().m_data.m_expr);
            case CompilerValue::TYPE_SYMBOL: return getType(expr->lVal().m_data.m_symbol);
        }
    }

    // Control flow should never reach this line; if it does, there is an error in the implementation!
    return STYPE_UNSPECIFIED;
}

void AsmSymbolTable::resolveSymbols ( CompilerExpr * expr,
                                      int codePointer )
{
    for ( ; nullptr != expr; expr = expr->next() )
    {
        CompilerValue * value = &(expr->m_lValue);
        for ( int i = 0; i < 2; i++ )
        {
            // Keep IDs defined with `DEFINE' directive intact until phase #2 where they will be substituted.
            if ( ( 0 == i ) && ( expr->oper() == CompilerExpr::OPER_CALL ) )
            {
                value = &(expr->m_rValue);
                continue;
            }

            if ( CompilerValue::TYPE_SYMBOL == value->m_type )
            {
                const char * symbolName = value->m_data.m_symbol;
                if ( ( '$' == symbolName[0] ) && ( '\0' == symbolName[1] ) )
                {
                    delete [] value->m_data.m_symbol;
                    value->m_type = CompilerValue::TYPE_INT;
                    value->m_data.m_integer = codePointer;
                }
                else
                {
                    CompilerExpr * subExpr = getValue(symbolName)->copyChainLink();
                    if ( nullptr != subExpr )
                    {
                        resolveSymbols(subExpr, codePointer);
                        delete [] value->m_data.m_symbol;
                        value->m_type = CompilerValue::TYPE_EXPR;
                        value->m_data.m_expr = subExpr;
                    }
                }
            }
            else if ( CompilerValue::TYPE_EXPR == value->m_type )
            {
                resolveSymbols(value->m_data.m_expr, codePointer);
            }

            value = &(expr->m_rValue);
        }
    }
}

void AsmSymbolTable::removeSymbol ( const std::string & name,
                                    const CompilerSourceLocation & location,
                                    const SymbolType type )
{
    if ( PREDEFINED_SYMBOLS.cend() != PREDEFINED_SYMBOLS.find(name) )
    {
        m_compilerCore -> semanticMessage ( location,
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "cannot remove predefined symbol: `%1'" )
                                                        . arg(name.c_str())
                                                        . toStdString() );
        return;
    }

    auto it = m_table.find(name);
    if ( it == m_table.end() )
    {
        m_compilerCore -> semanticMessage ( location,
                                            CompilerBase::MT_ERROR,
                                            QObject::tr("symbol not defined: `%1'").arg(name.c_str()).toStdString() );
    }
    else if ( STYPE_UNSPECIFIED == type || type == it->second.m_type )
    {
        m_deletedSymbols.insert(*it);
        m_table.erase(it);
    }
}

bool AsmSymbolTable::isDefined ( const std::string & name,
                                 const SymbolType type ) const
{
    if ( PREDEFINED_SYMBOLS.cend() != PREDEFINED_SYMBOLS.find(name) )
    {
        return true;
    }

    const auto it = m_table.find(name);
    if ( ( m_table.cend() != it ) && ( STYPE_UNSPECIFIED == type || type == it->second.m_type ) )
    {
        return true;
    }

    return false;
}

int AsmSymbolTable::assignValue ( const std::string & name,
                                  const CompilerExpr * value,
                                  const CompilerSourceLocation * location,
                                  const SymbolType type,
                                  bool resolve )
{
    if ( PREDEFINED_SYMBOLS.cend() != PREDEFINED_SYMBOLS.find(name) )
    {
        m_compilerCore -> semanticMessage ( *location,
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "cannot change value of predefined symbol: `%1'" )
                                                        . arg(name.c_str())
                                                        . toStdString() );
        return -1;
    }

    int finalValue = -1;

    std::map<std::string,Symbol>::iterator it = m_table.find(name);
    if ( it == m_table.end() )
    {
        return finalValue;
    }

    if ( STYPE_UNSPECIFIED == type || type == it->second.m_type )
    {
        if ( true == resolve )
        {
            finalValue = (int) resolveExpr(value, -1);
            it->second.m_finalValue = finalValue;
            it->second.m_value->completeDelete();
            it->second.m_value = new CompilerExpr(finalValue);
        }
        else
        {
            it->second.m_finalValue = -1;
            it->second.m_value->completeDelete();
            it->second.m_value = value->copyChainLink();
        }
    }
    else
    {
        m_compilerCore -> semanticMessage ( *location,
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "symbol `%1' has been already defined with type: " )
                                                        . arg ( name.c_str() )
                                                        . toStdString()
                                                        + "`" + symType2Str(it->second.m_type) +"'" );
    }

    return finalValue;
}

const CompilerExpr * AsmSymbolTable::getValue ( const std::string & name,
                                                const SymbolType type )
{
    {
        const auto it = PREDEFINED_SYMBOLS.find(name);
        if ( PREDEFINED_SYMBOLS.cend() != it )
        {
            switch ( it->second )
            {
                case PRE_DEF_MDS_VERSION:
                    return &S_MDS_VERSION;
            }
        }
    }

    {
        auto it = m_table.find(name);
        if (
               ( it != m_table.end() )
                   &&
               ( STYPE_UNSPECIFIED == type || type == it->second.m_type )
                   &&
               ( false == it->second.m_masked )
           )
        {
            it->second.m_used = true;
            return it->second.m_value;
        }
    }

    return nullptr;
}

int AsmSymbolTable::getExprValue ( ExprValSide side,
                                   const CompilerExpr * expr,
                                   const CompilerExpr * argList )
{
    const CompilerValue * value;

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
        case CompilerValue::TYPE_EMPTY:
        {
            m_compilerCore -> semanticMessage ( expr->m_location,
                                                CompilerBase::MT_ERROR,
                                                QObject::tr("blank value").toStdString(),
                                                true );
            break;
        }
        case CompilerValue::TYPE_INT:
        {
            return value->m_data.m_integer;
        }
        case CompilerValue::TYPE_REAL:
        {
            m_compilerCore -> semanticMessage ( expr->m_location,
                                                CompilerBase::MT_ERROR,
                                                QObject::tr("real numbers are not supported in this assembler")
                                                           .toStdString(),
                                                true );
            break;
        }
        case CompilerValue::TYPE_EXPR:
        {
            return computeExpr(value->m_data.m_expr, argList);
        }
        case CompilerValue::TYPE_SYMBOL:
        {
            const CompilerExpr * symbolValue = getValue(value->m_data.m_symbol);
            if ( nullptr == symbolValue )
            {
                m_compilerCore -> semanticMessage ( expr->m_location,
                                                    CompilerBase::MT_ERROR,
                                                    QObject::tr ( "symbol not defined: `%1'" )
                                                                . arg ( value->m_data.m_symbol )
                                                                . toStdString() );
                break;
            }
            else
            {
                if ( nullptr != argList )
                {
                    CompilerExpr * newValue = substituteArgs(symbolValue, argList);
                    int result = computeExpr(newValue);
                    newValue->completeDelete();
                    return result;
                }
                else
                {
                    return computeExpr(symbolValue);
                }
            }
        }
        case CompilerValue::TYPE_ARRAY:
        {
            m_compilerCore -> semanticMessage ( expr->m_location,
                                                CompilerBase::MT_ERROR,
                                                QObject::tr ( "this value is not valid inside an expression" )
                                                            . toStdString(),
                                                true );
            break;
        }
    }

    return 1;
}

inline CompilerExpr * AsmSymbolTable::substituteArgs ( const CompilerExpr * expr,
                                                       const CompilerExpr * argList )
{
    CompilerExpr * result = expr->copyEntireChain();

    int i = 0;
    for ( const CompilerExpr * arg = argList;
          nullptr != arg;
          arg = arg->next() )
    {
        if ( false == substArg(result, arg, i) )
        {
            m_compilerCore -> semanticMessage ( arg->location(),
                                                CompilerBase::MT_WARNING,
                                                QObject::tr("argument #%1 not used").arg(i).toStdString(),
                                                true );
        }
        i++;
    }

    return result;
}

bool AsmSymbolTable::substArg ( CompilerExpr * expr,
                                const CompilerExpr * subst,
                                const int position )
{
    bool result = false;

    for ( ; nullptr != expr; expr = expr->next() )
    {
        CompilerValue * value = &( expr->m_lValue );
        for ( int valIdx = 0; valIdx < 2; valIdx ++ )
        {
            if ( CompilerValue::TYPE_SYMBOL == value->m_type )
            {
                char * symbol = value->m_data.m_symbol;
                size_t len = strlen(symbol);
                if ( len >= 3 )
                {
                    if ( ( '{' == symbol[0] ) && ( '}' == symbol[len-1] ) )
                    {
                        char buf[len-1];
                        for ( size_t i = 1, j = 0; i < ( len - 1 ); i++, j++ )
                        {
                            buf[j] = symbol[i];
                        }
                        buf[len-2] = '\0';

                        int mark;
                        sscanf(buf, "%d", &mark);
                        if ( position == mark )
                        {
                            delete [] symbol;
                            CompilerExpr * substCopy = subst->copyChainLink();
                            rewriteExprLoc(substCopy, m_compilerCore->locationTrack().add(expr->location()));

                            if ( ( 0 == valIdx ) && ( CompilerExpr::OPER_NONE == expr->oper() ) )
                            {
                                value->m_type = CompilerValue::TYPE_EXPR;
                                value->m_data.m_expr = substCopy;
                                expr->m_location = substCopy->location();
                            }
                            else
                            {
                                value->m_type = CompilerValue::TYPE_EXPR;
                                value->m_data.m_expr = substCopy;
                            }

                            result = true;
                        }
                    }
                }
            }
            else if ( CompilerValue::TYPE_EXPR == value->m_type )
            {
                result |= substArg(value->m_data.m_expr, subst, position);
            }
            value = &( expr->m_rValue );
        }
    }

    return result;
}

unsigned int AsmSymbolTable::substitute ( const std::string & origSymbol,
                                          const CompilerExpr * newSymbol,
                                          CompilerExpr * expr )
{
    unsigned int result = 0;

    if ( nullptr == expr )
    {
        return result;
    }

    for ( ; nullptr != expr; expr = expr->m_next )
    {
        CompilerValue * value = &(expr->m_lValue);
        for ( int i = 0; i < 2; i++ )
        {
            switch ( value->m_type )
            {
                case CompilerValue::TYPE_EXPR:
                    result += substitute(origSymbol, newSymbol, value->m_data.m_expr);
                    break;
                case CompilerValue::TYPE_SYMBOL:
                    if ( origSymbol == value->m_data.m_symbol )
                    {
                        delete [] value->m_data.m_symbol;
                        CompilerExpr * newSymbolCopy = newSymbol->copyChainLink();
                        rewriteExprLoc(newSymbolCopy, m_compilerCore->locationTrack().add(expr->location()));
                        *value = CompilerValue(newSymbolCopy);
                        if ( CompilerExpr::OPER_NONE == expr->oper() )
                        {
                            expr->m_location = newSymbolCopy->location();
                        }
                        result++;
                    }
                    break;
                default:
                    break;
            }
            value = &(expr->m_rValue);
        }
    }
    return result;
}

int AsmSymbolTable::computeExpr ( const CompilerExpr * expr,
                                  const CompilerExpr * argList )
{
    switch ( expr->m_operator )
    {
        case CompilerExpr::OPER_NONE:
        case CompilerExpr::OPER_INT_PROM:
            return getExprValue(LEFT, expr, argList);
        case CompilerExpr::OPER_ADD_INV:
            return -getExprValue(LEFT, expr,  argList);
        case CompilerExpr::OPER_ADD:
            return getExprValue(LEFT, expr,  argList) + getExprValue(RIGHT, expr,  argList);
        case CompilerExpr::OPER_SUB:
            return getExprValue(LEFT, expr,  argList) - getExprValue(RIGHT, expr,  argList);
        case CompilerExpr::OPER_MULT:
            return getExprValue(LEFT, expr,  argList) * getExprValue(RIGHT, expr,  argList);
        case CompilerExpr::OPER_DIV:
        {
            int right = getExprValue(RIGHT, expr,  argList);
            if ( 0 == right )
            {
                const CompilerSourceLocation & loc = ( CompilerValue::TYPE_EXPR == expr->rVal().m_type )
                                                     ? expr->rVal().m_data.m_expr->location() : expr->location();

                m_compilerCore -> semanticMessage ( loc,
                                                    CompilerBase::MT_ERROR,
                                                    QObject::tr("division by zero").toStdString(),
                                                    true );
                return 1;
            }
            return getExprValue(LEFT, expr,  argList) / right;
        }
        case CompilerExpr::OPER_LOW:
            return ( 0xff & getExprValue(LEFT, expr,  argList) );
        case CompilerExpr::OPER_HIGH:
            return ( ( 0xff00 & getExprValue(LEFT, expr,  argList) ) >> 8 );
        case CompilerExpr::OPER_CMPL:
            return ~getExprValue(LEFT, expr,  argList);
        case CompilerExpr::OPER_NOT:
            return ( 0 == getExprValue(LEFT, expr,  argList) ) ? 1 : 0;
        case CompilerExpr::OPER_MOD:
        {
            int right = getExprValue(RIGHT, expr,  argList);
            if ( 0 == right )
            {
                const CompilerSourceLocation & loc = ( CompilerValue::TYPE_EXPR == expr->rVal().m_type )
                                                     ? expr->rVal().m_data.m_expr->location() : expr->location();

                m_compilerCore -> semanticMessage ( loc,
                                                    CompilerBase::MT_ERROR,
                                                    QObject::tr("division by zero").toStdString(),
                                                    true );
                return 1;
            }
            return getExprValue(LEFT, expr,  argList) % right;
        }
        case CompilerExpr::OPER_LOR:
            return ( getExprValue(LEFT, expr, argList) || getExprValue(RIGHT, expr, argList) ) ? 1 : 0;
        case CompilerExpr::OPER_LAND:
            return ( getExprValue(LEFT, expr, argList) && getExprValue(RIGHT, expr, argList) ) ? 1 : 0;
        case CompilerExpr::OPER_BOR:
            return ( getExprValue(LEFT, expr, argList) |  getExprValue(RIGHT, expr, argList) );
        case CompilerExpr::OPER_BXOR:
            return ( getExprValue(LEFT, expr, argList) ^  getExprValue(RIGHT, expr, argList) );
        case CompilerExpr::OPER_BAND:
            return ( getExprValue(LEFT, expr, argList) &  getExprValue(RIGHT, expr, argList) );
        case CompilerExpr::OPER_EQ:
            return ( getExprValue(LEFT, expr, argList) == getExprValue(RIGHT, expr, argList) ) ? 1 : 0;
        case CompilerExpr::OPER_NE:
            return ( getExprValue(LEFT, expr, argList) != getExprValue(RIGHT, expr, argList) ) ? 1 : 0;
        case CompilerExpr::OPER_LT:
            return ( getExprValue(LEFT, expr, argList) <  getExprValue(RIGHT, expr, argList) ) ? 1 : 0;
        case CompilerExpr::OPER_LE:
            return ( getExprValue(LEFT, expr, argList) <= getExprValue(RIGHT, expr, argList) ) ? 1 : 0;
        case CompilerExpr::OPER_GE:
            return ( getExprValue(LEFT, expr, argList) >= getExprValue(RIGHT, expr, argList) ) ? 1 : 0;
        case CompilerExpr::OPER_GT:
            return ( getExprValue(LEFT, expr, argList) >  getExprValue(RIGHT, expr, argList) ) ? 1 : 0;
        case CompilerExpr::OPER_SHR:
            return ( getExprValue(LEFT, expr, argList) >> getExprValue(RIGHT, expr, argList) );
        case CompilerExpr::OPER_SHL:
            return ( getExprValue(LEFT, expr, argList) << getExprValue(RIGHT, expr, argList) );
        case CompilerExpr::OPER_CALL:
            return getExprValue(LEFT, expr, expr->m_rValue.m_data.m_expr);
        default:
            // Unsupported operator.
            break;
    }

    m_compilerCore -> semanticMessage ( expr->m_location,
                                        CompilerBase::MT_ERROR,
                                        QObject::tr("unable to resolve this expression").toStdString(),
                                        true );
    return 1;
}

unsigned int AsmSymbolTable::resolveExpr ( const CompilerExpr * expr,
                                           int bitsMax )
{
    int resultOrig = computeExpr(expr);
    unsigned int result = (unsigned int) resultOrig;

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
            if ( 0 == ( result & ( 1 << ( bitsMax - 1 ) ) ) )
            {
                m_compilerCore -> semanticMessage ( expr->location(),
                                                    CompilerBase::MT_WARNING,
                                                    QObject::tr ( "sign overflow. Result is negative number lower that "
                                                                  "the lowest negative number representable in two's "
                                                                  "complement arithmetic by the given number of bits "
                                                                  "(%1 bits in this case) " )
                                                                . arg ( bitsMax )
                                                                . toStdString() );
            }
            else
            {
                m_compilerCore -> semanticMessage ( expr->location(),
                                                    CompilerBase::MT_REMARK,
                                                    QObject::tr ( "result is negative number: 0x%1, this will "
                                                                  "represented as %2-bit number in two's complement "
                                                                  "arithmetic which makes it: 0x%3" )
                                                                . arg ( resultOrig, 0, 16 )
                                                                . arg ( bitsMax )
                                                                . arg ( result, 0, 16 )
                                                                . toStdString() );
            }
        }
        else if ( ( result & mask ) != result )
        {
            m_compilerCore -> semanticMessage ( expr->location(),
                                                CompilerBase::MT_WARNING,
                                                QObject::tr ( "value out of range: 0x%1, allowed range is [0x0,0x%2] "
                                                              "(trimmed to %3 bits) which makes it 0x%4" )
                                                            . arg ( result, 0, 16 )
                                                            . arg ( mask, 0, 16 )
                                                            . arg ( bitsMax )
                                                            . arg ( (result & mask), 0, 16 )
                                                            . toStdString() );
            result &= mask;
        }
    }

    return result;
}

void AsmSymbolTable::rewriteExprLoc ( CompilerExpr * expr,
                                      int origin ) const
{
    for ( ; nullptr != expr; expr = expr->next() )
    {
        expr->m_location.m_origin = origin;

        if ( CompilerValue::TYPE_EXPR == expr->lVal().m_type )
        {
            rewriteExprLoc(expr->lVal().m_data.m_expr, origin);
        }
        if ( CompilerValue::TYPE_EXPR == expr->rVal().m_type )
        {
            rewriteExprLoc(expr->rVal().m_data.m_expr, origin);
        }
    }
}

void AsmSymbolTable::output()
{
    if ( true == m_opts->m_symbolTable.empty() )
    {
        return;
    }

    std::ofstream file ( m_opts->m_symbolTable, ( std::fstream::out | std::fstream::trunc ) );

    if ( false == file.is_open() )
    {
        m_compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "unable to open: " ).toStdString()
                                                        + '"' + m_opts->m_symbolTable  + '"' );
        return;
    }

    file << this;

    if ( true == file.bad() )
    {
        m_compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "unable to write to: " ).toStdString()
                                                        + '"' + m_opts->m_symbolTable  + '"' );
        return;
    }
}

void AsmSymbolTable::clear()
{
    for ( const auto & symbol : m_table )
    {
        symbol.second.m_value->completeDelete();
    }
    m_table.clear();

    for ( const auto & symbol : m_deletedSymbols )
    {
        symbol.second.m_value->completeDelete();
    }
    m_deletedSymbols.clear();
}

inline void AsmSymbolTable::printSymLocation ( std::ostream & out,
                                               const CompilerSourceLocation & location ) const
{
    if ( false == location.isSet() )
    {
        return;
    }

    out << m_compilerCore->locationToStr(location);
}

const char * AsmSymbolTable::symType2Str ( const AsmSymbolTable::SymbolType symbolType,
                                           bool constLength )
{
    if ( true == constLength )
    {
        switch ( symbolType )
        {
            case AsmSymbolTable::STYPE_UNSPECIFIED: return "       ";
            case AsmSymbolTable::STYPE_NUMBER:      return "NUMBER ";
            case AsmSymbolTable::STYPE_REGISTER:    return "REG.   ";
            case AsmSymbolTable::STYPE_LABEL:       return "LABEL  ";
            case AsmSymbolTable::STYPE_PORT:        return "PORT   ";
            case AsmSymbolTable::STYPE_PORTIN:      return "PORTIN ";
            case AsmSymbolTable::STYPE_PORTOUT:     return "PORTOUT";
            case AsmSymbolTable::STYPE_EXPRESSION:  return "EXPR.  ";
            case AsmSymbolTable::STYPE_DATA:        return "DATA   ";
        }
    }
    else
    {
        switch ( symbolType )
        {
            default:
            case AsmSymbolTable::STYPE_UNSPECIFIED: return "";
            case AsmSymbolTable::STYPE_NUMBER:      return "NUMBER";
            case AsmSymbolTable::STYPE_REGISTER:    return "REG.";
            case AsmSymbolTable::STYPE_LABEL:       return "LABEL";
            case AsmSymbolTable::STYPE_PORT:        return "PORT";
            case AsmSymbolTable::STYPE_PORTIN:      return "PORTIN";
            case AsmSymbolTable::STYPE_PORTOUT:     return "PORTOUT";
            case AsmSymbolTable::STYPE_EXPRESSION:  return "EXPR.";
            case AsmSymbolTable::STYPE_DATA:        return "DATA";
        }
    }

    return "";
}

std::ostream & operator << ( std::ostream & out,
                             const AsmSymbolTable * symbolTable )
{
    const std::map<std::string,AsmSymbolTable::Symbol> * table = &(symbolTable->m_table);

    for ( int i = 0; i < 2; i++ )
    {
        for ( const auto & symbol : *table )
        {
            out << symbol.first;
            out << ' ';
            for ( unsigned int i = symbol.first.size(); i < 35; i++ )
            {
                if ( i % 2 )
                {
                    out << ' ';
                }
                else
                {
                    out << '.';
                }
            }

            out << ' ' << AsmSymbolTable::symType2Str(symbol.second.m_type, true);

            if ( -1 == symbol.second.m_finalValue )
            {
                out << "        ";
            }
            else
            {
                char finalValue[9];
                sprintf(finalValue, "0x%05X ", symbol.second.m_finalValue);
                out << finalValue;
            }

            if ( true == symbol.second.m_used )
            {
                out << "USED     ";
            }
            else
            {
                out << "NOT USED ";
            }

            if ( true == symbol.second.m_constant )
            {
                out << "CONSTANT    ";
            }
            else
            {
                out << "REDEFINABLE ";
            }

            if ( AsmMacros::MANGLE_PREFIX == symbol.first[0] )
            {
                out << "LOCAL";
            }
            else
            {
                if ( -1 == symbol.second.m_location.m_fileNumber )
                {
                    out << "IMPLICIT";
                }
                else
                {
                    symbolTable->printSymLocation(out, symbol.second.m_location);
                }
            }

            out << std::endl;
        }

        if ( 0 == i )
        {
            if ( true == symbolTable->m_deletedSymbols.empty() )
            {
                break;
            }
            out << std::endl << "REMOVED SYMBOLS:" << std::endl;
            table = &(symbolTable->m_deletedSymbols);
        }
    }

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const AsmSymbolTable::SymbolType symbolType )
{
    switch ( symbolType )
    {
        case AsmSymbolTable::STYPE_UNSPECIFIED: out << "STYPE_UNSPECIFIED"; break;
        case AsmSymbolTable::STYPE_NUMBER:      out << "STYPE_NUMBER";      break;
        case AsmSymbolTable::STYPE_REGISTER:    out << "STYPE_REGISTER";    break;
        case AsmSymbolTable::STYPE_LABEL:       out << "STYPE_LABEL";       break;
        case AsmSymbolTable::STYPE_PORT:        out << "STYPE_PORT";        break;
        case AsmSymbolTable::STYPE_PORTIN:      out << "STYPE_PORTIN";      break;
        case AsmSymbolTable::STYPE_PORTOUT:     out << "STYPE_PORTOUT";     break;
        case AsmSymbolTable::STYPE_EXPRESSION:  out << "STYPE_EXPRESSION";  break;
        case AsmSymbolTable::STYPE_DATA:        out << "STYPE_DATA";        break;
    }

    return out;
}
