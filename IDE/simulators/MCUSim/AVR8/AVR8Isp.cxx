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
 * @file AVR8Isp.cxx
 */
// =============================================================================

#include "AVR8Isp.h"
#include "AVR8ProgramMemory.h"

AVR8Isp * AVR8Isp::link ( MCUSimEventLogger     * eventLogger,
                          AVR8ProgramMemory       * programMemory )
{
    MCUSimSubsys::link(eventLogger, ID_ISP);
    m_programMemory = programMemory;
    return this;
}

void AVR8Isp::reset ( MCUSimBase::ResetMode /*mode*/ )
{
}

inline void AVR8Isp::resetToInitialValues()
{
}

inline void AVR8Isp::mcuReset()
{
}
