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
    unsigned char mask = 0x01;
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
