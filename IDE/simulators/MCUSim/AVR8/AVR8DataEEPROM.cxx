// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2012 Moravia Microsystems, s.r.o.
 *
 * @authors Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup AVR8
 * @file AVR8DataEEPROM.cxx
 */
// =============================================================================

#include "AVR8DataEEPROM.h"
#include "AVR8DataMemory.h"
#include "AVR8InterruptController.h"
#include "MCUDataFiles/DataFile.h"

#include <cstdlib>

AVR8DataEEPROM::AVR8DataEEPROM()
{
    m_memory = NULL;
}

AVR8DataEEPROM * AVR8DataEEPROM::link ( MCUSim::EventLogger     * eventLogger,
                                        AVR8DataMemory          * dataMemory,
                                        AVR8InterruptController * interruptController )
{
    Memory::link(eventLogger, SP_EEPROM);

    m_dataMemory = dataMemory;
    m_interruptController = interruptController;

    return this;
}

AVR8DataEEPROM::~AVR8DataEEPROM()
{
    if ( NULL != m_memory )
    {
        delete[] m_memory;
    }
}

void AVR8DataEEPROM::loadDataFile ( const DataFile * file )
{
    unsigned int size = file->maxSize();

    for ( unsigned int i = 0; i < size; i++ )
    {
        if ( i >= m_config.m_size )
        {
            break;
        }

        int byte = (*file)[i];
        if ( -1 == byte )
        {
             byte = MFLAG_UNDEFINED;
        }
        m_memory[i] = byte;
    }
    for ( unsigned int i = size; i < m_config.m_size; i++ )
    {
        m_memory[i] = MFLAG_UNDEFINED;
    }
}

void AVR8DataEEPROM::storeInDataFile ( DataFile * file ) const
{
    unsigned int size = file->maxSize();

    file->clear();
    for ( unsigned int i = 0; i < m_config.m_size; i++ )
    {
        if ( i >= size )
        {
            break;
        }

        int byte = m_memory[i];

        if ( MFLAG_UNDEFINED & byte )
        {
            file->unset(i);
        }
        else
        {
            file->set(i, byte & 0xff);
        }
    }
}

inline unsigned int AVR8DataEEPROM::readEecr ( const unsigned int clockCycles )
{
    using namespace AVR8RegNames;

    unsigned int eecr =  m_dataMemory->readFast(EECR);

    if ( m_eecrLast != eecr )
    {
        const unsigned int eecrOrig = eecr;

        // EEWE flag in EECR reg. can be set only if EEWME was set previously in pevious 4 clock cycles
        if ( EECR_EEWE & eecr )
        {
            if ( 0 == m_eecr_timer )
            {
                // Inhibit changes to the EEWE flag
                eecr &= (0xff ^ EECR_EEWE);
                m_dataMemory->writeFast(EECR, eecr);
            }
        }
        else if ( true == m_writeInProgress )
        {
            // Keep the EEWE flag set while write is in progress
            eecr |= EECR_EEWE;
            m_dataMemory->writeFast(EECR, eecr);
        }

        if ( (EECR_EEMWE & eecr) && !(EECR_EEWE & eecr) )
        {
            // Allow changes to the EEWE flag in EECR register for the NEXT four clock cycles (not right now)
            m_eecr_timer = 4 + clockCycles;
        }

        // Inhibit changes to EEAR reg, while EEWE flag in EECR reg. is set
        if ( EECR_EEWE & eecr )
        {
            unsigned int addr = m_dataMemory->readFast(EEARL);
            if ( m_config.m_addrRegWidth > 8 )
            {
                addr |= ( m_dataMemory->readFast(EEARH) << 8 );
            }

            if ( m_eearLast != addr )
            {
                m_dataMemory->writeFast(EEARL, (m_eearLast & 0xff));
                if ( m_config.m_addrRegWidth > 8 )
                {
                    m_dataMemory->writeFast(EEARH, (m_eearLast >> 8));
                }
            }
        }

        if ( eecrOrig != eecr )
        {
            logEvent(EVENT_EEPROM_INVALID_CR_CHAGE);
        }

        m_eecrLast = eecr;
    }

    // Manage the timer which keeps the EEWME flag set for 4 clock cycles.
    if ( 0 != m_eecr_timer )
    {
        // Decrement the timer
        m_eecr_timer -= clockCycles;
        if ( m_eecr_timer < 0 )
        {
            m_eecr_timer = 0;
        }

        if ( 0 == m_eecr_timer )
        {
            // Clear the EEWME flag (after 4 clock cycles)
            eecr &= (0xff ^ EECR_EEMWE);
            m_dataMemory->writeFast(EECR, eecr);
        }
        else
        {
            // The EEWME flag cannot be changed by software
            eecr |= EECR_EEMWE;
            m_dataMemory->writeFast(EECR, eecr);
        }
    }

    return eecr;
}

inline void AVR8DataEEPROM::writeByte ( float timeStep,
                                        unsigned int eecr )
{
    using namespace AVR8RegNames;

    if ( true == m_writeInProgress )
    {
        if ( m_writeTimer > 0 )
        {
            m_writeTimer -= timeStep;
            return;
        }

        m_writeInProgress = false;
        m_dataMemory->clearBitFast(EECR, EECR_EEWE);

        if ( m_writeBuffer[WB_ADDR] >= m_config.m_size )
        {
            logEvent(EVENT_MEM_ERR_WR_NOT_IMPLEMENTED, m_writeBuffer[WB_ADDR]);
        }
        else if ( m_memory[m_writeBuffer[WB_ADDR]] != m_writeBuffer[WB_DATA] )
        {
            m_memory[m_writeBuffer[WB_ADDR]] &= (0xffffff00 ^ (EVENT_MEM_WRN_RD_DEFAULT | EVENT_MEM_WRN_RD_UNDEFINED));
            m_memory[m_writeBuffer[WB_ADDR]] |= m_writeBuffer[WB_DATA];
            logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, m_writeBuffer[WB_ADDR], m_writeBuffer[WB_DATA]);
        }
        else
        {
            logEvent(EVENT_MEM_INF_WR_VAL_WRITTEN, m_writeBuffer[WB_ADDR]);
        }

        if ( eecr & EECR_EERIE )
        {
            m_interruptController->genIntReq(AVR8InterruptController::INTVEC_EE_RDY);
        }

        return;
    }

    m_writeBuffer[WB_ADDR] = m_dataMemory->readFast(EEARL);
    if ( m_config.m_addrRegWidth > 8 )
    {
        m_writeBuffer[WB_ADDR] |= ( m_dataMemory->readFast(EEARH) << 8 );
    }
    m_eearLast = m_writeBuffer[WB_ADDR];

    if ( m_writeBuffer[WB_ADDR] >= m_config.m_size )
    {
        logEvent(EVENT_MEM_ERR_WR_NOT_IMPLEMENTED);
        return;
    }

    m_writeInProgress = true;
    m_writeBuffer[WB_DATA] = m_dataMemory->readFast(EEDR);
    m_writeTimer = m_config.m_writeTime;
}

inline unsigned int AVR8DataEEPROM::readByte()
{
    using namespace AVR8RegNames;

    unsigned int data;
    unsigned int addr = m_dataMemory->readFast(EEARL);
    if ( m_config.m_addrRegWidth > 8 )
    {
        addr |= ( m_dataMemory->readFast(EEARH) << 8 );
    }
    m_eearLast = addr;

    if ( true == m_writeInProgress )
    {
        logEvent(EVENT_MEM_ERR_RD_ACCESS_DENIED, addr);
        data = getUndefVal();
    }
    else if ( addr >= m_config.m_size )
    {
        logEvent(EVENT_MEM_ERR_RD_NOT_IMPLEMENTED, addr);
        data = getUndefVal();
    }
    else
    {
        data = m_memory[addr];

        if ( MFLAG_DEFAULT & data )
        {
            logEvent(EVENT_MEM_WRN_RD_DEFAULT, addr);
        }
        else if ( MFLAG_UNDEFINED & data )
        {
            logEvent(EVENT_MEM_WRN_RD_UNDEFINED, addr);
        }
        else
        {
            logEvent(EVENT_MEM_INF_RD_VAL_READ, addr);
        }

        data &= 0xff;
    }

    m_dataMemory->clearBitFast(EECR, EECR_EERE);
    m_dataMemory->writeFast(EEDR, data);
    return 4;
}

unsigned int AVR8DataEEPROM::timeStep ( float timeStep,
                                        unsigned int clockCycles )
{
    using namespace AVR8RegNames;

    unsigned int eecr;
    if ( 0 == clockCycles )
    {
        eecr = m_eecrLast;
    }
    else
    {
        // EECR can be changed only on certain conditions
        eecr = readEecr(clockCycles);
    }

    if ( eecr & EECR_EEWE )
    {
        writeByte(timeStep, eecr);
    }

    if ( eecr & EECR_EERE )
    {
        return readByte();
    }

    return 0;
}

MCUSim::RetCode AVR8DataEEPROM::directRead ( unsigned int addr,
                                             unsigned int & data ) const
{
    if ( addr >= m_config.m_size )
    {
        return MCUSim::RC_ADDR_OUT_OF_RANGE;
    }

    data = m_memory[addr] & 0xff;
    return MCUSim::RC_OK;

}

MCUSim::RetCode AVR8DataEEPROM::directWrite ( unsigned int addr,
                                              unsigned int data )
{
    if ( addr >= m_config.m_size )
    {
        return MCUSim::RC_ADDR_OUT_OF_RANGE;
    }

    data &= 0xff;
    m_memory[addr] &= 0xffffff00;
    m_memory[addr] |= data;

    return MCUSim::RC_OK;
}
void AVR8DataEEPROM::resize ( unsigned int newSize )
{
    uint32_t * memoryOrig = m_memory;
    m_memory = new uint32_t[newSize];

    if ( NULL == memoryOrig )
    {
        m_config.m_size = 0;
    }

    unsigned int sizeToCopy = ( m_config.m_size <= newSize ) ? m_config.m_size : newSize;
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
    m_config.m_size = newSize; // <-- This might look weird but it should make sense after all.
}

void AVR8DataEEPROM::reset ( MCUSim::ResetMode mode )
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

inline void AVR8DataEEPROM::loadConfig()
{
    resize(m_config.m_size);
}

inline void AVR8DataEEPROM::resetToInitialValues()
{
    for ( unsigned int i = 0; i < m_config.m_size; i++ )
    {
        m_memory[i] = MFLAG_UNDEFINED;
    }
}

inline void AVR8DataEEPROM::mcuReset()
{
    m_eearLast = m_dataMemory->readFast(AVR8RegNames::EEARL);
    if ( m_config.m_addrRegWidth > 8 )
    {
        m_eearLast |= ( m_dataMemory->readFast(AVR8RegNames::EEARH) << 8 );
    }
    m_eecrLast = m_dataMemory->readFast(AVR8RegNames::EECR);
    m_eecr_timer = 0;
    m_writeInProgress = false;
}

unsigned int AVR8DataEEPROM::getUndefVal() const
{
    if ( -1 == m_config.m_undefinedValue )
    {
        // Generate random value
        return ( (unsigned int)rand() & ((1 << 8) - 1) );
    }
    else
    {
        // Return predefined value
        return ( m_config.m_undefinedValue & ((1 << 8) - 1) );
    }
}
