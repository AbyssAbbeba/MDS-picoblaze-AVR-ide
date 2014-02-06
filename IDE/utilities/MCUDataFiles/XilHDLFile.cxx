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
 * @file XilHDLFile.cxx
 */
// =============================================================================

#include "XilHDLFile.h"

#include <ctime>
#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>

// Initialize private static constants.
const char * const XilHDLFile::MARK_TIMESTAMP  = "{timestamp}";
const char * const XilHDLFile::MARK_NAME       = "{name}";
const char * const XilHDLFile::MARK_START      = "{begin template}";
const char * const XilHDLFile::MARK_INIT_E     = "}";
const char * const XilHDLFile::MARK_INITP_E    = "}";
const char * const XilHDLFile::EOL_SEQUENCE    = "\r\n";
const char * const XilHDLFile::MARK_INITP_S[3] = { "{INITP_", "{[8:0]_INITP_", "{[17:9]_INITP_" };
const char * const XilHDLFile::MARK_INIT_S[6]  =
{
    "{INIT_",
    "{[8:0]_INIT_",
    "{[17:9]_INIT_",
    "INIT64_",
    "INIT128_",
    "INIT256_"
};

void XilHDLFile::clearAndLoad ( const char * filename )
{
    clearAndLoad(std::string(filename));
}

void XilHDLFile::clearAndLoad ( const std::string & filename )
{
    std::ifstream file(filename);

    if ( false == file.is_open() )
    {
        throw DataFileException(DataFileException::EXP_IO_ERROR, "Unable to open file: " + filename);
    }

    clear();

    uint32_t byteInt;
    int iterLimit;
    int addr;
    std::string line;
    std::string hexField;
    std::vector<uint32_t> ints;
    char byteStr[5];

    while ( false == file.eof() )
    {
        ints.clear();
        std::getline(file, line);
        if ( true == file.bad() )
        {
            throw DataFileException(DataFileException::EXP_IO_ERROR);
        }

        if ( -1 != ( addr = extractHexField(line, &hexField, "INIT_") ) )
        {
            iterLimit = ( hexField.size() - 4 );

            addr *= ( 16 * ( ( SIZE_16b == m_opCodeSize ) ? 2 : 3 ) );
            byteStr[4] = '\0';

            for ( int i = 0; i <= iterLimit; )
            {
                for ( int j = 0; j < 4; j++ )
                {
                    byteStr[j] = hexField [ i++ ];
                }

                sscanf(byteStr, "%x", &byteInt);
                ints.push_back(byteInt);
            }

            for ( int i = ( (int) ints.size() - 1 ); i >= 0; i-- )
            {
                if ( SIZE_18b == m_opCodeSize )
                {
                    addr++;
                }

                if ( (unsigned int) addr >= m_arrsize )
                {
                    throw DataFileException(DataFileException::EXP_MEMORY_OVERFLOW);
                }

                m_memory[addr++] = (uint16_t) ( 0xff & ( ints[i] >> 8 ) );
                m_memory[addr++] = (uint16_t) ( 0xff & ints[i] );
            }
        }
        else if ( ( SIZE_18b == m_opCodeSize ) && ( -1 != ( addr = extractHexField(line, &hexField, "INITP_") ) ) )
        {
            iterLimit = ( hexField.size() - 2 );

            addr *= ( 8 * 16 * ( ( SIZE_16b == m_opCodeSize ) ? 2 : 3 ) );
            byteStr[2] = '\0';

            for ( int i = 0; i <= iterLimit; )
            {
                byteStr[0] = hexField [ i++ ];
                byteStr[1] = hexField [ i++ ];

                sscanf(byteStr, "%x", &byteInt);
                ints.push_back(byteInt);
            }

            for ( int i = ( (int) ints.size() - 1 ); i >= 0; i-- )
            {
                for ( int shift = 0; shift < 8; shift += 2 )
                {
                    if ( (unsigned int) addr >= m_arrsize )
                    {
                        throw DataFileException(DataFileException::EXP_MEMORY_OVERFLOW);
                    }

                    m_memory[addr] = uint16_t( 0x3 & ( ints[i] >> shift ) );
                    addr += 3;

                    if ( (unsigned int) addr >= m_arrsize )
                    {
                        break;
                    }
                }
            }
        }
    }
}

unsigned int XilHDLFile::hexDigit2uint ( char digit ) const
{
    switch ( digit )
    {
        case '0':           return 0x0;
        case '1':           return 0x1;
        case '2':           return 0x2;
        case '3':           return 0x3;
        case '4':           return 0x4;
        case '5':           return 0x5;
        case '6':           return 0x6;
        case '7':           return 0x7;
        case '8':           return 0x8;
        case '9':           return 0x9;
        case 'A': case 'a': return 0xA;
        case 'B': case 'b': return 0xB;
        case 'C': case 'c': return 0xC;
        case 'D': case 'd': return 0xD;
        case 'E': case 'e': return 0xE;
        default:            return 0xF;
    }
}

void XilHDLFile::save ( const char * filename,
                         bool makeBackup )
{
    save(std::string(filename), makeBackup);
}

void XilHDLFile::save ( const std::string & filename,
                         bool makeBackup )
{
    // Create backup file
    if ( true == makeBackup )
    {
        rename(filename.c_str(), (filename + "~").c_str());
    }

    std::ifstream tmplFile(m_templateFile);
    std::ofstream outFile(filename, ( std::ios_base::out | std::ios_base::trunc ));

    if ( false == tmplFile.is_open() )
    {
        throw DataFileException(DataFileException::EXP_IO_ERROR, "Unable to open template file: " + m_templateFile);
    }
    if ( false == outFile.is_open() )
    {
        throw DataFileException(DataFileException::EXP_IO_ERROR, "Unable to open file: " + filename);
    }

    std::string line;
    size_t position;
    bool starMarkFound = false;

    while ( false == tmplFile.eof() )
    {
        std::getline(tmplFile, line);

        if ( true == tmplFile.bad() || true == outFile.bad() )
        {
            throw DataFileException(DataFileException::EXP_IO_ERROR);
        }

        if ( ( false == starMarkFound ) && ( std::string::npos != line.find(MARK_START) ) )
        {
            starMarkFound = true;
            continue;
        }

        if ( false == starMarkFound )
        {
            continue;
        }

        position = line.find(MARK_TIMESTAMP);
        if ( std::string::npos != position )
        {
            // yyyy-MM-dd hh:mm:ss
            time_t rawtime;
            time(&rawtime);
            const tm * timeStruct = localtime(&rawtime);
            char timeStr[20];
            sprintf ( timeStr,
                      "%04d-%02d-%02d %02d:%02d:%02d",
                      1900 + timeStruct->tm_year,
                      1 + timeStruct->tm_mon,
                      timeStruct->tm_mday,
                      timeStruct->tm_hour,
                      timeStruct->tm_min,
                      timeStruct->tm_sec );

            line.replace(position, strlen(MARK_TIMESTAMP), timeStr);
        }

        position = line.find(MARK_NAME);
        if ( std::string::npos != position )
        {
            line.replace(position, strlen(MARK_NAME), m_name);
        }

        for ( int i = 0; i < 6; i++ )
        {
            if ( std::string::npos != line.find(MARK_INIT_S[i]) )
            {
                substDataMark(line, MARK_INIT_S[i], MARK_INIT_E, MemoryMode(i));
                break;
            }
        }

        if ( SIZE_18b == m_opCodeSize )
        {
            for ( int i = 0; i < 3; i++ )
            {
                if ( std::string::npos != line.find(MARK_INITP_S[i]) )
                {
                    substDataMark(line, MARK_INITP_S[i], MARK_INITP_E, MemoryMode(i), true);
                    break;
                }
            }
        }

        outFile << line << EOL_SEQUENCE;
    }
}

void XilHDLFile::substDataMark ( std::string & line,
                                 const char * markStart,
                                 const char * markEnd,
                                 MemoryMode mode,
                                 bool parity ) const
{
    size_t markStartPos = line.find(markStart);
    if ( std::string::npos == markStartPos )
    {
        return;
    }

    size_t markEndPos = line.find(markEnd, markStartPos);
    if ( std::string::npos == markEndPos )
    {
        throw DataFileException(DataFileException::EXP_TEMPLATE_ERROR);
    }

    size_t markSLen = strlen(markStart);
    size_t markELen = strlen(markEnd);
    size_t addrLen  = ( 1 + markEndPos - markStartPos - markSLen - markELen );

    line.erase(markEndPos, markELen);
    line.erase(markStartPos, markSLen);

    std::string addrStr = line.substr(markStartPos, addrLen);

    if ( false == checkHex(addrStr) )
    {
        throw DataFileException(DataFileException::EXP_TEMPLATE_ERROR);
    }

    unsigned int addr;
    sscanf(addrStr.c_str(), "%x", &addr);
    addr *= ( SIZE_18b == m_opCodeSize ) ? 3 : 2; // Multiply by # of bytes occupied by one instruction the mem. array.

    std::string dataField;
    switch ( mode )
    {
        case MODE_WORD:
            generateDataFieldWord(&dataField, addr, parity);
            break;
        case MODE_LSB:
            generateDataFieldByte(&dataField, addr, parity, true);
            break;
        case MODE_MSB:
            generateDataFieldByte(&dataField, addr, parity, false);
            break;
        case MODE_64b:
            generateDataFieldBit(&dataField, addr, 64);
            break;
        case MODE_128b:
            generateDataFieldBit(&dataField, addr, 128);
            break;
        case MODE_256b:
            generateDataFieldBit(&dataField, addr, 256);
            break;
        default:
            throw DataFileException(DataFileException::EXP_INTERNAL_ERROR, "Invalid Memory Mode");
    }
    line.replace(markStartPos, addrLen, dataField);
}

inline void XilHDLFile::generateDataFieldWord ( std::string * dataField,
                                                unsigned int addr,
                                                bool parity ) const
{
    int byteInt;
    uint32_t data[8];
    char hexString[9];

    if ( false == parity )
    {
        addr *= 16; // 16 == instructions per data field.

        for ( int i = 0; i < 8; i++ )
        {
            data[i] = 0;

            for ( int j = 0; j < 2; j++ )
            {
                if ( SIZE_18b == m_opCodeSize )
                {
                    addr++;
                }

                for ( int k = 0; k < 2; k++ )
                {
                    byteInt = getByte(addr++);

                    data[i] <<= 8;
                    data[i] |= byteInt;
                }
            }

            data[i] = ( ( 0xffff & data[i] ) << 16 ) | ( 0xffff & ( data[i] >> 16) );
        }
    }
    else
    {
        addr *= 128; // 128 == instructions per data field.

        for ( int i = 0; i < 8; i++ )
        {
            data[i] = 0;

            for ( int j = 0; j < 16; j++ )
            {
                byteInt = getByte(addr);

                data[i] >>= 2;
                data[i] |= ( byteInt & 0x3 ) << 30;

                addr += 3;
            }
        }
    }

    for ( int i = 7; i >= 0; i-- )
    {
        sprintf(hexString, "%08X", data[i] );
        dataField->append(hexString);
    }
}

inline void XilHDLFile::generateDataFieldByte ( std::string * dataField,
                                                unsigned int addr,
                                                bool parity,
                                                bool lsb ) const
{
    int byteInt;
    uint32_t data[8];
    char hexString[9];

    if ( false == parity )
    {
        addr *= 32; // 32 == instructions per data field.

        for ( int i = 0; i < 8; i++ )
        {
            data[i] = 0;

            for ( int j = 0; j < 4; j++ )
            {
                if ( true == lsb )
                {
                    addr += 2;
                    byteInt = getByte(addr);
                }
                else
                {
                    byteInt  = ( ( 0x01 & getByte(addr++) ) << 7 );
                    byteInt |= ( ( 0xfe & getByte(addr++) ) >> 1 );
                }

                addr++;
                data[i] >>= 8;
                data[i] |= ( byteInt << 24 );
            }
        }
    }
    else
    {
        addr *= 256; // 256 == instructions per data field.

        for ( int i = 0; i < 8; i++ )
        {
            data[i] = 0;

            for ( int j = 0; j < 32; j++ )
            {
                data[i] >>= 1;
                if ( true == lsb )
                {
                    addr++;
                    byteInt = getByte(addr++);
                    data[i] |= ( byteInt & 1 ) << 31;
                }
                else
                {
                    byteInt = getByte(addr++);
                    addr++;
                    data[i] |= ( ( byteInt >> 1 ) & 1 ) << 31;
                }
                addr++;
            }
        }
    }

    for ( int i = 7; i >= 0; i-- )
    {
        sprintf(hexString, "%08X", data[i] );
        dataField->append(hexString);
    }
}

inline void XilHDLFile::generateDataFieldBit ( std::string * dataField,
                                               unsigned int bit,
                                               unsigned int size ) const
{
    int byteInt;
    unsigned int addr = 0;
    uint32_t data[8];
    char hexString[9];

    for ( int i = 0; i < ( (int) size / 32 ); i++ )
    {
        data[i] = 0;

        for ( int j = 0; j < 32; j++ )
        {
            addr += ( SIZE_18b == m_opCodeSize ) ? 3 : 2;

            if ( bit < 8 )
            {
                byteInt = getByte ( addr - 1 );
            }
            else if ( bit < 16 )
            {
                byteInt = getByte ( addr - 2 );
            }
            else if ( bit < 18 )
            {
                if ( SIZE_18b == m_opCodeSize )
                {
                    byteInt = getByte ( addr - 3 );
                }
                else
                {
                    byteInt = 0;
                }
            }
            else
            {
                byteInt = 0;
            }

            data[i] >>= 1;
            data[i] |= ( 1 & ( byteInt >> ( bit % 8 ) ) ) << 31;
        }
    }

    for ( int i = ( ( (int) size / 32 ) - 1 ); i >= 0; i-- )
    {
        sprintf(hexString, "%08X", data[i] );
        dataField->append(hexString);
    }
}

inline int XilHDLFile::getByte ( unsigned int addr ) const
{
    if ( addr < m_arrsize )
    {
        int byteInt = m_memory[addr];

        if ( -1 == byteInt )
        {
            return 0;
        }

        return byteInt;
    }

    return 0;
}

bool XilHDLFile::checkHex ( const std::string & str ) const
{
    if ( true == str.empty() )
    {
        return false;
    }

    for ( size_t i = 0; i < str.size(); i++ )
    {
        if ( 0 == isxdigit(str[i]) )
        {
            return false;
        }
    }

    return true;
}
