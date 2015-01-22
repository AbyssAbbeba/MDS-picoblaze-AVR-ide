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
 * @file MCUSimMemory.cxx
 */
// =============================================================================

#include "MCUSimMemory.h"

void MCUSimMemory::eventToString ( std::ostream & out,
                                   int eventId,
                                   int locationOrReason,
                                   int detail )
{
    const char * space = nullptr;
    switch ( m_space )
    {
        case SP_DATA:   space = "data";     break;
        case SP_CODE:   space = "code";     break;
        case SP_STACK:  space = "stack";    break;
        case SP_EEPROM: space = "eeprom";   break;
        case SP_REGS:   space = "register"; break;

        case SP_INVALID:
        case SP__MAX__:
            return;
    }

    switch ( Event(eventId) )
    {
        case EVENT_MEM_SYS_FATAL_ERROR:
            out << ">>> (" << space << ") mem_sys_fatal_error" << std::endl;
            return;

        case EVENT_MEM_ERR_RD_NONEXISTENT:
            out << ">>> (" << space << ") mem_err_rd_nonexistent @ " << locationOrReason << std::endl;
            return;
        case EVENT_MEM_ERR_WR_NONEXISTENT:
            out << ">>> (" << space << ") mem_err_wr_nonexistent @ " << locationOrReason << std::endl;
            return;
        case EVENT_MEM_ERR_RD_NOT_IMPLEMENTED:
            out << ">>> (" << space << ") mem_err_rd_not_implemented @ " << locationOrReason << std::endl;
            return;
        case EVENT_MEM_ERR_WR_NOT_IMPLEMENTED:
            out << ">>> (" << space << ") mem_err_wr_not_implemented @ " << locationOrReason << std::endl;
            return;
        case EVENT_MEM_ERR_RD_ACCESS_DENIED:
            out << ">>> (" << space << ") mem_err_rd_access_denied @ " << locationOrReason << std::endl;
            return;
        case EVENT_MEM_ERR_WR_ACCESS_DENIED:
            out << ">>> (" << space << ") mem_err_wr_access_denied @ " << locationOrReason << std::endl;
            return;

        case EVENT_MEM_WRN_RD_UNDEFINED:
            out << ">>> (" << space << ") mem_wrn_rd_undefined @ " << locationOrReason << std::endl;
            return;
        case EVENT_MEM_WRN_RD_DEFAULT:
            out << ">>> (" << space << ") mem_wrn_rd_default @ " << locationOrReason << std::endl;
            return;
        case EVENT_MEM_WRN_RD_WRITE_ONLY:
            out << ">>> (" << space << ") mem_wrn_rd_write_only @ " << locationOrReason << std::endl;
            return;
        case EVENT_MEM_WRN_WR_READ_ONLY:
            out << ">>> (" << space << ") mem_wrn_wr_read_only @ " << locationOrReason << std::endl;
            return;
        case EVENT_MEM_WRN_RD_PAR_WRITE_ONLY:
            out << ">>> (" << space << ") mem_wrn_rd_par_write_only @ " << locationOrReason << std::endl;
            return;
        case EVENT_MEM_WRN_WR_PAR_READ_ONLY:
            out << ">>> (" << space << ") mem_wrn_wr_par_read_only @ " << locationOrReason << std::endl;
            return;
        case EVENT_MEM_WRN_RD_RESERVED_READ:
            out << ">>> (" << space << ") mem_wrn_rd_reserved_read @ " << locationOrReason << std::endl;
            return;
        case EVENT_MEM_WRN_WR_RESERVED_WRITTEN:
            out << ">>> (" << space << ") mem_wrn_wr_reserved_written @ " << locationOrReason << std::endl;
            return;

        case EVENT_MEM_INF_WR_VAL_CHANGED:
            out << ">>> (" << space << ") mem_inf_wr_val_changed @ " << locationOrReason << std::endl;
            return;
        case EVENT_MEM_INF_WR_VAL_WRITTEN:
            out << ">>> (" << space << ") mem_inf_wr_val_written @ " << locationOrReason << std::endl;
            return;
        case EVENT_MEM_INF_RD_VAL_READ:
            out << ">>> (" << space << ") mem_inf_rd_val_read @ " << locationOrReason << std::endl;
            return;
        case EVENT_MEM_INF_RD_ADDR_OVERFLOW:
            out << ">>> (" << space << ") mem_rd_addr_overflow @ " << locationOrReason << " : " << detail << std::endl;
            return;
        case EVENT_MEM_INF_WR_ADDR_OVERFLOW:
            out << ">>> (" << space << ") mem_wr_addr_overflow @ " << locationOrReason << " : " << detail << std::endl;
            return;

        case EVENT_MEM__MAX__:
            break;
    }

    MCUSimSubsys::eventToString(out, eventId, locationOrReason, detail);
}
