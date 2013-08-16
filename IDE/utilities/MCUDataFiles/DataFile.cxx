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
 * @file DataFile.cxx
 */
// =============================================================================

#include "DataFile.h"

DataFile::DataFile ( unsigned int arrsize ) : m_arrsize ( arrsize )
{
    allocateMemory();
}

DataFile::~DataFile()
{
    // Deallocate the memory array
    if ( NULL != m_memory )
    {
        delete[] m_memory;
    }
}

inline void DataFile::allocateMemory()
{
    // Allocate the memory array
    if ( 0 != m_arrsize )
    {
        m_memory = new int16_t[m_arrsize];
    }
    else
    {
        m_memory = NULL;
    }

    for ( unsigned int i = 0; i < m_arrsize; i++ )
    {
        m_memory[i] = -1;
    }
}

int DataFile::at ( int idx ) const throw ( DataFileException )
{
    if ( idx >= int(m_arrsize) || idx < 0 )
    {
        throw DataFileException(DataFileException::EXP_OUT_OF_RANGE);
    }

    return int(m_memory[idx]);
}

void DataFile::set ( unsigned int address,
                     uint8_t value ) throw ( DataFileException )
{
    // Check for valid address
    if ( address >= m_arrsize )
    {
        throw DataFileException(DataFileException::EXP_OUT_OF_RANGE);
    }

    m_memory[address] = value;
}

void DataFile::unset ( unsigned int address ) throw ( DataFileException )
{
    // Check for valid address
    if ( address >= m_arrsize )
    {
        throw DataFileException(DataFileException::EXP_OUT_OF_RANGE);
    }

    m_memory[address] = -1;
}

void DataFile::clear()
{
    for ( unsigned int i = 0; i < m_arrsize; i++ )
    {
        m_memory[i]=-1;
    }
}

unsigned int DataFile::getNumberOfNotEmptyPages ( unsigned int pageSize ) const
{
    // Local variables
    unsigned int result = 0; // Final result
    unsigned int address; // Address of the current block
    unsigned int numOfBlocks = m_arrsize / pageSize; // Number of block of that arrsize

    // (It's possible to have one block larger than the whole memory array)
    if ( m_arrsize % pageSize )
    {
        numOfBlocks++;
    }

    // Iterate over the memory array
    for ( unsigned int i = 0; i < numOfBlocks; i++ )
    {
        address = i * pageSize;

        // Search for cell with value different from -1
        for ( unsigned int j = 0; j < pageSize; j++ )
        {
            if ( m_memory[address] != -1 )
            {
                result++;
                break;
            }

            address++;
        }
    }

    return result;
}

bool DataFile::isPageEmpty ( unsigned int pageSize,
                             unsigned int pageNumber ) const
{
    for ( unsigned int i = pageNumber * pageSize;
          i < ( pageNumber + 1 ) * pageSize;
          i++ )
    {
        if ( m_memory[i] > 0 )
        {
            return 0;
        }
    }

    return 1;
}

int16_t * DataFile::getData()
{
    return m_memory;
}

void DataFile::setData ( int16_t * data )
{
    if ( NULL != m_memory)
    {
        delete[] m_memory;
    }

    m_memory = data;
}

bool DataFile::operator == ( const DataFile & obj ) const
{
    // Assume that both objects are equivalent, and try to prove otherwise.
    unsigned int i = 0;
    for ( ; i < maxSize(); i++ )
    {
        if ( get(i) != obj.get(i) )
        {
            return false;
        }
    }
    for ( ; i < obj.maxSize(); i++ )
    {
        if ( get(i) != obj.get(i) )
        {
            return false;
        }
    }
    return true;
}
