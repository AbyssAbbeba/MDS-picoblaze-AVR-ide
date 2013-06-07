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
 * @file AVR8AnalogComparator.cxx
 */
// =============================================================================

#include "AVR8AnalogComparator.h"
#include "AVR8DataMemory.h"

AVR8AnalogComparator * AVR8AnalogComparator::link ( MCUSim::EventLogger     * eventLogger,
                                                    AVR8DataMemory          * dataMemory )
{
    Subsys::link(eventLogger, ID_ACOMP);
    m_dataMemory = dataMemory;
    return this;
}

void AVR8AnalogComparator::reset ( MCUSim::ResetMode /*mode*/ )
{
}

inline void AVR8AnalogComparator::resetToInitialValues()
{
}

inline void AVR8AnalogComparator::mcuReset()
{
}
