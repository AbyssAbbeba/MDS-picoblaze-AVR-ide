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
 * @ingroup PicoBlaze
 * @file PicoBlazeConfig.cxx
 */
// =============================================================================

#include "PicoBlazeConfig.h"
#include "PicoBlazeSim.h"

PicoBlazeConfig::PicoBlazeConfig() : MCUSimConfig ( MCUSim::ARCH_PICOBLAZE )
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

void PicoBlazeConfig::configure ( MCUSim::Family dev )
{
    m_family = dev;

    switch ( dev )
    {
        case MCUSim::FAMILY_KCPSM2:
            m_name = "kcpsm2";
            m_configDataMemory->m_size = 0;
            m_configInstructionSet->m_dev = MCUSim::FAMILY_KCPSM2;
            m_configInstructionSet->m_interruptVector = 0x3ff;
            m_configInstructionSet->m_pcMax = 1024;
            m_configInstructionSet->m_hwbuild = 0;
            m_configProgramMemory->m_size = 1024;
            m_configRegisters->m_size = 32;
            m_configRegisters->m_banks = false;
            m_configStack->m_size = 31;
            break;
        case MCUSim::FAMILY_KCPSM3:
            m_name = "kcpsm3";
            m_configDataMemory->m_size = 64;
            m_configInstructionSet->m_dev = MCUSim::FAMILY_KCPSM3;
            m_configInstructionSet->m_interruptVector = 0x3ff;
            m_configInstructionSet->m_pcMax = 1024;
            m_configInstructionSet->m_hwbuild = 0;
            m_configProgramMemory->m_size = 1024;
            m_configRegisters->m_size = 16;
            m_configRegisters->m_banks = false;
            m_configStack->m_size = 31;
            break;
        case MCUSim::FAMILY_KCPSM6:
            m_name = "kcpsm6";
            m_configDataMemory->m_size = 256;
            m_configInstructionSet->m_dev = MCUSim::FAMILY_KCPSM6;
            m_configInstructionSet->m_interruptVector = 4095;
            m_configInstructionSet->m_pcMax = 4096;
            m_configInstructionSet->m_hwbuild = 0;
            m_configProgramMemory->m_size = 4096;
            m_configRegisters->m_size = 32;
            m_configRegisters->m_banks = true;
            m_configStack->m_size = 30;
            break;
        default:
            break;
    }
}
