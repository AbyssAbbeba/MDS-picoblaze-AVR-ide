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
    setPreZero(getZero());
    setPreCarry(getCarry());
    setInte(false);
    setInterrupted(getInterrupted() + 1);
}

void PicoBlazeStatusFlags::returni()
{
    setZero(getPreZero());
    setCarry(getPreCarry());
    setInterrupted(getInterrupted() - 1);
}
