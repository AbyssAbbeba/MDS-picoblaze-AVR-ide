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

#include "AVR8IO.h"
#include "AVR8DataMemory.h"

AVR8IO::AVR8IO(
		MCUSim::EventLogger * eventLogger,
		AVR8DataMemory * dataMemory)
		 :
		MCUSim::IO(eventLogger),
		m_dataMemory(dataMemory)
{
}

void AVR8IO::clockCycles(unsigned int numberOf) {
	// TODO: Not implemented yet!
}

void AVR8IO::timeStep(float timeStep) {
	// TODO: Not implemented yet!
}

void AVR8IO::reset(MCUSim::Subsys::SubsysResetMode mode) {
	switch ( mode ) {
		case RSTMD_INITIAL_VALUES:
			resetToInitialValues();
			break;
		case RSTMD_MCU_RESET:
			mcuReset();
			break;
		default:
			// Irrelevant requests are silently ignored
			break;
	}
}

inline void AVR8IO::resetToInitialValues() {
	// TODO: Not implemented yet!
}

inline void AVR8IO::mcuReset() {
	// TODO: Not implemented yet!
}
