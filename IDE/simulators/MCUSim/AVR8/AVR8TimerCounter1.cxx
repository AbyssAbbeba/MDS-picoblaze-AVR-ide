// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2012 Moravia Microsystems, s.r.o.
 *
 * @authors Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup AVR8
 * @file AVR8TimerCounter1.cxx
 */
// =============================================================================

#include "AVR8TimerCounter1.h"
#include "AVR8DataMemory.h"

AVR8TimerCounter1 * AVR8TimerCounter1::link ( MCUSim::EventLogger     * eventLogger,
                                              MCUSim::Subsys::SubsysId  subsysId,
                                              AVR8DataMemory          * dataMemory )
{
    Subsys::link(eventLogger, subsysId);
    m_dataMemory = dataMemory;
    return this;
}

void AVR8TimerCounter1::reset(MCUSim::ResetMode mode)
{
}

inline void AVR8TimerCounter1::resetToInitialValues()
{
}

inline void AVR8TimerCounter1::mcuReset()
{
}
