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
 * @file PIC8IO.cxx
 */
// =============================================================================

//          ╭────────────╮
// PIN █──┬─┤ RESISTANCE ├─────▎← VOLTAGE_INT
//        │ ╰────────────╯
//        ╰────────────────────▎→ REAL_VOLTAGE
//

#include "PIC8IO.h"
#include "PIC8DataMemory.h"

PIC8IO::PIC8IO()
{
    m_lowLevelInterface = new SimFloatType * [ II__MAX__ ];
    for ( int i = 0; i < II__MAX__; i++ )
    {
        m_lowLevelInterface[i] = new SimFloatType [ NUMBER_OF_PINS ];

        for ( unsigned int j = 0; j < NUMBER_OF_PINS; j++ )
        {
            m_lowLevelInterface[i][j] = 0;
        }
    }

    setSourceVoltage(5); // default voltage is 5V
    m_enabled = false;
}

PIC8IO * PIC8IO::link ( MCUSim::EventLogger     * eventLogger,
                        PIC8DataMemory          * dataMemory )
{
    IO::link(eventLogger);
    m_dataMemory = dataMemory;
    return this;
}

PIC8IO::~PIC8IO()
{
    if ( NULL != m_lowLevelInterface )
    {
        for ( int i = 0; i < II__MAX__; i++ )
        {
            delete[] m_lowLevelInterface[i];
        }
        delete[] m_lowLevelInterface;
    }
}

void PIC8IO::clockCycles()
{
    if ( false == m_enabled )
    {
        return;
    }

    /*
     * - Read port states and set new ones.
     * - Adjust port configurations to the TRISx registers
     */
    if ( true == m_config.m_hasPort[0] )
    {
        portSetDirection(PIC8PinNames::PIN_RA0, PIC8RegNames::TRISA);
        portReadWrite(PIC8PinNames::PIN_RA0, PIC8RegNames::TRISA, PIC8RegNames::PORTA);
    }
    if ( true == m_config.m_hasPort[1] )
    {
        portSetDirection(PIC8PinNames::PIN_RB0, PIC8RegNames::TRISB);
        portReadWrite(PIC8PinNames::PIN_RB0, PIC8RegNames::TRISB, PIC8RegNames::PORTB);
    }
    if ( true == m_config.m_hasPort[2] )
    {
        portSetDirection(PIC8PinNames::PIN_RC0, PIC8RegNames::TRISC);
        portReadWrite(PIC8PinNames::PIN_RC0, PIC8RegNames::TRISC, PIC8RegNames::PORTC);
    }
    if ( true == m_config.m_hasPort[3] )
    {
        portSetDirection(PIC8PinNames::PIN_RD0, PIC8RegNames::TRISD);
        portReadWrite(PIC8PinNames::PIN_RD0, PIC8RegNames::TRISD, PIC8RegNames::PORTD);
    }
}

inline void PIC8IO::portSetDirection ( unsigned int pinIdx,
                                       unsigned int trisRegAddr )
{
    const unsigned int tris = m_dataMemory->readFast(trisRegAddr);
    const bool rbpu = m_dataMemory->readBitFast(PIC8RegNames::OPTION_REG, PIC8RegNames::OPTION_REG_RBPU);

    for ( int i = 0x1; i < 0x100; i <<= 1, pinIdx++ )
    {
        if ( ( PINMD_NOT_IMPLEMENTED | PINMD_OUTPUT_OVERRIDE ) & m_pinMode[pinIdx] )
        {
            continue;
        }

        SimFloatType resistance;
        if ( tris & i )
        {
            // ( TRISx[i] = 1 ) => Input
            if ( false == rbpu )
            {
                resistance = m_config.m_pullUpresistance;
                setLog ( ( PIC8PinNames::PIN ) pinIdx, true );
            }
            else
            {
                resistance = INFINITY;
            }
        }
        else
        {
            // ( TRISx[i] = 0 ) => Output
            resistance = 0;
        }
        setResistance ( ( PIC8PinNames::PIN ) pinIdx, resistance );
    }
}

inline void PIC8IO::portReadWrite ( unsigned int pinIdx,
                                    unsigned int trisRegAddr,
                                    unsigned int portRegAddr )
{
    unsigned int in = 0;
    unsigned int out = m_dataMemory->readFast(portRegAddr);
    unsigned int tris = m_dataMemory->readFast(trisRegAddr);

    for ( int i = 0x1; i < 0x100; i <<= 1, pinIdx++ )
    {
        if ( PINMD_NOT_IMPLEMENTED & m_pinMode[pinIdx] )
        {
            continue;
        }

        // Read pin state
        if ( !( PINMD_INPUT_OVERRIDE & m_pinMode[pinIdx] ) )
        {
            if ( true == getLog ( ( PIC8PinNames::PIN ) pinIdx ) )
            {
                in |= i;
            }
        }

        // Set new pin state
        if ( ( 0 == ( PINMD_OUTPUT_OVERRIDE & m_pinMode[pinIdx] ) )
                &&
             ( 0 == ( tris & i ) ) )
        {
            setLog ( (PIC8PinNames::PIN)pinIdx, bool(out & i) );
        }
    }

    m_dataMemory->writeFast(portRegAddr, in);
}

void PIC8IO::reset ( MCUSim::ResetMode mode )
{
    switch ( mode )
    {
        case MCUSim::RSTMD_NEW_CONFIG:
            loadConfig();
            break;
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

inline void PIC8IO::loadConfig()
{
    for ( unsigned int i = 0; i < NUMBER_OF_PINS; i++ )
    {
        if ( true == m_config.m_availablePins[i] )
        {
            m_pinMode[i] = PINMD_NOT_IMPLEMENTED;
        }
        else
        {
            m_pinMode[i] = PINMD_GENERAL_IO;
        }
    }
}

inline void PIC8IO::resetToInitialValues()
{
    // TODO: Not implemented yet!
}

inline void PIC8IO::mcuReset()
{
    for ( unsigned int i = 0; i < NUMBER_OF_PINS; i++ )
    {
        if ( PINMD_NOT_IMPLEMENTED & m_pinMode[i] )
        {
            m_lowLevelInterface[II_VOLTAGE_INT][i]  = 0;
            m_lowLevelInterface[II_REAL_VOLTAGE][i] = 0;
            m_lowLevelInterface[II_CURRENT][i]      = 0;
            m_lowLevelInterface[II_RESISTANCE][i]   = NAN; // pin is not implemented
        }
        else
        {
            for ( int j = 0; j < II__MAX__; j++ )
            {
                m_lowLevelInterface[j][i] = 0;
            }

            m_pinMode[i] = PINMD_GENERAL_IO;
        }
    }
}

bool PIC8IO::getLog ( PIC8PinNames::SPF pin )
{
    return getLog(m_config.m_specFuncMap[pin]);
}

bool PIC8IO::getLog ( PIC8PinNames::PIN pin )
{
    assert(PIC8PinNames::PIN_NC != pin);

    if ( m_lowLevelInterface[II_REAL_VOLTAGE][pin] > m_logThreshold1 )
    {
        return true;
    }
    else if ( m_lowLevelInterface[II_REAL_VOLTAGE][pin] < m_logThreshold0 )
    {
        return false;
    }
    else
    {
        logEvent(EVENT_IO_INDETERMINABLE_LOG, pin);

        switch ( m_config.m_random )
        {
            case RVM_RANDOM:
                return ( ( rand() > (RAND_MAX / 2) ) ? true : false );
            case RVM_HIGH:
                return true;
            case RVM_LOW:
                return false;
        }
    }

    assert(0);
    return false; // <-- Program flow control MUST NEVER reach this.
}

void PIC8IO::setLog ( PIC8PinNames::SPF pin,
                      bool val )
{
    setLog(m_config.m_specFuncMap[pin], val);
}

void PIC8IO::setLog ( PIC8PinNames::PIN pin,
                      bool val )
{
    assert(PIC8PinNames::PIN_NC != pin);

    if ( true == val )
    {
        m_lowLevelInterface[II_VOLTAGE_INT][pin] = m_sourceVoltage;
    }
    else
    {
        m_lowLevelInterface[II_VOLTAGE_INT][pin] = 0;
    }
}
