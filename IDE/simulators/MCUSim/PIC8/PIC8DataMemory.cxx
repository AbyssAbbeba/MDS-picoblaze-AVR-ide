// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup PIC8
 * @file PIC8DataMemory.cxx
 */
// =============================================================================

#include "PIC8DataMemory.h"

#include "MCUDataFiles/DataFile.h"
#include "PIC8InstructionSet.h"
#include "PIC8ExternalInterrupts.h"

#include <cstdlib>
#include <cstring>

PIC8DataMemory::PIC8DataMemory()
{
    m_size = 0;
    m_memory = NULL;
}

PIC8DataMemory * PIC8DataMemory::link ( MCUSim::EventLogger    * eventLogger,
                                        PIC8ExternalInterrupts * externalInterrupts,
                                        PIC8InstructionSet     * instructionSet )
{
    Memory::link(eventLogger, SP_DATA);
    m_externalInterrupts = externalInterrupts;
    m_instructionSet = instructionSet;
    return this;
}

PIC8DataMemory::~PIC8DataMemory()
{
    if ( NULL != m_memory )
    {
        delete[] m_memory;
    }
}

void PIC8DataMemory::loadDataFile ( const DataFile * file )
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

void PIC8DataMemory::storeInDataFile ( DataFile * file ) const
{
    unsigned int size = file->maxSize();

    file->clear();
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        if ( i >= size )
        {
            break;
        }

        if ( false == isAddrGenerallyValid(i) )
        {
            continue;
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

inline int PIC8DataMemory::addrTrans ( int addr,
                                       bool allowIndirect ) const
{
    // Convert to absolute address
    if ( addr < 0 )
    {
        // Already absolute address but negative
        addr = -addr;
    }
    else
    {
        // Address relative to the origin of currently active bank
        addr += ( NOMINAL_BANK_SIZE * getActiveBank() );
    }

    if ( addr >= (int) m_size )
    {
        return -1;
    }
    else
    {
        addr = m_config.m_addrTransTab[addr];

        if ( -PIC8RegNames::INDF == addr )
        {
            if ( true == allowIndirect )
            {
                addr = addrTrans(readFast(-PIC8RegNames::FSR), false);
            }
            else
            {
                /*
                 * Reading the INDF register itself indirectly (FSR = '0') will read 00h.
                 * Writing to the INDF register indirectly results in a no-operation.
                 */
                return -2;
            }
        }

        return addr;
    }
}

inline unsigned int PIC8DataMemory::getActiveBank() const
{
    unsigned int statusReg = readFast(-PIC8RegNames::STATUS);
    unsigned int result = 0;

    result += ( PIC8RegNames::STATUS_RP0 & statusReg ) ? 1 : 0;
    result += ( PIC8RegNames::STATUS_RP1 & statusReg ) ? 2 : 0;

    return result;
}

inline bool PIC8DataMemory::isAddrGenerallyValid ( const unsigned int addr ) const
{
    if ( ( addr >= m_size )
           ||
         ( (int)addr != m_config.m_addrTransTab[addr] )
           ||
         ( -PIC8RegNames::INDF == addr ) )
    {
        return false;
    }

    return true;
}

MCUSim::RetCode PIC8DataMemory::directRead ( unsigned int addr,
                                             unsigned int & data ) const
{
    if ( true == isAddrGenerallyValid(addr) )
    {
        return MCUSim::RC_ADDR_OUT_OF_RANGE;
    }
    else
    {
        data = ( 0xff & m_memory[addr] );
        return MCUSim::RC_OK;
    }
}

MCUSim::RetCode PIC8DataMemory::directWrite ( unsigned int addr,
                                              unsigned int data )
{
    if ( true == isAddrGenerallyValid(addr) )
    {
        return MCUSim::RC_ADDR_OUT_OF_RANGE;
    }
    else
    {
        m_memory[addr] = ( 0xff & data );
        return MCUSim::RC_OK;
    }
}

void PIC8DataMemory::resize ( unsigned int newSize )
{
    uint32_t * memoryOrig = m_memory;
    m_memory = new uint32_t[newSize];

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

void PIC8DataMemory::reset(MCUSim::ResetMode mode)
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

inline void PIC8DataMemory::loadConfig()
{
    // Set memory size
    resize(m_config.m_size);

    // Intialize IO memory, i.e. set memory cell flags
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        if ( false == isAddrGenerallyValid(i) )
        {
            continue;
        }

        m_memory[i] &= 0xff; // Preserve only the register content, not configuration
        m_memory[i] |= ( m_config.m_initValues[i] & 0xffffff00 );
    }
}

inline void PIC8DataMemory::resetToInitialValues()
{
    // Mark all registers as containing default value
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        if ( false == isAddrGenerallyValid(i) )
        {
            continue;
        }

        m_memory[i] |= MFLAG_DEFAULT;
    }

    // Initialize the memory
    mcuReset();
}

inline void PIC8DataMemory::mcuReset()
{
    // (re-)Initialize the memory, i.e. set memory cell flags
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        if ( false == isAddrGenerallyValid(i) )
        {
            continue;
        }

        m_memory[i] &= 0xffffff00; // Preserve only the register configuration, not its content
        m_memory[i] |= ( m_config.m_initValues[i] & 0xff );

        // Generate random values according to the default value initialization mask
        if ( 0 != m_config.m_randomInit[i] )
        {
            m_memory[i] ^= ( getUndefVal() & m_config.m_randomInit[i] );
        }
    }
}

PIC8DataMemory::Config::Config()
{
    m_undefinedValue = -1;

    m_size = 0;
    m_addrTransTab = NULL;
    m_randomInit = NULL;
    m_initValues = NULL;
}

PIC8DataMemory::Config::~Config()
{
    if ( NULL != m_addrTransTab )
    {
        delete[] m_addrTransTab;
    }
    if ( NULL != m_randomInit )
    {
        delete[] m_randomInit;
    }
    if ( NULL != m_initValues )
    {
        delete[] m_initValues;
    }
}

inline unsigned int PIC8DataMemory::getUndefVal() const
{
    if ( -1 == m_config.m_undefinedValue )
    {
        // Generate random value
        return ( (unsigned int)rand() &  0xff );
    }
    else
    {
        // Return predefined value
        return ( m_config.m_undefinedValue & 0xff );
    }
}

void PIC8DataMemory::write ( int addr,
                             unsigned int val )
{
    // Check whether the memory is even implemented
    if ( 0 == m_size )
    {
        logEvent(EVENT_MEM_ERR_WR_NONEXISTENT, addr);
        return;
    }

    int absoluteAddr = addrTrans(addr);
    if ( -1 == absoluteAddr )
    {
        logEvent(EVENT_MEM_ERR_WR_NOT_IMPLEMENTED, addr);
        return;
    }
    else if ( -2 == absoluteAddr )
    {
        logEvent(EVENT_MEM_ERR_WR_ACCESS_DENIED, -PIC8RegNames::INDF);
        return;
    }

    uint32_t result = m_memory[absoluteAddr];

    if ( 0 == ( result & 0xffff00 ) )
    {
        logEvent(EVENT_MEM_ERR_WR_NOT_IMPLEMENTED, addr);
        return;
    }
    if ( 0 == ( result & 0xff00 ) )
    {
        logEvent(EVENT_MEM_WRN_WR_READ_ONLY, addr);
        return;
    }

    if ( result & MFLAG_RESERVED )
    {
        logEvent(EVENT_MEM_WRN_WR_RESERVED_WRITTEN, addr);
    }

    const uint32_t valueOrig = ( result & 0xff );
    val &= ( ( result & 0xff00 ) >> 8 );

    result &= ( 0xffffff00 ^ ( MFLAG_UNDEFINED | MFLAG_DEFAULT ) );
    result |= val;

    m_memory[absoluteAddr] = result;
    if ( -PIC8RegNames::PCL == absoluteAddr )
    {
        unsigned int newPc = ( m_memory[-PIC8RegNames::PCLATH] << 8 ) | val;
        m_instructionSet->setProgramCounter(newPc);
    }
    else

    logEvent(EVENT_MEM_INF_WR_VAL_WRITTEN, addr);

    if ( valueOrig != val)
    {
        logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, addr);
    }
}

unsigned int PIC8DataMemory::read ( int addr )
{
    // Check whether the memory is even implemented
    if ( 0 == m_size )
    {
        logEvent(EVENT_MEM_ERR_RD_NONEXISTENT, addr);
        return getUndefVal();
    }

    // Translate the given address to absolute address in the memory space
    int absoluteAddr = addrTrans(addr);
    if ( -1 == absoluteAddr )
    {
        logEvent(EVENT_MEM_ERR_RD_NOT_IMPLEMENTED, addr);
        return getUndefVal();
    }
    else if ( -2 == absoluteAddr )
    {
        logEvent(EVENT_MEM_ERR_RD_ACCESS_DENIED, -PIC8RegNames::INDF);
        return 0;
    }

    uint32_t result = m_memory[absoluteAddr];

    if ( 0 == ( result & 0xffff00 ) )
    {
        logEvent(EVENT_MEM_ERR_RD_NOT_IMPLEMENTED, addr);
    }
    if ( 0 == ( result & 0xff0000 ) )
    {
        logEvent(EVENT_MEM_WRN_RD_WRITE_ONLY, addr);
    }
    if ( result & MFLAG_RESERVED )
    {
        logEvent(EVENT_MEM_WRN_RD_RESERVED_READ, addr);
    }
    if ( result & MFLAG_UNDEFINED )
    {
        logEvent(EVENT_MEM_WRN_RD_UNDEFINED, addr);
    }
    if ( result & MFLAG_DEFAULT )
    {
        logEvent(EVENT_MEM_WRN_RD_DEFAULT, addr);
    }

    logEvent(EVENT_MEM_INF_RD_VAL_READ, addr);

    if ( 0 != ( result & 0xff0000 ) )
    {
        result ^= ((result >> 16) & 0xff) & getUndefVal();
    }

    result &= 0x0ff;

    if ( -PIC8RegNames::PORTB == absoluteAddr )
    {
        m_externalInterrupts->portBRead(result);
    }

    return result;
}
