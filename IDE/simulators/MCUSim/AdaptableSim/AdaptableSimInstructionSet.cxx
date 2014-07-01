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
 * @file AdaptableSimInstructionSet.cxx
 */
// =============================================================================

#include "AdaptableSimInstructionSet.h"

#include "AdaptableSimStack.h"

AdaptableSimInstructionSet * AdaptableSimInstructionSet::link ( MCUSimEventLogger               * eventLogger,
                                                                AdaptableSimIO                  * io,
                                                                AdaptableSimStack               * stack,
                                                                AdaptableSimRegisters           * registers,
                                                                AdaptableSimDataMemory          * dataMemory,
                                                                AdaptableSimStatusFlags         * statusFlags,
                                                                AdaptableSimProgramMemory       * programMemory,
                                                                AdaptableSimInterruptController * interruptController )
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

void AdaptableSimInstructionSet::reset ( MCUSimBase::ResetMode mode )
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

inline void AdaptableSimInstructionSet::mcuReset()
{
    m_pc = 0;
    m_actSubprogCounter = 0;
    m_lastInstruction = AdaptableSimInsNames::INS_NONE;
    logEvent(EVENT_CPU_PC_CHANGED, m_pc);
}

inline void AdaptableSimInstructionSet::resetToInitialValues()
{
    mcuReset();

    for ( int i = 0; i < AdaptableSimInsNames::INS__MAX__; i++ )
    {
        m_instructionCounter[i] = 0;
    }
}

void AdaptableSimInstructionSet::irq()
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

int AdaptableSimInstructionSet::execInstruction()
{
    const int pcOrig = m_pc;
    unsigned int opCode = m_programMemory->readRaw(m_pc);

    if ( opCode & 0xffff0000 )
    {
        logEvent ( MCUSimEventLogger::FLAG_HI_PRIO, EVENT_CPU_ERR_UNDEFINED_OPCODE, m_pc );

        if ( true == m_config.m_ignoreUndefinedOpCodes )
        {
            incrPc();
            return -1;
        }
        else
        {
            opCode &= 0x0ffff;
        }
    }

    incrPc();

    // Execute instruction from dispatch table.
    ( this ->* ( m_opCodeDispatchTable [ opCode >> 12 ] ) ) ( opCode );

    if ( pcOrig != m_pc )
    {
        logEvent ( EVENT_CPU_PC_CHANGED, m_pc );
    }

    return 1;
}
