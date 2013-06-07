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
 * @file PIC8DataEEPROM.cxx
 */
// =============================================================================

#include "PIC8DataEEPROM.h"

#include "PIC8DataMemory.h"
#include "MCUDataFiles/DataFile.h"

#include <cstdlib>
#include <cstring>

PIC8DataEEPROM::PIC8DataEEPROM()
{
    m_memory = NULL;
}

PIC8DataEEPROM::~PIC8DataEEPROM()
{
    if ( NULL != m_memory )
    {
        delete[] m_memory;
    }
}

PIC8DataEEPROM * PIC8DataEEPROM::link ( MCUSim::EventLogger * eventLogger,
                                        PIC8DataMemory      * dataMemory )
{
    Memory::link(eventLogger, SP_EEPROM);
    m_dataMemory = dataMemory;
    return this;
}

MCUSim::RetCode PIC8DataEEPROM::directRead ( unsigned int addr,
                                             unsigned int & data ) const
{
    if ( addr >= m_config.m_size )
    {
        return MCUSim::RC_ADDR_OUT_OF_RANGE;
    }

    data = m_memory[addr] & 0xff;
    return MCUSim::RC_OK;
}

MCUSim::RetCode PIC8DataEEPROM::directWrite ( unsigned int addr,
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

void PIC8DataEEPROM::resize ( unsigned int newSize )
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

void PIC8DataEEPROM::reset ( MCUSim::ResetMode mode )
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

inline void PIC8DataEEPROM::loadConfig()
{
    resize(m_config.m_size);
}

inline void PIC8DataEEPROM::resetToInitialValues()
{
    for ( unsigned int i = 0; i < m_config.m_size; i++ )
    {
        m_memory[i] = MFLAG_UNDEFINED | MFLAG_DEFAULT;
    }
}

inline void PIC8DataEEPROM::mcuReset()
{
    if ( true == m_writeInProgress )
    {
        logEvent(EVENT_EEPROM_WRITE_CANCELED, m_writeAddress);

        m_dataMemory->setBitFast(PIC8RegNames::EECON1, PIC8RegNames::EECON1_WRERR);
        if ( m_writeAddress < m_size )
        {
            m_memory[m_writeAddress] = getUndefVal() | MFLAG_UNDEFINED;
            logEvent(EVENT_MEM_INF_WR_VAL_WRITTEN, m_writeAddress, ( 0xff & m_memory[m_writeAddress] ) );
        }
    }

    m_writeInProgress = false;
    m_writeAllowed = 0;
    m_writeAddress = 0;
    m_writeTimer = 0;
}

void PIC8DataEEPROM::loadDataFile ( const DataFile * file )
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

void PIC8DataEEPROM::storeInDataFile ( DataFile * file ) const
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

unsigned int PIC8DataEEPROM::getUndefVal() const
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

inline void PIC8DataEEPROM::readEecon2()
{
    unsigned int eecon2 = m_dataMemory->readFast(PIC8RegNames::EECON2);

    if ( 0x55 == eecon2 )
    {
        m_writeAllowed = -1;
        return;
    }
    else if ( 0xAA == eecon2 )
    {
        if ( -1 == m_writeAllowed )
        {
            m_writeAllowed = 1234; // any number above 0 means that write is allowed
            logEvent(EVENT_EEPROM_WRITE_ALLOWED);
            return;
        }
        else if ( m_writeAllowed > 0 )
        {
            return;
        }
    }

    if ( m_writeAllowed > 0 )
    {
        logEvent(EVENT_EEPROM_WRITE_FORBIDDEN);
    }

    m_writeAllowed = 0;
}

inline void PIC8DataEEPROM::readEecon1()
{
    using namespace PIC8RegNames;

    unsigned int eecon1 = m_dataMemory->readFast(EECON1);
    const unsigned int eecon1Orig = eecon1;

    if ( EECON1_RD & eecon1 )
    {
        readByte(&eecon1);
    }

    if ( EECON1_WR & eecon1 )
    {
        writeByte(&eecon1);
    }
    else if ( true == m_writeInProgress )
    {
        logEvent(EVENT_EEPROM_INVALID_CR_CHAGE);
        eecon1 |= EECON1_WR;
    }

    if ( eecon1Orig != eecon1 )
    {
        m_dataMemory->writeFast(EECON1, eecon1);
    }
}

inline void PIC8DataEEPROM::readByte ( unsigned int * eecon1 )
{
    using namespace PIC8RegNames;

    unsigned int eeaddr = m_dataMemory->readFast(EEADR);
    if ( eeaddr >= m_size )
    {
        logEvent(EVENT_MEM_ERR_RD_NOT_IMPLEMENTED, eeaddr);
    }
    else
    {
        unsigned int eedata = m_memory[eeaddr];
        if ( eedata & MFLAG_DEFAULT )
        {
            logEvent(EVENT_MEM_WRN_RD_DEFAULT, eeaddr);
        }
        eedata &= 0xff;

        logEvent(EVENT_MEM_INF_RD_VAL_READ, eeaddr, eedata);
        m_dataMemory->writeFast(EEDATA, eedata);
    }

    (*eecon1) &= (~EECON1_RD);
}

inline void PIC8DataEEPROM::writeByte ( unsigned int * eecon1 )
{
    using namespace PIC8RegNames;

    unsigned int eeaddr = m_dataMemory->readFast(EEADR);
    if ( EECON1_WREN & (*eecon1) )
    {
        if ( m_writeAllowed > 0 )
        {
            if ( true == m_writeInProgress )
            {
                logEvent(EVENT_EEPROM_WRITE_INHIBITED, eeaddr, REASON_ANOTHER_WRITE_IN_PROGRESS);
            }
            else
            {
                unsigned int eedata = m_dataMemory->readFast(EEDATA);

                if ( eeaddr >= m_size )
                {
                    logEvent(EVENT_EEPROM_WRITE_INHIBITED, eeaddr, REASON_INVALID_ADDRESS);
                    logEvent(EVENT_MEM_ERR_WR_NOT_IMPLEMENTED, eeaddr, eedata);
                }
                else
                {
                    m_writeTimer = NOMINAL_WRITE_PERIOD;
                    m_writeAddress = eeaddr;
                    m_memory[eeaddr] = eedata;

                    m_writeInProgress = true;
                    logEvent(EVENT_EEPROM_WRITE_STARTED, eeaddr, eedata);

                    m_writeAllowed = 0;
                    logEvent(EVENT_EEPROM_WRITE_FORBIDDEN);
                }
            }
        }
        else
        {
            logEvent(EVENT_EEPROM_WRITE_INHIBITED, eeaddr, REASON_EECON2_55AA_NOT_FOLLOWED);
        }
    }
    else
    {
        logEvent(EVENT_EEPROM_WRITE_INHIBITED, eeaddr, REASON_WREN_NOT_SET);
    }

    if ( false == m_writeInProgress )
    {
        (*eecon1) &= (~EECON1_WR);
    }
}

void PIC8DataEEPROM::timeStep ( float timeStep,
                                unsigned int clockCycles )
{
    if ( 0 == m_size )
    {
        return;
    }

    if ( true == m_writeInProgress )
    {
        m_writeTimer -= timeStep;
        if ( m_writeTimer <= 0 )
        {
            // Write finished
            logEvent(EVENT_MEM_INF_WR_VAL_WRITTEN, m_writeAddress, ( 0xff & m_memory[m_writeAddress] ));
            unsigned int eecon1 = m_dataMemory->readFast(PIC8RegNames::EECON1);
            eecon1 |= PIC8RegNames::EECON1_EEIF;
            eecon1 &= ~PIC8RegNames::EECON1_WR;
            m_dataMemory->writeFast(PIC8RegNames::EECON1, eecon1);
            m_writeInProgress = false;
            m_writeAddress = 0;
            m_writeTimer = 0;
        }
    }

    if ( 0 != clockCycles )
    {
        readEecon2();
        readEecon1();
    }
}

unsigned int PIC8DataEEPROM::read ( unsigned int addr )
{
    if ( addr >= m_size )
    {
        if ( 0 == m_size )
        {
            logEvent(EVENT_MEM_ERR_RD_NONEXISTENT, addr);
            return getUndefVal();
        }
        else
        {
            logEvent(EVENT_MEM_ERR_WR_NOT_IMPLEMENTED, addr);
            return getUndefVal();
        }
    }

    int result = m_memory[addr];
    if ( result & MFLAG_UNDEFINED )
    {
        logEvent(EVENT_MEM_WRN_RD_UNDEFINED, addr);
    }

    result &= 0xff;
    return result;
}

void PIC8DataEEPROM::write ( unsigned int addr,
                             unsigned int val )
{
    if ( addr >= m_size )
    {
        if ( 0 == m_size )
        {
            logEvent(EVENT_MEM_ERR_WR_NONEXISTENT, addr);
            return;
        }
        else
        {
            logEvent(EVENT_MEM_ERR_WR_NOT_IMPLEMENTED, addr);
            return;
        }
    }

    m_memory[addr] &= ( 0xff000000 ^ MFLAG_UNDEFINED );
    m_memory[addr] |= val;

    logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, addr);
}
