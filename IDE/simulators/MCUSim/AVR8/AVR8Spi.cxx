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
 * @file AVR8Spi.cxx
 */
// =============================================================================

#include "AVR8Spi.h"
#include "AVR8DataMemory.h"

AVR8Spi * AVR8Spi::link ( MCUSimEventLogger     * eventLogger,
                          AVR8DataMemory          * dataMemory )
{
    MCUSimSubsys::link(eventLogger, ID_SPI);

    m_dataMemory = dataMemory;

    return this;
}

void AVR8Spi::reset ( MCUSimBase::ResetMode /*mode*/ )
{
}

inline void AVR8Spi::resetToInitialValues()
{
}

inline void AVR8Spi::mcuReset()
{
}
