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
 * @ingroup AVR8
 * @file AVR8TimerCounter2.cxx
 */
// =============================================================================

#include "AVR8TimerCounter2.h"
#include "AVR8DataMemory.h"

AVR8TimerCounter2 * AVR8TimerCounter2::link ( MCUSimEventLogger     * eventLogger,
                                              MCUSimSubsys::SubsysId  subsysId,
                                              AVR8DataMemory          * dataMemory )
{
    MCUSimSubsys::link(eventLogger, subsysId);
    m_dataMemory = dataMemory;
    return this;
}

void AVR8TimerCounter2::reset ( MCUSimBase::ResetMode /*mode*/ )
{
}

inline void AVR8TimerCounter2::resetToInitialValues()
{
}

inline void AVR8TimerCounter2::mcuReset()
{
}
