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
 * @file PicoBlazeStack.cxx
 */
// =============================================================================

#include "PicoBlazeStack.h"
#include "MCUDataFiles/DataFile.h"

PicoBlazeStack::PicoBlazeStack()
{
    m_data = NULL;
    m_position = 0;
}

PicoBlazeStack::~PicoBlazeStack()
{
    if ( NULL != m_data )
    {
        delete[] m_data;
    }
}

PicoBlazeStack * PicoBlazeStack::link ( MCUSimEventLogger * eventLogger )
{
    MCUSimMemory::link(eventLogger, SP_STACK);
    return this;
}

void PicoBlazeStack::reset ( MCUSimBase::ResetMode mode )
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

inline void PicoBlazeStack::loadConfig()
{
    resize ( m_config.m_size );
}

inline void PicoBlazeStack::resetToInitialValues()
{
}

inline void PicoBlazeStack::mcuReset()
{
    m_position = 0;
    for ( unsigned int i = 0; i < m_config.m_size; i++ )
    {
        m_data[i] = MFLAG_UNDEFINED;
    }
}

MCUSim::RetCode PicoBlazeStack::directRead ( unsigned int addr,
                                             unsigned int & data ) const
{
    if ( addr >= m_config.m_size )
    {
        return MCUSim::RC_ADDR_OUT_OF_RANGE;
    }

    data = ( 0x3ff & m_data[addr] );
    return MCUSim::RC_OK;
}

MCUSim::RetCode PicoBlazeStack::directWrite ( unsigned int addr,
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

void PicoBlazeStack::resize ( unsigned int newSize )
{
    if ( NULL != m_data )
    {
        delete[] m_data;
    }
    m_data = new unsigned int [ newSize ];
}

void PicoBlazeStack::loadDataFile ( const DataFile * file )
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

void PicoBlazeStack::storeInDataFile ( DataFile * file ) const
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
