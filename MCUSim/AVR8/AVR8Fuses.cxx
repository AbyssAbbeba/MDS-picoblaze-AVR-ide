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

#include "AVR8Fuses.h"

AVR8Fuses::AVR8Fuses(MCUSim::EventLogger * eventLogger) :
		MCUSim::Subsys(eventLogger, ID_FUSES)
{
}

void AVR8Fuses::reset(MCUSim::Subsys::SubsysResetMode mode) {
	switch ( mode ) {
		case RSTMD_INITIAL_VALUES:
			resetToInitialValues();
			break;
		default:
			// Irrelevant requests are silently ignored
			break;
	}
}

inline void AVR8Fuses::resetToInitialValues() {
	for ( int i = 0; i < FUSE__MAX__; i++ ) {
		m_fuses[i] = false;
	}
}
