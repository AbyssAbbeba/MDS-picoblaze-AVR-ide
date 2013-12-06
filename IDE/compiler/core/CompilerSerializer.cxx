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


// Initialize static constants.
const char * const CompilerSerializer::COMMON_FILE_HEADER = "Moravia Microsystems, s.r.o. precompiled code";

CompilerSerializer::CompilerSerializer ( std::istream & input,
                                         std::vector<std::string> & files,
                                         CompilerBase::LangId lang,
                                         CompilerBase::TargetArch arch,
                                         bool hide )
                                       : m_input  ( &input ),
                                         m_output ( NULL ),
                                         m_role   ( DESERIALIZER )
{
    size_t headerLength = ( 1 + strlen(COMMON_FILE_HEADER) );
    char header [ headerLength ];
    read_c_str ( header, headerLength );

    if (
            ( 0 != strncmp(COMMON_FILE_HEADER, header, headerLength) )
                ||
            ( INTERFACE_VERSION != (int16_t) read_ui16() )
                ||
            ( lang != CompilerBase::LangId(read_ui16()) )
                ||
            ( arch != CompilerBase::TargetArch(read_ui16()) )
       )
    {
        input.setstate(std::ios_base::failbit);
        return;
    }

    const unsigned int numberOfFiles = (unsigned int) read_ui32();
    for ( unsigned int i = 0; i < numberOfFiles; i++ )
    {
        std::string filename;
        int fileNumber = -1;
        bool found = false;

        read_std_str(filename);

        if ( true == hide )
        {
            m_fileNumberMap.push_back(-1);
        }
        else
        {
            for ( std::vector<std::string>::const_iterator it = files.cbegin();
                it != files.cend();
                it++ )
            {
                fileNumber++;
                if ( *it == filename )
                {
                    found = true;
                    break;
                }
            }

            if ( false == found )
            {
                fileNumber++;
                files.push_back(filename);
            }

            m_fileNumberMap.push_back(fileNumber);
        }
    }
}

CompilerSerializer::CompilerSerializer ( std::ostream & output,
                                         const std::vector<std::string> & files,
                                         CompilerBase::LangId lang,
                                         CompilerBase::TargetArch arch )
                                       : m_input  ( NULL ),
                                         m_output ( &output ),
                                         m_role   ( SERIALIZER )
{
    write ( COMMON_FILE_HEADER );
    write ( (uint16_t) INTERFACE_VERSION );
    write ( (uint16_t) lang );
    write ( (uint16_t) arch );

    write ( (uint32_t) files.size() );
    for ( std::vector<std::string>::const_iterator it = files.cbegin();
          it != files.cend();
          it++ )
    {
        write ( *it );
    }
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
    uint8_t result = (uint8_t) m_input->get();
    return result;
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
    for ( int i = 3; i > 0; i-- )
    {
        result |= buf[i];
        result <<= 8;
    }
    result |= buf[0];

    return result;
}

uint64_t CompilerSerializer::read_ui64()
{
    deserial();
    unsigned char buf[8];
    m_input->read ( (char*)buf, 8 );

    uint64_t result = 0;
    for ( int i = 7; i > 0; i-- )
    {
        result |= buf[i];
        result <<= 8;
    }
    result |= buf[0];

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
    char * result = new char [ len + 1 ];
    m_input->read(result, len);
    result[len] = '\0';

    return result;
}

char * CompilerSerializer::read_c_str ( char * buffer,
                                        size_t n )
{
    uint32_t len = read_ui32();
    if ( len >= n )
    {
        throw Exception ( Exception::BUFFER_OVERFLOW );
    }
    m_input->read(buffer, len);
    buffer[len] = '\0';

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

int CompilerSerializer::translateFileNumber ( int number ) const
{
    if ( m_fileNumberMap.size() <= (size_t) number )
    {
        return -1;
    }
    else
    {
        return m_fileNumberMap[number];
    }
}
