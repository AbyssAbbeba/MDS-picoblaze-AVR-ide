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
 * @ingroup Compiler
 * @file CompilerValue.cxx
 */
// =============================================================================

#include "CompilerValue.h"

// Compiler compiler header files.
#include "CompilerExpr.h"

// Standard header files.
#include <cstring>
#include <cstdlib>

CompilerValue::CompilerValue()
{
    m_type = TYPE_EMPTY;
}

CompilerValue::CompilerValue ( int value )
{
    m_type = TYPE_INT;
    m_data.m_integer = (long long) value;
}

CompilerValue::CompilerValue ( long long value )
{
    m_type = TYPE_INT;
    m_data.m_integer = value;
}

CompilerValue::CompilerValue ( float value )
{
    m_type = TYPE_REAL;
    m_data.m_real = (double) value;
}

CompilerValue::CompilerValue ( double value )
{
    m_type = TYPE_REAL;
    m_data.m_real = value;
}

CompilerValue::CompilerValue ( CompilerExpr * expr )
{
    m_type = TYPE_EXPR;
    m_data.m_expr = expr->first();
}

CompilerValue::CompilerValue ( const char * string )
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

CompilerValue::CompilerValue ( const std::string & string )
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

CompilerValue::CompilerValue ( const unsigned char * array,
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

CompilerValue::CompilerValue ( unsigned char * array,
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

CompilerValue & CompilerValue::makeCopy() const
{
    CompilerValue * result = new CompilerValue();
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

void CompilerValue::completeDelete()
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

std::ostream & operator << ( std::ostream & out,
                             const CompilerValue & val )
{
    switch ( val.m_type )
    {
        case CompilerValue::TYPE_EMPTY:
            out << "<EMPTY>";
            break;
        case CompilerValue::TYPE_INT:
            out << "0x" << std::hex << val.m_data.m_integer << std::dec;
            break;
        case CompilerValue::TYPE_REAL:
            out << std::scientific << val.m_data.m_real;
            break;
        case CompilerValue::TYPE_EXPR:
            out << val.m_data.m_expr;
            break;
        case CompilerValue::TYPE_SYMBOL:
            out << "'" << val.m_data.m_symbol << "'";
            break;
        case CompilerValue::TYPE_ARRAY:
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
