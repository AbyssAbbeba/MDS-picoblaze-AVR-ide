// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2015 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup SimCmdLine
 * @file SimCmdLineObserver.cxx
 */
// =============================================================================

#include "SimCmdLineObserver.h"

// Standard headers.
#include <iostream>

// Simulator control unit.
#include "../SimControl/MCUSimControl.h"

// Simulator subsystems.
#include "MCUSimCPU.h"

void SimCmdLineObserver::handleEvent ( int subsysId,
                                       int eventId,
                                       int locationOrReason,
                                       int detail )
{
    MCUSimSubsys * subsys = m_simControlUnit->getSimSubsys ( MCUSimSubsys::SubsysId(subsysId) );
    if ( nullptr != subsys )
    {
        subsys->eventToString(std::cout, eventId, locationOrReason, detail);
    }

    if ( ( MCUSimSubsys::ID_CPU == subsysId ) && ( MCUSimCPU::EVENT_CPU_PC_CHANGED == eventId ) )
    {
        std::cout << ">>> cycles " << m_simControlUnit->getTotalMCycles() << std::endl;
    }
}
