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

#include "AVR8TimerCounter2.h"
#include "AVR8DataMemory.h"

AVR8TimerCounter2 * AVR8TimerCounter2::link(
	MCUSim::EventLogger	* eventLogger,
	MCUSim::Subsys::SubsysId  subsysId,
	AVR8DataMemory		* dataMemory
) {
	Subsys::link(eventLogger, subsysId);

	m_dataMemory = dataMemory;

	return this;
}

void AVR8TimerCounter2::reset(MCUSim::ResetMode mode) {

}

inline void AVR8TimerCounter2::resetToInitialValues() {
}

inline void AVR8TimerCounter2::mcuReset() {
}
