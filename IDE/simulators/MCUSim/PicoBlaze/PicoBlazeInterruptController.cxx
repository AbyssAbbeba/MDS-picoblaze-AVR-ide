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
 * @file PicoBlazeInterruptController.cxx
 */
// =============================================================================

#include "PicoBlazeInterruptController.h"
#include "PicoBlazeInstructionSet.h"

PicoBlazeInterruptController::PicoBlazeInterruptController()
{
}

PicoBlazeInterruptController::~PicoBlazeInterruptController()
{
}

PicoBlazeInterruptController * PicoBlazeInterruptController::link ( MCUSim::EventLogger      * eventLogger,
                                                                    PicoBlazeInstructionSet  * instructionSet )
{
    Subsys::link(eventLogger, ID_INTERRUPTS);
    m_instructionSet = instructionSet;
    return this;
}

void PicoBlazeInterruptController::reset ( MCUSim::ResetMode mode )
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
    else
    {
        m_instructionSet->irq();
        m_irq = false;
        return 1;
    }
}

void PicoBlazeInterruptController::genIntReq()
{
    logEvent ( EVENT_INT_ENTERING_INTERRUPT );
    m_irq = true;
}

void PicoBlazeInterruptController::returni()
{
    logEvent ( EVENT_INT_LEAVING_INTERRUPT );
}
