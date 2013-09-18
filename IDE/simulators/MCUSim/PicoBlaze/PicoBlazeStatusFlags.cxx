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
 * @file PicoBlazeStatusFlags.cxx
 */
// =============================================================================

#include "PicoBlazeStatusFlags.h"

PicoBlazeStatusFlags::PicoBlazeStatusFlags()
{
    reset(MCUSim::RSTMD_MCU_RESET);
}

void PicoBlazeStatusFlags::reset ( MCUSimBase::ResetMode mode )
{
    if ( MCUSim::RSTMD_MCU_RESET == mode )
    {
std::cout << "PicoBlazeStatusFlags::reset\n";
        m_carry       = false;
        m_zero        = false;
        m_inte        = false;
        m_preCarry    = false;
        m_preZero     = false;
        m_interrupted = 0;
    }
}

PicoBlazeStatusFlags * PicoBlazeStatusFlags::link ( MCUSimEventLogger * eventLogger )
{
    MCUSimSubsys::link(eventLogger, ID_FLAGS);
    return this;
}

void PicoBlazeStatusFlags::interrupt()
{
    m_preCarry = m_carry;
    m_preZero  = m_zero;
    m_inte     = false;
    m_interrupted++;
}

void PicoBlazeStatusFlags::returni()
{
    m_carry = m_preCarry;
    m_zero  = m_preZero;
    m_interrupted--;
}
