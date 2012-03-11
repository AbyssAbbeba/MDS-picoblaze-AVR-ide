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

#include "AVR8ParalelProg.h"
#include "AVR8ProgramMemory.h"

AVR8ParalelProg * AVR8ParalelProg::link(
	MCUSim::EventLogger	* eventLogger,
	AVR8ProgramMemory	* programMemory
) {
	Subsys::link(eventLogger, ID_PPROG);

	m_programMemory = programMemory;

	return this;
}

void AVR8ParalelProg::reset(MCUSim::ResetMode mode) {
}

inline void AVR8ParalelProg::resetToInitialValues() {
}

inline void AVR8ParalelProg::mcuReset() {
}
