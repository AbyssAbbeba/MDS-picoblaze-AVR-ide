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
 * @ingroup Compiler
 * @file CompilerExpr.cxx
 */
// =============================================================================

#include "CompilerExpr.h"

#include <cstring>
#include <cctype>
#include <cstdlib>
#include <iostream> // DEBUG

CompilerExpr::Value::Value()
{
    m_type = TYPE_EMPTY;
}

CompilerExpr::Value::Value ( int value )
{
    m_type = TYPE_INT;
    m_data.m_integer = (long long) value;
}

CompilerExpr::Value::Value ( long long value )
{
    m_type = TYPE_INT;
    m_data.m_integer = value;
}

CompilerExpr::Value::Value ( float value )
{
    m_type = TYPE_REAL;
    m_data.m_real = (double) value;
}

CompilerExpr::Value::Value ( double value )
{
    m_type = TYPE_REAL;
    m_data.m_real = value;
}

CompilerExpr::Value::Value ( CompilerExpr * expr )
{
    m_type = TYPE_EXPR;
    m_data.m_expr = expr->first();
}

CompilerExpr::Value::Value ( const char * string )
{
    m_type = TYPE_SYMBOL;
    int length = ( 1 + strlen(string) );
    m_data.m_symbol = new char[length];
    memcpy(m_data.m_symbol, string, length);
}

CompilerExpr::Value::Value ( const unsigned char * array,
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

CompilerExpr::Value::Value ( unsigned char * array,
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

CompilerExpr::Value & CompilerExpr::Value::makeCopy() const
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
            result->m_data.m_expr = m_data.m_expr->makeCopy();
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

void CompilerExpr::Value::completeDelete()
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

CompilerExpr::CompilerExpr()
{
    m_operator = OPER_NONE;

    m_next = NULL;
    m_prev = NULL;
}

CompilerExpr::CompilerExpr ( Value value )
{
    m_lValue = value;
    m_operator = OPER_NONE;

    m_next = NULL;
    m_prev = NULL;
}

CompilerExpr::CompilerExpr ( Operator oper,
                             Value value )
{
    m_operator = oper;
    m_rValue = value;

    m_next = NULL;
    m_prev = NULL;
}

CompilerExpr::CompilerExpr ( char oper,
                             Value value )
{
    m_operator = Operator(oper);
    m_rValue = value;

    m_next = NULL;
    m_prev = NULL;
}

CompilerExpr::CompilerExpr ( Value value,
                             Operator oper )
{
    m_operator = Operator(oper);
    m_rValue = value;

    m_next = NULL;
    m_prev = NULL;
}

CompilerExpr::CompilerExpr ( Value value,
                             char oper )
{
    m_operator = Operator(oper);
    m_rValue = value;

    m_next = NULL;
    m_prev = NULL;
}

CompilerExpr::CompilerExpr ( Value lValue,
                             Operator oper,
                             Value rValue )
{
    m_lValue = lValue;
    m_operator = oper;
    m_rValue = rValue;

    m_next = NULL;
    m_prev = NULL;
}

CompilerExpr::CompilerExpr ( Value lValue,
                             char oper,
                             Value rValue )
{
    m_lValue = lValue;
    m_operator = Operator(oper);
    m_rValue = rValue;

    m_next = NULL;
    m_prev = NULL;
}

CompilerExpr * CompilerExpr::makeCopy() const
{
    if ( NULL == this )
    {
        return NULL;
    }

    CompilerExpr * result = new CompilerExpr();
    result->m_lValue = m_lValue.makeCopy();
    result->m_operator = m_operator;
    result->m_rValue = m_rValue.makeCopy();
    return result;
}

CompilerExpr * CompilerExpr::first()
{
    CompilerExpr * expr = this;

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

CompilerExpr * CompilerExpr::addLink ( CompilerExpr * next )
{
    if ( NULL == next )
    {
        return this;
    }
    if ( NULL == this )
    {
        return next;
    }

    next = next->first();

    CompilerExpr * expr = this;
    while ( NULL != expr->m_next )
    {
        expr = expr->m_next;
    }
    expr->m_next = next;
    next->m_prev = expr;

    return next;
}

void CompilerExpr::completeDelete ( CompilerExpr * expr )
{
    expr->completeDelete();
}

void CompilerExpr::completeDelete()
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

std::ostream & operator << ( std::ostream & out,
                             const CompilerExpr::Value & val )
{
    switch ( val.m_type )
    {
        case CompilerExpr::Value::TYPE_EMPTY:
            out << "<EMPTY>";
            break;
        case CompilerExpr::Value::TYPE_INT:
            out << "0x" << std::hex << val.m_data.m_integer << std::dec;
            break;
        case CompilerExpr::Value::TYPE_REAL:
            out << std::scientific << val.m_data.m_real;
            break;
        case CompilerExpr::Value::TYPE_EXPR:
            out << val.m_data.m_expr;
            break;
        case CompilerExpr::Value::TYPE_SYMBOL:
            out << "'" << val.m_data.m_symbol << "'";
            break;
        case CompilerExpr::Value::TYPE_ARRAY:
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
                             const CompilerExpr::Operator & opr )
{
    switch ( opr )
    {
        case CompilerExpr::OPER_NONE:                           break;
        case CompilerExpr::OPER_ADD:         out << "+";        break;
        case CompilerExpr::OPER_SUB:         out << "-";        break;
        case CompilerExpr::OPER_MULT:        out << "*";        break;
        case CompilerExpr::OPER_DIV:         out << "/";        break;
        case CompilerExpr::OPER_MOD:         out << "%";        break;
        case CompilerExpr::OPER_DOT:         out << ".";        break;
        case CompilerExpr::OPER_BOR:         out << "|";        break;
        case CompilerExpr::OPER_BXOR:        out << "^";        break;
        case CompilerExpr::OPER_BAND:        out << "&";        break;
        case CompilerExpr::OPER_LOR:         out << "||";       break;
        case CompilerExpr::OPER_LXOR:        out << "^^";       break;
        case CompilerExpr::OPER_LAND:        out << "&&";       break;
        case CompilerExpr::OPER_LOW:         out << "LOW ";     break;
        case CompilerExpr::OPER_HIGH:        out << "HIGH ";    break;
        case CompilerExpr::OPER_EQ:          out << "==";       break;
        case CompilerExpr::OPER_NE:          out << "!=";       break;
        case CompilerExpr::OPER_LT:          out << "<";        break;
        case CompilerExpr::OPER_LE:          out << "<=";       break;
        case CompilerExpr::OPER_GE:          out << ">=";       break;
        case CompilerExpr::OPER_GT:          out << ">";        break;
        case CompilerExpr::OPER_SHR:         out << ">>";       break;
        case CompilerExpr::OPER_SHL:         out << "<<";       break;
        case CompilerExpr::OPER_CALL:        out << "<CALL>";   break;
        case CompilerExpr::OPER_CMPL:        out << "~";        break;
        case CompilerExpr::OPER_NOT:         out << "!";        break;
        case CompilerExpr::OPER_ADD_ASSIGN:  out << "+=";       break;
        case CompilerExpr::OPER_SUB_ASSIGN:  out << "-=";       break;
        case CompilerExpr::OPER_MUL_ASSIGN:  out << "*=";       break;
        case CompilerExpr::OPER_DIV_ASSIGN:  out << "/=";       break;
        case CompilerExpr::OPER_MOD_ASSIGN:  out << "%=";       break;
        case CompilerExpr::OPER_SHL_ASSIGN:  out << "<<=";      break;
        case CompilerExpr::OPER_SHR_ASSIGN:  out << ">>=";      break;
        case CompilerExpr::OPER_AND_ASSIGN:  out << "&=";       break;
        case CompilerExpr::OPER_ORB_ASSIGN:  out << "|=";       break;
        case CompilerExpr::OPER_XOR_ASSIGN:  out << "^=";       break;
        case CompilerExpr::OPER_INC:         out << "++";       break;
        case CompilerExpr::OPER_DEC:         out << "--";       break;
        case CompilerExpr::OPER_AT:          out << "@";        break;
        case CompilerExpr::OPER_INTERVALS:   out << "..";       break;
        case CompilerExpr::OPER_ASSIGN:      out << "=";        break;
    }
    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const CompilerExpr * expr )
{
    if ( NULL == expr )
    {
        out << "(<ERROR:NULL!>)";
        return out;
    }

    if ( CompilerExpr::OPER_NONE == expr->m_operator )
    {
        out << "(" << expr->m_lValue << ")";
    }
    else
    {
        out << "(" << expr->m_lValue << expr->m_operator << expr->m_rValue << std::string(")");
    }

    if ( NULL != expr->m_next )
    {
        out << " | " << expr->m_next;
    }

    return out;
}
