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
 * @ingroup MoraviaScript
 * @file MScriptExpr.cxx
 */
// =============================================================================

#include "MScriptExpr.h"

// Standard header files
#include <cstring>
#include <cctype>
#include <cstdlib>

MScriptExpr::Value::Value()
{
    m_type = TYPE_EMPTY;
}

MScriptExpr::Value::Value ( int value )
{
    m_type = TYPE_INT;
    m_data.m_integer = (long long) value;
}

MScriptExpr::Value::Value ( long long value )
{
    m_type = TYPE_INT;
    m_data.m_integer = value;
}

MScriptExpr::Value::Value ( float value )
{
    m_type = TYPE_REAL;
    m_data.m_real = (double) value;
}

MScriptExpr::Value::Value ( double value )
{
    m_type = TYPE_REAL;
    m_data.m_real = value;
}

MScriptExpr::Value::Value ( MScriptExpr * expr )
{
    m_type = TYPE_EXPR;
    m_data.m_expr = expr->first();
}

MScriptExpr::Value::Value ( const char * string )
{
//     if ( ( TYPE_SYMBOL == m_type ) && ( NULL != m_data.m_symbol ) )
//     {
//         delete [] m_data.m_symbol;
//     }

    m_type = TYPE_SYMBOL;
    int length = ( 1 + strlen(string) );
    m_data.m_symbol = new char [ length ];
    memcpy(m_data.m_symbol, string, length);
}

MScriptExpr::Value::Value ( const std::string & string )
{
//     if ( ( TYPE_SYMBOL == m_type ) && ( NULL != m_data.m_symbol ) )
//     {
//         delete [] m_data.m_symbol;
//     }

    m_type = TYPE_SYMBOL;
    int length = ( 1 + string.size() );
    m_data.m_symbol = new char [ length ];
    memcpy(m_data.m_symbol, string.c_str(), length);
}

MScriptExpr::Value::Value ( const unsigned char * array,
                             int size )
{
    m_type = TYPE_ARRAY;
    m_data.m_array.m_size = size;
    if ( size > 0 )
    {
        m_data.m_array.m_data = (unsigned char*) malloc(size);
        memcpy(m_data.m_array.m_data, array, size);
    }
}

MScriptExpr::Value::Value ( unsigned char * array,
                             int size,
                             bool copy )
{
    m_type = TYPE_ARRAY;
    m_data.m_array.m_size = size;

    if ( true == copy )
    {
        if ( size > 0 )
        {
            m_data.m_array.m_data = (unsigned char*) malloc(size);
            memcpy(m_data.m_array.m_data, array, size);
        }
    }
    else
    {
        m_data.m_array.m_data = array;
    }
}

MScriptExpr::Value & MScriptExpr::Value::makeCopy() const
{
    Value * result = new Value();
    result->m_type = m_type;
    switch ( m_type )
    {
        case TYPE_EMPTY:
            break;
        case TYPE_INT:
            result->m_data.m_integer = m_data.m_integer;
            break;
        case TYPE_REAL:
            result->m_data.m_real = m_data.m_real;
            break;
        case TYPE_EXPR:
            result->m_data.m_expr = m_data.m_expr->copyEntireChain();
            break;
        case TYPE_SYMBOL:
        {
            int length = ( 1 + strlen(m_data.m_symbol) );
            result->m_data.m_symbol = new char[length];
            memcpy(result->m_data.m_symbol, m_data.m_symbol, length);
            break;
        }
        case TYPE_ARRAY:
            result->m_data.m_array.m_size = m_data.m_array.m_size;
            result->m_data.m_array.m_data = (unsigned char*) malloc(m_data.m_array.m_size);
            memcpy(result->m_data.m_array.m_data, m_data.m_array.m_data, m_data.m_array.m_size);
            break;
    }
    return * result;
}

void MScriptExpr::Value::completeDelete()
{
    if ( TYPE_SYMBOL == m_type )
    {
        delete[] m_data.m_symbol;
    }
    else if ( TYPE_ARRAY == m_type )
    {
        free(m_data.m_array.m_data);
    }
    else if ( TYPE_EXPR == m_type )
    {
        m_data.m_expr->completeDelete();
    }
}

MScriptExpr::MScriptExpr ( MScriptSrcLocation location )
{
    m_operator = OPER_NONE;
    m_location = location;

    m_next = NULL;
    m_prev = NULL;
}

MScriptExpr::MScriptExpr ( Value value,
                             MScriptSrcLocation location )
{
    m_lValue = value;
    m_operator = OPER_NONE;
    m_location = location;

    m_next = NULL;
    m_prev = NULL;
}

MScriptExpr::MScriptExpr ( Operator oper,
                             Value value,
                             MScriptSrcLocation location )
{
    m_operator = oper;
    m_rValue = value;
    m_location = location;

    m_next = NULL;
    m_prev = NULL;
}

MScriptExpr::MScriptExpr ( char oper,
                             Value value,
                             MScriptSrcLocation location )
{
    m_operator = Operator(oper);
    m_rValue = value;
    m_location = location;

    m_next = NULL;
    m_prev = NULL;
}

MScriptExpr::MScriptExpr ( Value value,
                             Operator oper,
                             MScriptSrcLocation location )
{
    m_operator = Operator(oper);
    m_rValue = value;
    m_location = location;

    m_next = NULL;
    m_prev = NULL;
}

MScriptExpr::MScriptExpr ( Value value,
                             char oper,
                             MScriptSrcLocation location )
{
    m_operator = Operator(oper);
    m_rValue = value;
    m_location = location;

    m_next = NULL;
    m_prev = NULL;
}

MScriptExpr::MScriptExpr ( Value lValue,
                             Operator oper,
                             Value rValue,
                             MScriptSrcLocation location )
{
    m_lValue = lValue;
    m_operator = oper;
    m_rValue = rValue;
    m_location = location;

    m_next = NULL;
    m_prev = NULL;
}

MScriptExpr::MScriptExpr ( Value lValue,
                             char oper,
                             Value rValue,
                             MScriptSrcLocation location )
{
    m_lValue = lValue;
    m_operator = Operator(oper);
    m_rValue = rValue;
    m_location = location;

    m_next = NULL;
    m_prev = NULL;
}

MScriptExpr * MScriptExpr::first()
{
    MScriptExpr * expr = this;

    if ( NULL == this )
    {
        return NULL;
    }

    while ( NULL != expr->m_prev )
    {
        expr = expr->m_prev;
    }

    return expr;
}

MScriptExpr * MScriptExpr::last()
{
    MScriptExpr * result = this;
    while ( NULL != result->m_next )
    {
        result = result->m_next;
    }
    return result;
}

MScriptExpr * MScriptExpr::insertLink ( MScriptExpr * chainLink )
{
    if ( NULL == chainLink )
    {
        return this;
    }
    if ( NULL == this )
    {
        return chainLink;
    }

    MScriptExpr * chainLinkOrig = chainLink;
    MScriptExpr * chainLinkLast = chainLink->last();
    MScriptExpr * nextOrig = m_next;

    chainLink = chainLink->first();

    m_next = chainLink;
    chainLink->m_prev = this;

    chainLinkLast->m_next = nextOrig;
    nextOrig->m_prev = chainLinkLast;

    return chainLinkOrig;
}

MScriptExpr * MScriptExpr::appendLink ( MScriptExpr * chainLink )
{
    if ( NULL == chainLink )
    {
        return this;
    }
    if ( NULL == this )
    {
        return chainLink;
    }

    MScriptExpr * chainLinkOrig = chainLink;
    MScriptExpr * lastLink = last();

    chainLink = chainLink->first();
    lastLink->m_next = chainLink;
    chainLink->m_prev = lastLink;

    return chainLinkOrig;
}

MScriptExpr * MScriptExpr::prependLink ( MScriptExpr * chainLink )
{
    if ( NULL == chainLink )
    {
        return this;
    }
    if ( NULL == this )
    {
        return chainLink;
    }

    MScriptExpr * chainLinkOrig = chainLink;
    MScriptExpr * firstLink = first();

    chainLink = chainLink->last();
    chainLink->m_next = firstLink;
    firstLink->m_prev = chainLink;

    return chainLinkOrig;
}

void MScriptExpr::completeDelete ( MScriptExpr * expr )
{
    expr->completeDelete();
}

void MScriptExpr::completeDelete()
{
    if ( NULL == this )
    {
        return;
    }

    m_lValue.completeDelete();
    m_rValue.completeDelete();

    if ( NULL != m_next )
    {
        m_next->m_prev = NULL;
        m_next->completeDelete();
    }

    if ( NULL != m_prev )
    {
        m_prev->m_next = NULL;
        m_prev->completeDelete();
    }

    delete this;
}

MScriptExpr * MScriptExpr::operator [] ( int index )
{
    MScriptExpr * result = this;
    for ( int i = 0; i < index; i++ )
    {
        result = result->m_next;
        if ( NULL == result )
        {
            return NULL;
        }
    }
    return result;
}

MScriptExpr * MScriptExpr::copyEntireChain() const
{
    if ( NULL == this )
    {
        return NULL;
    }

    MScriptExpr * result = copyChainLink();

    const MScriptExpr * next = this;
    while ( NULL != ( next = next->m_next ) )
    {
        result->appendLink(next->copyChainLink());
    }

    const MScriptExpr * prev = this;
    while ( NULL != ( prev = prev->m_prev ) )
    {
        result->prependLink(prev->copyChainLink());
    }

    return result;
}

MScriptExpr * MScriptExpr::copyChainLink() const
{
    if ( NULL == this )
    {
        return NULL;
    }

    MScriptExpr * result = new MScriptExpr();
    result->m_lValue = m_lValue.makeCopy();
    result->m_operator = m_operator;
    result->m_rValue = m_rValue.makeCopy();
    result->m_location = m_location;
    return result;
}

MScriptExpr * MScriptExpr::unlink()
{
    m_next->m_prev = NULL;
    m_next = NULL;
    return this;
}

int MScriptExpr::eval() const
{
    return 0;
}

std::ostream & operator << ( std::ostream & out,
                             const MScriptExpr::Value & val )
{
    switch ( val.m_type )
    {
        case MScriptExpr::Value::TYPE_EMPTY:
            out << "<EMPTY>";
            break;
        case MScriptExpr::Value::TYPE_INT:
            out << "0x" << std::hex << val.m_data.m_integer << std::dec;
            break;
        case MScriptExpr::Value::TYPE_REAL:
            out << std::scientific << val.m_data.m_real;
            break;
        case MScriptExpr::Value::TYPE_EXPR:
            out << val.m_data.m_expr;
            break;
        case MScriptExpr::Value::TYPE_SYMBOL:
            out << "'" << val.m_data.m_symbol << "'";
            break;
        case MScriptExpr::Value::TYPE_ARRAY:
            out << "{" << std::dec << val.m_data.m_array.m_size << "}:\"";
            for ( int i = 0; i < val.m_data.m_array.m_size; i++ )
            {
                if ( 0 == isprint(int(val.m_data.m_array.m_data[i])) )
                {
                    if ( val.m_data.m_array.m_data[i] < 16 )
                    {
                        out << "\\x0";
                    }
                    else
                    {
                        out << "\\x";
                    }
                    out << std::hex << int(val.m_data.m_array.m_data[i]) << std::dec;
                }
                else
                {
                    out << val.m_data.m_array.m_data[i];
                }
            }
            out << "\"";
            break;
    }
    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const MScriptExpr::Operator & opr )
{
    switch ( opr )
    {
        case MScriptExpr::OPER_NONE:                            break;
        case MScriptExpr::OPER_ADD:             out << "+";     break;
        case MScriptExpr::OPER_SUB:             out << "-";     break;
        case MScriptExpr::OPER_MULT:            out << "*";     break;
        case MScriptExpr::OPER_DIV:             out << "/";     break;
        case MScriptExpr::OPER_MOD:             out << "%";     break;
        case MScriptExpr::OPER_BOR:             out << "|";     break;
        case MScriptExpr::OPER_BXOR:            out << "^";     break;
        case MScriptExpr::OPER_BAND:            out << "&";     break;
        case MScriptExpr::OPER_LOR:             out << "||";    break;
        case MScriptExpr::OPER_LAND:            out << "&&";    break;
        case MScriptExpr::OPER_EQ:              out << "==";    break;
        case MScriptExpr::OPER_NE:              out << "!=";    break;
        case MScriptExpr::OPER_LT:              out << "<";     break;
        case MScriptExpr::OPER_LE:              out << "<=";    break;
        case MScriptExpr::OPER_GE:              out << ">=";    break;
        case MScriptExpr::OPER_GT:              out << ">";     break;
        case MScriptExpr::OPER_SHR:             out << ">>";    break;
        case MScriptExpr::OPER_SHL:             out << "<<";    break;
        case MScriptExpr::OPER_CMPL:            out << "~";     break;
        case MScriptExpr::OPER_NOT:             out << "!";     break;
        case MScriptExpr::OPER_ADD_ASSIGN:      out << "+=";    break;
        case MScriptExpr::OPER_SUB_ASSIGN:      out << "-=";    break;
        case MScriptExpr::OPER_MUL_ASSIGN:      out << "*=";    break;
        case MScriptExpr::OPER_DIV_ASSIGN:      out << "/=";    break;
        case MScriptExpr::OPER_MOD_ASSIGN:      out << "%=";    break;
        case MScriptExpr::OPER_SHL_ASSIGN:      out << "<<=";   break;
        case MScriptExpr::OPER_SHR_ASSIGN:      out << ">>=";   break;
        case MScriptExpr::OPER_AND_ASSIGN:      out << "&=";    break;
        case MScriptExpr::OPER_ORB_ASSIGN:      out << "|=";    break;
        case MScriptExpr::OPER_XOR_ASSIGN:      out << "^=";    break;
        case MScriptExpr::OPER_ASSIGN:          out << "=";     break;
        case MScriptExpr::OPER_TERNARY:         out << "?";     break;
        case MScriptExpr::OPER_COLON:           out << ":";     break;
        case MScriptExpr::OPER_COMMA:           out << ",";     break;

        case MScriptExpr::OPER_INDEX:           out << "idx[]"; break;
        case MScriptExpr::OPER_PRE_INC:         out << "++pre"; break;
        case MScriptExpr::OPER_PRE_DEC:         out << "--pre"; break;
        case MScriptExpr::OPER_POST_INC:        out << "post++";break;
        case MScriptExpr::OPER_POST_DEC:        out << "post--";break;
        case MScriptExpr::OPER_CALL:            out << "<call>";break;
        case MScriptExpr::OPER_REF:             out << "<&ref>";break;
        case MScriptExpr::OPER_INT_PROM:        out << "<+pos>";break;
        case MScriptExpr::OPER_ADD_INV:         out << "<-neg>";break;
    }
    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const MScriptExpr * const expr )
{
    if ( NULL == expr )
    {
        out << "(<ERROR:NULL!>)";
        return out;
    }

    if ( MScriptExpr::OPER_NONE == expr->m_operator )
    {
        out << "(" << expr->m_lValue << ")";
    }
    else
    {
        out << "(" << expr->m_lValue << " " << expr->m_operator << " " << expr->m_rValue << std::string(")");
    }

    out << " {";
    out << expr->m_location;
    out << "}";

    if ( NULL != expr->m_next )
    {
        out << " | " << expr->m_next;
    }

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const MScriptExpr & expr )
{
    out << &expr;
    return out;
}
