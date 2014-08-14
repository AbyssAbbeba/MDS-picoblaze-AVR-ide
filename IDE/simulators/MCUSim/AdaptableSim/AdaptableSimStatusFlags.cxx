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
        m_overflow    = false;
        m_negative    = false;
        m_halfCarry   = false;
        m_parity      = false;
        m_flag       = false;

        m_preCarry    = false;
        m_preZero     = false;
        m_preOverflow = false;
        m_preNegative = false;
        m_preHalfCarry= false;
        m_preParity   = false;

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
    m_interrupted++;

    if ( true == m_config.m_backupWhenInterrupted )
    {
        m_preCarry     = m_carry;
        m_preZero      = m_zero;
        m_preOverflow  = m_overflow;
        m_preNegative  = m_negative;
        m_preParity    = m_parity;
    }

    if ( true == m_config.m_autoDisableInterrupts )
    {
        m_inte = false;
    }
}

void AdaptableSimStatusFlags::returni()
{
    m_interrupted--;

    if ( true == m_config.m_backupWhenInterrupted )
    {
        m_carry     = m_preCarry;
        m_zero      = m_preZero;
        m_overflow  = m_preOverflow;
        m_negative  = m_preNegative;
        m_halfCarry = m_preHalfCarry;
        m_parity    = m_preParity;
    }

    if ( true == m_config.m_autoEnableInterrupts )
    {
        m_inte = true;
    }
}

bool AdaptableSimStatusFlags::getFlag ( AdaptableSimInstruction::OperParam::ProcFlag flag ) const
{
    switch ( flag )
    {
        case AdaptableSimInstruction::OperParam::F_ZERO:        return m_zero;
        case AdaptableSimInstruction::OperParam::F_CARRY:       return m_carry;
        case AdaptableSimInstruction::OperParam::F_OVERFLOW:    return m_overflow;
        case AdaptableSimInstruction::OperParam::F_NEGATIVE:    return m_negative;
        case AdaptableSimInstruction::OperParam::F_HALF_CARRY:  return m_halfCarry;
        case AdaptableSimInstruction::OperParam::F_INTR_ENABLE: return m_inte;
        case AdaptableSimInstruction::OperParam::F_PARITY:      return m_parity;
        case AdaptableSimInstruction::OperParam::F_FLAG:        return m_flag;
    }

    return false;
}

void AdaptableSimStatusFlags::setFlag ( AdaptableSimInstruction::OperParam::ProcFlag flag,
                                        bool value )
{
    switch ( flag )
    {
        case AdaptableSimInstruction::OperParam::F_ZERO:
            logEvent(EVENT_FLAGS_ZERO_CHANGED, value);
            m_zero = value;
            break;
        case AdaptableSimInstruction::OperParam::F_CARRY:
            logEvent(EVENT_FLAGS_CARRY_CHANGED, value);
            m_carry = value;
            break;
        case AdaptableSimInstruction::OperParam::F_OVERFLOW:
            logEvent(EVENT_FLAGS_OVERFLOW_CHANGED, value);
            m_overflow = value;
            break;
        case AdaptableSimInstruction::OperParam::F_NEGATIVE:
            logEvent(EVENT_FLAGS_NEGATIVE_CHANGED, value);
            m_negative = value;
            break;
        case AdaptableSimInstruction::OperParam::F_HALF_CARRY:
            logEvent(EVENT_FLAGS_HALF_CARRY_CHANGED, value);
            m_halfCarry = value;
            break;
        case AdaptableSimInstruction::OperParam::F_INTR_ENABLE:
            logEvent(EVENT_FLAGS_IE_CHANGED, value);
            m_inte = value;
            break;
        case AdaptableSimInstruction::OperParam::F_PARITY:
            logEvent(EVENT_FLAGS_FLAG_PARITY_CHANGED, value);
            m_parity = value;
            break;
        case AdaptableSimInstruction::OperParam::F_FLAG:
            logEvent(EVENT_FLAGS_FLAG_CHANGED, value);
            m_flag = value;
            break;
    }
}
