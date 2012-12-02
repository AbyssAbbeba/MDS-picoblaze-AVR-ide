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
 * @file AVR8Spi.cxx
 */
// =============================================================================

#include "AVR8Spi.h"
#include "AVR8DataMemory.h"

AVR8Spi * AVR8Spi::link ( MCUSim::EventLogger     * eventLogger,
                          AVR8DataMemory          * dataMemory )
{
    Subsys::link(eventLogger, ID_SPI);

    m_dataMemory = dataMemory;

    return this;
}

void AVR8Spi::reset ( MCUSim::ResetMode /*mode*/ )
{
}

inline void AVR8Spi::resetToInitialValues()
{
}

inline void AVR8Spi::mcuReset()
{
}
