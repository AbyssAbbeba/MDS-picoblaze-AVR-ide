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
 * @file AdaptableSimConfig.cxx
 */
// =============================================================================

#include "AdaptableSimConfig.h"
#include "AdaptableSimSim.h"

AdaptableSimConfig::AdaptableSimConfig() : MCUSimConfig ( MCUSim::ARCH_ADAPTABLE )
{
    m_isLinked = false;

    m_configInterruptController = nullptr;
    m_configInstructionSet      = nullptr;
    m_configProgramMemory       = nullptr;
    m_configClockControl        = nullptr;
    m_configStatusFlags         = nullptr;
    m_configDataMemory          = nullptr;
    m_configRegisters           = nullptr;
    m_configStack               = nullptr;
    m_configIO                  = nullptr;
}

void AdaptableSimConfig::link ( AdaptableSimSim * system )
{
    m_isLinked = true;

    m_configInterruptController = & ( system -> m_interruptController -> m_config );
    m_configInstructionSet      = & ( system -> m_instructionSet      -> m_config );
    m_configProgramMemory       = & ( system -> m_programMemory       -> m_config );
    m_configClockControl        = & ( system -> m_clockControl        -> m_config );
    m_configStatusFlags         = & ( system -> m_statusFlags         -> m_config );
    m_configDataMemory          = & ( system -> m_dataMemory          -> m_config );
    m_configRegisters           = & ( system -> m_registers           -> m_config );
    m_configStack               = & ( system -> m_stack               -> m_config );
    m_configIO                  = & ( system -> m_io                  -> m_config );
}
