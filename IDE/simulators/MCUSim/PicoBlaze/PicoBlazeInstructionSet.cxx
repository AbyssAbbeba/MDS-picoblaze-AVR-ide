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
 * @ingroup PicoBlaze
 * @file PicoBlazeInstructionSet.cxx
 */
// =============================================================================

#include "PicoBlazeInstructionSet.h"

#include "PicoBlazeStack.h"

PicoBlazeInstructionSet * PicoBlazeInstructionSet::link ( MCUSimEventLogger            * eventLogger,
                                                          PicoBlazeIO                  * io,
                                                          PicoBlazeStack               * stack,
                                                          PicoBlazeRegisters           * registers,
                                                          PicoBlazeDataMemory          * dataMemory,
                                                          PicoBlazeStatusFlags         * statusFlags,
                                                          PicoBlazeProgramMemory       * programMemory,
                                                          PicoBlazeInterruptController * interruptController )
{
    MCUSimCPU::link(eventLogger);

    m_io                  = io;
    m_stack               = stack;
    m_registers           = registers;
    m_dataMemory          = dataMemory;
    m_statusFlags         = statusFlags;
    m_programMemory       = programMemory;
    m_interruptController = interruptController;

    return this;
}

void PicoBlazeInstructionSet::adapt ( const PicoBlazeInstructionSet * obj )
{
    link ( obj->m_eventLogger,
           obj->m_io,
           obj->m_stack,
           obj->m_registers,
           obj->m_dataMemory,
           obj->m_statusFlags,
           obj->m_programMemory,
           obj->m_interruptController);

    m_pc = obj->m_pc;
    m_config = obj->m_config;
    m_lastInstruction = obj->m_lastInstruction;
    m_actSubprogCounter = obj->m_actSubprogCounter;

    for ( int i = 0; i < PicoBlazeInsNames::INS__MAX__; i++ )
    {
        m_instructionCounter[i] = obj->m_instructionCounter[i];
    }
}

void PicoBlazeInstructionSet::reset ( MCUSimBase::ResetMode mode )
{
    switch ( mode )
    {
        case MCUSim::RSTMD_INITIAL_VALUES:
            resetToInitialValues();
            break;
        case MCUSim::RSTMD_MCU_RESET:
            mcuReset();
            break;
        default:
            // Irrelevant requests are silently ignored
            break;
    }
}

inline void PicoBlazeInstructionSet::mcuReset()
{
    m_pc = 0;
    m_actSubprogCounter = 0;
    m_lastInstruction = PicoBlazeInsNames::INS_NONE;
    logEvent(EVENT_CPU_PC_CHANGED, m_pc);
}

inline void PicoBlazeInstructionSet::resetToInitialValues()
{
    mcuReset();

    for ( int i = 0; i < PicoBlazeInsNames::INS__MAX__; i++ )
    {
        m_instructionCounter[i] = 0;
    }
}

void PicoBlazeInstructionSet::irq()
{
    if ( m_statusFlags->getInterrupted() > 0 )
    {
        logEvent ( MCUSimEventLogger::FLAG_HI_PRIO, EVENT_CPU_WRN_INVALID_IRQ, m_pc );
    }

    logEvent ( EVENT_CPU_IRQ, m_pc );
    m_stack -> pushOnStack ( m_pc );
    m_pc = m_config.m_interruptVector;
    m_statusFlags->interrupt();
}
