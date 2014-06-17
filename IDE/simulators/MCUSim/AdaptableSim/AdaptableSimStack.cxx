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
 * @file AdaptableSimStack.cxx
 */
// =============================================================================

#include "AdaptableSimStack.h"
#include "MCUDataFiles/DataFile.h"

AdaptableSimStack::AdaptableSimStack()
{
    m_data = nullptr;
    m_position = 0;
}

AdaptableSimStack::~AdaptableSimStack()
{
    if ( nullptr != m_data )
    {
        delete[] m_data;
    }
}

AdaptableSimStack * AdaptableSimStack::link ( MCUSimEventLogger * eventLogger )
{
    MCUSimMemory::link(eventLogger, SP_STACK);
    return this;
}

void AdaptableSimStack::reset ( MCUSimBase::ResetMode mode )
{
    switch ( mode )
    {
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

inline void AdaptableSimStack::loadConfig()
{
    resize ( m_config.m_size );
}

inline void AdaptableSimStack::resetToInitialValues()
{
}

inline void AdaptableSimStack::mcuReset()
{
    m_position = 0;
    for ( unsigned int i = 0; i < m_config.m_size; i++ )
    {
        m_data[i] = MFLAG_UNDEFINED;
    }
}

MCUSim::RetCode AdaptableSimStack::directRead ( unsigned int addr,
                                             unsigned int & data ) const
{
    if ( addr >= m_config.m_size )
    {
        return MCUSim::RC_ADDR_OUT_OF_RANGE;
    }

    data = ( 0x3ff & m_data[addr] );
    return MCUSim::RC_OK;
}

MCUSim::RetCode AdaptableSimStack::directWrite ( unsigned int addr,
                                              unsigned int data )
{
    if ( addr >= m_config.m_size )
    {
        return MCUSim::RC_ADDR_OUT_OF_RANGE;
    }

    m_data[addr] &= ~0x3ff;
    m_data[addr] |= ( data & 0x3ff );
    return MCUSim::RC_OK;
}

void AdaptableSimStack::resize ( unsigned int newSize )
{
    if ( nullptr != m_data )
    {
        delete[] m_data;
    }
    m_data = new unsigned int [ newSize ];
}

void AdaptableSimStack::loadDataFile ( const DataFile * file )
{
    unsigned int fileSize = file->maxSize();

    for ( unsigned int fileAddr = 0, memAddr = 0;
          ( fileAddr < fileSize ) && ( memAddr < m_config.m_size );
          fileAddr++, memAddr++ )
    {
        unsigned int word = 0;
        int byte;
        bool undefined = false;

        for ( int shift = 8; shift >= 0; shift -= 8 )
        {
            byte = ( *file ) [ fileAddr ];
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
            m_data [ memAddr ] = ( MFLAG_UNDEFINED | word );
        }
        else
        {
            m_data [ memAddr ] = word;
        }
    }

    for ( unsigned int i = ( fileSize / 2 ); i < m_config.m_size; i++ )
    {
        m_data[i] =  MFLAG_UNDEFINED;
    }
}

void AdaptableSimStack::storeInDataFile ( DataFile * file ) const
{
    unsigned int fileSize = file->maxSize();
    file->clear();

    for ( unsigned int fileAddr = 0, memAddr = 0;
          ( fileAddr < fileSize ) && ( memAddr < m_config.m_size );
          fileAddr++, memAddr++ )
    {
        unsigned int byte = m_data [ memAddr ];

        if ( MFLAG_UNDEFINED & byte )
        {
            file -> unset (   fileAddr );
            file -> unset ( ++fileAddr );
        }
        else
        {
            file -> set (   fileAddr, ( byte & 0xff00 ) >>  8 );
            file -> set ( ++fileAddr, ( byte & 0x00ff ) >>  0 );
        }
    }
}
