/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 *
 */

#include "MCUSimObserver.h"

#include "MCUSimControl.h"

MCUSimObserver::MCUSimObserver() : m_simControlUnit(NULL) {}

MCUSimObserver::~MCUSimObserver() {
	if ( NULL != m_simControlUnit ) {
		m_simControlUnit->unregisterObserver(this);
	}
}

void MCUSimObserver::setControlUnit(MCUSimControl * managedBy) {
	m_simControlUnit = managedBy;
}
