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
 * @file PIC8Config.cxx
 */
// =============================================================================

#include "PIC8Config.h"

PIC8Config::PIC8Config() : MCUSim::Config ( MCUSim::ARCH_PIC8 )
{
    m_isLinked = false;

    m_configIO                  = NULL;
    m_configDataMemory          = NULL;
    m_configClockControl        = NULL;
    m_configInstructionSet      = NULL;
    m_configProgramMemory       = NULL;
    m_configConfigWord          = NULL;
    m_configStack               = NULL;
    m_configInterruptController = NULL;
    m_configDataEEPROM          = NULL;
    m_configWatchDogTimer       = NULL;
    m_configTimerCounter0       = NULL;
    m_configTimer0WdtPrescaller = NULL;
    m_configExternalInterrupts  = NULL;
    m_configISP                 = NULL;
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
