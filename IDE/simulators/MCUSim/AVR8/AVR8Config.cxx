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
 * @file AVR8Config.cxx
 */
// =============================================================================

#include "AVR8Config.h"

AVR8Config::AVR8Config() : MCUSimConfig ( MCUSim::ARCH_AVR8 )
{
    m_isLinked = false;

    m_configIO                      = nullptr;
    m_configBootLoader              = nullptr;
    m_configDataMemory              = nullptr;
    m_configDataEEPROM              = nullptr;
    m_configClockControl            = nullptr;
    m_configInstructionSet          = nullptr;
    m_configProgramMemory           = nullptr;
    m_configFusesAndLocks           = nullptr;
    m_configTimerCounter0           = nullptr;
    m_configTimerCounter1           = nullptr;
    m_configTimerCounter2           = nullptr;
    m_configWatchdogTimer           = nullptr;
    m_configSystemControl           = nullptr;
    m_configExternalInterrupts      = nullptr;
    m_configInterruptController     = nullptr;
    m_configSpi                     = nullptr;
    m_configUsart                   = nullptr;
    m_configTwi                     = nullptr;
    m_configAdc                     = nullptr;
    m_configAcomp                   = nullptr;
    m_configIsp                     = nullptr;
    m_configPprog                   = nullptr;
}

void AVR8Config::link ( AVR8Sim * system )
{
    m_isLinked = true;

    m_configIO                      = & ( system -> m_io                    -> m_config );
    m_configBootLoader              = & ( system -> m_bootLoader            -> m_config );
    m_configDataMemory              = & ( system -> m_dataMemory            -> m_config );
    m_configDataEEPROM              = & ( system -> m_dataEEPROM            -> m_config );
    m_configClockControl            = & ( system -> m_clockControl          -> m_config );
    m_configInstructionSet          = & ( system -> m_instructionSet        -> m_config );
    m_configProgramMemory           = & ( system -> m_programMemory         -> m_config );
    m_configFusesAndLocks           = & ( system -> m_fusesAndLocks         -> m_config );
    m_configTimerCounter0           = & ( system -> m_timerCounter0         -> m_config );
    m_configTimerCounter1           = & ( system -> m_timerCounter1         -> m_config );
    m_configTimerCounter2           = & ( system -> m_timerCounter2         -> m_config );
    m_configWatchdogTimer           = & ( system -> m_watchdogTimer         -> m_config );
    m_configSystemControl           = & ( system -> m_systemControl         -> m_config );
    m_configExternalInterrupts      = & ( system -> m_externalInterrupts    -> m_config );
    m_configInterruptController     = & ( system -> m_interrupts            -> m_config );
    m_configSpi                     = & ( system -> m_spi                   -> m_config );
    m_configUsart                   = & ( system -> m_usart                 -> m_config );
    m_configTwi                     = & ( system -> m_twi                   -> m_config );
    m_configAdc                     = & ( system -> m_adc                   -> m_config );
    m_configAcomp                   = & ( system -> m_acomp                 -> m_config );
    m_configIsp                     = & ( system -> m_isp                   -> m_config );
    m_configPprog                   = & ( system -> m_pprog                 -> m_config );
}
