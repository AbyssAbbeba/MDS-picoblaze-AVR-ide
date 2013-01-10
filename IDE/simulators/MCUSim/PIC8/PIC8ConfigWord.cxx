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
 * @ingroup PIC8
 * @file PIC8ConfigWord.cxx
 */
// =============================================================================

#include "PIC8ConfigWord.h"

#include <cstdlib>

PIC8ConfigWord * PIC8ConfigWord::link ( MCUSim::EventLogger * eventLogger )
{
    Subsys::link(eventLogger, ID_FUSES);
    return this;
}

void PIC8ConfigWord::reset ( MCUSim::ResetMode mode )
{
    switch ( mode )
    {
        case MCUSim::RSTMD_NEW_CONFIG:
            loadConfig();
            break;
        case MCUSim::RSTMD_INITIAL_VALUES:
            resetToInitialValues();
            break;
        default:
            // Irrelevant requests are silently ignored
            break;
    }
}

inline void PIC8ConfigWord::loadConfig()
{
    // Set to default values
    uint8_t mask = 0x01;
    for ( int i = 0; i < CFGW__MAX__; i++ )
    {
        if ( mask & m_config.m_defaultCfgWord )
        {
            m_cfgWord[i] = true;
        }
        else
        {
            m_cfgWord[i] = false;
        }
        mask <<= 1;
    }
}

inline void PIC8ConfigWord::resetToInitialValues()
{
    for ( int i = 0; i < CFGW__MAX__; i++ )
    {
        m_cfgWord[i] = false;
    }
}

void PIC8ConfigWord::setWord ( unsigned int configWord )
{
    for ( int i = 0; i < 14; i++ )
    {
        m_cfgWord[i] = ( configWord & ( 1 << i ) ) ? true : false;
    }
}

unsigned int PIC8ConfigWord::getWord()
{
    unsigned int result = 0;
    for ( int i = 0; i < 14; i++ )
    {
        if ( true == m_cfgWord[i] )
        {
            result |= ( 1 << i );
        }
    }
    return result;
}
