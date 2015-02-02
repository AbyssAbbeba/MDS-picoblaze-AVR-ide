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
 * @ingroup AdaptableSim
 * @file AdaptableSimProgramMemory.cxx
 */
// =============================================================================

#include "AdaptableSimProgramMemory.h"

#include "MCUDataFiles/DataFile.h"
#include <cstdlib>

AdaptableSimProgramMemory::AdaptableSimProgramMemory()
{
    m_memory = nullptr;
    m_size = 0;
}

AdaptableSimProgramMemory * AdaptableSimProgramMemory::link ( MCUSimEventLogger * eventLogger )
{
    MCUSimMemory::link(eventLogger, SP_CODE);
    return this;
}

AdaptableSimProgramMemory::~AdaptableSimProgramMemory()
{
    if ( nullptr != m_memory )
    {
        delete[] m_memory;
    }
}

void AdaptableSimProgramMemory::loadDataFile ( const DataFile * file )
{
    unsigned int fileSize = file->maxSize();

    for ( unsigned int fileAddr = 0, memAddr = 0;
          ( fileAddr < fileSize ) && ( memAddr < m_size );
          memAddr++ )
    {
        unsigned int word = 0;
        int byte;
        bool undefined = false;

        int init = 0;
        switch ( m_config.m_wordSize )
        {
            case WORD_1B: init = 0; break;
            case WORD_2B: init = 8; break;
            case WORD_3B: init = 16; break;
        }

        for ( int shift = init; shift >= 0; shift -= 8 )
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
            word |= ( byte << ( ( END_BIG == m_config.m_endian ) ? shift : ( init - shift ) ) );
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

void AdaptableSimProgramMemory::storeInDataFile ( DataFile * file ) const
{
    unsigned int fileSize = file->maxSize();
    file->clear();

    unsigned int mask[3];
    if ( END_BIG == m_config.m_endian )
    {
        mask[2] = 0xff0000;
        mask[1] = 0x00ff00;
        mask[0] = 0x0000ff;
    }
    else
    {
        mask[2] = 0x0000ff;
        mask[1] = 0x00ff00;
        mask[0] = 0xff0000;
    }

    for ( unsigned int fileAddr = 0, memAddr = 0;
          ( fileAddr < fileSize ) && ( memAddr < m_size );
          fileAddr++, memAddr++ )
    {
        unsigned int byte = m_memory [ memAddr ];

        if ( MFLAG_UNDEFINED & byte )
        {
            if ( WORD_3B == m_config.m_wordSize )
            {
                file -> unset ( fileAddr++ );
            }
            if ( ( WORD_3B == m_config.m_wordSize ) || ( WORD_2B == m_config.m_wordSize ) )
            {
                file -> unset ( fileAddr++ );
            }
            file -> unset ( fileAddr );
        }
        else
        {
            if ( WORD_3B == m_config.m_wordSize )
            {
                file -> set ( fileAddr++, ( byte & mask[2] ) >> 16 );
            }
            if ( ( WORD_3B == m_config.m_wordSize ) || ( WORD_2B == m_config.m_wordSize ) )
            {
                file -> set ( fileAddr++, ( byte & mask[1] ) >>  8 );
            }
            file -> set ( fileAddr, ( byte & mask[0] ) >>  0 );
        }
    }
}

MCUSim::RetCode AdaptableSimProgramMemory::directRead ( unsigned int addr,
                                                    unsigned int & data ) const
{
    if ( addr >= m_size )
    {
        return MCUSim::RC_ADDR_OUT_OF_RANGE;
    }

    data = ( m_memory[addr] & 0x3ffff );
    return MCUSim::RC_OK;
}

MCUSim::RetCode AdaptableSimProgramMemory::directWrite ( unsigned int addr,
                                                      unsigned int data )
{
    if ( addr >= m_size )
    {
        return MCUSim::RC_ADDR_OUT_OF_RANGE;
    }

    logEvent(EVENT_MEM_INF_WR_VAL_WRITTEN, addr);

    m_memory[addr] = ( data & 0x3ffff );
    return MCUSim::RC_OK;
}

void AdaptableSimProgramMemory::resize ( unsigned int newSize )
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

    if ( nullptr != memoryOrig )
    {
        delete[] memoryOrig;
    }
    m_size = newSize;
}

void AdaptableSimProgramMemory::reset ( MCUSimBase::ResetMode mode )
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

inline void AdaptableSimProgramMemory::loadConfig()
{
    resize(m_config.m_size);
}

inline void AdaptableSimProgramMemory::mcuReset()
{
}

inline void AdaptableSimProgramMemory::resetToInitialValues()
{
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        m_memory[i] = ( 0x3ffff | MFLAG_UNDEFINED );
    }
}

unsigned int AdaptableSimProgramMemory::getUndefVal() const
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
