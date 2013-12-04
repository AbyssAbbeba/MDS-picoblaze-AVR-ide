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
 * @ingroup PIC8
 * @file PIC8ProgramMemory.cxx
 */
// =============================================================================

#include "PIC8ProgramMemory.h"

#include "MCUDataFiles/DataFile.h"
#include <cstdlib>

PIC8ProgramMemory::PIC8ProgramMemory()
{
    m_memory = NULL;
    m_idLocations = NULL;
    m_size = 0;
}

PIC8ProgramMemory * PIC8ProgramMemory::link ( MCUSimEventLogger * eventLogger,
                                              PIC8ConfigWord * configWord )
{
    MCUSimMemory::link(eventLogger, SP_CODE);
    m_configWord = configWord;
    return this;
}

PIC8ProgramMemory::~PIC8ProgramMemory()
{
    if ( NULL != m_memory )
    {
        delete[] m_memory;
    }
    if ( NULL != m_idLocations )
    {
        delete[] m_idLocations;
    }
}

void PIC8ProgramMemory::loadDataFile ( const DataFile * file )
{
    unsigned int size = file->maxSize();

    for ( unsigned int i = 0, j = 0;
          i < size;
          i++, j++ )
    {
        if ( j >= m_size )
        {
            break;
        }

        int byte = (*file)[i];

        if ( -1 == byte )
        {
            byte = ( 0x3fff | MFLAG_UNDEFINED );
        }
        else
        {
            byte |= ( (*file)[++i] << 8 );
        }

        if ( false == handleSpecialAddressWR(j, byte) )
        {
            m_memory[j] = byte;
        }
    }

    for ( unsigned int i = ( size / 2 ); i < m_size; i++ )
    {
        m_memory[i] = ( 0x3fff | MFLAG_UNDEFINED );
    }
}

void PIC8ProgramMemory::storeInDataFile ( DataFile * file ) const
{
    unsigned int size = file->maxSize();

    file->clear();

    for ( unsigned int i = 0, j = 0;
          i < size;
          i++, j++ )
    {
        if ( j >= m_size )
        {
            break;
        }

        unsigned int byte;
        if ( false == handleSpecialAddressRD(j, &byte) )
        {
            byte = m_memory[j];
        }

        if ( MFLAG_UNDEFINED & byte )
        {
            file->unset(i);
            file->unset(++i);
        }
        else
        {
            file->set(i, byte & 0xff);
            file->set(++i, (byte & 0xff00) >> 8);
        }
    }
}

MCUSim::RetCode PIC8ProgramMemory::directRead ( unsigned int addr,
                                                unsigned int & data ) const
{
    if ( true == handleSpecialAddressRD(addr, &data) )
    {
        return MCUSim::RC_OK;
    }

    data = m_memory[addr];

    if ( addr >= m_size )
    {
        return MCUSim::RC_ADDR_OUT_OF_RANGE;
    }

    if ( MFLAG_UNDEFINED & data )
    {
        data &= 0x3fff;
        return MCUSim::RC_NONDETERMINISTIC;
    }
    else
    {
        data &= 0x3fff;
        return MCUSim::RC_OK;
    }
}

MCUSim::RetCode PIC8ProgramMemory::directWrite ( unsigned int addr,
                                                 unsigned int data )
{

    if ( true == handleSpecialAddressWR(addr, data) )
    {
        return MCUSim::RC_OK;
    }

    if ( addr >= m_size )
    {
        return MCUSim::RC_ADDR_OUT_OF_RANGE;
    }

    m_memory[addr] = (data & 0x3fff);
    return MCUSim::RC_OK;
}

void PIC8ProgramMemory::resize ( unsigned int newSize )
{
    unsigned int * memoryOrig = m_memory;
    m_memory = new unsigned int[newSize];

    const unsigned int sizeToCopy = ( m_size <= newSize ) ? m_size : newSize;
    for ( unsigned int i = 0; i < sizeToCopy; i++ )
    {
        m_memory[i] = memoryOrig[i];
    }

    for ( unsigned int i = sizeToCopy; i < newSize; i++ )
    {
        m_memory[i] = ( getUndefVal() | MFLAG_UNDEFINED );
    }

    if ( NULL != memoryOrig )
    {
        delete[] memoryOrig;
    }
    m_size = newSize;
}

void PIC8ProgramMemory::reset ( MCUSimBase::ResetMode mode )
{
    switch ( mode ) {
        case MCUSim::RSTMD_NEW_CONFIG:
            loadConfig();
            break;
        case MCUSim::RSTMD_INITIAL_VALUES:
            resetToInitialValues();
            break;
        case MCUSim::RSTMD_MCU_RESET:
            mcuReset();
            break;
        default:
            // Irrelevant requests are silently ignored
            break;
    }
}

inline void PIC8ProgramMemory::loadConfig()
{
    resize(m_config.m_size);

    if ( NULL != m_idLocations )
    {
        delete[] m_idLocations;
    }

    int n = 1 + m_config.m_idLocationsRange[1] - m_config.m_idLocationsRange[0];
    if ( n > 0 )
    {
        m_idLocations = new unsigned int [ n ];
    }
}

inline void PIC8ProgramMemory::mcuReset()
{
}

inline void PIC8ProgramMemory::resetToInitialValues()
{
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        m_memory[i] = ( 0x3fff | MFLAG_UNDEFINED );
    }

    int n = 1 + m_config.m_idLocationsRange[1] - m_config.m_idLocationsRange[0];
    for ( int i = 0; i < n; i++ )
    {
        m_idLocations[i] = ( 0x3ff | MFLAG_UNDEFINED );
    }
}

unsigned int PIC8ProgramMemory::getUndefVal() const
{
    if ( -1 == m_config.m_undefinedValue )
    {
        // Generate random value
        return ( (unsigned int)rand() & 0x3fff );
    }
    else
    {
        // Return predefined value
        return ( m_config.m_undefinedValue & 0x3fff );
    }
}
