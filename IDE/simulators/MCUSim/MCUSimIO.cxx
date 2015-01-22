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
 * @ingroup PicoBlaze
 * @file MCUSimIO.cxx
 */
// =============================================================================

#include "MCUSimIO.h"

void MCUSimIO::eventToString ( std::ostream & out,
                               int eventId,
                               int locationOrReason,
                               int detail )
{
    switch ( Event(eventId) )
    {
        case EVENT_IO_INDETERMINABLE_LOG:
            out << ">>> io_indeterminable_log @ " << locationOrReason << std::endl;
            return;
        case EVENT_IO__MAX__:
            break;
    }

    MCUSimSubsys::eventToString(out, eventId, locationOrReason, detail);
}
