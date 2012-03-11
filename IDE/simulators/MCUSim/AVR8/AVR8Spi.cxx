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

#include "AVR8Spi.h"
#include "AVR8DataMemory.h"

AVR8Spi * AVR8Spi::link(
	MCUSim::EventLogger	* eventLogger,
	AVR8DataMemory		* dataMemory
) {
	Subsys::link(eventLogger, ID_SPI);

	m_dataMemory = dataMemory;

	return this;
}

void AVR8Spi::reset(MCUSim::ResetMode mode) {
	
}

inline void AVR8Spi::resetToInitialValues() {
	
}

inline void AVR8Spi::mcuReset() {
	
}