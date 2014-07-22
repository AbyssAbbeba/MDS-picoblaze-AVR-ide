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
 * @file AdaptableSimClockControl.cxx
 */
// =============================================================================

#include "AdaptableSimClockControl.h"

AdaptableSimClockControl * AdaptableSimClockControl::link ( MCUSimEventLogger * eventLogger )
{
    MCUSimClock::link(eventLogger);
    return this;
}

void AdaptableSimClockControl::reset ( MCUSimBase::ResetMode )
{
}

float AdaptableSimClockControl::getFrequency()
{
    if ( ClockSource::TYPE_EXTERNAL != m_clockSource.getType() )
    {
        logEvent ( MCUSimEventLogger::FLAG_HI_PRIO, EVENT_CLK_ERR_INCOMPATIBLE_SOURCE );
        return 0;
    }

    return m_clockSource.getFrequency();
}
