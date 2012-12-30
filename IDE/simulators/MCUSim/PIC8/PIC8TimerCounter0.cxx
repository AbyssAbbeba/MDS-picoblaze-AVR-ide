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
 * @ingroup PIC8
 * @file PIC8TimerCounter0.cxx
 */
// =============================================================================

#include "PIC8TimerCounter0.h"

#include "PIC8DataMemory.h"
#include "PIC8IO.h"
#include "PIC8Timer0WdtPrescaller.h"

PIC8TimerCounter0::PIC8TimerCounter0()
{
}

PIC8TimerCounter0::~PIC8TimerCounter0()
{
}

PIC8TimerCounter0 * PIC8TimerCounter0::link ( MCUSim::EventLogger     * eventLogger,
                                PIC8DataMemory          * dataMemory,
                                PIC8IO                  * io,
                                PIC8Timer0WdtPrescaller * timer0WdtPrescaller )
{
    Subsys::link(eventLogger, ID_COUNTER_0);
    m_dataMemory = dataMemory;
    m_io = io;
    m_timer0WdtPrescaller = timer0WdtPrescaller;
    return this;
}

void PIC8TimerCounter0::reset ( MCUSim::ResetMode mode )
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

void PIC8TimerCounter0::clockCycles ( unsigned int cycles )
{
    using namespace PIC8RegNames;

    if ( false == m_config.m_enabled )
    {
        return;
    }

    unsigned int n;

    if ( false == m_dataMemory->readBitFast(OPTION_REG, OPTION_REG_T0CS) )
    {
        // Timer mode
        n = cycles;
    }
    else
    {
        // Counter mode
        bool clkIn = ( m_io->getVoltage(PIC8PinNames::SPF_T0CKI) > ( m_io->getSourceVoltage() / 2.0 ) );
        if ( true == m_dataMemory->readBitFast(OPTION_REG, OPTION_REG_T0SE) )
        {
            // Falling edge
            if ( ( true == m_lastClkIn ) && ( false == clkIn ) )
            {
                n = 1;
            }
        }
        else
        {
            // Rising edge
            if ( ( false == m_lastClkIn ) && ( true == clkIn ) )
            {
                n = 1;
            }
        }
        m_lastClkIn = clkIn;
    }

    if ( true == m_timer0WdtPrescaller->isAssignedToTimer0() )
    {
        m_timer0WdtPrescaller->increment(n);
        n = m_timer0WdtPrescaller->flagReadAndClear();
    }

    unsigned int value = m_dataMemory->readFast(TMR0);
    value += m_valToIncrBy;
    m_valToIncrBy = n;
    if ( value > 0xff )
    {
        value &= 0xff;
        m_dataMemory->setBitFast(INTCON, INTCON_T0IF);
    }
    m_dataMemory->writeFast(TMR0, value);
}

inline void PIC8TimerCounter0::mcuReset()
{
    m_valToIncrBy = 0;
    m_lastClkIn = false;
}
