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
 * @file AVR8Usart.cxx
 */
// =============================================================================

#include "AVR8Usart.h"
#include "AVR8DataMemory.h"

AVR8Usart * AVR8Usart::link ( MCUSim::EventLogger * eventLogger,
                              AVR8DataMemory      * dataMemory )
{
    Subsys::link(eventLogger, ID_USART);
    m_dataMemory = dataMemory;
    return this;
}

void AVR8Usart::reset ( MCUSim::ResetMode /*mode*/ )
{
}

inline void AVR8Usart::resetToInitialValues()
{
}

inline void AVR8Usart::mcuReset()
{
}
