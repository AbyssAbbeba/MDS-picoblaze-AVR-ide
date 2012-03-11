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

#include "AVR8TimerCounter1.h"
#include "AVR8DataMemory.h"

AVR8TimerCounter1 * AVR8TimerCounter1::link(
	MCUSim::EventLogger	* eventLogger,
	MCUSim::Subsys::SubsysId  subsysId,
	AVR8DataMemory		* dataMemory
) {
	Subsys::link(eventLogger, subsysId);

	m_dataMemory = dataMemory;

	return this;
}

void AVR8TimerCounter1::reset(MCUSim::ResetMode mode) {

}

inline void AVR8TimerCounter1::resetToInitialValues() {
}

inline void AVR8TimerCounter1::mcuReset() {
}
