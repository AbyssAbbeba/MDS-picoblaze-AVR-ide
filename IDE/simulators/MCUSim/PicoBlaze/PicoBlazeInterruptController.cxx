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

void PicoBlazeInterruptController::eventToString ( std::ostream & out,
                                                   int eventId,
                                                   int locationOrReason,
                                                   int detail )
{
    switch ( Event(eventId) )
    {
        case EVENT_INT_IRQ_DENIED:
            out << ">>> int_irq_denied" << std::endl;
            return;
        case EVENT_INT_ENTERING_INTERRUPT:
            out << ">>> int_entering_interrupt" << std::endl;
            return;
        case EVENT_INT_LEAVING_INTERRUPT:
            out << ">>> int_leaving_interrupt" << std::endl;
            return;

        case EVENT_INT__MAX__:
            break;
    }

    MCUSimSubsys::eventToString(out, eventId, locationOrReason, detail);
}
