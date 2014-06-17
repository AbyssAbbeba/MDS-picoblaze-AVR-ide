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
 * @file AdaptableSimIO.cxx
 */
// =============================================================================

#include "AdaptableSimIO.h"

AdaptableSimIO::AdaptableSimIO()
{
    m_numberOfBits   = ( NUMBER_OF_PORTS * NUMBER_OF_BITS_PER_PORT );
    m_inputBitArray  = new char [ NUMBER_OF_PORTS ];
    m_outputBitArray = new char [ NUMBER_OF_PORTS ];

    m_readStrobe = false;
    m_writeStrobe = false;
}

AdaptableSimIO::~AdaptableSimIO()
{
    delete [] m_inputBitArray;
    delete [] m_outputBitArray;
}

AdaptableSimIO * AdaptableSimIO::link ( MCUSimEventLogger * eventLogger )
{
    MCUSimPureLogicIO::link ( eventLogger );
    return this;
}

void AdaptableSimIO::reset ( MCUSimBase::ResetMode mode )
{
    switch ( mode )
    {
        case MCUSim::RSTMD_NEW_CONFIG:
            break;
        case MCUSim::RSTMD_INITIAL_VALUES:
            for ( unsigned int i = 0; i < NUMBER_OF_PORTS; i++ )
            {
                m_inputBitArray[i] = 0;
                m_inputBitArray[i] = 0;
            }
            break;
        case MCUSim::RSTMD_MCU_RESET:
            m_readStrobe = false;
            m_writeStrobe = false;
            break;
        default:
            // Irrelevant requests are silently ignored
            break;
    }
}

unsigned int AdaptableSimIO::getNumberOfPorts()
{
    return NUMBER_OF_PORTS;
}

unsigned int AdaptableSimIO::getNumberOfBitsPerPort()
{
    return NUMBER_OF_BITS_PER_PORT;
}

unsigned int AdaptableSimIO::getNumberOfBitsTotal()
{
    return ( NUMBER_OF_PORTS * NUMBER_OF_BITS_PER_PORT );
}

bool AdaptableSimIO::read ( unsigned int port,
                         unsigned int bit )
{
    if ( port >= NUMBER_OF_PORTS )
    {
        return false;
    }

    return ( bool ) ( m_outputBitArray [ port ] & ( 1 << bit ) );
}

unsigned int AdaptableSimIO::read ( unsigned int port )
{
    if ( port >= NUMBER_OF_PORTS )
    {
        return 0;
    }

    return ( unsigned int ) ( m_outputBitArray [ port ] );
}

void AdaptableSimIO::write ( unsigned int port,
                          unsigned int bit,
                          bool value )
{
    if ( port >= NUMBER_OF_PORTS )
    {
        return;
    }

    if ( true == value )
    {
        m_inputBitArray [ port ] |= ( 1 << bit );
    }
    else
    {
        m_inputBitArray [ port ] &= ~( 1 << bit );
    }
}

void AdaptableSimIO::write ( unsigned int port,
                          unsigned int value )
{
    if ( port >= NUMBER_OF_PORTS )
    {
        return;
    }

    m_inputBitArray [ port ] = value;
}
