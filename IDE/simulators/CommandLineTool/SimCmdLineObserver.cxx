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
    switch ( subsysId )
    {
        case MCUSimSubsys::ID_CPU:
            switch ( eventId )
            {
                case MCUSimCPU::EVENT_CPU_PC_CHANGED:
                    if ( true == m_trackTime )
                    {
                        std::cout << m_simControlUnit->getTotalMCycles() << ' ' << locationOrReason << std::endl;
                    }
                    break;
            }
            break;
    }
}
