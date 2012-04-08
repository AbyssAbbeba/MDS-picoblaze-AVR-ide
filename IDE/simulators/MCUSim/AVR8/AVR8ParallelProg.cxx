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

#include "AVR8ParallelProg.h"
#include "AVR8ProgramMemory.h"

AVR8ParallelProg * AVR8ParallelProg::link(
	MCUSim::EventLogger	* eventLogger,
	AVR8ProgramMemory	* programMemory
) {
	Subsys::link(eventLogger, ID_PPROG);

	m_programMemory = programMemory;

	return this;
}

void AVR8ParallelProg::reset(MCUSim::ResetMode mode) {
}

inline void AVR8ParallelProg::resetToInitialValues() {
}

inline void AVR8ParallelProg::mcuReset() {
}
