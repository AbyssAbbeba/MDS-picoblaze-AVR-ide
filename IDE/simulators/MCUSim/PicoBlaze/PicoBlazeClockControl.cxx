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
 * @file PicoBlazeClockControl.cxx
 */
// =============================================================================

#include "PicoBlazeClockControl.h"

PicoBlazeClockControl * PicoBlazeClockControl::link ( MCUSim::EventLogger * eventLogger )
{
    Clock::link(eventLogger);
    return this;
}

void PicoBlazeClockControl::reset ( MCUSim::ResetMode )
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
