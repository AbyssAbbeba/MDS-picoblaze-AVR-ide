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
 * @file XilHDLFile.cxx
 */
// =============================================================================

#include "XilHDLFile.h"

#include <fstream>
#include <cstring>
#include <cstdio>
#include <string>
#include <ctime>

// Initialize private static constants.
const char * const XilHDLFile::MARK_TIMESTAMP = "{timestamp}";
const char * const XilHDLFile::MARK_NAME      = "{name}";
const char * const XilHDLFile::MARK_START     = "{begin template}";
const char * const XilHDLFile::MARK_INIT_S    = "{INIT_";
const char * const XilHDLFile::MARK_INIT_E    = "}";
const char * const XilHDLFile::MARK_INITP_S   = "{INITP_";
const char * const XilHDLFile::MARK_INITP_E   = "}";
const char * const XilHDLFile::EOL_SEQUENCE   = "\r\n";

void XilHDLFile::clearAndLoad ( const char * filename ) throw ( DataFileException )
{
    clearAndLoad(std::string(filename));
}

void XilHDLFile::clearAndLoad ( const std::string & filename ) throw ( DataFileException )
{
    std::ifstream file(filename);

    if ( false == file.is_open() )
    {
        throw DataFileException(DataFileException::EXP_IO_ERROR, "Unable to open file: " + filename);
    }

    clear();

    std::string line;
    while ( false == file.eof() )
    {
        std::getline(file, line);
        if ( true == file.bad() )
        {
            throw DataFileException(DataFileException::EXP_IO_ERROR);
        }

        int addr;
        std::string hexField;

        if ( -1 != ( addr = extractHexField(line, &hexField, "INIT_") ) )
        {
            int iterLimit = ( hexField.size() - 4 );
            char byteStr[3];

            addr *= ( 16 * ( ( SIZE_16b == m_opCodeSize ) ? 2 : 3 ) );
            byteStr[2] = '\0';

            for ( int i = 0; i <= iterLimit; )
            {
                if ( SIZE_18b == m_opCodeSize )
                {
                    addr++;
                }

                if ( (unsigned int) addr >= m_arrsize )
                {
                    throw DataFileException(DataFileException::EXP_MEMORY_OVERFLOW);
                }

                for ( int j = 0; j < 2; j++ )
                {
                    unsigned int byteInt;

                    byteStr[0] = hexField [ i++ ];
                    byteStr[1] = hexField [ i++ ];

                    sscanf(byteStr, "%x", &byteInt);

                    m_memory[addr++] = (uint16_t) byteInt;
                }
            }
        }
        else if ( ( SIZE_18b == m_opCodeSize ) && ( -1 != ( addr = extractHexField(line, &hexField, "INITP_") ) ) )
        {
            int iterLimit = ( hexField.size() - 4 );
            char byteStr[3];

            addr *= ( 16 * ( ( SIZE_16b == m_opCodeSize ) ? 2 : 3 ) );
            byteStr[2] = '\0';

            for ( int i = 0; i <= iterLimit; )
            {
                if ( (unsigned int) addr >= m_arrsize )
                {
                    throw DataFileException(DataFileException::EXP_MEMORY_OVERFLOW);
                }

                unsigned int byteInt;

                byteStr[0] = hexField [ i++ ];
                byteStr[1] = hexField [ i++ ];

                sscanf(byteStr, "%x", &byteInt);

                for ( int shift = 6; shift >= 0; shift -= 2)
                {
                    m_memory[addr] = uint16_t( 0x3 & ( byteInt >> shift ) );

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
                         bool makeBackup ) throw ( DataFileException )
{
    save(std::string(filename), makeBackup);
}

void XilHDLFile::save ( const std::string & filename,
                         bool makeBackup ) throw ( DataFileException )
{
    // Create backup file
    if ( true == makeBackup )
    {
        rename(filename.c_str(), (filename + "~").c_str());
    }

    std::ifstream tmplFile(m_templateFile);
    std::ofstream outFile(filename);

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

        substDataMark(line, MARK_INIT_S, MARK_INIT_E);
        if ( SIZE_18b == m_opCodeSize )
        {
            substDataMark(line, MARK_INITP_S, MARK_INITP_E, true);
        }

        outFile << line << EOL_SEQUENCE;
    }
}

void XilHDLFile::substDataMark ( std::string & line,
                                 const char * markStart,
                                 const char * markEnd,
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

    std::string dataField;
    generateDataField(&dataField, addr, parity);
    line.replace(markStartPos, addrLen, dataField);
}

void XilHDLFile::generateDataField ( std::string * dataField,
                                     unsigned int addr,
                                     bool parity ) const
{
    dataField->clear();

    if ( SIZE_18b == m_opCodeSize )
    {
        addr *= 3; // 3 == space occupied by one instruction the mem. array.
    }
    else
    {
        addr *= 2; // 2 == space occupied by one instruction the mem. array.
    }
    addr *= 16; // 16 == instructions per data field.

    if ( false == parity )
    {
        int byteInt;
        char byteHex[3];

        for ( int i = 0; i < 16; i++ )
        {
            if ( SIZE_18b == m_opCodeSize )
            {
                addr++;
            }

            for ( int j = 0; j < 2; j++ )
            {
                if ( addr < m_arrsize )
                {
                    byteInt = m_memory[addr++];
                    if ( -1 == byteInt )
                    {
                        byteInt = 0;
                    }
                }
                else
                {
                    byteInt = 0;
                }

                sprintf(byteHex, "%02X", byteInt);
                dataField->append(byteHex);
            }
        }
    }
    else
    {
        char hexString[9];
        for ( int i = 0; i < ( 128 / 16 ); i++ )
        {
            int byteInt;
            uint32_t parityBits = 0;
            for ( int j = 0; j < 16; j++ )
            {
                if ( m_arrsize > addr )
                {
                    byteInt = m_memory[addr];
                    if ( -1 == byteInt )
                    {
                        byteInt = 0;
                    }
                }
                else
                {
                    byteInt = 0;
                }

                addr += 3;

                parityBits <<= 2;
                parityBits |= ( byteInt & 0x3 );
            }

            sprintf(hexString, "%08X", parityBits);
            dataField->append(hexString);
        }
    }
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
