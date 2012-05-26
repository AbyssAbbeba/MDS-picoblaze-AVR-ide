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

#include "AVR8Isp.h"
#include "AVR8ProgramMemory.h"

AVR8Isp * AVR8Isp::link(
	MCUSim::EventLogger	* eventLogger,
	AVR8ProgramMemory	* programMemory
) {
	Subsys::link(eventLogger, ID_ISP);

	m_programMemory = programMemory;

	return this;
}

void AVR8Isp::reset(MCUSim::ResetMode /*mode*/) {
}

inline void AVR8Isp::resetToInitialValues() {
}

inline void AVR8Isp::mcuReset() {
}
