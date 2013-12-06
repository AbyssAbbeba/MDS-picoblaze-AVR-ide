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

#include "PicoBlazeStatusFlags.h"
#include "PicoBlazeInstructionSet.h"

PicoBlazeInterruptController::PicoBlazeInterruptController()
{
}

PicoBlazeInterruptController::~PicoBlazeInterruptController()
{
}

PicoBlazeInterruptController * PicoBlazeInterruptController::link ( MCUSimEventLogger        * eventLogger,
                                                                    PicoBlazeInstructionSet  * instructionSet,
                                                                    PicoBlazeStatusFlags     * statusFlags )
{
    MCUSimSubsys::link(eventLogger, ID_INTERRUPTS);
    m_instructionSet = instructionSet;
    m_statusFlags = statusFlags;
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

int PicoBlazeInterruptController::autoInterrupt()
{
    if ( false == m_irq )
    {
        return 0;
    }
    else if ( true == m_statusFlags->getInte() )
    {
        m_irq = false;
        m_instructionSet->irq();
        logEvent ( EVENT_INT_ENTERING_INTERRUPT );
        return 1;
    }
    else
    {
        m_irq = false;
        logEvent ( EVENT_INT_IRQ_DENIED );
        return 0;
    }
}

void PicoBlazeInterruptController::irq()
{
    m_irq = true;
}

void PicoBlazeInterruptController::returni()
{
    logEvent ( EVENT_INT_LEAVING_INTERRUPT );
}
