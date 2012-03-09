/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 *
 */

#ifndef AVR8TIMERCOUNTER2_H
#define AVR8TIMERCOUNTER2_H

class AVR8DataMemory;

#include "../MCUSim.h"

class AVR8TimerCounter2 : public MCUSim::Subsys {
public:
	AVR8TimerCounter2() {};

	struct Config {
	};

	Config m_config;

	AVR8TimerCounter2 * link(
		MCUSim::EventLogger	* eventLogger,
		MCUSim::Subsys::SubsysId  subsysId,
		AVR8DataMemory		* dataMemory);

	void reset(MCUSim::ResetMode mode);

// 	void clockCycles(unsigned int numberOf);
// 	void timeStep(float timeStep, unsigned int clockCycles = 0);

protected:
	AVR8DataMemory * m_dataMemory;

	inline void resetToInitialValues();
	inline void mcuReset();
};

#endif // AVR8TIMERCOUNTER2_H
