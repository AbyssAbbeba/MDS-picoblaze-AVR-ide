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
 * @ingroup PIC8
 * @file PIC8ExternalInterrupts.cxx
 */
// =============================================================================

#include "PIC8ExternalInterrupts.h"

#include "PIC8DataMemory.h"
#include "PIC8IO.h"

PIC8ExternalInterrupts * PIC8ExternalInterrupts::link ( MCUSim::EventLogger * eventLogger,
                                                        PIC8DataMemory      * dataMemory,
                                                        PIC8IO              * io)
{
    Subsys::link(eventLogger, ID_EXT_INT);
    m_dataMemory = dataMemory;
    m_io = io;
    return this;
}
