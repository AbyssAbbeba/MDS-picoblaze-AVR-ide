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
 * @ingroup PIC8
 * @file PIC8Stack.cxx
 */
// =============================================================================

#include "PIC8Stack.h"

PIC8Stack::PIC8Stack()
{
    m_data = nullptr;
    m_position = 0;
}

PIC8Stack::~PIC8Stack()
{
    if ( nullptr != m_data )
    {
        delete[] m_data;
    }
}

PIC8Stack * PIC8Stack::link ( MCUSimEventLogger * eventLogger )
{
    MCUSimSubsys::link(eventLogger, ID_STACK);
    return this;
}

void PIC8Stack::reset ( MCUSimBase::ResetMode mode )
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

inline void PIC8Stack::loadConfig()
{
    if ( nullptr != m_data )
    {
        delete[] m_data;
    }
    m_data = new unsigned int [ m_config.m_size ];
}

inline void PIC8Stack::resetToInitialValues()
{
    for ( unsigned int i = 0; i < m_config.m_size; i++ )
    {
        m_data[i] = 0;
    }
}

inline void PIC8Stack::mcuReset()
{
    m_position = 0;
}
