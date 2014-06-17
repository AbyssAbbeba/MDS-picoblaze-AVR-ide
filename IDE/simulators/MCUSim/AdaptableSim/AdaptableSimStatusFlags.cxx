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
 * @file AdaptableSimStatusFlags.cxx
 */
// =============================================================================

#include "AdaptableSimStatusFlags.h"

AdaptableSimStatusFlags::AdaptableSimStatusFlags()
{
    reset(MCUSim::RSTMD_MCU_RESET);
}

void AdaptableSimStatusFlags::reset ( MCUSimBase::ResetMode mode )
{
    if ( MCUSim::RSTMD_MCU_RESET == mode )
    {
        m_carry       = false;
        m_zero        = false;
        m_inte        = false;
        m_preCarry    = false;
        m_preZero     = false;
        m_interrupted = 0;
    }
}

AdaptableSimStatusFlags * AdaptableSimStatusFlags::link ( MCUSimEventLogger * eventLogger )
{
    MCUSimSubsys::link(eventLogger, ID_FLAGS);
    return this;
}

void AdaptableSimStatusFlags::interrupt()
{
    m_preCarry = m_carry;
    m_preZero  = m_zero;
    m_inte     = false;
    m_interrupted++;
}

void AdaptableSimStatusFlags::returni()
{
    m_carry = m_preCarry;
    m_zero  = m_preZero;
    m_interrupted--;
}
