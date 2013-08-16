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
    reset();
}

void PicoBlazeStatusFlags::reset()
{
    m_carry       = false;
    m_zero        = false;
    m_inte        = false;
    m_preCarry    = false;
    m_preZero     = false;
    m_interrupted = 0;
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
