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
 * @file PIC8ExternalInterrupts.cxx
 */
// =============================================================================

#include "PIC8ExternalInterrupts.h"

#include "PIC8DataMemory.h"
#include "PIC8IO.h"
#include "PIC8InterruptController.h"

PIC8ExternalInterrupts * PIC8ExternalInterrupts::link ( MCUSim::EventLogger     * eventLogger,
                                                        PIC8DataMemory          * dataMemory,
                                                        PIC8IO                  * io,
                                                        PIC8InterruptController * interruptController )
{
    Subsys::link(eventLogger, ID_EXT_INT);
    m_dataMemory = dataMemory;
    m_io = io;
    m_interruptController = interruptController;
    return this;
}

void PIC8ExternalInterrupts::reset ( MCUSim::ResetMode mode )
{
    switch ( mode )
    {
        case MCUSim::RSTMD_MCU_RESET:
            mcuReset();
            break;
        default:
            // Irrelevant requests are silently ignored
            break;
    }
}

inline void PIC8ExternalInterrupts::mcuReset()
{
    m_portB = ( 0xf0 & m_dataMemory->readFast(PIC8RegNames::PORTB) );
    m_int0 = m_io->getLog(PIC8PinNames::SPF_INT);
}

void PIC8ExternalInterrupts::portBRead ( unsigned int portB )
{
    if ( false == m_config.m_enabled )
    {
        return;
    }

    m_portB = (0xf0 & portB);
}

void PIC8ExternalInterrupts::clockCycles()
{
    using namespace PIC8RegNames;

    if ( false == m_config.m_enabled )
    {
        return;
    }

    // MCLR
    if ( false == m_io->getLog(PIC8PinNames::SPF_MCLR) )
    {
        m_interruptController->genIntReq(PIC8InterruptController::INTVEC_RESET);
    }

    // RB7:4
    if ( m_portB != (0xf0 & m_dataMemory->readFast(PORTB)) )
    {
        m_dataMemory->setBitFast(INTCON, INTCON_RBIF);
    }
    else
    {
        m_dataMemory->clearBitFast(INTCON, INTCON_RBIF);
    }

    // INT0
    bool int0 = m_io->getLog(PIC8PinNames::SPF_INT);
    if ( false == m_dataMemory->readBitFast(OPTION_REG, OPTION_REG_INTEDG) )
    {
        // Falling edge
        if ( ( true == m_int0 ) && ( false == int0 ) )
        {
            m_dataMemory->setBitFast(INTCON, INTCON_INTF);
        }
    }
    else
    {
        // Rising edge
        if ( ( false == m_int0 ) && ( true == int0 ) )
        {
            m_dataMemory->setBitFast(INTCON, INTCON_INTF);
        }
    }
    m_int0 = int0;
}
