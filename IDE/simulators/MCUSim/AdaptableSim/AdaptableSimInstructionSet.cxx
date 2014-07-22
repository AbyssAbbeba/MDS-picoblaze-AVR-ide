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
#include "AdaptableSimProgramMemory.h"

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
        case MCUSim::RSTMD_NEW_CONFIG:
            newConfig();
            break;
        default:
            // Irrelevant requests are silently ignored
            break;
    }
}

inline void AdaptableSimInstructionSet::newConfig()
{
    size_t numberOfInstr = 0;

    for ( const auto i : m_config.m_instructions )
    {
        if ( i.m_id > numberOfInstr )
        {
            numberOfInstr = (size_t) i.m_id;
        }
    }

    m_instructionCounter.assign(numberOfInstr, 0);
}

inline void AdaptableSimInstructionSet::mcuReset()
{
    m_pc = 0;
    m_actSubprogCounter = 0;
    m_lastInstruction = -1;
    logEvent(EVENT_CPU_PC_CHANGED, m_pc);
}

inline void AdaptableSimInstructionSet::resetToInitialValues()
{
    mcuReset();

    for ( auto & i : m_instructionCounter )
    {
        i = 0;
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
    unsigned int time = 0;
    const int pcOrig = m_pc;
    unsigned int opCode = m_programMemory->readRaw(m_pc);

    if ( opCode & 0xff000000 )
    {
        logEvent ( MCUSimEventLogger::FLAG_HI_PRIO, EVENT_CPU_ERR_UNDEFINED_OPCODE, m_pc );

        if ( true == m_config.m_ignoreUndefinedOpCodes )
        {
            incrPc();
            return -1;
        }
        else
        {
            opCode &= 0x00ffffff;
        }
    }

    incrPc();

    // Execute instruction.
    for ( const AdaptableSimInstruction & inst : m_config.m_instructions )
    {
        if ( inst.m_code == ( inst.m_mask & opCode ) )
        {
            performOP(inst, time, opCode);

            if ( pcOrig != m_pc )
            {
                logEvent ( EVENT_CPU_PC_CHANGED, m_pc );
            }

            return time;
        }
    }

    logEvent ( EVENT_CPU_PC_CHANGED, m_pc );
    logEvent ( MCUSimEventLogger::FLAG_HI_PRIO, EVENT_CPU_ERR_INVALID_OPCODE, m_pc, opCode );
    return -1;
}

void AdaptableSimInstructionSet::performOP ( const AdaptableSimInstruction & inst,
                                             unsigned int & time,
                                             unsigned int opCode )
{
    unsigned int operands [ AdaptableSimInstruction::OPERANDS_MAX ];
    for ( unsigned int operandNo = 0;
            operandNo < AdaptableSimInstruction::OPERANDS_MAX;
            ++operandNo )
    {
        if ( true == inst.m_fixedOperands.use(operandNo) )
        {
            operands[operandNo] = inst.m_fixedOperands.value(operandNo);
        }
        else
        {
            operands[operandNo] = 0;
            unsigned int bitNumber = 0;
            for ( const unsigned char permutation : inst.m_permutation [ operandNo ] )
            {
                operands[operandNo] |= ( ( opCode & ( 1 << permutation ) ) >> permutation ) << bitNumber;
                bitNumber++;
            }
        }
    }

    bool condPositive = operationSwitch ( inst.m_operation, operands, inst );

    m_lastInstruction = (int) inst.m_id;
    m_instructionCounter[m_lastInstruction]++;

    if ( true == condPositive )
    {
        // Operation condition - POSITIVE.
        time += inst.m_timeP;
        if ( true == inst.m_parameters.nextP() )
        {
            performOP(inst.m_nextP, time, opCode);
        }
    }
    else
    {
        // Operation condition - NEGATIVE.
        time += inst.m_timeN;
        if ( true == inst.m_parameters.nextN() )
        {
            performOP(inst.m_nextN, time, opCode);
        }
    }
}

void AdaptableSimInstructionSet::performOP ( unsigned short id,
                                             unsigned int & time,
                                             unsigned int opCode )
{
    for ( const AdaptableSimInstruction & inst : m_config.m_instructions )
    {
        if ( inst.m_id == id )
        {
            performOP(inst, time, opCode);
            return;
        }
    }
}
