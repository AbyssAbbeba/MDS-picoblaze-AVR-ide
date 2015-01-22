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
 * @file MCUSimPureLogicIO.cxx
 */
// =============================================================================

#include "MCUSimPureLogicIO.h"

void MCUSimPureLogicIO::eventToString ( std::ostream & out,
                                        int eventId,
                                        int locationOrReason,
                                        int detail )
{
    switch ( Event(eventId) )
    {
        case EVENT_PLIO_READ:
            out << ">>> plio_read @ " << locationOrReason << std::endl;
            return;
        case EVENT_PLIO_READ_END:
            out << ">>> plio_read_end" << std::endl;
            return;
        case EVENT_PLIO_WRITE:
            out << ">>> plio_write @ " << locationOrReason << " : " << detail << std::endl;
            return;
        case EVENT_PLIO_WRITE_END:
            out << ">>> plio_write_end" << std::endl;
            return;

        case EVENT_PLIO__MAX__:
            break;
    }

    MCUSimSubsys::eventToString(out, eventId, locationOrReason, detail);
}
