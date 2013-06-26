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
 * @ingroup MCUDataFiles
 * @file BinFile.cxx
 */
// =============================================================================

#include "BinFile.h"

#include <fstream>
#include <cstdint>

void BinFile::clearAndLoad ( const char * filename ) throw ( DataFile::Exception )
{
    clearAndLoad(std::string(filename));
}

void BinFile::clearAndLoad ( const std::string & filename ) throw ( DataFile::Exception )
{
    std::fstream file ( filename, std::fstream::in | std::fstream::binary );

    if ( false == file.is_open() )
    {
        throw Exception(Exception::EXP_IO_ERROR, "Unable to open file: " + filename);
    }

    unsigned int address = 0;

    while ( false == file.eof() )
    {
        int charRead = file.get();

        if ( true == file.fail() )
        {
            throw Exception(Exception::EXP_IO_ERROR);
        }

        m_memory[address] = uint16_t(charRead);
        address++;

        if ( m_arrsize == address )
        {
            break;
        }
    }

    file.close();
}

void BinFile::save ( const char * filename,
                     bool makeBackup ) throw ( DataFile::Exception )
{
    save(std::string(filename), makeBackup);
}

void BinFile::save ( const std::string & filename,
                     bool makeBackup ) throw ( DataFile::Exception )
{
    // Create backup file
    if ( true == makeBackup )
    {
        rename(filename.c_str(), (filename + "~").c_str());
    }

    std::fstream file (filename, (std::fstream::out | std::fstream::trunc | std::fstream::binary) );

    if ( false == file.is_open())
    {
        throw Exception(Exception::EXP_IO_ERROR);
    }

    for ( unsigned int i = 0; i < m_arrsize; i++ )
    {
        unsigned char octet;

        if ( m_memory[i] != -1 )
        {
            octet = (unsigned char) m_memory[i];
        }
        else
        {
            octet = 0xff;
        }

        file.put(octet);
    }

    file.close();

    if ( true == file.fail() )
    {
        throw Exception(Exception::EXP_IO_ERROR);
    }
}
