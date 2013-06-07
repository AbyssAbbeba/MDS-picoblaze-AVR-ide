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
 * @file AVR8ParallelProg.cxx
 */
// =============================================================================

#include "AVR8ParallelProg.h"
#include "AVR8ProgramMemory.h"

AVR8ParallelProg * AVR8ParallelProg::link ( MCUSim::EventLogger     * eventLogger,
                                            AVR8ProgramMemory       * programMemory )
{
    Subsys::link(eventLogger, ID_PPROG);
    m_programMemory = programMemory;
    return this;
}

void AVR8ParallelProg::reset ( MCUSim::ResetMode /*mode*/ )
{
}

inline void AVR8ParallelProg::resetToInitialValues()
{
}

inline void AVR8ParallelProg::mcuReset()
{
}
