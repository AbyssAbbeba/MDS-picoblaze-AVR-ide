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
 * @file PIC8WatchDogTimer.cxx
 */
// =============================================================================

#include "PIC8WatchDogTimer.h"

#include "PIC8Timer0WdtPrescaller.h"
#include "PIC8DataMemory.h"
#include "PIC8ConfigWord.h"
#include "PIC8InterruptController.h"

// NOTE: see declaration of "constexpr static float NOMINAL_TIME_OUT = 18e3;" in the header file.
#define NOMINAL_TIME_OUT 18e3

PIC8WatchDogTimer::PIC8WatchDogTimer()
{
}

PIC8WatchDogTimer::~PIC8WatchDogTimer()
{
}

PIC8WatchDogTimer * PIC8WatchDogTimer::link ( MCUSim::EventLogger     * eventLogger,
                                              PIC8Timer0WdtPrescaller * timer0WdtPrescaller,
                                              PIC8DataMemory          * dataMemory,
                                              PIC8ConfigWord          * configWord,
                                              PIC8InterruptController * interruptCtrl )
{
    Subsys::link(eventLogger, ID_WATCHDOG);
    m_timer0WdtPrescaller = timer0WdtPrescaller;
    m_dataMemory = dataMemory;
    m_configWord = configWord;
    m_interruptController = interruptCtrl;
    return this;
}

void PIC8WatchDogTimer::reset ( MCUSim::ResetMode mode )
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

inline void PIC8WatchDogTimer::mcuReset()
{
    m_time = 0;
}

void PIC8WatchDogTimer::watchDogReset()
{
    m_time = 0;
}

void PIC8WatchDogTimer::watchDogPrescalerReset()
{
    m_timer0WdtPrescaller->prescalerReset();
}

void PIC8WatchDogTimer::timeStep ( float timeStep,
                                   unsigned int )
{
    if ( false == m_config.m_enabled )
    {
        return;
    }

    if ( false == (*m_configWord)[PIC8ConfigWord::CFGW_WDTE] )
    {
        return;
    }

    m_time += ( timeStep * 1e6f /* s -> µs */ );

    if ( m_time > NOMINAL_TIME_OUT )
    {
        unsigned int n = ( unsigned int ) ( m_time / NOMINAL_TIME_OUT );
        m_time -= ( n * NOMINAL_TIME_OUT );

        if ( false == m_timer0WdtPrescaller->isAssignedToTimer0())
        {
            m_timer0WdtPrescaller->increment(n);
            if ( 0 != m_timer0WdtPrescaller->flagReadAndClear() )
            {
                logEvent(EVENT_WDT_RESET);
                m_dataMemory->clearBitFast(PIC8RegNames::STATUS, PIC8RegNames::STATUS_TO);
                m_interruptController->genIntReq(PIC8InterruptController::INTVEC_RESET);
            }
        }
        else
        {
            logEvent(EVENT_WDT_RESET);
            m_dataMemory->clearBitFast(PIC8RegNames::STATUS, PIC8RegNames::STATUS_TO);
            m_interruptController->genIntReq(PIC8InterruptController::INTVEC_RESET);
        }
    }
}
