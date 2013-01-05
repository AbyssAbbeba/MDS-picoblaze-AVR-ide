// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup AVR8
 * @file AVR8TimerCounter0.cxx
 */
// =============================================================================

#include "AVR8TimerCounter0.h"
#include "AVR8DataMemory.h"
#include "AVR8IO.h"

AVR8TimerCounter0 * AVR8TimerCounter0::link ( MCUSim::EventLogger * eventLogger,
                                              MCUSim::Subsys::SubsysId subsysId,
                                              AVR8DataMemory * dataMemory,
                                              AVR8IO * io )
{
    Subsys::link(eventLogger, subsysId);
    m_dataMemory = dataMemory;
    m_io = io;
    return this;
}

inline void AVR8TimerCounter0::determinateClockSource()
{
    /*
     * CS02  CS01  CS00   Description
     *  0     0     0     No clock source (Timer/Counter stopped).
     *  0     0     1     clkI/O/(No prescaling)
     *  0     1     0     clkI/O/8 (From prescaler)
     *  0     1     1     clkI/O/64 (From prescaler)
     *  1     0     0     clkI/O/256 (From prescaler)
     *  1     0     1     clkI/O/1024 (From prescaler)
     *  1     1     0     External clock source on T0 pin. Clock on falling edge.
     *  1     1     1     External clock source on T0 pin. Clock on rising edge.
     */

    m_clockSource = m_dataMemory->readFast(AVR8RegNames::TCCR0);
    m_clockSource &= ( AVR8RegNames::TCCR0_CS02 | AVR8RegNames::TCCR0_CS01 | AVR8RegNames::TCCR0_CS00 );
}

inline unsigned int AVR8TimerCounter0::incrementPrescaler ( unsigned int number,
                                                            unsigned int max )
{
    unsigned int result = 0;

    while ( 0 != number )
    {
        m_prescaler++;
        number--;

        if ( max & m_prescaler )
        {
            result++;
        }
    }

    return result;
}

inline void AVR8TimerCounter0::incrementTimer ( unsigned int number )
{
    if ( 0 == number )
    {
        return;
    }

    number += m_dataMemory->readFast(AVR8RegNames::TCNT0);
    if ( number > 0xff )
    {
        number = 0;
        m_dataMemory->setBitFast(AVR8RegNames::TIFR, AVR8RegNames::TIFR_TOV0);
    }
    m_dataMemory->writeFast(AVR8RegNames::TCNT0, number);
}

inline void AVR8TimerCounter0::resetPrescalerOnCond()
{
    if ( false == m_dataMemory->readBitFast(AVR8RegNames::SFIOR, AVR8RegNames::SFIOR_PSR10) )
    {
        return;
    }

    m_prescaler = 0;
    m_dataMemory->clearBitFast(AVR8RegNames::SFIOR, AVR8RegNames::SFIOR_PSR10);
}

void AVR8TimerCounter0::clockCycles ( unsigned int numberOf )
{
    using namespace AVR8RegNames;

    unsigned int prescalerMax;

    determinateClockSource();

    switch ( m_clockSource )
    {
        case 0:
            // No clock source (Timer/Counter stopped).
            return;
        case ( TCCR0_CS00 ):
            // clkI/O/(No prescaling)
            prescalerMax = 1;
            break;
        case ( TCCR0_CS01 ):
            // clkI/O/8 (From prescaler)
            prescalerMax = 8;
            break;
        case ( TCCR0_CS01 | TCCR0_CS00 ):
            // clkI/O/64 (From prescaler)
            prescalerMax = 64;
            break;
        case ( TCCR0_CS02 ):
            // clkI/O/256 (From prescaler)
            prescalerMax = 256;
            break;
        case ( TCCR0_CS02 | TCCR0_CS00 ):
            // clkI/O/1024 (From prescaler)
            prescalerMax = 1024;
            break;
        case ( TCCR0_CS02 | TCCR0_CS01 ):
            // External clock source on T0 pin. Clock on falling edge.
        case ( TCCR0_CS02 | TCCR0_CS01 | TCCR0_CS00 ):
            // External clock source on T0 pin. Clock on rising edge.
            incrementWithDelay(numberOf);
            return;
    }

    incrementTimer(incrementPrescaler(numberOf, prescalerMax));

    resetPrescalerOnCond();
    clearDelayArray();
}

inline void AVR8TimerCounter0::incrementWithDelay ( unsigned int number )
{
    if ( number >= COUNTER_DELAY )
    {
        number = (COUNTER_DELAY - 1);
    }

    while ( number > 0 )
    {
        incrementTimer(m_incrementWithDelay[m_iwdIndexOut]);
        m_incrementWithDelay[m_iwdIndexOut] = 0;

        m_iwdIndexIn++;
        m_iwdIndexOut++;

        m_iwdIndexIn %= COUNTER_DELAY;
        m_iwdIndexOut %= COUNTER_DELAY;

        number--;
    }
}

inline void AVR8TimerCounter0::sampleT0()
{
    bool prevT0Log = m_t0Log;
    m_t0Log = m_io->getLog(AVR8PinNames::SPF_T0);

    if ( 6 == m_clockSource )
    {
        // External clock source on T0 pin. Clock on falling edge.
        m_incrementWithDelay[m_iwdIndexIn] += ( ( (true == prevT0Log) && (false == m_t0Log) ) ? 1 : 0 );

    }
    else if ( 7 == m_clockSource )
    {
        // External clock source on T0 pin. Clock on rising edge.
        m_incrementWithDelay[m_iwdIndexIn] += ( ( (false == prevT0Log) && (true == m_t0Log) ) ? 1 : 0 );
    }
}

inline void AVR8TimerCounter0::clearDelayArray()
{
    for ( unsigned int i = 0; i < COUNTER_DELAY; i++ )
    {
        m_incrementWithDelay[i] = 0;
    }
}

void AVR8TimerCounter0::reset ( MCUSim::ResetMode mode )
{
    switch ( mode )
    {
        case MCUSim::RSTMD_INITIAL_VALUES:
            resetToInitialValues();
            break;
        case MCUSim::RSTMD_MCU_RESET:
            mcuReset();
            break;
        default:
            // Irrelevant requests are silently ignored
            break;
    }
}

inline void AVR8TimerCounter0::resetToInitialValues()
{
    m_t0Log = m_io->getLog(AVR8PinNames::SPF_T0);

    clearDelayArray();
    m_iwdIndexIn = 0;
    m_iwdIndexOut = (COUNTER_DELAY - 1);
}

inline void AVR8TimerCounter0::mcuReset()
{
    m_prescaler = 0;
}
