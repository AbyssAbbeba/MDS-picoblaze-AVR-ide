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
 * @ingroup PicoBlaze
 * @file PicoBlazeProgramMemory.cxx
 */
// =============================================================================

#include "PicoBlazeProgramMemory.h"

#include "MCUDataFiles/DataFile.h"
#include <cstdlib>

PicoBlazeProgramMemory::PicoBlazeProgramMemory()
{
    m_memory = NULL;
    m_size = 0;
}

PicoBlazeProgramMemory * PicoBlazeProgramMemory::link ( MCUSimEventLogger * eventLogger )
{
    MCUSimMemory::link(eventLogger, SP_CODE);
    return this;
}

PicoBlazeProgramMemory::~PicoBlazeProgramMemory()
{
    if ( NULL != m_memory )
    {
        delete[] m_memory;
    }
}

void PicoBlazeProgramMemory::loadDataFile ( const DataFile * file )
{
    unsigned int fileSize = file->maxSize();

    for ( unsigned int fileAddr = 0, memAddr = 0;
          ( fileAddr < fileSize ) && ( memAddr < m_size );
          memAddr++ )
    {
        unsigned int word = 0;
        int byte;
        bool undefined = false;

        for ( int shift = ( ( WS_18b == m_config.m_wordSize ) ? 16 : 8 ); shift >= 0; shift -= 8 )
        {
            if ( fileAddr < fileSize )
            {
                byte = ( *file ) [ fileAddr ];
            }
            else
            {
                byte = -1;
            }

            if ( -1 == byte )
            {
                undefined = true;
                byte = 0;
            }
            word |= byte << shift;
            fileAddr++;
        }

        if ( true == undefined )
        {
            m_memory [ memAddr ] = ( MFLAG_UNDEFINED | word );
        }
        else
        {
            m_memory [ memAddr ] = word;
        }
    }

    for ( unsigned int i = ( 1 + ( fileSize / 3 ) ); i < m_size; i++ )
    {
        m_memory[i] =  MFLAG_UNDEFINED;
    }
}

void PicoBlazeProgramMemory::storeInDataFile ( DataFile * file ) const
{
    unsigned int fileSize = file->maxSize();
    file->clear();

    for ( unsigned int fileAddr = 0, memAddr = 0;
          ( fileAddr < fileSize ) && ( memAddr < m_size );
          fileAddr++, memAddr++ )
    {
        unsigned int byte = m_memory [ memAddr ];

        if ( MFLAG_UNDEFINED & byte )
        {
            if ( WS_18b == m_config.m_wordSize )
            {
                file -> unset ( fileAddr++ );
            }
            file -> unset ( fileAddr++ );
            file -> unset ( fileAddr );
        }
        else
        {
            if ( WS_18b == m_config.m_wordSize )
            {
                file -> set ( fileAddr++, ( byte & 0xff0000 ) >> 16 );
            }
            file -> set ( fileAddr++, ( byte & 0x00ff00 ) >>  8 );
            file -> set ( fileAddr, ( byte & 0x0000ff ) >>  0 );
        }
    }
}

MCUSim::RetCode PicoBlazeProgramMemory::directRead ( unsigned int addr,
                                                    unsigned int & data ) const
{
    if ( addr >= m_size )
    {
        return MCUSim::RC_ADDR_OUT_OF_RANGE;
    }

    data = ( m_memory[addr] & 0x3ffff );
    return MCUSim::RC_OK;
}

MCUSim::RetCode PicoBlazeProgramMemory::directWrite ( unsigned int addr,
                                                      unsigned int data )
{
    if ( addr >= m_size )
    {
        return MCUSim::RC_ADDR_OUT_OF_RANGE;
    }

    m_memory[addr] = ( data & 0x3ffff );
    return MCUSim::RC_OK;
}

void PicoBlazeProgramMemory::resize ( unsigned int newSize )
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

void PicoBlazeProgramMemory::reset ( MCUSimBase::ResetMode mode )
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

inline void PicoBlazeProgramMemory::loadConfig()
{
    resize(m_config.m_size);
}

inline void PicoBlazeProgramMemory::mcuReset()
{
}

inline void PicoBlazeProgramMemory::resetToInitialValues()
{
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        m_memory[i] = ( 0x3ffff | MFLAG_UNDEFINED );
    }
}

unsigned int PicoBlazeProgramMemory::getUndefVal() const
{
    if ( -1 == m_config.m_undefinedValue )
    {
        // Generate random value
        return ( (unsigned int)rand() & 0x3ffff );
    }
    else
    {
        // Return predefined value
        return ( m_config.m_undefinedValue & 0x3ffff );
    }
}
