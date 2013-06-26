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

/**
 * @brief
 * @param[in] array
 * @param[in] idx
 */
#define ASSEMBLE_INT32(array, idx)   \
    binArray [ 0 + 4 * idx ] << 24 | \
    binArray [ 1 + 4 * idx ] << 16 | \
    binArray [ 2 + 4 * idx ] <<  8 | \
    binArray [ 3 + 4 * idx ]


DbgFileNative::DbgFileNative()
{
    m_addrToLineMap = NULL;
    m_lineToAddrMap = NULL;
}

DbgFileNative::DbgFileNative ( const std::string & file)
{
    m_addrToLineMap = NULL;
    m_lineToAddrMap = NULL;
    openFile(file);
}

DbgFileNative::~DbgFileNative()
{
    if ( NULL != m_addrToLineMap )
    {
        delete[] m_addrToLineMap;
    }

    if ( NULL != m_lineToAddrMap )
    {
//         for ( unsigned int i = 0; i < m_fileNames.size(); i++ )
//         {
//             if ( NULL != m_lineToAddrMap[i] )
//             {
//                 delete[] m_lineToAddrMap[i];
//             }
//         }
        delete[] m_lineToAddrMap;
    }
}

void DbgFileNative::openFile ( const std::string & filename ) throw ( Exception )
{
    try
    {
        loadFile(filename);
    }
    catch ( Exception & e )
    {
        clear();
        throw(e);
    }
}

int DbgFileNative::getLineByAddr ( unsigned int addr ) const
{
    if ( (int)(addr) > m_lastAddress )
    {
        return -1;
    }

    return m_addrToLineMap[addr];
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
            // Read binary section.
            static const int size = ( 3 * 4 );
            unsigned char binArray[size];
            for ( int i = 0; i < size; i++ )
            {
                charRead = file.get();
                if ( EOF == charRead )
                {
                    if ( 0 == i )
                    {
                        break;
                    }
                    throw Exception(Exception::IO_ERROR, "Binary section ends unexpectedly, file: " + filename);
                }
                else
                {
                    binArray[i] = (unsigned char) charRead;
                }
            }

            unsigned int address    = ASSEMBLE_INT32(binArray, 0);
            unsigned int fileNumber = ASSEMBLE_INT32(binArray, 1);
            unsigned int lineNo     = ASSEMBLE_INT32(binArray, 2);

            m_lineRecords.push_back(LineRecord(fileNumber, lineNo, 0, 0, address));

            if ( lineNo > m_numberOfLines[fileNumber] )
            {
                m_numberOfLines[fileNumber] = lineNo;
            }
            if ( (int)address > m_lastAddress )
            {
                m_lastAddress = address;
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

    if ( true == file.fail() )
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

void DbgFileNative::directSetupFiles ( const std::vector<std::string> & files )
{
    for ( std::vector<std::string>::const_iterator it = files.cbegin();
          it != files.cend();
          it++ )
    {
        m_fileNames.push_back(*it);
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

void DbgFileNative::directSetupFinalize()
{
    generateLineAddressMaps();
}

inline void DbgFileNative::clear()
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
    if ( NULL != m_addrToLineMap )
    {
        delete[] m_addrToLineMap;
    }
    m_addrToLineMap = new int [ m_lastAddress + 1 ];
    for ( int i = 0; i <= m_lastAddress; i++ )
    {
        m_addrToLineMap[i] = -1;
    }

    if ( NULL != m_lineToAddrMap )
    {
        for ( unsigned int i = 0; i < m_fileNames.size(); i++ )
        {
            if ( NULL != m_lineToAddrMap[i] )
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
