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
 * @ingroup PicoBlaze
 * @file PicoBlazeConfig.cxx
 */
// =============================================================================

#include "PicoBlazeConfig.h"
#include "PicoBlazeSim.h"

PicoBlazeConfig::PicoBlazeConfig() : MCUSim::Config ( MCUSim::ARCH_PICOBLAZE )
{
    m_isLinked = false;

    m_configInterruptController = NULL;
    m_configInstructionSet      = NULL;
    m_configProgramMemory       = NULL;
    m_configClockControl        = NULL;
    m_configDataMemory          = NULL;
    m_configRegisters           = NULL;
    m_configStack               = NULL;
    m_configIO                  = NULL;
}

void PicoBlazeConfig::link ( PicoBlazeSim * system )
{
    m_isLinked = true;

    m_configInterruptController = & ( system -> m_interruptController -> m_config );
    m_configInstructionSet      = & ( system -> m_instructionSet      -> m_config );
    m_configProgramMemory       = & ( system -> m_programMemory       -> m_config );
    m_configClockControl        = & ( system -> m_clockControl        -> m_config );
    m_configDataMemory          = & ( system -> m_dataMemory          -> m_config );
    m_configRegisters           = & ( system -> m_registers           -> m_config );
    m_configStack               = & ( system -> m_stack               -> m_config );
    m_configIO                  = & ( system -> m_io                  -> m_config );
}
