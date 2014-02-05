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
 * @file DbgFileNative.cxx
 */
// =============================================================================

#include "DbgFileNative.h"

// Compiler header files.
#include "../../compiler/core/CompilerFileHeaders.h"

// Standard header files.
#include <fstream>
#include <string>

DbgFileNative::DbgFileNative()
{
    m_addrToLineMap = nullptr;
    m_lineToAddrMap = nullptr;
}

DbgFileNative::DbgFileNative ( const std::string & file)
{
    m_addrToLineMap = nullptr;
    m_lineToAddrMap = nullptr;
    openFile(file);
}

DbgFileNative::~DbgFileNative()
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

void DbgFileNative::openFile ( const std::string & filename )
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

void DbgFileNative::getLineByAddr ( unsigned int addr,
                                    std::vector<unsigned int> & recordNumbers ) const
{
    if ( (int)(addr) > m_lastAddress )
    {
        recordNumbers.clear();
        return;
    }

    recordNumbers = m_addrToLineMap[addr];
}

int DbgFileNative::getAddrByLine ( unsigned int line,
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

inline void DbgFileNative::loadFile ( const std::string & filename )
{
    clear();
    m_sourceFile = filename;

    std::fstream file(filename, std::fstream::in | std::fstream::binary );
    if ( false == file.is_open())
    {
        throw Exception(Exception::IO_ERROR, "Unable to open " + filename);
    }

    int charRead;
    int lineNumber = 0; // -1 means binary data, 0 means file header, >0 means file name

    while ( false == file.eof() )
    {
        if ( -1 == lineNumber )
        {
            /*
             * Read binary section.
             */

            unsigned int address;
            unsigned int lineNo;
            unsigned int fileNumber;

            bool stop = false;
            unsigned char binArray[4];

            for ( int i = 0, j = 0; i < ( 4 + 1 + 2 + 4 ); i++, j++ )
            {
                if ( -1 == ( charRead = file.get() ) )
                {
                    if ( 0 == i )
                    {
                        break;
                    }

                    throw Exception(Exception::IO_ERROR, "Binary section ends unexpectedly, file: " + filename);
                }

                binArray[j] = (unsigned char) charRead;

                switch ( i )
                {
                    case 3:
                    {
                        // Read adddress (4B).
                        address = ( binArray[0] << 24 ) | ( binArray[1] << 16 ) | ( binArray[2] << 8 ) | binArray[3];

                        // Start refilling the binArray from start.
                        j = -1;

                        // Adjust highest address met so far.
                        if ( (int) address > m_lastAddress )
                        {
                            m_lastAddress = address;
                        }

                        // Continue reading the file.
                        break;
                    }
                    case 4:
                    {
                        // Read `NEXT' mark (1B).
                        switch ( charRead )
                        {
                            case '\0':
                                // End of record.
                                stop = true;
                                break;
                            case '\1':
                                // Record continues.
                                break;
                            default:
                                // Record is corrupted.
                                throw Exception(Exception::IO_ERROR, "Binary section is corrupted, file: " + filename);
                        }

                        // Start refilling the binArray from start.
                        j = -1;

                        // Continue reading the file.
                        break;
                    }
                    case 6:
                    {
                        // Read file number (2B).
                        fileNumber = ( binArray[0] << 8 ) | binArray[1];

                        // Check the file number.
                        if ( fileNumber >= m_fileNames.size() )
                        {
                            throw Exception(Exception::PARSE_ERROR, "( file number >= number of files )" );
                        }

                        // Start refilling the binArray from start.
                        j = -1;

                        // Continue reading the file.
                        break;
                    }
                    case 10:
                    {
                        // Read line number (4B).
                        lineNo = ( binArray[0] << 24 ) | ( binArray[1] << 16 ) | ( binArray[2] << 8 ) | binArray[3];

                        // Start refilling the binArray from start.
                        j = -1;

                        // Go back to read the next `NEXT' mark.
                        i = 3;

                        // Create the actual line record.
                        m_lineRecords.push_back(LineRecord(fileNumber, lineNo, 0, 0, address));

                        // Adjust recorded number of lines for a particular file number.
                        if ( lineNo > m_numberOfLines[fileNumber] )
                        {
                            m_numberOfLines[fileNumber] = lineNo;
                        }

                        // Continue reading the file.
                        break;
                    }
                }

                if ( true == stop )
                {
                    break;
                }
            }
        }
        else if ( 0 == lineNumber )
        {
            // Read file header.
            std::string header;
            while ( false == file.eof() && ( '\n' != (charRead = file.get()) ) )
            {
                header.push_back(charRead);
            }
            lineNumber++;

            // Check whether the read file header is supported.
            if ( CompilerFileHeaders::AsmNativeDgbFile != header )
            {
                throw Exception(Exception::IO_ERROR, "Unsupported file format, file: " + filename);
            }
        }
        else
        {
            // Read file names section.
            std::string filename;
            while ( false == file.eof() && ( '\n' != (charRead = file.get()) ) )
            {
                filename.push_back((char)charRead);
            }

            if ( false == filename.empty() )
            {
                // Add the read file name to the list of source files.
                m_fileNames.push_back(filename);
                m_numberOfLines.push_back(0);
                lineNumber++;
            }
            else
            {
                // Enter the binary section in the next iteration.
                lineNumber = -1;
            }
        }
    }

    if ( true == file.bad() )
    {
        throw Exception(Exception::IO_ERROR, "Unable to read " + filename);
    }

    file.close();
    generateLineAddressMaps();
}

void DbgFileNative::directSetupPrepare()
{
    clear();
}

void DbgFileNative::directSetupFiles ( const std::vector<std::pair<std::string,FILE*>> & files )
{
    for ( const auto & file : files )
    {
        m_fileNames.push_back(file.first);
        m_numberOfLines.push_back(0);
    }
}

void DbgFileNative::directSetupRelation ( unsigned int addr,
                                          unsigned int file,
                                          unsigned int line )
{
    m_lineRecords.push_back(LineRecord(file, line, 0, 0, addr));

    if ( line > m_numberOfLines[file] )
    {
        m_numberOfLines[file] = line;
    }

    if ( (int)addr > m_lastAddress )
    {
        m_lastAddress = addr;
    }
}

void DbgFileNative::directSetupRelation ( unsigned int addr,
                                          const std::vector<std::pair<unsigned int, unsigned int>> & locations )
{
    for ( auto loc : locations )
    {
        directSetupRelation(addr, loc.first, loc.second);
    }
}

void DbgFileNative::directSetupFinalize()
{
    generateLineAddressMaps();
}

inline void DbgFileNative::clear()
{
    m_lastAddress = -1;

    // Clear vectors
    m_fileNames.clear();
    m_sourceFile.clear();
    m_lineRecords.clear();
    m_typeRecords.clear();
    m_numberOfLines.clear();
    m_moduleRecords.clear();
    m_symbolRecords.clear();
    m_functionRecords.clear();
}

inline int DbgFileNative::getFileNumber ( const std::string & filename ) const
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

inline void DbgFileNative::generateLineAddressMaps()
{
    if ( nullptr != m_addrToLineMap )
    {
        delete[] m_addrToLineMap;
    }

    m_addrToLineMap = new std::vector<unsigned int> [ m_lastAddress + 1 ];

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
        m_lineToAddrMap[i] = new int [ 1 + m_numberOfLines[i] ];
        for ( unsigned int j = 0; j < ( 1 + m_numberOfLines[i] ); j++ )
        {
            m_lineToAddrMap[i][j] = -1;
        }
    }

    for ( unsigned int i = 0; i < m_lineRecords.size(); i++ )
    {
        m_addrToLineMap [ m_lineRecords[i].m_address ] . push_back(i);
        m_lineToAddrMap [ m_lineRecords[i].m_fileNumber ] [ m_lineRecords[i].m_lineNumber ] = i;
    }
}
