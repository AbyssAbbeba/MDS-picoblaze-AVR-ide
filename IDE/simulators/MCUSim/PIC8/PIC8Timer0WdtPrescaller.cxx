// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2012 Moravia Microsystems, s.r.o.
 *
 * @authors Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup PIC8
 * @file PIC8Timer0WdtPrescaller.cxx
 */
// =============================================================================

#include "PIC8Timer0WdtPrescaller.h"

#include "PIC8DataMemory.h"

PIC8Timer0WdtPrescaller::PIC8Timer0WdtPrescaller()
{
    m_assignedToTimer0 = false;
}

PIC8Timer0WdtPrescaller::~PIC8Timer0WdtPrescaller()
{
}

PIC8Timer0WdtPrescaller * PIC8Timer0WdtPrescaller::link ( MCUSim::EventLogger * eventLogger,
                                                          PIC8DataMemory      * dataMemory )
{
    Subsys::link(eventLogger, ID_PRESCALLER);
    m_dataMemory = dataMemory;
    return this;
}

void PIC8Timer0WdtPrescaller::reset ( MCUSim::ResetMode mode )
{
    switch ( mode )
    {
        case MCUSim::RSTMD_MCU_RESET:
            mcuReset();
            break;
        default:
            // Irrelevant requests are silently ignored
            break;
    }
}

inline void PIC8Timer0WdtPrescaller::mcuReset()
{
    m_ps = 0;
    m_value = 0;
    m_flag = 0;
}

void PIC8Timer0WdtPrescaller::clockCycles()
{
    using namespace PIC8RegNames;

    unsigned int optionReg = m_dataMemory->readFast(OPTION_REG);

    switch ( optionReg & ( OPTION_REG_PS2 | OPTION_REG_PS1 | OPTION_REG_PS0 ) )
    {
        // 0b000
        case 0:
            m_ps = 1;
            break;
        // 0b001
        case ( OPTION_REG_PS0 ):
            m_ps = 2;
            break;
        // 0b010
        case ( OPTION_REG_PS1 ):
            m_ps = 4;
            break;
        // 0b011
        case ( OPTION_REG_PS1 | OPTION_REG_PS0 ):
            m_ps = 8;
            break;
        // 0b100
        case ( OPTION_REG_PS2 ):
            m_ps = 16;
            break;
        // 0b101
        case ( OPTION_REG_PS2 | OPTION_REG_PS0 ):
            m_ps = 32;
            break;
        // 0b110
        case ( OPTION_REG_PS2 | OPTION_REG_PS1 ):
            m_ps = 64;
            break;
        // 0b111
        case ( OPTION_REG_PS2 | OPTION_REG_PS1 | OPTION_REG_PS0 ):
            m_ps = 128;
            break;
        // nonsense! --> ERROR condition
        default:
            m_ps = 0;
    }

    if ( OPTION_REG_PSA & optionReg )
    {
        m_assignedToTimer0 = true;
        m_ps *= 2;
    }
}

unsigned int PIC8Timer0WdtPrescaller::getPrescalerValue() const
{
    return m_value;
}

void PIC8Timer0WdtPrescaller::prescalerReset()
{
    m_value = 0;
    m_flag = 0;
    logEvent(EVENT_PSC_PRESCALER_CHANGED, m_value, 0);
}

void PIC8Timer0WdtPrescaller::increment ( unsigned int by )
{
    m_value += by;
    if ( m_value >= m_ps )
    {
        m_flag = ( m_value / m_ps );
        m_value %= m_ps;
    }

    logEvent(EVENT_PSC_PRESCALER_CHANGED, m_value, by);
}

unsigned int PIC8Timer0WdtPrescaller::flagReadAndClear()
{
    unsigned int flag = m_flag;
    m_flag = 0;
    return flag;
}
