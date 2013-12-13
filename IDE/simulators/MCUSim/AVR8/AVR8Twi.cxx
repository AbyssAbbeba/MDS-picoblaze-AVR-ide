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
 * @file AVR8Twi.cxx
 */
// =============================================================================

#include "AVR8Twi.h"
#include "AVR8DataMemory.h"

AVR8Twi * AVR8Twi::link ( MCUSimEventLogger * eventLogger,
                          AVR8DataMemory      * dataMemory )
{
    MCUSimSubsys::link(eventLogger, ID_TWI);
    m_dataMemory = dataMemory;
    return this;
}

void AVR8Twi::reset ( MCUSimBase::ResetMode /*mode*/ )
{
}

inline void AVR8Twi::resetToInitialValues()
{
}

inline void AVR8Twi::mcuReset()
{
}
