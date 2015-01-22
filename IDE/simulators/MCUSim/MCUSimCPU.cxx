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
 * @file MCUSimCPU.cxx
 */
// =============================================================================

#include "MCUSimCPU.h"

void MCUSimCPU::eventToString ( std::ostream & out,
                                int eventId,
                                int locationOrReason,
                                int detail )
{
    switch ( Event(eventId) )
    {
        case EVENT_CPU_ERR_UNDEFINED_OPCODE:
            out << ">>> cpu_undefined_opcode @ " << locationOrReason << std::endl;
            return;
        case EVENT_CPU_ERR_INVALID_OPCODE:
            out << ">>> cpu_invalid_opcode @ " << locationOrReason << " : " << detail << std::endl;
            return;
        case EVENT_CPU_WRN_INVALID_IRQ:
            out << ">>> cpu_invalid_irq @ " << locationOrReason << std::endl;
            return;
        case EVENT_CPU_ERR_INVALID_RET:
            out << ">>> cpu_invalid_ret @ " << locationOrReason << std::endl;
            return;
        case EVENT_CPU_ERR_INVALID_RETI:
            out << ">>> cpu_invalid_reti @ " << locationOrReason << std::endl;
            return;
        case EVENT_CPU_PC_OVERFLOW:
            out << ">>> cpu_pc_overflow " << locationOrReason << std::endl;
            return;
        case EVENT_CPU_PC_UNDERFLOW:
            out << ">>> cpu_pc_underflow " << locationOrReason << std::endl;
            return;
        case EVENT_CPU_PC_CHANGED:
            out << ">>> cpu_pc_changed " << locationOrReason << std::endl;
            return;
        case EVENT_CPU_CALL:
            out << ">>> cpu_call @ " << locationOrReason << " : " << detail << std::endl;
            return;
        case EVENT_CPU_RETURN:
            out << ">>> cpu_return @ " << locationOrReason << std::endl;
            return;
        case EVENT_CPU_IRQ:
            out << ">>> cpu_irq @ " << locationOrReason << std::endl;
            return;
        case EVENT_CPU_RETURN_FROM_ISR:
            out << ">>> cpu_return_from_isr @ " << locationOrReason << std::endl;
            return;

        case EVENT_CPU__MAX__:
            break;
    }

    MCUSimSubsys::eventToString(out, eventId, locationOrReason, detail);
}
