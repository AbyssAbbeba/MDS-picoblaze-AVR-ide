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
 * @file AdaptableSimInterruptController.cxx
 */
// =============================================================================

#include "AdaptableSimInterruptController.h"

AdaptableSimInterruptController::AdaptableSimInterruptController()
{
}

AdaptableSimInterruptController::~AdaptableSimInterruptController()
{
}

AdaptableSimInterruptController * AdaptableSimInterruptController::link ( MCUSimEventLogger           * eventLogger,
                                                                          AdaptableSimInstructionSet  * instructionSet,
                                                                          AdaptableSimStatusFlags     * statusFlags )
{
    MCUSimSubsys::link(eventLogger, ID_INTERRUPTS);
    m_instructionSet = instructionSet;
    m_statusFlags = statusFlags;
    return this;
}

void AdaptableSimInterruptController::reset ( MCUSimBase::ResetMode mode )
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

inline void AdaptableSimInterruptController::mcuReset()
{
    m_irq = false;
}

void AdaptableSimInterruptController::irq()
{
    m_irq = true;
}

void AdaptableSimInterruptController::returni()
{
    logEvent ( EVENT_INT_LEAVING_INTERRUPT );
}
