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

#include "AVR8AnalogComparator.h"
#include "AVR8DataMemory.h"

AVR8AnalogComparator * AVR8AnalogComparator::link(
	MCUSim::EventLogger	* eventLogger,
	AVR8DataMemory		* dataMemory
) {
	Subsys::link(eventLogger, ID_ACOMP);
	
	m_dataMemory = dataMemory;

	return this;
}

void AVR8AnalogComparator::reset(MCUSim::ResetMode mode) {
}

inline void AVR8AnalogComparator::resetToInitialValues() {
}

inline void AVR8AnalogComparator::mcuReset() {
}
