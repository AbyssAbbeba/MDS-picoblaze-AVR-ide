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
 * @file AVR8Adc.cxx
 */
// =============================================================================

#include "AVR8Adc.h"
#include "AVR8DataMemory.h"

AVR8Adc * AVR8Adc::link ( MCUSim::EventLogger     * eventLogger,
                          AVR8DataMemory          * dataMemory )
{
    Subsys::link(eventLogger, ID_ADC);
    m_dataMemory = dataMemory;
    return this;
}

void AVR8Adc::reset ( MCUSim::ResetMode /*mode*/ )
{
}

inline void AVR8Adc::resetToInitialValues()
{
}

inline void AVR8Adc::mcuReset()
{
}
