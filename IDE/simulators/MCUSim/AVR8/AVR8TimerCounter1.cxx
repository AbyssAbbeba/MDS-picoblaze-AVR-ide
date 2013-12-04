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
 * @ingroup AVR8
 * @file AVR8TimerCounter1.cxx
 */
// =============================================================================

#include "AVR8TimerCounter1.h"
#include "AVR8DataMemory.h"

AVR8TimerCounter1 * AVR8TimerCounter1::link ( MCUSimEventLogger     * eventLogger,
                                              MCUSimSubsys::SubsysId  subsysId,
                                              AVR8DataMemory          * dataMemory )
{
    MCUSimSubsys::link(eventLogger, subsysId);
    m_dataMemory = dataMemory;
    return this;
}

void AVR8TimerCounter1::reset ( MCUSimBase::ResetMode mode )
{
}

inline void AVR8TimerCounter1::resetToInitialValues()
{
}

inline void AVR8TimerCounter1::mcuReset()
{
}
