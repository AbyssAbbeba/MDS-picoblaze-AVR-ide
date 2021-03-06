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
 * @file AVR8Adc.cxx
 */
// =============================================================================

#include "AVR8Adc.h"
#include "AVR8DataMemory.h"

AVR8Adc * AVR8Adc::link ( MCUSimEventLogger     * eventLogger,
                          AVR8DataMemory        * dataMemory )
{
    MCUSimSubsys::link(eventLogger, ID_ADC);
    m_dataMemory = dataMemory;
    return this;
}

void AVR8Adc::reset ( MCUSimBase::ResetMode /*mode*/ )
{
}

inline void AVR8Adc::resetToInitialValues()
{
}

inline void AVR8Adc::mcuReset()
{
}
