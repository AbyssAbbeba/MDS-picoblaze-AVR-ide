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
#include "MScriptBase.h"
#include "MScriptExpr.h"
#include "MScriptInterpretInterface.h"

// Standard header files.
#include <cstring>
#include <cctype>
#include <cstdlib>

// Used for i18n only.
#include <QObject>

#include <iostream> // DEBUG

MScriptValue::MScriptValue()
{
    m_type = TYPE_EMPTY;
}

MScriptValue::MScriptValue ( bool value )
{
    m_type = TYPE_BOOL;
    m_data.m_bool = value;
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
    m_type = TYPE_FLOAT;
    m_data.m_float = (double) value;
}

MScriptValue::MScriptValue ( double value )
{
    m_type = TYPE_FLOAT;
    m_data.m_float = value;
}

MScriptValue::MScriptValue ( double real,
                             double img )
{
    m_type = TYPE_COMPLEX;
    m_data.m_complex.m_r = real;
    m_data.m_complex.m_i = img;
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

MScriptValue::MScriptValue ( const char * array,
                             int size )
{
    m_type = TYPE_STRING;
    m_data.m_string.m_size = size;
    if ( size > 0 )
    {
        m_data.m_string.m_data = (char*) malloc(size);
        memcpy(m_data.m_string.m_data, array, size);
    }
}

MScriptValue::MScriptValue ( char * array,
                             int size,
                             bool copy )
{
    m_type = TYPE_STRING;
    m_data.m_string.m_size = size;

    if ( true == copy )
    {
        if ( size > 0 )
        {
            m_data.m_string.m_data = (char*) malloc(size);
            memcpy(m_data.m_string.m_data, array, size);
        }
    }
    else
    {
        m_data.m_string.m_data = array;
    }
}

MScriptValue & MScriptValue::makeCopy() const
{
    MScriptValue * result = new MScriptValue();
    makeCopy(*result);
    return *result;
}

void MScriptValue::makeCopy ( MScriptValue & result ) const
{
    result.m_type = m_type;
    switch ( m_type )
    {
        case TYPE_EMPTY:
            break;
        case TYPE_INT:
            result.m_data.m_integer = m_data.m_integer;
            break;
        case TYPE_FLOAT:
            result.m_data.m_float = m_data.m_float;
            break;
        case TYPE_EXPR:
            result.m_data.m_expr = m_data.m_expr->copyEntireChain();
            break;
        case TYPE_SYMBOL:
        {
            int length = ( 1 + strlen(m_data.m_symbol) );
            result.m_data.m_symbol = new char[length];
            memcpy(result.m_data.m_symbol, m_data.m_symbol, length);
            break;
        }
        case TYPE_STRING:
            result.m_data.m_string.m_size = m_data.m_string.m_size;
            result.m_data.m_string.m_data = (char*) malloc(m_data.m_string.m_size);
            memcpy(result.m_data.m_string.m_data, m_data.m_string.m_data, m_data.m_string.m_size);
            break;
        case TYPE_BOOL:
            result.m_data.m_bool = m_data.m_bool;
            break;
        case TYPE_COMPLEX:
            result.m_data.m_complex = m_data.m_complex;
            break;
    }
}

void MScriptValue::completeDelete()
{
std::cout << "MScriptValue::completeDelete() : `" << *this << "'\n" << std::flush;
    if ( TYPE_SYMBOL == m_type )
    {
        delete[] m_data.m_symbol;
    }
    else if ( TYPE_STRING == m_type )
    {
        free(m_data.m_string.m_data);
    }
    else if ( TYPE_EXPR == m_type )
    {
        m_data.m_expr->completeDelete();
    }

    m_type = TYPE_EMPTY;
}

bool MScriptValue::operator == ( const MScriptValue & obj ) const
{
    if ( obj.m_type != m_type )
    {
        return false;
    }

    switch ( m_type )
    {
        case MScriptValue::TYPE_EMPTY:
            return false;
        case MScriptValue::TYPE_INT:
            return ( obj.m_data.m_integer == m_data.m_integer );
        case MScriptValue::TYPE_FLOAT:
            return ( obj.m_data.m_float == m_data.m_float );
        case MScriptValue::TYPE_EXPR:
            return false;
        case MScriptValue::TYPE_SYMBOL:
            if ( 0 == strcmp ( obj.m_data.m_symbol, m_data.m_symbol ) )
            {
                return false;
            }
            break;
        case MScriptValue::TYPE_STRING:
            if ( obj.m_data.m_string.m_size != m_data.m_string.m_size )
            {
                return false;
            }
            for ( unsigned int i = 0; i < m_data.m_string.m_size; i++ )
            {
                if ( obj.m_data.m_string.m_data[i] != m_data.m_string.m_data[i] )
                {
                    return false;
                }
            }
            break;
        case TYPE_BOOL:
            return ( obj.m_data.m_bool == m_data.m_bool );
        case TYPE_COMPLEX:
            return ( obj.m_data.m_complex == m_data.m_complex );
    }

    return true;
}

bool MScriptValue::toBool ( MScriptInterpretInterface * interpret,
                            const MScriptSrcLocation & location ) const
{
    switch ( m_type )
    {
        case MScriptValue::TYPE_INT:
            return ( 0 != m_data.m_integer );
        case MScriptValue::TYPE_STRING:
            return ( 0 != m_data.m_string.m_size );
        case MScriptValue::TYPE_FLOAT:
            return ( ( 0 != m_data.m_float ) && ( false == ::isnan(m_data.m_float) ) );
        case MScriptValue::TYPE_EXPR:
        case MScriptValue::TYPE_EMPTY:
        case MScriptValue::TYPE_SYMBOL:
            interpret->interpreterMessage ( location,
                                            MScriptBase::MT_ERROR,
                                            QObject::tr ( "cannot interpret this value as bool" ) . toStdString() );
            return false;
        case TYPE_BOOL:
            return m_data.m_bool;
        case TYPE_COMPLEX:
            return ( ( 0 != m_data.m_complex.m_r ) || ( 0 != m_data.m_complex.m_i ) );
    }

    return false;
}

long long MScriptValue::toInt ( MScriptInterpretInterface * interpret,
                                const MScriptSrcLocation & location ) const
{
    switch ( m_type )
    {
        case MScriptValue::TYPE_INT:
            return m_data.m_integer;
        case MScriptValue::TYPE_STRING:
            interpret->interpreterMessage ( location,
                                            MScriptBase::MT_ERROR,
                                            QObject::tr ( "cannot interpret string as integer" ) . toStdString() );
            return 0;
        case MScriptValue::TYPE_FLOAT:
            interpret->interpreterMessage ( location,
                                            MScriptBase::MT_ERROR,
                                            QObject::tr ( "cannot interpret float as integer" ) . toStdString() );
            return 0;
        case MScriptValue::TYPE_EXPR:
        case MScriptValue::TYPE_EMPTY:
        case MScriptValue::TYPE_SYMBOL:
            interpret->interpreterMessage ( location,
                                            MScriptBase::MT_ERROR,
                                            QObject::tr ( "cannot interpret this value as integer" )
                                                        . toStdString() );
            return 0;
        case TYPE_BOOL:
            interpret->interpreterMessage ( location,
                                            MScriptBase::MT_ERROR,
                                            QObject::tr ( "cannot interpret bool as integer" ) . toStdString() );
            return 0;
        case TYPE_COMPLEX:
            interpret->interpreterMessage ( location,
                                            MScriptBase::MT_ERROR,
                                            QObject::tr ( "cannot interpret complex number as integer" )
                                                        . toStdString() );
            return 0;
    }

    return 0;
}

void MScriptValue::serialize ( MScriptSerializer & output ) const
{
    if ( NULL == this )
    {
        output.write ( (uint8_t) TYPE_EMPTY );
        return;
    }
    else
    {
        output.write ( (uint8_t) m_type );
    }

    switch ( m_type )
    {
        case TYPE_EMPTY:
            break;
        case TYPE_EXPR:
            output << m_data.m_expr;
            break;
        case TYPE_SYMBOL:
            output.write(m_data.m_symbol);
            break;
        case TYPE_INT:
            output.write ( (uint64_t) m_data.m_integer );
            break;
        case TYPE_FLOAT:
            output.write(m_data.m_float);
            break;
        case TYPE_STRING:
            output.write ( std::string ( ( char* ) m_data.m_string.m_data,
                                          m_data.m_string.m_size ) );
            break;
        case TYPE_BOOL:
            output.write(m_data.m_bool);
            break;
        case TYPE_COMPLEX:
            output.write(m_data.m_complex.m_r);
            output.write(m_data.m_complex.m_i);
            break;
    }
}

void MScriptValue::deserialize ( MScriptSerializer & input )
{
    m_type = Type(input.read_ui8());
    switch ( m_type )
    {
        case TYPE_EMPTY:
            break;
        case TYPE_EXPR:
            m_data.m_expr = new MScriptExpr(input);
            break;
        case TYPE_SYMBOL:
            m_data.m_symbol = input.read_c_str_copy();
            break;
        case TYPE_INT:
            m_data.m_integer = (long long) input.read_ui64();
            break;
        case TYPE_FLOAT:
            m_data.m_float = input.read_double();
            break;
        case TYPE_STRING:
        {
            std::string buffer;
            input.read_std_str(buffer);
            m_data.m_string.m_size = buffer.size();
            memcpy(m_data.m_string.m_data, buffer.c_str(), buffer.size());
            break;
        }
        case TYPE_BOOL:
            m_data.m_bool = input.read_bool();
            break;
        case TYPE_COMPLEX:
            m_data.m_complex.m_r = input.read_double();
            m_data.m_complex.m_i = input.read_double();
            break;
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
        case MScriptValue::TYPE_FLOAT:
            out << std::scientific << val.m_data.m_float;
            break;
        case MScriptValue::TYPE_EXPR:
            out << val.m_data.m_expr;
            break;
        case MScriptValue::TYPE_SYMBOL:
            out << "'" << val.m_data.m_symbol << "'";
            break;
        case MScriptValue::TYPE_STRING:
            out << "{" << std::dec << val.m_data.m_string.m_size << "}:\"";
            for ( unsigned int i = 0; i < val.m_data.m_string.m_size; i++ )
            {
                if ( 0 == isprint(int(val.m_data.m_string.m_data[i])) )
                {
                    if ( val.m_data.m_string.m_data[i] < 16 )
                    {
                        out << "\\x0";
                    }
                    else
                    {
                        out << "\\x";
                    }
                    out << std::hex << int(val.m_data.m_string.m_data[i]) << std::dec;
                }
                else
                {
                    out << val.m_data.m_string.m_data[i];
                }
            }
            out << "\"";
            break;
        case MScriptValue::TYPE_BOOL:
            out << std::boolalpha << val.m_data.m_bool << std::noboolalpha;
            break;
        case MScriptValue::TYPE_COMPLEX:
            out << std::scientific << val.m_data.m_complex.m_r << " + " << val.m_data.m_complex.m_i << "i";
            break;
    }

    return out;
}
