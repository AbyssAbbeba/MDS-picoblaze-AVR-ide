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
 * @file HexFile.cxx
 */
// =============================================================================


#include "HexFile.h"

#include <string>
#include <fstream>
#include <cstdint>
#include <cstdio>
#include <cstring>

inline int HexFile::computeCRC ( const char * data ) const
{
    int len = strlen(data);
    int result = 0;
    int byte;

    for ( int addr = 0; addr < len; addr += 2 )
    {
        sscanf(data + addr, "%2X", &byte);
        result -= byte;

        if ( result < 0 )
        {
            result += 0x100;
        }
    }

    return result;
}

void HexFile::clearAndLoad ( const char * filename ) throw ( DataFileException )
{
    clearAndLoad(std::string(filename));
}

void HexFile::clearAndLoad ( const std::string & filename ) throw ( DataFileException )
{
    // Local variables
    int segmentAddress = 0; // Address specified by Extended Segment Address Record (I16HEX)
    int hexAddress;         // Contents of field "address"
    int hexLength;          // Contents of field "length"
    int hexType;            // Contents of field "type"
    int hexCrc;             // Contents of field "CRC"
    int dataCell;           // One octet in data field
    char tmp[5];            // Tempotary variable for extracting data
    char hexData[510];      // Contents of field "data"
    char line[522];         // Line loaded from the file
    bool overflow = false;  // Memory overflow detected
    std::fstream file;      // Source file stream

    clear();

    // Open the file
    file.open(filename, std::fstream::in);
    if ( false == file.is_open())
    {
        throw DataFileException(DataFileException::EXP_IO_ERROR, "Unable to open file: " + filename);
    }

    // Iterate over lines in the file
    while ( false == file.eof() )
    {
        // Check if the file is still readable
        if ( true == file.bad() )
        {
            throw DataFileException(DataFileException::EXP_IO_ERROR);
        }

        // Load IHEX record from the file
        file.getline(line, 522);
        if ( ':' != line[0] )
        {
            continue;
        }

        // Remove Microsoft's sin (CR before LF)
        if ( line[strlen(line)-1] == '\r' )
        {
            line[strlen(line)-1] = '\0';
        }

        // Check for minimum record length
        if ( strlen(line) < 11 )
        {
            throw DataFileException(DataFileException::EXP_BAD_RECORD_LENGTH);
        }

        // Extract field "length"
        strncpy(tmp, line + 1, 2);
        tmp[2] = '\0';
        sscanf(tmp, "%X", &hexLength);

        // Extract field "address"
        strncpy(tmp, line + 3, 4);
        tmp[4] = '\0';
        sscanf(tmp, "%X", &hexAddress);

        // Extract field "type"
        strncpy(tmp, line + 7, 2);
        tmp[2] = '\0';
        sscanf(tmp, "%X", &hexType);
        if ( strlen(line) != ( hexLength * 2 + 11 ) )
        {
            throw DataFileException(DataFileException::EXP_BAD_RECORD_LENGTH);
        }

        // Extract field "data"
        strncpy(hexData, line + 9, hexLength * 2);

        // Extract field "CRC"
        strncpy(tmp, line + (hexLength * 2) + 9, 2);
        tmp[2] = '\0';
        sscanf(tmp, "%X", &hexCrc);

        // Check for valid CRC
        line [ strlen(line) - 2 ] = '\0';
        if ( computeCRC ( line + 1 ) != hexCrc )
        {
            throw DataFileException(DataFileException::EXP_BAD_CRC);
        }

        if ( 1 == hexType)
        {
            // EOF record
            break;
        }
        else if ( 2 == hexType )
        {
            // Extended Segment Address Record

            if ( 0 != hexAddress )
            {
                throw DataFileException(DataFileException::EXP_BAD_RECORD_FORMAT);
            }

            strncpy(tmp, hexData, 2);
            sscanf(tmp, "%X", &dataCell);
            segmentAddress = dataCell << 8;

            strncpy(tmp, hexData + 2, 2);
            sscanf(tmp, "%X", &dataCell);
            segmentAddress += dataCell;

            segmentAddress <<= 4;
            continue;
        }
        else if ( 0 != hexType )
        {
            // Invalid record type
            throw DataFileException(DataFileException::EXP_BAD_RECORD_TYPE);
        }

        // Load record data into the memory array
        tmp[2] = '\0';
        hexAddress += segmentAddress;
        for ( int i = 0; i < hexLength * 2; i += 2 )
        {
            if ( hexAddress > (int)m_arrsize )
            {
                overflow=true;
                break;
            }

            strncpy(tmp, hexData + i, 2);
            sscanf(tmp, "%X", &dataCell);
            m_memory[hexAddress] = int16_t(dataCell);
            hexAddress++;
        }
    }

    // Clean up
    file.close();
    if ( true == overflow )
    {
        throw DataFileException(DataFileException::EXP_MEMORY_OVERFLOW);
    }
}

void HexFile::save ( const char * filename,
                     bool makeBackup ) throw ( DataFileException )
{
    save(std::string(filename), makeBackup);
}

void HexFile::save ( const std::string & filename,
                     bool makeBackup ) throw ( DataFileException )
{
    // Local variables
    int segmentAddress = 0; // Segment address (for I16HEX only)
    int hexLen = 0;         // Length of IHEX record
    int hexAddr = 0;        // Address of current IHEX record
    char tmp[5];            // Tempotary variable for constructing IHEX8 record
    std::string hexData;    // Data field
    std::string line;       // Whole IHEX-8 line
    std::fstream file;      // Target file stream

    // Create backup file
    if ( true == makeBackup )
    {
        rename(filename.c_str(), (filename + "~").c_str());
    }

    file.open ( filename, (std::fstream::out | std::fstream::trunc | std::fstream::binary) );

    if ( false == file.is_open())
    {
        throw DataFileException(DataFileException::EXP_IO_ERROR);
    }

    // Allocate memory for string variables
    hexData.reserve(511);
    line.reserve(522);

    // Iterate over the memory array
    for ( unsigned int i = 0; i < m_arrsize; i++ )
    {
        // Write record if data field is full or if te current cell is undefined
        if ( ( -1 == m_memory[i] ) || ( m_maxRecLength == hexLen ) )
        {
            // HEX record must not be ampty
            if ( 0 == hexLen )
            {
                continue;
            }

            // HEX record must start with colon
            line.append(":");

            // Determinate field "length"
            sprintf(tmp, "%02X", hexLen);
            line.append(tmp);

            // Determinate field "address"
            sprintf(tmp, "%04X", hexAddr - segmentAddress);

            // Append fields length, address and data
            line.append(tmp);
            line.append("00");
            line.append(hexData.data());

            // Write this record to the file
            file.write(line.data(), line.size());

            // Compute CRC and write it to the file
            sprintf(tmp, "%02X", computeCRC(line.data()+1));
            file.write(tmp, 2);
            file.put('\n');

            // Prepare for creating next record
            hexData.clear();
            line.clear();
            hexLen=0;
        }

        // Append byte to the current IHEX-8 record
        if ( -1 != m_memory[i] )
        {
            if ( 0 == hexLen )
            {
                hexAddr = i;

                // Address is too high -> use I16HEX Extended Segment Address Record
                if ( hexAddr > 0x0FFFF )
                {
                    if ( ( hexAddr & 0xFFF00 ) > ( segmentAddress + 0x0FFFF ) )
                    {
                        segmentAddress = ( hexAddr & 0xFFF00 );

                        // Start I16HEX Extended Segment Address Record
                        line = ":02000002";

                        // Write segment address
                        sprintf(tmp, "%04X", segmentAddress >> 4);
                        line.append(tmp);
                        file.write(line.data(), 13);

                        // Write CRC
                        sprintf(tmp, "%02X", computeCRC(line.data()+1));
                        file.write(tmp, 2);
                        file.put('\n');
                    }
                }
            }

            sprintf(tmp, "%02X", m_memory[i]);
            hexData.append(tmp);
            hexLen++;
        }
    }

    // Write the last HEX record
    if ( 0 != hexLen )
    {
        // HEX record must start with colon
        line.append(":");

        // Determinate field "length"
        sprintf(tmp, "%02X", hexLen);
        line.append(tmp);

        // Determinate field "address"
        sprintf(tmp, "%04X", hexAddr - segmentAddress);

        // Append fields length, address and data
        line.append(tmp);
        line.append("00");
        line.append(hexData.data());

        // Write this record to the file
        file.write(line.data(), line.size());

        // Compute CRC and write it to the file
        sprintf(tmp, "%02X", computeCRC(line.data()+1));
        file.write(tmp, 2);
        file.put('\n');

        // Clean up
        hexData.clear();
        line.clear();
    }

    // Write EOF and cloase the file
    file.write(":00000001FF\n", 12);
    file.close();
    if ( true == file.bad() )
    {
        throw DataFileException(DataFileException::EXP_IO_ERROR);
    }
}
