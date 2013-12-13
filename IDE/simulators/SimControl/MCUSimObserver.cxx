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
 * @ingroup SimControl
 * @file MCUSimObserver.cxx
 */
// =============================================================================

#include "MCUSimObserver.h"

#include "MCUSimControl.h"

MCUSimObserver::MCUSimObserver() : m_simControlUnit(NULL) {}

MCUSimObserver::~MCUSimObserver()
{
    if ( NULL != m_simControlUnit )
    {
        m_simControlUnit->unregisterObserver(this);
    }
}

void MCUSimObserver::setControlUnit ( MCUSimControl * managedBy )
{
    m_simControlUnit = managedBy;
}
