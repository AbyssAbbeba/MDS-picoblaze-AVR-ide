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
 * @file AVR8Usart.cxx
 */
// =============================================================================

#include "AVR8Usart.h"
#include "AVR8DataMemory.h"

AVR8Usart * AVR8Usart::link ( MCUSimEventLogger * eventLogger,
                              AVR8DataMemory      * dataMemory )
{
    MCUSimSubsys::link(eventLogger, ID_USART);
    m_dataMemory = dataMemory;
    return this;
}

void AVR8Usart::reset ( MCUSimBase::ResetMode /*mode*/ )
{
}

inline void AVR8Usart::resetToInitialValues()
{
}

inline void AVR8Usart::mcuReset()
{
}
