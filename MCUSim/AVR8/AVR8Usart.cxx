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

#include "AVR8Usart.h"
#include "AVR8DataMemory.h"

AVR8Usart * AVR8Usart::link(
	MCUSim::EventLogger	* eventLogger,
	AVR8DataMemory		* dataMemory
) {
	Subsys::link(eventLogger, ID_USART);

	m_dataMemory = dataMemory;

	return this;
}

void AVR8Usart::reset(MCUSim::ResetMode mode) {
	
}

inline void AVR8Usart::resetToInitialValues() {
	
}

inline void AVR8Usart::mcuReset() {
	
}