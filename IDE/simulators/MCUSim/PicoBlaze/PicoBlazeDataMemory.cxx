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
 * @ingroup PicoBlaze
 * @file PicoBlazeDataMemory.cxx
 */
// =============================================================================

#include "PicoBlazeDataMemory.h"
#include "MCUDataFiles/DataFile.h"

PicoBlazeDataMemory::PicoBlazeDataMemory()
{
    m_size = 0;
    m_memory = NULL;
}

PicoBlazeDataMemory * PicoBlazeDataMemory::link ( MCUSimEventLogger * eventLogger )
{
    MCUSimMemory::link(eventLogger, SP_DATA);
    return this;
}

PicoBlazeDataMemory::~PicoBlazeDataMemory()
{
    if ( NULL != m_memory )
    {
        delete[] m_memory;
    }
}

void PicoBlazeDataMemory::loadDataFile ( const DataFile * file )
{
    unsigned int size = file->maxSize();

    for ( unsigned int i = 0; i < size; i++ )
    {
        if ( i >= m_size )
        {
            break;
        }

        int byte = (*file)[i];
        if ( -1 == byte )
        {
            byte |= MFLAG_UNDEFINED;
        }
        m_memory[i] &= 0xff;
        m_memory[i] |= byte;
    }

    for ( unsigned int i = size; i < m_size; i++ )
    {
        m_memory[i] |= MFLAG_UNDEFINED;
    }
}

void PicoBlazeDataMemory::storeInDataFile ( DataFile * file ) const
{
    unsigned int size = file->maxSize();

    file->clear();
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        if ( i >= size )
        {
            break;
        }

        file->set(i, m_memory[i] & 0xff);
    }
}

MCUSim::RetCode PicoBlazeDataMemory::directRead ( unsigned int addr,
                                                  unsigned int & data ) const
{
    if ( addr >= m_size )
    {
        return MCUSim::RC_ADDR_OUT_OF_RANGE;
    }

    data = ( 0xff & m_memory[addr] );
    return MCUSim::RC_OK;
}

MCUSim::RetCode PicoBlazeDataMemory::directWrite ( unsigned int addr,
                                                   unsigned int data )
{
    if ( addr >= m_size )
    {
        return MCUSim::RC_ADDR_OUT_OF_RANGE;
    }

    m_memory[addr] = ( data & 0xff );
    return MCUSim::RC_OK;
}

void PicoBlazeDataMemory::resize ( unsigned int newSize )
{
    uint32_t * memoryOrig = m_memory;
    m_memory = new uint32_t [ newSize ];

    const unsigned int sizeToCopy = ( m_size <= newSize ) ? m_size : newSize;
    for ( unsigned int i = 0; i < sizeToCopy; i++ )
    {
        m_memory[i] = memoryOrig[i];
    }

    for ( unsigned int i = sizeToCopy; i < newSize; i++ )
    {
        m_memory[i] = getUndefVal();
    }

    if ( NULL != memoryOrig )
    {
        delete memoryOrig;
    }
    m_size = newSize;
}

void PicoBlazeDataMemory::reset ( MCUSimBase::ResetMode mode )
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

inline void PicoBlazeDataMemory::loadConfig()
{
    // Set memory size
    resize(m_config.m_size);

    // Intialize the memory
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        m_memory[i] &= 0xff; // Preserve only the register content, not its configuration.
    }
}

inline void PicoBlazeDataMemory::resetToInitialValues()
{
    // Mark all registers as containing default value
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        m_memory[i] = MFLAG_DEFAULT;
    }
}

inline void PicoBlazeDataMemory::mcuReset()
{
}
