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

#include "AVR8Adc.h"
#include "AVR8DataMemory.h"

AVR8Adc * AVR8Adc::link(
	MCUSim::EventLogger	* eventLogger,
	AVR8DataMemory		* dataMemory
) {
	Subsys::link(eventLogger, ID_ADC);
	
	m_dataMemory = dataMemory;
	
	return this;
}

void AVR8Adc::reset(MCUSim::ResetMode mode) {
	
}

inline void AVR8Adc::resetToInitialValues() {
	
}

inline void AVR8Adc::mcuReset() {
	
}
