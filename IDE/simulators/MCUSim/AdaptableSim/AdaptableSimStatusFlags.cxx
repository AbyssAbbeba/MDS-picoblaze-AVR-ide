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
 * @ingroup AdaptableSim
 * @file AdaptableSimStatusFlags.cxx
 */
// =============================================================================

#include "AdaptableSimStatusFlags.h"

AdaptableSimStatusFlags::AdaptableSimStatusFlags()
{
    reset(MCUSim::RSTMD_MCU_RESET);
}

void AdaptableSimStatusFlags::reset ( MCUSimBase::ResetMode mode )
{
    if ( MCUSim::RSTMD_MCU_RESET == mode )
    {
        m_carry       = false;
        m_zero        = false;
        m_inte        = false;
        m_preCarry    = false;
        m_preZero     = false;
        m_overflow    = false;
        m_negative    = false;
        m_halmCarry   = false;
        m_flag0       = false;
        m_flag1       = false;

        m_interrupted = 0;
    }
}

AdaptableSimStatusFlags * AdaptableSimStatusFlags::link ( MCUSimEventLogger * eventLogger )
{
    MCUSimSubsys::link(eventLogger, ID_FLAGS);
    return this;
}

void AdaptableSimStatusFlags::interrupt()
{
    m_preCarry = m_carry;
    m_preZero  = m_zero;
    m_inte     = false;
    m_interrupted++;
}

void AdaptableSimStatusFlags::returni()
{
    m_carry = m_preCarry;
    m_zero  = m_preZero;
    m_interrupted--;
}

bool AdaptableSimStatusFlags::getFlag ( AdaptableSimInstruction::OperParam::ProcFlag flag ) const
{
    switch ( flag )
    {
        case AdaptableSimInstruction::OperParam::F_ZERO:        return m_zero;
        case AdaptableSimInstruction::OperParam::F_CARRY:       return m_carry;
        case AdaptableSimInstruction::OperParam::F_OVERFLOW:    return m_overflow;
        case AdaptableSimInstruction::OperParam::F_NEGATIVE:    return m_negative;
        case AdaptableSimInstruction::OperParam::F_HALF_CARRY:  return m_halmCarry;
        case AdaptableSimInstruction::OperParam::F_INTR_ENABLE: return m_inte;
        case AdaptableSimInstruction::OperParam::F_FLAG_0:      return m_flag0;
        case AdaptableSimInstruction::OperParam::F_FLAG_1:      return m_flag1;
    }

    return false;
}

void AdaptableSimStatusFlags::setFlag ( AdaptableSimInstruction::OperParam::ProcFlag flag,
               bool value )
{
    switch ( flag )
    {
        case AdaptableSimInstruction::OperParam::F_ZERO:
            m_zero = value;
            break;
        case AdaptableSimInstruction::OperParam::F_CARRY:
            m_carry = value;
            break;
        case AdaptableSimInstruction::OperParam::F_OVERFLOW:
            m_overflow = value;
            break;
        case AdaptableSimInstruction::OperParam::F_NEGATIVE:
            m_negative = value;
            break;
        case AdaptableSimInstruction::OperParam::F_HALF_CARRY:
            m_halmCarry = value;
            break;
        case AdaptableSimInstruction::OperParam::F_INTR_ENABLE:
            m_inte = value;
            break;
        case AdaptableSimInstruction::OperParam::F_FLAG_0:
            m_flag0 = value;
            break;
        case AdaptableSimInstruction::OperParam::F_FLAG_1:
            m_flag1 = value;
            break;
    }
}
