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
 * @ingroup DbgFile
 * @file DbgFileAvraLst.cxx
 */
// =============================================================================

#include "DbgFileAvraLst.h"

#include <cstdlib>
#include <fstream>
#include <cstring>
#include <cctype>

DbgFileAvraLst::DbgFileAvraLst()
{
    m_addrToLineMap = nullptr;
    m_lineToAddrMap = nullptr;
}

DbgFileAvraLst::~DbgFileAvraLst()
{
    if ( nullptr != m_addrToLineMap )
    {
        delete[] m_addrToLineMap;
    }

    if ( nullptr != m_lineToAddrMap )
    {
        for ( unsigned int i = 0; i < m_fileNames.size(); i++ )
        {
            if ( nullptr != m_lineToAddrMap[i] )
            {
                delete[] m_lineToAddrMap[i];
            }
        }
        delete[] m_lineToAddrMap;
    }
}

inline void DbgFileAvraLst::clear()
{
    m_lastAddress = -1;

    m_sourceFile.clear();

    // Clear vectors
    m_numberOfLines.clear();
    m_fileNames.clear();
    m_lineRecords.clear();
    m_symbolRecords.clear();
    m_functionRecords.clear();
    m_typeRecords.clear();
    m_moduleRecords.clear();
}

void DbgFileAvraLst::openFile ( const std::string & filename )
{
    try
    {
        loadFile(filename);
    }
    catch ( const Exception & e )
    {
        clear();
        throw(e);
    }
}

inline void DbgFileAvraLst::loadFile ( const std::string & filename )
{
    clear();
    m_sourceFile = filename;

    std::fstream file(filename, std::fstream::in | std::fstream::binary );
    if ( false == file.is_open())
    {
        throw Exception(Exception::IO_ERROR, "Unable to open " + filename);
    }

    int lineNumber = -4; // Skip the first five lines
    char line [ MAX_LINE_LENGTH + 1 ];
    while ( false == file.eof() )
    {
        ++lineNumber;
        file.getline(line, MAX_LINE_LENGTH);

        if ( true == file.bad() )
        {
            throw Exception(Exception::IO_ERROR, "Read failed, file: " + filename);
        }

        // Skip empty lines
        if ( '\0' == line[0] )
        {
            continue;
        }

        // Chech for valid LST file header (e.g. "AVRA   Ver. 1.3.0 avr8_test0.asm Mon Feb  4 11:35:24 2013")
        if ( -2 == lineNumber )
        {
            std::string header(line);
            size_t verStrPos = header.find("Ver. ");
            size_t fileNamePosStart = std::string::npos;
            size_t fileNamePosEnd = std::string::npos;

            // Check whether there is "AVRA" string and "Ver. " string present in the LST file header
            if ( ( 0 != header.find("AVRA") ) || ( std::string::npos == verStrPos ) )
            {
                throw Exception ( Exception::PARSE_ERROR,
                                         "The listing file does not seem to be generated by the AVRA assembler." );
            }

            // Extract name of the source file from the LST file header
            fileNamePosStart = 1 + header.find(' ', verStrPos + 5 /* 5 == len("Ver. ") */ );
            fileNamePosEnd = header.find(' ', fileNamePosStart);
            m_fileNames.push_back(header.substr(fileNamePosStart, ( fileNamePosEnd - fileNamePosStart )));
        }

        // Skip the first five lines, they don't contain anything useful
        if ( lineNumber < 1 )
        {
            continue;
        }

        // Line beginning with "C:xxxxxx", i.e. line cotaining link to the CODE (program) memory.
        if ( 'C' == line[0] && ':' == line[1] )
        {
            line[8] = '\0'; // trim to 8 characters
            int address = int(strtol(checkHexNumber ( line + 2 ), nullptr, 16));

            m_lineRecords.push_back(LineRecord(0, lineNumber, 0, 0, address));

            if ( address > m_lastAddress )
            {
                m_lastAddress = address;
            }
        }
    }

    if ( true == file.bad() )
    {
        throw Exception(Exception::IO_ERROR, "Unable to read " + filename);
    }

    m_numberOfLines.push_back(lineNumber);

    file.close();
    generateLineAddressMaps();
}

void DbgFileAvraLst::getLineByAddr ( unsigned int addr,
                                     std::vector<unsigned int> & recordNumbers ) const
{
    recordNumbers.clear();

    if ( (int)(addr) > m_lastAddress )
    {
        return;
    }

    if ( -1 != m_addrToLineMap[addr] )
    {
        recordNumbers.push_back ( (unsigned int) m_addrToLineMap[addr] );
    }
}

int DbgFileAvraLst::getAddrByLine ( unsigned int line,
                                    const std::string & filename ) const
{
    int fileNumber = getFileNumber(filename);
    if ( -1 == fileNumber)
    {
        return -1;
    }

    if ( line > m_numberOfLines[fileNumber] )
    {
        return -1;
    }

    return m_lineToAddrMap[fileNumber][line];
}

inline const char * DbgFileAvraLst::checkHexNumber ( const char * str ) const
{
    int length = strlen(str);

    if ( 0 == length )
    {
        throw Exception(Exception::PARSE_ERROR, "Record ends prematurely.");
    }

    for ( int i = 0; i < length; i++ )
    {
        if ( 0 == isxdigit(str[i]) )
        {
            throw Exception(Exception::PARSE_ERROR, "Hexadecimal digit expected.");
        }
    }

    return str;
}

inline int DbgFileAvraLst::getFileNumber ( const std::string & filename ) const
{
    for ( unsigned int i = 0; i < m_fileNames.size(); i++ )
    {
        if ( filename == m_fileNames[i] )
        {
            return i;
        }
    }

    return -1;
}

inline void DbgFileAvraLst::generateLineAddressMaps()
{
    if ( nullptr != m_addrToLineMap )
    {
        delete[] m_addrToLineMap;
    }
    m_addrToLineMap = new int [ m_lastAddress + 1 ];
    for ( int i = 0; i <= m_lastAddress; i++ )
    {
        m_addrToLineMap[i] = -1;
    }

    if ( nullptr != m_lineToAddrMap )
    {
        for ( unsigned int i = 0; i < m_fileNames.size(); i++ )
        {
            if ( nullptr != m_lineToAddrMap[i] )
            {
                delete[] m_lineToAddrMap[i];
            }
        }
        delete[] m_lineToAddrMap;
    }

    m_lineToAddrMap = new int * [ m_fileNames.size() ];

    for ( unsigned int i = 0; i < m_fileNames.size(); i++ )
    {
        m_lineToAddrMap[i] = new int [ m_numberOfLines[i] ];
        for ( unsigned int j = 0; j < m_numberOfLines[i]; j++ )
        {
            m_lineToAddrMap[i][j] = -1;
        }
    }

    for ( unsigned int i = 0; i < m_lineRecords.size(); i++ )
    {
        m_addrToLineMap [ m_lineRecords[i].m_address ] = i;
        m_lineToAddrMap [ m_lineRecords[i].m_fileNumber ] [ m_lineRecords[i].m_lineNumber ] = i;
    }
}
