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
 * @file PicoBlazeClockControl.cxx
 */
// =============================================================================

#include "PicoBlazeClockControl.h"

PicoBlazeClockControl * PicoBlazeClockControl::link ( MCUSimEventLogger * eventLogger )
{
    MCUSimClock::link(eventLogger);
    return this;
}

void PicoBlazeClockControl::reset ( MCUSimBase::ResetMode )
{
}

float PicoBlazeClockControl::getFrequency()
{
    if ( ClockSource::TYPE_EXTERNAL != m_clockSource.getType() )
    {
        logEvent ( EVENT_CLK_ERR_INCOMPATIBLE_SOURCE );
        return 0;
    }

    return m_clockSource.getFrequency();
}
