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
 * @file CompilerSerializer.cxx
 */
// =============================================================================

#include "CompilerSerializer.h"

// Standard header files.
#include <cstring>

/**
 * @brief
 * @param[in] a
 * @param[in] n
 * @return
 */
#define BYTE_N(a, n) \
    ( char ) ( ( a & ( 0xffULL << ( 8ULL * n ) ) ) >> ( 8ULL * n ) )

CompilerSerializer::CompilerSerializer ( std::istream & input )
                                       : m_input  ( &input ),
                                         m_output ( NULL ),
                                         m_role   ( DESERIALIZER )
{
}

CompilerSerializer::CompilerSerializer ( std::ostream & output )
                                       : m_input  ( NULL ),
                                         m_output ( &output ),
                                         m_role   ( SERIALIZER )
{
}

void CompilerSerializer::write ( uint8_t val )
{
    serial();
    m_output->put ( BYTE_N(val, 0) );
}

void CompilerSerializer::write ( uint16_t val )
{
    serial();
    m_output->put ( BYTE_N(val, 0) );
    m_output->put ( BYTE_N(val, 1) );
}

void CompilerSerializer::write ( uint32_t val )
{
    serial();
    for ( int i = 0; i < 4; i++ )
    {
        m_output->put ( BYTE_N(val, i) );
    }
}

void CompilerSerializer::write ( uint64_t val )
{
    serial();
    for ( int i = 0; i < 8; i++ )
    {
        m_output->put ( BYTE_N(val, i) );
    }
}

void CompilerSerializer::write ( float val )
{
    serial();
    SFloat2UInt32 value = val;
    for ( int i = 0; i < 4; i++ )
    {
        m_output->put ( BYTE_N(value.m_uint32, i) );
    }
}

void CompilerSerializer::write ( double val )
{
    serial();
    DFloat2UInt64 value = val;
    for ( int i = 0; i < 8; i++ )
    {
        m_output->put ( BYTE_N(value.m_uint64, i) );
    }
}

void CompilerSerializer::write ( const char * val )
{
    size_t len = strlen(val);
    write ( (uint32_t) len );
    m_output->write(val, len);
}

void CompilerSerializer::write ( const std::string & val )
{
    size_t len = val.size();
    write ( (uint32_t) len );
    m_output->write(val.c_str(), len);
}

uint8_t CompilerSerializer::read_ui8()
{
    deserial();
    return (uint8_t) m_input->get();
}

uint16_t CompilerSerializer::read_ui16()
{
    deserial();
    unsigned char buf[2];
    m_input->read ( (char*)buf, 2 );
    return (uint16_t) ( ( buf[1] << 8 ) | buf[0] );
}

uint32_t CompilerSerializer::read_ui32()
{
    deserial();
    unsigned char buf[4];
    m_input->read ( (char*)buf, 4 );

    uint32_t result = 0;
    for ( int i = 3; i >= 0; i-- )
    {
        result |= buf[i];
        result <<= 8;
    }
    return result;
}

uint64_t CompilerSerializer::read_ui64()
{
    deserial();
    unsigned char buf[8];
    m_input->read ( (char*)buf, 8 );

    uint64_t result = 0;
    for ( int i = 7; i >= 0; i-- )
    {
        result |= buf[i];
        result <<= 8;
    }
    return result;
}

float CompilerSerializer::read_float()
{
    SFloat2UInt32 val = read_ui32();
    return val.m_uint32;
}

double CompilerSerializer::read_double()
{
    DFloat2UInt64 val = read_ui64();
    return val.m_uint64;
}

char * CompilerSerializer::read_c_str_copy()
{
    uint32_t len = read_ui32();
    char * result = new char [ len ];
    m_input->read(result, len);
    return result;
}

char * CompilerSerializer::read_c_str ( char * buffer,
                                     size_t n )
{
    uint32_t len = read_ui32();
    if ( len > n )
    {
        throw Exception ( Exception::BUFFER_OVERFLOW );
    }
    m_input->read(buffer, len);
    return buffer;
}

void CompilerSerializer::read_std_str ( std::string & buffer )
{
    uint32_t len = read_ui32();
    buffer.clear();
    for ( uint32_t i = 0; i < len; i++ )
    {
        buffer.push_back ( (char) m_input->get() );
    }
}

inline void CompilerSerializer::serial()
{
    if ( SERIALIZER != m_role )
    {
        throw Exception ( Exception::ILLEGAL_ACCESS );
    }
    if ( true == m_output->fail() )
    {
        throw Exception ( Exception::DATA_STREAM_FAILURE );
    }
}

inline void CompilerSerializer::deserial()
{
    if ( DESERIALIZER != m_role )
    {
        throw Exception ( Exception::ILLEGAL_ACCESS );
    }
    if ( true == m_input->fail() )
    {
        throw Exception ( Exception::DATA_STREAM_FAILURE );
    }
    if ( true == m_input->eof() )
    {
        throw Exception ( Exception::BUFFER_UNDERFLOW );
    }
}
