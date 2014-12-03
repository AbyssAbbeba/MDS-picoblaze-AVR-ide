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
 * @file PicoBlazeInterruptController.cxx
 */
// =============================================================================

#include "PicoBlazeInterruptController.h"
#include "PicoBlazeRegisters.h"

PicoBlazeInterruptController::PicoBlazeInterruptController()
{
}

PicoBlazeInterruptController::~PicoBlazeInterruptController()
{
}

PicoBlazeInterruptController * PicoBlazeInterruptController::link ( MCUSimEventLogger        * eventLogger,
                                                                    PicoBlazeInstructionSet  * instructionSet,
                                                                    PicoBlazeStatusFlags     * statusFlags,
                                                                    PicoBlazeRegisters       * registers )
{
    MCUSimSubsys::link(eventLogger, ID_INTERRUPTS);

    m_instructionSet = instructionSet;
    m_statusFlags = statusFlags;
    m_registers = registers;

    return this;
}

void PicoBlazeInterruptController::reset ( MCUSimBase::ResetMode mode )
{
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

inline void PicoBlazeInterruptController::mcuReset()
{
    m_irq = false;
}

void PicoBlazeInterruptController::irq()
{
    m_irq = true;
}

void PicoBlazeInterruptController::returni()
{
    m_statusFlags->returni();
    m_registers->setBank(m_regBank);
    logEvent ( EVENT_INT_LEAVING_INTERRUPT );
}
