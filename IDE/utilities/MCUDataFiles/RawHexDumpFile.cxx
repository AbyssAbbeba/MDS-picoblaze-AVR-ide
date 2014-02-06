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
 * @file RawHexDumpFile.cxx
 */
// =============================================================================

#include "RawHexDumpFile.h"

#include <cstdio>
#include <climits>
#include <fstream>
#include <cstdlib>

void RawHexDumpFile::clearAndLoad ( const char * filename )
{
    clearAndLoad(std::string(filename));
}

void RawHexDumpFile::clearAndLoad ( const std::string & filename )
{
    unsigned long number;
    unsigned int addr = 0;
    std::string line;
    std::ifstream file(filename);

    while ( false == getline(file, line).eof() )
    {
        if ( true == file.bad() )
        {
            throw DataFileException(DataFileException::EXP_IO_ERROR);
        }

        if ( ULONG_MAX == ( number = strtoul(line.c_str(), nullptr, 16) ) )
        {
            throw DataFileException(DataFileException::EXP_BAD_RECORD_FORMAT, "Invalid number: `" + line + '\'' );
        }

        if ( SIZE_18b == m_opCodeSize )
        {
            checkAddr(addr);
            m_memory[addr++] = ( ( number >> 16 ) & 0xff );
        }

        checkAddr(addr);
        m_memory[addr++] = ( ( number >> 8 ) & 0xff );

        checkAddr(addr);
        m_memory[addr++] = ( number & 0xff );
    }
}

inline void RawHexDumpFile::checkAddr ( unsigned int addr )
{
    if ( addr >= m_arrsize )
    {
        throw DataFileException(DataFileException::EXP_MEMORY_OVERFLOW);
    }
}

void RawHexDumpFile::save ( const char * filename,
                            bool makeBackup )
{
    save(std::string(filename), makeBackup);
}

void RawHexDumpFile::save ( const std::string & filename,
                            bool makeBackup )
{
    // Create backup file
    if ( true == makeBackup )
    {
        rename(filename.c_str(), (filename + "~").c_str());
    }

    std::ofstream file(filename, ( std::ios_base::out | std::ios_base::trunc ));

    unsigned int opCodeSizeBInt = ( ( SIZE_18b == m_opCodeSize ) ? 3 : 2 );
    char buf[6];

    for ( unsigned int i = ( opCodeSizeBInt - 1 ); i < m_arrsize; i += opCodeSizeBInt )
    {
        unsigned int opCode;
        if ( SIZE_18b == m_opCodeSize )
        {
            opCode = ( ( m_memory [ i - 2 ] << 16 ) | ( m_memory [ i - 1 ] << 8 ) | m_memory [ i ] );
        }
        else
        {
            opCode = ( ( m_memory [ i - 1 ] << 8 ) | m_memory [ i ] );
        }

        if ( true == file.bad() )
        {
            throw DataFileException(DataFileException::EXP_IO_ERROR);
        }

        if ( opCode & 0xfe0000 )
        {
            opCode = 0;
        }

        sprintf(buf, "%05X", opCode);
        file << buf << std::endl;
    }
}
