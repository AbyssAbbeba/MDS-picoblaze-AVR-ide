// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup PIC8
 * @file PIC8ClockControl.cxx
 */
// =============================================================================

#include "PIC8ClockControl.h"

#include "PIC8ConfigWord.h"

PIC8ClockControl * PIC8ClockControl::link ( MCUSim::EventLogger * eventLogger,
                                            PIC8ConfigWord      * configWord )
{
    Clock::link(eventLogger);
    m_configWord = configWord;
    return this;
}

void PIC8ClockControl::reset ( MCUSim::ResetMode )
{
}

float PIC8ClockControl::getFrequency()
{
    switch ( getOscSel() )
    {
        case 0b11:
            // RC - Resistor/Capacitor
            return freqRC();

        case 0b10:
            // HS - High Speed Crystal/Resonator
            return freqHS();

        case 0b01:
            // XT - Crystal/Resonator
            return freqXT();

        case 0b00:
            // LP - Low Power Crystal
            return freqLP();

        default:
            // Unknown setting
            return 0;
    }
}

PIC8ClockControl::StartUpTime PIC8ClockControl::startUpTime()
{
    StartUpTime startUpTime;

    if ( ClockSource::TYPE_RC != m_clockSource.getType() )
    {
        startUpTime.m_startUpTime = 72e-3;
    }
    else
    {
        startUpTime.m_startUpTime = 72e-3 + ( 1024.0 / m_clockSource.getFrequency() );
    }

    return startUpTime;
}

inline unsigned int PIC8ClockControl::getOscSel() const
{
    /*
     *  Table: Device Oscillator Selection Bits:
     * ╔════════════════════════════════════╤═════════════╗
     * ║ Device Clock Generator Oscillator  │ FOSC1:FOSC0 ║
     * ╟────────────────────────────────────┼─────────────╢
     * ║ RC - Resistor/Capacitor            │     11      ║
     * ║ HS - High Speed Crystal/Resonator  │     10      ║
     * ║ XT - Crystal/Resonator             │     01      ║
     * ║ LP - Low Power Crystal             │     00      ║
     * ╚════════════════════════════════════╧═════════════╝
     */

    return ( ( ( true == (*m_configWord)[PIC8ConfigWord::CFGW_F0SC1] ) ? 0x2 : 0 ) |
             ( ( true == (*m_configWord)[PIC8ConfigWord::CFGW_F0SC0] ) ? 0x1 : 0 ) );
}

inline float PIC8ClockControl::freqRC()
{
    if ( ClockSource::TYPE_RC != m_clockSource.getType() )
    {
        logEvent(EVENT_CLK_ERR_INCOMPATIBLE_SOURCE);
        return 0;
    }

    if ( ( 0 == m_clockSource.getResistance() ) || ( 0 == m_clockSource.getCapacity() ) )
    {
        logEvent(EVENT_CLK_ERR_SOURCE_USELESS);
        return 0;
    }

    // R_ext should be in range [5kΩ;100kΩ]
    if ( m_clockSource.getResistance() < 5e3 )
    {
        logEvent(EVENT_CLK_WRN_RESISTANCE_TOO_LOW);
    }
    else if ( m_clockSource.getResistance() > 100e3 )
    {
        logEvent(EVENT_CLK_WRN_RESISTANCE_TOO_HIGH);
    }

    // C_ext should be > 20pF
    if ( m_clockSource.getCapacity() <= 20e-9 )
    {
        logEvent(EVENT_CLK_WRN_CAPACITY_TOO_LOW);
    }

    // f = 1/(3RC)
    return ( 1.0 / 3 * m_clockSource.getResistance() * m_clockSource.getCapacity() );
}

inline float PIC8ClockControl::freqHS()
{
    if ( ( ClockSource::TYPE_CRYSTAL != m_clockSource.getType() )
             &&
         ( ClockSource::TYPE_CERAMIC_RES != m_clockSource.getType() )
             &&
         ( ClockSource::TYPE_EXTERNAL != m_clockSource.getType() ) )
    {
        logEvent(EVENT_CLK_ERR_INCOMPATIBLE_SOURCE);
        return 0;
    }

    return m_clockSource.getFrequency();
}

inline float PIC8ClockControl::freqXT()
{
    if ( ( ClockSource::TYPE_CRYSTAL != m_clockSource.getType() )
             &&
         ( ClockSource::TYPE_CERAMIC_RES != m_clockSource.getType() )
             &&
         ( ClockSource::TYPE_EXTERNAL != m_clockSource.getType() ) )
    {
        logEvent(EVENT_CLK_ERR_INCOMPATIBLE_SOURCE);
        return 0;
    }

    if ( ( ClockSource::TYPE_CERAMIC_RES != m_clockSource.getType() )
             &&
         ( m_clockSource.getFrequency() > 3.5e6 ) )
    {
        logEvent(EVENT_CLK_WRN_ATYPICAL_FREQ);
    }

    return m_clockSource.getFrequency();
}

inline float PIC8ClockControl::freqLP()
{
    if ( ( ClockSource::TYPE_LOW_FREQ_CRYSTAL != m_clockSource.getType() )
             &&
         ( ClockSource::TYPE_EXTERNAL != m_clockSource.getType() ) )
    {
        logEvent(EVENT_CLK_ERR_INCOMPATIBLE_SOURCE);
        return 0;
    }

    return m_clockSource.getFrequency();
}
