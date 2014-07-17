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

AdaptableSimStack * AdaptableSimStack::link ( MCUSimEventLogger      * eventLogger,
                                              AdaptableSimRegisters  * registers,
                                              AdaptableSimDataMemory * dataMemory )
{
    MCUSimMemory::link(eventLogger, SP_STACK);

    m_registers  = registers;
    m_dataMemory = dataMemory;

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

void AdaptableSimStack::pushOnStack ( unsigned int value )
{
    if ( true == m_config.m_useDesignatedStack )
    {
        if ( (int) m_config.m_size == m_position )
        {
            logEvent(MCUSimEventLogger::FLAG_HI_PRIO, EVENT_STACK_OVERFLOW, m_position, value);
            m_position = 0;
        }

        logEvent(EVENT_MEM_INF_WR_VAL_WRITTEN, m_position, value);
        if ( m_data[m_position] != value )
        {
            logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, m_position, value);
        }
        m_data[m_position] = value;
        m_position++;
        logEvent(EVENT_STACK_SP_CHANGED, m_position);
    }
    else
    {
        switch ( m_config.m_simpleStack.m_operation )
        {
            case Config::SimpleStack::OP_PREINC:
                incrSP(1);
                pushBySP(value);
                break;
            case Config::SimpleStack::OP_POSTINC:
                pushBySP(value);
                incrSP(1);
                break;
            case Config::SimpleStack::OP_PREDEC:
                incrSP(-1);
                pushBySP(value);
                break;
            case Config::SimpleStack::OP_POSTDEC:
                pushBySP(value);
                incrSP(-1);
                break;
        }
    }
}

unsigned int AdaptableSimStack::popFromStack()
{
    if ( true == m_config.m_useDesignatedStack )
    {
        m_position--;
        if ( -1 == m_position )
        {
            logEvent(MCUSimEventLogger::FLAG_HI_PRIO, EVENT_STACK_UNDERFLOW, m_position, -1 );
            m_position = (int) m_config.m_size - 1;
        }

        unsigned int result = ( 0x3ff & m_data[m_position] );

        logEvent(EVENT_STACK_SP_CHANGED, m_position);
        logEvent(EVENT_MEM_INF_RD_VAL_READ, m_position, result);

        return result;
    }
    else
    {
        unsigned int result = 0;

        switch ( m_config.m_simpleStack.m_operation )
        {
            case Config::SimpleStack::OP_PREINC:
                result = popBySP();
                incrSP(-1);
                break;
            case Config::SimpleStack::OP_POSTINC:
                incrSP(-1);
                result = popBySP();
                break;
            case Config::SimpleStack::OP_PREDEC:
                result = popBySP();
                incrSP(1);
                break;
            case Config::SimpleStack::OP_POSTDEC:
                incrSP(1);
                result = popBySP();
                break;
        }

        return result;
    }
}

void AdaptableSimStack::incrSP ( int by )
{
    const Config::SimpleStack::Pointer & spCfg = m_config.m_simpleStack.m_pointer;
    unsigned int spVal;

    if ( Config::SimpleStack::SP_REG == spCfg.m_space )
    {
        spVal = m_registers->read((unsigned int) spCfg.m_address);
    }
    else
    {
        spVal = m_dataMemory->read((unsigned int) spCfg.m_address);
    }

    if ( true == spCfg.m_indirect )
    {
        if ( Config::SimpleStack::SP_REG == spCfg.m_space )
        {
            spVal = m_registers->read(spVal);
        }
        else
        {
            spVal = m_dataMemory->read(spVal);
        }
    }

    spVal += by;

    if ( (int) spVal < 0 )
    {
        logEvent(MCUSimEventLogger::FLAG_HI_PRIO, EVENT_STACK_UNDERFLOW);
    }
    else if ( ( (int) spVal > spCfg.m_maxSize ) || ( spVal & ~0xffU ) )
    {
        logEvent(MCUSimEventLogger::FLAG_HI_PRIO, EVENT_STACK_OVERFLOW);
    }

    spVal &= 0xff;

    if ( Config::SimpleStack::SP_REG == spCfg.m_space )
    {
        m_registers->write((unsigned int) spCfg.m_address, spVal);
    }
    else
    {
        m_dataMemory->write((unsigned int) spCfg.m_address, spVal);
    }
}

unsigned int AdaptableSimStack::getSPVal()
{
    const Config::SimpleStack::Pointer & spCfg = m_config.m_simpleStack.m_pointer;
    unsigned int spVal;

    if ( Config::SimpleStack::SP_REG == spCfg.m_space )
    {
        spVal = m_registers->read((unsigned int) spCfg.m_address);
    }
    else
    {
        spVal = m_dataMemory->read((unsigned int) spCfg.m_address);
    }

    if ( true == spCfg.m_indirect )
    {
        if ( Config::SimpleStack::SP_REG == spCfg.m_space )
        {
            spVal = m_registers->read(spVal);
        }
        else
        {
            spVal = m_dataMemory->read(spVal);
        }
    }

    return ( spVal + m_config.m_simpleStack.m_content.m_offset );
}

void AdaptableSimStack::pushBySP ( unsigned int value )
{
    if ( Config::SimpleStack::SP_REG == m_config.m_simpleStack.m_content.m_space )
    {
        m_registers->write(getSPVal(), value);
    }
    else
    {
        m_dataMemory->write(getSPVal(), value);
    }
}

unsigned int AdaptableSimStack::popBySP()
{
    if ( Config::SimpleStack::SP_REG == m_config.m_simpleStack.m_content.m_space )
    {
        return m_registers->read(getSPVal());
    }
    else
    {
        return m_dataMemory->read(getSPVal());
    }
}
