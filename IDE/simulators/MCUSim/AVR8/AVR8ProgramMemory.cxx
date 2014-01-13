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
 * @ingroup AVR8
 * @file AVR8ProgramMemory.cxx
 */
// =============================================================================

#include "AVR8ProgramMemory.h"
#include "AVR8DataMemory.h"
#include "AVR8DataEEPROM.h"
#include "AVR8InstructionSet.h"
#include "MCUDataFiles/DataFile.h"

#include <cstdlib>

AVR8ProgramMemory::AVR8ProgramMemory()
{
    m_memory = nullptr;
    m_size = 0;
}

AVR8ProgramMemory * AVR8ProgramMemory::link ( MCUSimEventLogger * eventLogger,
                                              AVR8BootLoader * bootLoader )
{
    MCUSimMemory::link(eventLogger, SP_CODE);
    m_bootLoader = bootLoader;
    return this;
}

AVR8ProgramMemory::~AVR8ProgramMemory()
{
    if ( nullptr != m_memory )
    {
        delete[] m_memory;
    }
}

void AVR8ProgramMemory::loadDataFile ( const DataFile * file )
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
            byte = ( 0xffff | MFLAG_UNDEFINED );
        }
        else
        {
            byte |= ( (*file)[++i] << 8 );
        }

        m_memory[j] = byte;
    }

    for ( unsigned int i = ( size / 2 ); i < m_size; i++ )
    {
        m_memory[i] = ( 0xffff | MFLAG_UNDEFINED );
    }
}

void AVR8ProgramMemory::storeInDataFile ( DataFile * file ) const
{
    unsigned int size = file->maxSize();

    file->clear();

    for ( unsigned int i = 0, j = 0; i < size; i++, j++ )
    {
        if ( j >= m_size )
        {
            break;
        }

        int byte = m_memory[j];

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

MCUSim::RetCode AVR8ProgramMemory::directRead ( unsigned int addr,
                                                unsigned int & data ) const
{
    if ( addr >= m_size )
    {
        return MCUSim::RC_ADDR_OUT_OF_RANGE;
    }
    data = m_memory[addr];

    if ( MFLAG_UNDEFINED & data )
    {
        data &= 0xffff;
        return MCUSim::RC_NONDETERMINISTIC;
    }
    else
    {
        data &= 0xffff;
        return MCUSim::RC_OK;
    }
}

MCUSim::RetCode AVR8ProgramMemory::directWrite ( unsigned int addr,
                                                 unsigned int data )
{
    if ( addr >= m_size )
    {
        return MCUSim::RC_ADDR_OUT_OF_RANGE;
    }

    m_memory[addr] = (data & 0xffff);
    return MCUSim::RC_OK;
}

void AVR8ProgramMemory::resize ( unsigned int newSize )
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
        m_memory[i] = (getUndefVal() | MFLAG_UNDEFINED);
    }

    if ( nullptr != memoryOrig )
    {
        delete[] memoryOrig;
    }
    m_size = newSize;
}

void AVR8ProgramMemory::reset ( MCUSimBase::ResetMode mode )
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

inline void AVR8ProgramMemory::loadConfig()
{
    resize(m_config.m_size);
}

inline void AVR8ProgramMemory::mcuReset()
{
}

inline void AVR8ProgramMemory::resetToInitialValues()
{
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        m_memory[i] = (0xffff | MFLAG_UNDEFINED);
    }
}

unsigned int AVR8ProgramMemory::getUndefVal() const
{
    if ( -1 == m_config.m_undefinedValue )
    {
        // Generate random value
        return ((unsigned int)rand() & ((1 << 16) - 1));
    }
    else
    {
        // Return predefined value
        return ( m_config.m_undefinedValue & ((1 << 16) - 1) );
    }
}
