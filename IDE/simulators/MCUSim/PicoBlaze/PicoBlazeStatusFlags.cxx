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

void PicoBlazeStatusFlags::eventToString ( std::ostream & out,
                                           int eventId,
                                           int locationOrReason,
                                           int detail )
{
    switch ( Event(eventId) )
    {
        case EVENT_FLAGS_Z_CHANGED:
            out << ">>> flags_z_changed : " << detail << std::endl;
            return;
        case EVENT_FLAGS_C_CHANGED:
            out << ">>> flags_c_changed : " << detail << std::endl;
            return;
        case EVENT_FLAGS_PZ_CHANGED:
            out << ">>> flags_pz_changed : " << detail << std::endl;
            return;
        case EVENT_FLAGS_PC_CHANGED:
            out << ">>> flags_pc_changed : " << detail << std::endl;
            return;
        case EVENT_FLAGS_IE_CHANGED:
            out << ">>> flags_ie_changed : " << detail << std::endl;
            return;
        case EVENT_FLAGS_INT_CHANGED:
            out << ">>> flags_int_changed : " << detail << std::endl;
            return;

        case EVENT_FLAGS__MAX__:
            break;
    }

    MCUSimSubsys::eventToString(out, eventId, locationOrReason, detail);
}
