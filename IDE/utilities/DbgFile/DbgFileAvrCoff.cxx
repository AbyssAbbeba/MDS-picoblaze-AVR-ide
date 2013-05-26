// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup DbgFile
 * @file DbgFileAvrCoff.cxx
 */
// =============================================================================

#include "DbgFileAvrCoff.h"

#include "MCUDataFiles/ObjectDataFile.h"

#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cstdint>

DbgFileAvrCoff::DbgFileAvrCoff()
{
    m_addrToLineMap = NULL;
    m_lineToAddrMap = NULL;
}

DbgFileAvrCoff::~DbgFileAvrCoff()
{
    if ( NULL != m_addrToLineMap )
    {
        delete[] m_addrToLineMap;
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
}

inline void DbgFileAvrCoff::clear()
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

void DbgFileAvrCoff::openFile ( const std::string & filename ) throw ( Exception )
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

inline void DbgFileAvrCoff::loadFile ( const std::string & filename )
{
    clear();
    m_sourceFile = filename;

    std::fstream file(filename, std::fstream::in | std::fstream::binary );
    if ( false == file.is_open())
    {
        throw Exception(Exception::IO_ERROR, "Unable to open " + filename);
    }

    unsigned int absolutePosition = 0;

    /*
     * Read the file header
     */
    char * rawHdr = new char [ 20 ];
    file.read(rawHdr, 20);
    checkBadAndEof(file, rawHdr);
    absolutePosition += 20;
    // Magic number (0xA12 for AVR COFF)
    uint16_t magicNumber   = ( rawHdr[ 1] <<  8 ) | ( rawHdr[ 0] );
    // Number of sections
    uint16_t noOfSections  = ( rawHdr[ 3] <<  8 ) | ( rawHdr[ 2] );
    // Time and date stamp indicating when the file was created, expressed as the number of elapsed seconds since 
    //+ 00:00:00 GMT, January 1, 1970
    uint32_t timeStamp     = ( rawHdr[ 7] << 24 ) | ( rawHdr[ 6] << 16 ) | ( rawHdr[ 5] << 8 ) | rawHdr[ 4];
    // File pointer to the symbol table
    uint32_t symbolTblPtr  = ( rawHdr[11] << 24 ) | ( rawHdr[10] << 16 ) | ( rawHdr[ 9] << 8 ) | rawHdr[ 8];
    // Number of entries in the symbol table
    uint32_t symbolTblNo   = ( rawHdr[15] << 24 ) | ( rawHdr[14] << 16 ) | ( rawHdr[13] << 8 ) | rawHdr[12];
    // Number of bytes in the optional header
    uint16_t optHeaderSize = ( rawHdr[17] <<  8 ) | ( rawHdr[16] );
    // Flags
    uint16_t flags         = ( rawHdr[19] <<  8 ) | ( rawHdr[18] );

    delete[] rawHdr;

    // 0xA12 identifies AVR COFF
    if ( 0xA12 != magicNumber )
    {
        throw Exception ( Exception::COMPATIBILITY_ERROR,
                                 "The 'Magic Number' has to be 0xA12 (AVR COFF)." );
    }

    switch ( flags )
    {
        // Relocation information stripped from the file
        case OFT_NORELOC:
            break;
        // The file has no unresolved external references
        case OFT_NOUNRES:
            break;
        // Line numbers stripped from the file
        case OFT_NOLINES:
            break;
        // Local symbols stripped from the file
        case OFT_NOSYMBOLS:
            break;
    }

    /*
     * Read the optional header
     */
    if ( 0 != optHeaderSize )
    {
        char * rawOptHdr = new char [ optHeaderSize ];
        file.read(rawOptHdr, optHeaderSize);
        checkBadAndEof(file, rawOptHdr);
        absolutePosition += optHeaderSize;
        // rawOptHdr here contains the optional header read from the COFF object file
        delete[] rawOptHdr;
    }

    /*
     * Read file sections
     */
    for ( unsigned int sectionNumber = 1; sectionNumber <= noOfSections; sectionNumber++ )
    {
        /*
         * Read the section header
         */
        char * rawSecHdr = new char [ 40 ];
        file.read(rawSecHdr, 40);
        checkBadAndEof(file, rawSecHdr);
        absolutePosition += 40;

        // 8-character null padded section name
        char sectionName[8];
        for ( unsigned int i = 0; i < 8; i++ )
        {
            sectionName[i] = rawSecHdr[i];
        }
        // Physical address of section
        uint32_t physicalAddr = ( rawSecHdr[11] << 24 ) | ( rawSecHdr[10] << 16 ) |
                                ( rawSecHdr[ 9] <<  8 ) | ( rawSecHdr[ 8] );

        // Virtual address of section
        uint32_t virtualAddr  = ( rawSecHdr[15] << 24 ) | ( rawSecHdr[14] << 16 ) |
                                ( rawSecHdr[13] <<  8 ) | ( rawSecHdr[12] );

        // Section size in bytes
        uint32_t sectionSize  = ( rawSecHdr[19] << 24 ) | ( rawSecHdr[18] << 16 ) |
                                ( rawSecHdr[17] <<  8 ) | ( rawSecHdr[16] );

        // File pointer to raw data
        uint32_t rawDataPtr   = ( rawSecHdr[23] << 24 ) | ( rawSecHdr[22] << 16 ) |
                                ( rawSecHdr[21] <<  8 ) | ( rawSecHdr[20] );

        // File pointer to relocation entries
        uint32_t relocPtr     = ( rawSecHdr[27] << 24 ) | ( rawSecHdr[26] << 16 ) |
                                ( rawSecHdr[25] <<  8 ) | ( rawSecHdr[24] );

        // File pointer to line number entries
        uint32_t lineNoPtr    = ( rawSecHdr[31] << 24 ) | ( rawSecHdr[30] << 16 ) |
                                ( rawSecHdr[29] <<  8 ) | ( rawSecHdr[28] );

        // Number of relocation entries
        uint16_t noOfRelocEnt = ( rawSecHdr[33] <<  8 ) | ( rawSecHdr[32] );

        // Number of line number entries
        uint16_t noOfLnNoEnt  = ( rawSecHdr[35] <<  8 ) | ( rawSecHdr[34] );

        // Flags
        uint32_t flags        = ( rawSecHdr[39] << 24 ) | ( rawSecHdr[38] << 16 ) |
                                ( rawSecHdr[37] <<  8 ) | ( rawSecHdr[36] );

        delete[] rawSecHdr;

        switch ( flags )
        {
            // Section contains executable text (.text)
            case SF_TEXT:
                break;
            // Section contains initialised data (.data)
            case SF_DATA:
                break; 
            // Section contains uninitialised data (.bss)
            case SF_BSS:
                break;
            // Comment section
            case SF_COMMENT:
                break;
        }

        /*
         * Read the section raw data
         */
        if ( 0 != sectionSize )
        {
            char * rawData = new char [ sectionSize ];
            file.read(rawData, sectionSize);
            checkBadAndEof(file, rawData);
            absolutePosition += sectionSize;
            // --> rawData
            delete[] rawData;
        }

        /*
         * Read the section relocation information
         */
        for ( unsigned int relocEntryNumber = 0; relocEntryNumber < noOfRelocEnt; relocEntryNumber++ )
        {
            char relocInfo[10];
            file.read(relocInfo, 10);
            checkBadAndEof(file);
            absolutePosition += 10;

            // Virtual address of reference
            uint32_t virtualAddr    = ( relocInfo[3] << 24 ) | ( relocInfo[2] << 16 ) |
                                      ( relocInfo[1] <<  8 ) | ( relocInfo[0] );
            // Symbol table index
            uint32_t symbolTblIndex = ( relocInfo[7] << 24 ) | ( relocInfo[6] << 16 ) |
                                      ( relocInfo[5] <<  8 ) | ( relocInfo[4] );
            // Relocation type
            uint16_t relocationType = ( relocInfo[9] <<  8 ) | ( relocInfo[8] );
        }

        /*
         * Read the section line numbers
         */
        for ( unsigned int lineNumberEntryNo = 0; lineNumberEntryNo < noOfLnNoEnt; lineNumberEntryNo++ )
        {
            char lineNumberRaw[6];
            file.read(lineNumberRaw, 6);
            checkBadAndEof(file);
            absolutePosition += 6;

            // Physical address OR symbol index
            uint32_t address    = ( lineNumberRaw[3] << 24 ) | ( lineNumberRaw[2] << 16 ) |
                                  ( lineNumberRaw[1] <<  8 ) | ( lineNumberRaw[0] );
            // Line number
            uint16_t lineNumber = ( lineNumberRaw[5] <<  8 ) | ( lineNumberRaw[4] );
        }
    }

    /*
     * Read the symbol table
     */
    for ( unsigned int symbolTblEntryNo = 0; symbolTblEntryNo < symbolTblNo; symbolTblEntryNo++ )
    {
        char symbolEntryRaw[18];
        file.read(symbolEntryRaw, 18);
        checkBadAndEof(file);
        absolutePosition += 18;

        // These 8 bytes have a symbol name or an index to a symbol in the string table.
        char symbolName[8];
        if ( NULL == symbolEntryRaw[0] )
        {
            for ( int i = 0; i < 8; i++ )
            {
                symbolName[i] = symbolEntryRaw[i];
            }
        }
        else
        {
            uint32_t symbolNameIdx = ( symbolEntryRaw[7] << 24 ) | ( symbolEntryRaw[6] << 16 ) |
                                     ( symbolEntryRaw[5] <<  8 ) | ( symbolEntryRaw[4] <<  0);
        }
        // Symbol value; storage class dependent
        uint32_t symbolValue  = ( symbolEntryRaw[11] << 24 ) | ( symbolEntryRaw[10] << 16 ) |
                                ( symbolEntryRaw[ 9] <<  8 ) | ( symbolEntryRaw[ 8] <<  0);
        // Section number of symbol
        uint16_t symbolSecNum = ( symbolEntryRaw[13] <<  8 ) | ( symbolEntryRaw[12] <<  0);
        // Basic and derived type specification
        uint16_t typeSpec     = ( symbolEntryRaw[15] <<  8 ) | ( symbolEntryRaw[14] <<  0);
        // Storage class of symbol
        uint8_t storageClass  = (uint8_t) symbolEntryRaw[16];
        // Number of auxiliary entries
        uint8_t auxEntries    = (uint8_t) symbolEntryRaw[17];
    }

    /*
     * Read the string table
     */
    char stringTblSize[4];
    file.read(stringTblSize, 4);
    checkBadAndEof(file);
    absolutePosition += 4;
    // Convert to 32-bit unsigned integer
    uint32_t stringTableSize = ( stringTblSize[3] << 24 ) | ( stringTblSize[2] << 16 ) |
                               ( stringTblSize[1] <<  8 ) | ( stringTblSize[0] <<  0);
    // Read the rest of the table
    char * stringTable = new char [ stringTableSize ];
    memcpy(stringTable, stringTblSize, 4);
    file.read(stringTable + 4, stringTableSize);
    checkBadAndEof(file, stringTable);
    absolutePosition += 4;

    file.close();
    generateLineAddressMaps();
}

inline void DbgFileAvrCoff::checkBadAndEof ( const std::fstream & file, char * buffer )
{
    if ( true == file.bad() || true == file.eof() )
    {
        if ( NULL != buffer )
        {
            delete[] buffer;
        }
        throw Exception ( Exception::IO_ERROR, "Read failed, file: " + m_sourceFile );
    }
}

int DbgFileAvrCoff::getLineByAddr ( unsigned int addr ) const
{
    if ( (int)(addr) > m_lastAddress )
    {
        return -1;
    }

    return m_addrToLineMap[addr];
}

int DbgFileAvrCoff::getAddrByLine ( unsigned int line,
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

inline const char * DbgFileAvrCoff::checkHexNumber ( const char * str ) const
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
            throw Exception(Exception::PARSE_ERROR, "Hexadecimal digit character expected.");
        }
    }

    return str;
}

inline int DbgFileAvrCoff::getFileNumber ( const std::string & filename ) const
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

inline void DbgFileAvrCoff::generateLineAddressMaps()
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


    for ( unsigned int i = m_lineRecords.size(); i < m_lineRecords.size(); i++ )
    {
        m_addrToLineMap [ m_lineRecords[i].m_address ] = i;
        m_lineToAddrMap [ m_lineRecords[i].m_fileNumber ] [ m_lineRecords[i].m_lineNumber ] = i;
    }
}

void DbgFileAvrCoff::assignCodeMemDataContainer ( ObjectDataFile * dataContainer )
{
}
