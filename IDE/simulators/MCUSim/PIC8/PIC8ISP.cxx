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
 * @file PIC8ISP.cxx
 */
// =============================================================================

#include "PIC8ISP.h"

#include "PIC8ProgramMemory.h"
#include "PIC8IO.h"
#include "PIC8ConfigWord.h"

PIC8ISP * PIC8ISP::link ( MCUSim::EventLogger * eventLogger,
                          PIC8ProgramMemory   * programMemory,
                          PIC8IO              * io,
                          PIC8ConfigWord      * configWord )
{
    Subsys::link(eventLogger, ID_ISP);
    m_programMemory = programMemory;
    m_io = io;
    m_configWord = configWord;
    return this;
}
