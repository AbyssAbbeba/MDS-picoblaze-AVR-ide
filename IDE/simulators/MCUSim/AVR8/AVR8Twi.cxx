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

#include "AVR8Twi.h"
#include "AVR8DataMemory.h"

AVR8Twi * AVR8Twi::link(
	MCUSim::EventLogger	* eventLogger,
	AVR8DataMemory		* dataMemory
) {
	Subsys::link(eventLogger, ID_TWI);

	m_dataMemory = dataMemory;

	return this;
}

void AVR8Twi::reset(MCUSim::ResetMode /*mode*/) {
	
}

inline void AVR8Twi::resetToInitialValues() {
	
}

inline void AVR8Twi::mcuReset() {
	
}