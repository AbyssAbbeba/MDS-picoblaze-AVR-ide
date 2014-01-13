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
 * @ingroup PIC8
 * @file PIC8Config.cxx
 */
// =============================================================================

#include "PIC8Config.h"

PIC8Config::PIC8Config() : MCUSimConfig ( MCUSim::ARCH_PIC8 )
{
    m_isLinked = false;

    m_configIO                  = nullptr;
    m_configDataMemory          = nullptr;
    m_configClockControl        = nullptr;
    m_configInstructionSet      = nullptr;
    m_configProgramMemory       = nullptr;
    m_configConfigWord          = nullptr;
    m_configStack               = nullptr;
    m_configInterruptController = nullptr;
    m_configDataEEPROM          = nullptr;
    m_configWatchDogTimer       = nullptr;
    m_configTimerCounter0       = nullptr;
    m_configTimer0WdtPrescaller = nullptr;
    m_configExternalInterrupts  = nullptr;
    m_configISP                 = nullptr;
}

void PIC8Config::link ( PIC8Sim * system )
{
    m_isLinked = true;

    m_configIO                  = & ( system -> m_io                    -> m_config );
    m_configDataMemory          = & ( system -> m_dataMemory            -> m_config );
    m_configClockControl        = & ( system -> m_clockControl          -> m_config );
    m_configInstructionSet      = & ( system -> m_instructionSet        -> m_config );
    m_configProgramMemory       = & ( system -> m_programMemory         -> m_config );
    m_configConfigWord          = & ( system -> m_configWord            -> m_config );
    m_configStack               = & ( system -> m_stack                 -> m_config );
    m_configInterruptController = & ( system -> m_interruptController   -> m_config );
    m_configDataEEPROM          = & ( system -> m_dataEEPROM            -> m_config );
    m_configWatchDogTimer       = & ( system -> m_watchDogTimer         -> m_config );
    m_configTimerCounter0       = & ( system -> m_timerCounter0         -> m_config );
    m_configTimer0WdtPrescaller = & ( system -> m_timer0WdtPrescaller   -> m_config );
    m_configExternalInterrupts  = & ( system -> m_externalInterrupts    -> m_config );
    m_configISP                 = & ( system -> m_isp                   -> m_config );
}
