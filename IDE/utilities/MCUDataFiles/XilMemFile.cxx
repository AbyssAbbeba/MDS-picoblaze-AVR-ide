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
 * @ingroup MCUDataFiles
 * @file XilMemFile.cxx
 */
// =============================================================================

#include "XilMemFile.h"

#include <cstring>
#include <fstream>
#include <memory>

void XilMemFile::clearAndLoad ( const char * filename )
{
    clearAndLoad(std::string(filename));
}

void XilMemFile::clearAndLoad ( const std::string & filename )
{
    std::ifstream file(filename);

    if ( false == file.is_open() )
    {
        throw DataFileException(DataFileException::EXP_IO_ERROR, "Unable to open file: " + filename);
    }

    clear();

    size_t lineLen;
    unsigned int addr = 0;
    std::unique_ptr<char[]> buffer ( new char [ MAX_LINE_LENGTH ] );

    while ( false == file.eof() )
    {
        char * line = buffer.get();
        file.getline(line, MAX_LINE_LENGTH);

        if ( true == file.bad() )
        {
            throw DataFileException(DataFileException::EXP_IO_ERROR);
        }

        lineLen = strlen(line);

        if ( 0 == lineLen )
        {
            continue;
        }
        if ( '@' != line[0] )
        {
            throw DataFileException(DataFileException::EXP_BAD_RECORD_FORMAT);
        }

        if ( '\r' == line [ lineLen -1 ] )
        {
            line [ lineLen -1 ] = '\0';
        }

        line++;

        if ( false == checkHex(line) )
        {
            throw DataFileException(DataFileException::EXP_BAD_RECORD_FORMAT);
        }

        char * field = getField(&line);
        if ( nullptr == field )
        {
            throw DataFileException(DataFileException::EXP_BAD_RECORD_FORMAT);
        }
        sscanf(field, "%x", &addr);

        addr *= m_bytesPerRecord;
        addr /= 4;

        while ( nullptr != ( field = getField(&line) ) )
        {
            unsigned int value;
            sscanf(field, "%x", &value);

            if ( ( 0 != value ) && ( m_arrsize <= ( m_bytesPerRecord + addr ) ) )
            {
                throw DataFileException(DataFileException::EXP_MEMORY_OVERFLOW);
            }

            unsigned int shift = ( 8 * ( m_bytesPerRecord - 1 ) );
            for ( unsigned int i = 0; i < m_bytesPerRecord; i++ )
            {
                m_memory [ addr++ ] = ( 0xff & ( value >> shift ) );
                shift -= 8;
            }
        }
    }
}

inline char * XilMemFile::getField ( char ** line )
{
    char * result = *line;

    if ( nullptr != *line )
    {
        *line = strchr(*line, ' ');
        if ( nullptr != *line )
        {
            (*line)[0] = '\0';
            (*line)++;
        }
    }

    return result;
}

inline bool XilMemFile::checkHex ( const char * string )
{
    size_t length = strlen(string);

    if ( 0 == length )
    {
        return false;
    }

    for ( size_t i = 0; i < length; i++ )
    {
        if ( ' ' != string[i] && 0 == isxdigit(string[i]) )
        {
            return false;
        }
    }

    return true;
}

void XilMemFile::save ( const char * filename,
                        bool makeBackup )
{
    save(std::string(filename), makeBackup);
}

void XilMemFile::save ( const std::string & filename,
                        bool makeBackup )
{
    // Create backup file
    if ( true == makeBackup )
    {
        rename(filename.c_str(), (filename + "~").c_str());
    }

    std::fstream file(filename, (std::fstream::out | std::fstream::trunc | std::fstream::binary));

    if ( false == file.is_open() )
    {
        throw DataFileException(DataFileException::EXP_IO_ERROR);
    }

    char addrFormat[7];
    char buffer[11];

    sprintf(buffer, "%0x", ( ( m_arrsize / 4 ) - 1 ));
    int addrLen = (int) strlen(buffer);
    if ( addrLen < 4 )
    {
        addrLen = 4;
    }
    sprintf(addrFormat, "@%%0%dX", addrLen );

    unsigned int addr = 0;
    unsigned int atAddr = 0;
    for ( unsigned int i = 0; i < m_linesInTotal; i++ )
    {
        sprintf(buffer, addrFormat, atAddr );
        file << buffer;

        for ( unsigned int recordNo = 0; recordNo < m_recordsPerLine; recordNo++ )
        {
            file << ' ';

            for ( unsigned int byteNo = m_bytesPerRecord; byteNo < 4; byteNo++ )
            {
                atAddr++;
                file << "00";
            }

            for ( unsigned int byteNo = 0; byteNo < m_bytesPerRecord; byteNo++ )
            {
                atAddr++;
                int16_t byte;
                if ( addr < m_arrsize )
                {
                    byte = m_memory[addr++];
                    if ( -1 == byte )
                    {
                        byte = 0;
                    }
                }
                else
                {
                    byte = 0;
                }
                sprintf(buffer, "%02X", byte);

                file << buffer;

                if ( true == file.bad() )
                {
                    throw DataFileException(DataFileException::EXP_IO_ERROR);
                }
            }
        }

        file << m_eol;
    }
}
