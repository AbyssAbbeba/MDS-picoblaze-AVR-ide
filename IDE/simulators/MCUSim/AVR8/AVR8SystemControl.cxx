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
 * @file AVR8SystemControl.cxx
 */
// =============================================================================

#include "AVR8SystemControl.h"
#include "AVR8DataMemory.h"
#include "AVR8InterruptController.h"
#include "AVR8WatchdogTimer.h"
#include "AVR8IO.h"
#include "AVR8FusesAndLocks.h"

#include <cmath>

#ifndef NAN
    #error 'NAN' macro is not defined
#endif

AVR8SystemControl * AVR8SystemControl::link ( MCUSimEventLogger     * eventLogger,
                                              AVR8DataMemory          * dataMemory,
                                              AVR8InterruptController * interruptController,
                                              AVR8WatchdogTimer       * watchdogTimer,
                                              AVR8IO                  * io,
                                              AVR8FusesAndLocks       * fusesAndLocks )
{
    MCUSimSubsys::link(eventLogger, ID_SYS_CONTROL);

    m_dataMemory = dataMemory;
    m_interruptController = interruptController;
    m_watchdogTimer = watchdogTimer;
    m_io = io;
    m_fusesAndLocks = fusesAndLocks;

    return this;
}

inline void AVR8SystemControl::handleExternalReset ( const float timeStep )
{
    if ( true == (*m_fusesAndLocks)[AVR8FusesAndLocks::FUSE_RSTDISBL] )
    {
        return;
    }

    float rstVoltage = m_io->getVoltage(AVR8PinNames::SPF_RESET);

    if ( rstVoltage != rstVoltage )
    {
        return;
    }

    if ( true == m_resetInProgress )
    {
        if ( rstVoltage < m_config.m_resetTresholdLow )
        {
            m_resetInProgress = false;
            logEvent(EVENT_EXT_RESET_CANCELED);
        }
        else
        {
            m_rstTime -= timeStep;

            if ( m_rstTime < 0 )
            {
                if ( rstVoltage < m_config.m_resetTresholdHigh )
                {
                    logEvent(EVENT_EXT_RESET_UNSTABLE);
                }
                logEvent(EVENT_EXT_RESET_INVOKED);
                m_dataMemory->setBitFast(AVR8RegNames::MCUCSR, AVR8RegNames::MCUCSR_EXTRF);
                m_interruptController->genIntReq(AVR8InterruptController::INTVEC_RESET);
                m_resetInProgress = false;
                return;
            }
        }

    }
    else if ( rstVoltage > m_config.m_resetTresholdLow )
    {
        m_resetInProgress = true;
        m_rstTime = m_config.m_minResetPulseWidth;
        logEvent(EVENT_EXT_RESET_START);
    }
}

inline void AVR8SystemControl::handleBrownOutReset ( const float timeStep )
{
    if ( false == (*m_fusesAndLocks)[AVR8FusesAndLocks::FUSE_BODEN] )
    {
        return;
    }

    int bodLevel = ( ( true == (*m_fusesAndLocks)[AVR8FusesAndLocks::FUSE_BODLEVEL] ) ? 0 : 1);
    const float treshold = m_config.m_brownOutTreshold[bodLevel];

    if ( true == m_brownOutReset )
    {
        if ( m_io->getVoltage(AVR8PinNames::PIN_VCC) < treshold )
        {
            m_interruptController->genIntReq(AVR8InterruptController::INTVEC_RESET);
        }
        else
        {
            m_brownOutReset = false;
            logEvent(EVENT_BOD_RESET_END);
        }
    }
    else
    {
        if ( m_io->getVoltage(AVR8PinNames::PIN_VCC) >= treshold )
        {
            return;
        }

        if ( m_brownOutTimer != m_brownOutTimer )
        {
            logEvent(EVENT_BOD_RESET_TMR_START);
            m_brownOutTimer = m_config.m_tBOD;

        }
        else if ( m_brownOutTimer < 0 )
        {
            m_brownOutTimer = NAN;
            m_brownOutReset = true;
            logEvent(EVENT_BOD_RESET_BEGIN);
            m_dataMemory->setBitFast(AVR8RegNames::MCUCSR, AVR8RegNames::MCUCSR_BORF);
            m_interruptController->genIntReq(AVR8InterruptController::INTVEC_RESET);
        }
        else
        {
            m_brownOutTimer -= timeStep;
        }
    }
}

inline void AVR8SystemControl::handlePowerOnReset()
{
    if ( true == m_powerOnReset )
    {
        if ( m_io->getVoltage(AVR8PinNames::PIN_VCC) < m_config.m_powerOnResetTresholdRaising )
        {
            m_interruptController->genIntReq(AVR8InterruptController::INTVEC_RESET);
        }
        else
        {
            m_powerOnReset = false;
            logEvent(EVENT_POWER_ON_RESET_END);
        }
    }
    else
    {
        if ( m_io->getVoltage(AVR8PinNames::PIN_VCC) < m_config.m_powerOnResetTresholdFalling )
        {
            m_powerOnReset = true;
            logEvent(EVENT_POWER_ON_RESET_BEGIN);
            m_dataMemory->setBitFast(AVR8RegNames::MCUCSR, AVR8RegNames::MCUCSR_PORF);
            m_interruptController->genIntReq(AVR8InterruptController::INTVEC_RESET);
        }
    }
}

void AVR8SystemControl::timeStep ( float /*timeStep*/,
                                   unsigned int /*clockCycles*/ )
{
// Not ready yet.
//      if ( true == m_config.m_hasPowerOnReset ) {
//              handlePowerOnReset();
//      }
//      if ( true == m_config.m_hasBrownOutReset ) {
//              handleBrownOutReset(timeStep);
//      }
//      if ( true == m_config.m_hasExternalReset ) {
//              handleExternalReset(timeStep);
//      }
//      if ( true == m_config.m_hasWatchDog ) {
//              m_watchdogTimer->timeStep(timeStep, clockCycles);
//      }
}

void AVR8SystemControl::watchDogReset()
{
    if ( true == m_config.m_hasWatchDog )
    {
        m_watchdogTimer->wdr();
    }
}

void AVR8SystemControl::reset ( MCUSimBase::ResetMode mode )
{
    m_watchdogTimer->reset(mode);

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

inline void AVR8SystemControl::mcuReset()
{
    m_brownOutTimer = NAN;
}
