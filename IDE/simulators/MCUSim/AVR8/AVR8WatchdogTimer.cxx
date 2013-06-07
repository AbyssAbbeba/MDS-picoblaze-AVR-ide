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
 * @ingroup AVR8
 * @file AVR8WatchdogTimer.cxx
 */
// =============================================================================

#include "AVR8WatchdogTimer.h"
#include "AVR8DataMemory.h"
#include "AVR8InterruptController.h"
#include "AVR8FusesAndLocks.h"

#include <cmath>

AVR8WatchdogTimer * AVR8WatchdogTimer::link ( MCUSim::EventLogger     * eventLogger,
                                              AVR8DataMemory          * dataMemory,
                                              AVR8InterruptController * interruptController,
                                              AVR8FusesAndLocks       * fusesAndLocks )
{
    Subsys::link(eventLogger, ID_WATCHDOG);

    m_dataMemory = dataMemory;
    m_interruptController = interruptController;
    m_fusesAndLocks = fusesAndLocks;

    return this;
}

inline unsigned int AVR8WatchdogTimer::readWdtcr ( const unsigned int clockCycles )
{
    using namespace AVR8RegNames;

    unsigned int wdtcr = m_dataMemory->readFast(WDTCR);

    if ( m_wdtcrLast != wdtcr )
    {
        const unsigned int wdtcrOrig = wdtcr;

        if ( WDTCR_WDCE & m_wdtcrLast )
        {
            // WDE stays on when FUSE_WDTON is programmed.
            if ( true == (*m_fusesAndLocks)[AVR8FusesAndLocks::FUSE_WDTON] )
            {
                wdtcr |= WDTCR_WDE;
            }

            // Accept the change (but don't change the WDCE flag, it's cleared automatically)
            wdtcr |= WDTCR_WDCE;
            m_wdtcrLast = wdtcr;
        }
        else
        {
            if ( (WDTCR_WDE | WDTCR_WDCE) == (wdtcr & (WDTCR_WDE | WDTCR_WDCE)) )
            {
                // Allow changes to the register for the next 4 clock cycles.
                m_wdce_timer = 4 + clockCycles;

                // Set WDCE flag but dont't change anything else.
                m_wdtcrLast |= WDTCR_WDCE;
                wdtcr = m_wdtcrLast;
            }
            else
            {
                // Overwrite the change
                wdtcr = m_wdtcrLast;
                m_dataMemory->writeFast(WDTCR, wdtcr);
            }
        }

        if ( wdtcrOrig != wdtcr )
        {
            logEvent(EVENT_WDT_INVALID_CR_CHAGE, WDTCR, wdtcrOrig);
        }
    }

    // Manage the timer which keeps the WDCE flag set for 4 clock cycles.
    if ( 0 != m_wdce_timer )
    {
        // Decrement the timer
        m_wdce_timer -= clockCycles;
        if ( m_wdce_timer < 0 )
        {
            m_wdce_timer = 0;
        }

        if ( 0 == m_wdce_timer )
        {
            // Clear the WDCE flag (after 4 clock cycles)
            wdtcr &= (0xff ^ WDTCR_WDCE);
            m_wdtcrLast = wdtcr;
            m_dataMemory->writeFast(WDTCR, wdtcr);
        }
    }

    return wdtcr;
}

void AVR8WatchdogTimer::timeStep ( float timeStep,
                                   unsigned int clockCycles )
{
    using namespace AVR8RegNames;

    // WDTCR can be changed only on certain conditions
    unsigned int wdtcr;
    if ( 0 == clockCycles)
    {
        wdtcr = m_wdtcrLast;
    }
    else
    {
        wdtcr = readWdtcr(clockCycles);
    }

    if ( 0 == (wdtcr & WDTCR_WDE) )
    {
        m_prescaler = 0;
        return;
    }

    timeStep *= 1e6f; // s -> µs
    m_time += timeStep;
    if ( m_time < 1 )
    {
        return;
    }
    double intPart;
    m_time = modf(m_time, &intPart);
    m_prescaler += (unsigned int)intPart;
    logEvent(EVENT_WDT_PRESCALER_CHANGED);

    // Watchdog Timer Prescale Select:
    // -------------------------------
    // WDP2 WDP1    WDP0    Number of WDT Oscillator Cycles
    // 0    0       0       16,384
    // 0    0       1       32,768
    // 0    1       0       65,536
    // 0    1       1       131,072
    // 1    0       0       262,144
    // 1    0       1       524,288
    // 1    1       0       1,048,576
    // 1    1       1       2,097,152
    unsigned int max;
    wdtcr &= ( WDTCR_WDP2 | WDTCR_WDP1 | WDTCR_WDP0 );
    switch ( wdtcr )
    {
        case 0:
            max = (0x4 << 12); // 16,384
            break;
        case WDTCR_WDP0:
            max = (0x8 << 12); // 32,768
            break;
        case WDTCR_WDP1:
            max = (0x1 << 16); // 65,536
            break;
        case ( WDTCR_WDP1 | WDTCR_WDP0 ):
            max = (0x2 << 16); // 131,072
            break;
        case WDTCR_WDP2:
            max = (0x4 << 16); // 262,144
            break;
        case ( WDTCR_WDP2 | WDTCR_WDP0 ):
            max = (0x8 << 16); // 524,288
            break;
        case ( WDTCR_WDP2 | WDTCR_WDP1 ):
            max = (0x1 << 20); // 1,048,576;
            break;
        case ( WDTCR_WDP2 | WDTCR_WDP1 | WDTCR_WDP0 ):
            max = (0x2 << 20); // 2,097,152
            break;
        default:
            // TODO: add some DEBUG error message here
            break;
    }

    if ( m_prescaler & max )
    {
        logEvent(EVENT_WDT_RESET);
        m_dataMemory->setBitFast(MCUCSR, MCUCSR_WDRF);
        m_interruptController->genIntReq(AVR8InterruptController::INTVEC_RESET);
    }
}

void AVR8WatchdogTimer::reset(MCUSim::ResetMode mode)
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

inline void AVR8WatchdogTimer::mcuReset()
{
    m_time = 0;
    m_prescaler = 0;
    m_wdce_timer = 0;
    m_wdtcrLast = m_dataMemory->readFast(AVR8RegNames::WDTCR);
}
