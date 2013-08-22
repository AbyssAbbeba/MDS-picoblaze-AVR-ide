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
 * @file SrecFile.cxx
 */
// =============================================================================

#include "SrecFile.h"
#include <cctype>
#include <fstream>
#include <cmath>

/**
 * @brief Define level of input data checking
 *
 * Possible values are:
 *
 * 0 = LOW LEVEL, checks for:
 *	- Allowed characters (only hexadecimal digits are allowed after leading 'S')
 *	- Valid record length (since it cannot be of arbitrary size)
 *
 * 1 = MODERATE LEVEL, checks for things from the low level plus:
 *	- Checksum
 *	- Valid record type (0, 1, 2, 3,   5,   7, 8 or 9)
 *	- Valid record count
 *
 * 2 = HIGH LEVEL, checks for things from all lower levels plus:
 *	- Good formatting, which means that address fields must be filled
 *	  with zeros if not used, data field must not be present there where
 *	  it is not needed and data field length is limited up to 64 bytes
 *
 * 3 = "PEDANTIC" LEVEL, checks for things from all lower levels plus:
 *	- Block header must be formated precisely acording to the manual.
 */
#define STRICTNESS_LEVEL 2

SrecFile::SrecFile ( unsigned int arrsize ) : DataFile(arrsize)
{
    m_startingExecutionAddress = 0;
}

void SrecFile::clearAndLoad ( const char * filename ) throw ( DataFileException )
{
    clearAndLoad(std::string(filename));
}

void SrecFile::clearAndLoad ( const std::string & filename ) throw ( DataFileException )
{
    int localRecordCount = 0;
    bool terminated = false;

    std::fstream file ( filename, std::fstream::in | std::fstream::binary );

    if ( false == file.is_open() )
    {
        throw DataFileException(DataFileException::EXP_IO_ERROR, "Unable to open file: " + filename);
    }

    m_startingExecutionAddress = 0;
    m_blockHeader.m_mname.clear();
    m_blockHeader.m_ver.clear();
    m_blockHeader.m_rev.clear();
    m_blockHeader.m_description.clear();
    clear();

    while ( ( false == file.eof() ) && ( false == terminated ) )
    {
        int type;
        int size;
        int count;
        int address;
        int checksum;
        int dataLength;
        int addressLength;
        std::string line;
        std::vector<unsigned char> data;

        getline(file, line);
        if ( true == file.bad() )
        {
            throw DataFileException(DataFileException::EXP_IO_ERROR);
        }

        size = line.size();

        // Skip empty lines
        if ( 0 == size )
        {
            continue;
        }

        // Skip lines which does not start with 'S'
        if ( 'S' != line[0] )
        {
            continue;
        }

        // Check for allowed characters
        for ( int i = 1; i < size; i++ )
        {
            if ( 0 == isxdigit(line[i]) )
            {
                throw DataFileException(DataFileException::EXP_BAD_RECORD_FORMAT);
            }
        }

        type = hex2int ( line.substr(1,1).c_str() );
        count = hex2int ( line.substr(2,2).c_str() );

        if ( ( count * 2 + 4 ) != size )
        {
            throw DataFileException(DataFileException::EXP_BAD_RECORD_LENGTH);
        }

        switch ( type )
        {
            case 0:
            case 1:
            case 5:
            case 9:
                addressLength = 2;
                break;
            case 2:
            case 8:
                addressLength=3;
                break;
            case 3:
            case 7:
                addressLength=4;
                break;
            default:
                if ( STRICTNESS_LEVEL > 0 )
                {
                    throw DataFileException(DataFileException::EXP_BAD_RECORD_FORMAT);
                }
                else
                {
                    continue;
                }
        }

        dataLength = (count - addressLength - 1);
        address = hex2int(line.substr(4, addressLength * 2));
        data = readHex(line.substr(4 + addressLength * 2, dataLength * 2));
        checksum = hex2int(line.substr(4 + addressLength * 2 + dataLength * 2, 2));

        #if STRICTNESS_LEVEL > 0
            if ( size != ( 6 + addressLength * 2 + dataLength * 2 ) )
            {
                throw DataFileException(DataFileException::EXP_BAD_RECORD_FORMAT);
            }
            if ( computeCRC(line.substr(0, size-2)) != checksum )
            {
                throw DataFileException(DataFileException::EXP_BAD_CRC);
            }
        #endif // STRICTNESS_LEVEL > 0

        #if STRICTNESS_LEVEL > 1
            if ( data.size() > 64 )
            {
                throw DataFileException(DataFileException::EXP_BAD_RECORD_FORMAT);
            }
        #endif // STRICTNESS_LEVEL > 1

        switch ( type )
        {
            case 0:	// Block header
                #if STRICTNESS_LEVEL > 2
                    if ( ( data.size() < 24 ) || ( data.size() > 60 ) )
                    {
                        throw DataFileException(DataFileException::EXP_BAD_RECORD_FORMAT);
                    }
                #endif // STRICTNESS_LEVEL > 0

                #if STRICTNESS_LEVEL > 1
                    if ( 0 != address )
                    {
                        throw DataFileException(DataFileException::EXP_BAD_RECORD_FORMAT);
                    }
                #endif // STRICTNESS_LEVEL > 1

                m_blockHeader.m_mname = vec2str(data).substr(0, 20);
                m_blockHeader.m_ver = vec2str(data).substr(20, 2);
                m_blockHeader.m_rev = vec2str(data).substr(22, 2);
                m_blockHeader.m_description = vec2str(data).substr(24, 36);
                break;

            case 1:	// Data sequence
            case 2:	// Data sequence
            case 3:	// Data sequence
                for ( int i = 0; i < dataLength; i++ )
                {
                    m_memory[address++] = data[i];
                }

                localRecordCount++;
                break;

            case 5:	// Record count
                #if STRICTNESS_LEVEL > 0
                    if ( address != localRecordCount )
                    {
                        throw DataFileException(DataFileException::EXP_BAD_RECORD_COUNT);
                    }
                #endif // STRICTNESS_LEVEL > 0

                #if STRICTNESS_LEVEL > 1
                    if ( 0 != data.size() )
                    {
                        throw DataFileException(DataFileException::EXP_BAD_RECORD_FORMAT);
                    }
                #endif // STRICTNESS_LEVEL > 1

                localRecordCount = 0;
                break;

            case 7:	//End of block
            case 8:	//End of block
            case 9:	//End of block
                #if STRICTNESS_LEVEL > 1
                    if ( 0 != data.size() )
                    {
                        throw DataFileException(DataFileException::EXP_BAD_RECORD_FORMAT);
                    }
                #endif // STRICTNESS_LEVEL > 1

                m_startingExecutionAddress = address;
                terminated = true;
                break;
        }
    }

    file.close();
}

void SrecFile::save ( const char * filename,
                      bool makeBackup ) throw ( DataFileException )
{
    save(std::string(filename), makeBackup);
}

void SrecFile::save ( const std::string & filename,
                      bool makeBackup ) throw ( DataFileException )
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

    // Write block header record
    if ( ( false == m_blockHeader.m_mname.empty() )
            ||
         ( false == m_blockHeader.m_ver.empty() )
            ||
         ( false == m_blockHeader.m_rev.empty() )
            ||
         ( false == m_blockHeader.m_description.empty() ) )
    {
        writeBlockHeaderSRecord ( file,
                                  m_blockHeader.m_mname,
                                  m_blockHeader.m_ver,
                                  m_blockHeader.m_rev,
                                  m_blockHeader.m_description );
    }

    // Write data records
    int dataLength = 0;
    int address = 0;
    int recordCount = 0;
    char tmp[3];
    std::string dataField;

    for ( unsigned int i = 0; i < m_arrsize; i++ )
    {
        if ( ( 32 == dataLength )
                ||
             ( ( 0 != dataLength ) && ( -1 == m_memory[i] ) ) )
        {
            writeDataSRecord(file, address, dataField);
            recordCount++;

            dataField.clear();
            address = i;
            dataLength = 0;
        }

        if ( 0xff == recordCount )
        {
            writeRecordCountSRecord(file, recordCount);
            recordCount = 0;
        }

        if ( -1 != m_memory[i] )
        {
            sprintf(tmp, "%02X", int(m_memory[i]));
            dataField.append(tmp);
            dataLength++;
        }
    }

    if ( true == writeDataSRecord(file, address, dataField) )
    {
        recordCount++;
    }

    writeRecordCountSRecord(file, recordCount);

    // Write End of block and close the file
    writeEndOfBlockSRecord(file, m_startingExecutionAddress);
    file.close();
}

inline void SrecFile::writeBlockHeaderSRecord ( std::fstream & file,
                                                const std::string & mname,
                                                const std::string & ver,
                                                const std::string & rev,
                                                const std::string & description ) const
{
    int len;

    file.write("S0", 2);

    len = mname.size() / 2;
    file.write(mname.data(), len);
    for ( int i = len; i < 20; i++ )
    {
        file.write("00", 2);
    }

    len = ver.size() / 2;
    file.write(ver.data(), len);
    for ( int i = len; i < 2; i++ )
    {
        file.write("00", 2);
    }

    len = rev.size() / 2;
    file.write(rev.data(), len);
    for ( int i = len; i < 2; i++ )
    {
        file.write("00", 2);
    }

    len = description.size() / 2;
    file.write(description.data(), len);
}

inline void SrecFile::writeRecordCountSRecord ( std::fstream & file,
                                                int recordCount ) const
{
    std::string record;
    char tmp[5];

    record.append("S503");
    sprintf(tmp, "%04X", recordCount);
    record.append(tmp);
    file.write(record.data(), record.size());

    sprintf(tmp, "%02X", computeCRC(record));
    file.write(tmp, 2);
    file.put('\n');
}

inline bool SrecFile::writeDataSRecord ( std::fstream & file,
                                         unsigned int address,
                                         const std::string & dataField ) const
{
    int addrLength;
    int dataLength = ( dataField.size() / 2 );
    char tmp[9];
    std::string record;

    if ( 0 == dataLength )
    {
        return false;
    }

    // Append type and determinate address length
    if ( address < 0xffff )
    {
        addrLength = 2;
        record.append("S1");
    }
    else if ( address < 0xffffff )
    {
        addrLength = 3;
        record.append("S2");
    }
    else
    {
        addrLength = 4;
        record.append("S3");
    }

    // Append byte count
    sprintf(tmp, "%02X", (addrLength + 1 + dataLength));
    record.append(tmp);

    // Append address
    switch ( addrLength )
    {
        case 2:
            sprintf(tmp, "%04X", address);
            break;
        case 3:
            sprintf(tmp, "%06X", address);
            break;
        case 4:
            sprintf(tmp, "%08X", address);
            break;
    }
    record.append(tmp);

    // Append data
    record.append(dataField);

    // Write record and its checksum
    file.write(record.data(), record.size());
    sprintf(tmp, "%02X", computeCRC(record));
    file.write(tmp, 2);
    file.put('\n');

    return true;
}

inline void SrecFile::writeEndOfBlockSRecord ( std::fstream & file,
                                               unsigned int sea ) const
{
    char tmp[9];
    std::string record;

    if ( sea < 0xffff )
    {
        record.append("S903");
        sprintf(tmp, "%04X", sea);
    }
    else if ( sea < 0xffffff )
    {
        record.append("S804");
        sprintf(tmp, "%06X", sea);
    }
    else
    {
        record.append("S705");
        sprintf(tmp, "%08X", sea);
    }
    record.append(tmp);

    file.write(record.data(), record.size());

    sprintf(tmp, "%02X", computeCRC(record));
    file.write(tmp, 2);
    file.put('\n');
}

inline int SrecFile::hex2int ( const std::string & source ) const
{
    int result = 0;

    for ( int j = 0, i = (source.size() - 1);
          i >= 0;
          j++, i-- )
    {
        int tmp = 0;
        switch ( source[i] )
        {
            case '0': tmp = 0; break;
            case '1': tmp = 1; break;
            case '2': tmp = 2; break;
            case '3': tmp = 3; break;
            case '4': tmp = 4; break;
            case '5': tmp = 5; break;
            case '6': tmp = 6; break;
            case '7': tmp = 7; break;
            case '8': tmp = 8; break;
            case '9': tmp = 9; break;

            case 'a':
            case 'A':
                tmp = 10;
                break;
            case 'b':
            case 'B':
                tmp = 11;
                break;
            case 'c':
            case 'C':
                tmp = 12;
                break;
            case 'd':
            case 'D':
                tmp = 13;
                break;
            case 'e':
            case 'E':
                tmp = 14;
                break;
            case 'f':
            case 'F':
                tmp = 15;
                break;
        }

        result += ( tmp * pow(16, j) );
    }

    return result;
}

inline std::string SrecFile::vec2str ( const std::vector<unsigned char> & source ) const
{
    std::string result;

    for ( std::vector<unsigned char>::const_iterator i = source.begin();
          i != source.end();
          i++ )
    {
        result.push_back(*i);
    }

    return result;
}

inline std::vector<unsigned char> SrecFile::readHex ( const std::string & source ) const
{
    std::vector<unsigned char> result;

    for ( unsigned int i = 0; i < source.size(); i += 2 )
    {
        result.push_back(hex2int(source.substr(i, 2)));
    }

    return result;
}

inline int SrecFile::computeCRC ( const std::string & data ) const
{
    int result = 0x0ff;

    for ( unsigned int i = 2; i < data.size(); i += 2 )
    {
        result -= hex2int(data.substr(i, 2));
    }

    return (result & 0x0ff);
}

const std::string & SrecFile::getModuleName() const
{
    return m_blockHeader.m_mname;
}

const std::string & SrecFile::getVersion() const
{
    return m_blockHeader.m_ver;
}

const std::string & SrecFile::getRevision() const
{
    return m_blockHeader.m_rev;
}

const std::string & SrecFile::getDescription() const
{
    return m_blockHeader.m_description;
}

unsigned int SrecFile::getStartingExecutionAddress() const
{
    return m_startingExecutionAddress;
}

bool SrecFile::setModuleName ( const std::string & mname )
{
    bool result = true;

    if ( mname.size() > 20 )
    {
        result = false;
    }

    m_blockHeader.m_mname = mname.substr(0, 20);
    return result;
}

bool SrecFile::setVersion ( const std::string & ver )
{
    bool result = true;

    if ( ver.size() > 2 )
    {
        result = false;
    }

    m_blockHeader.m_ver = ver.substr(0, 2);
    return result;
}

bool SrecFile::setRevision ( const std::string & rev )
{
    bool result = true;

    if ( rev.size() > 2 )
    {
        result = false;
    }

    m_blockHeader.m_rev = rev;
    return result;
}

bool SrecFile::setDescription ( const std::string & desc )
{
    bool result = true;

    if ( desc.size() > 36 )
    {
        result = false;
    }

    m_blockHeader.m_description = desc;
    return result;
}

bool SrecFile::setStartingExecutionAddress ( uint32_t addr )
{
    m_startingExecutionAddress = addr;
    return true;
}
