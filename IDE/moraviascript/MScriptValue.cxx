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
 * @file MScriptValue.cxx
 */
// =============================================================================

#include "MScriptValue.h"

// MScript language interpreter header files.
#include "MScriptExpr.h"

// Standard header files
#include <cstring>
#include <cctype>
#include <cstdlib>

MScriptValue::MScriptValue()
{
    m_type = TYPE_EMPTY;
}

MScriptValue::MScriptValue ( int value )
{
    m_type = TYPE_INT;
    m_data.m_integer = (long long) value;
}

MScriptValue::MScriptValue ( long long value )
{
    m_type = TYPE_INT;
    m_data.m_integer = value;
}

MScriptValue::MScriptValue ( float value )
{
    m_type = TYPE_REAL;
    m_data.m_real = (double) value;
}

MScriptValue::MScriptValue ( double value )
{
    m_type = TYPE_REAL;
    m_data.m_real = value;
}

MScriptValue::MScriptValue ( MScriptExpr * expr )
{
    m_type = TYPE_EXPR;
    m_data.m_expr = expr->first();
}

MScriptValue::MScriptValue ( const char * string )
{
    m_type = TYPE_SYMBOL;
    int length = ( 1 + strlen(string) );
    m_data.m_symbol = new char [ length ];
    memcpy(m_data.m_symbol, string, length);
}

MScriptValue::MScriptValue ( const std::string & string )
{
    m_type = TYPE_SYMBOL;
    int length = ( 1 + string.size() );
    m_data.m_symbol = new char [ length ];
    memcpy(m_data.m_symbol, string.c_str(), length);
}

MScriptValue::MScriptValue ( const unsigned char * array,
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

MScriptValue::MScriptValue ( unsigned char * array,
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

MScriptValue & MScriptValue::makeCopy() const
{
    MScriptValue * result = new MScriptValue();
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

void MScriptValue::completeDelete()
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
                             const MScriptValue & val )
{
    switch ( val.m_type )
    {
        case MScriptValue::TYPE_EMPTY:
            out << "<EMPTY>";
            break;
        case MScriptValue::TYPE_INT:
            out << "0x" << std::hex << val.m_data.m_integer << std::dec;
            break;
        case MScriptValue::TYPE_REAL:
            out << std::scientific << val.m_data.m_real;
            break;
        case MScriptValue::TYPE_EXPR:
            out << val.m_data.m_expr;
            break;
        case MScriptValue::TYPE_SYMBOL:
            out << "'" << val.m_data.m_symbol << "'";
            break;
        case MScriptValue::TYPE_ARRAY:
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
