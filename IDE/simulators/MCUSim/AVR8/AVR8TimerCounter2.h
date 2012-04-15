/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @ingroup AVR8
 * @file AVR8TimerCounter2.h
 */

#ifndef AVR8TIMERCOUNTER2_H
#define AVR8TIMERCOUNTER2_H

class AVR8DataMemory;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8TimerCounter2
 */
class AVR8TimerCounter2 : public MCUSim::Subsys {
public:
	AVR8TimerCounter2() {};

	struct Config {
		bool m_enabled;
	};

	Config m_config;

	AVR8TimerCounter2 * link(
		MCUSim::EventLogger	* eventLogger,
		MCUSim::Subsys::SubsysId  subsysId,
		AVR8DataMemory		* dataMemory);

	void reset(MCUSim::ResetMode mode);
	bool enabled() {
		return m_config.m_enabled;
	}

// 	void clockCycles(unsigned int numberOf);
// 	void timeStep(float timeStep, unsigned int clockCycles = 0);

protected:
	AVR8DataMemory * m_dataMemory;

	inline void resetToInitialValues();
	inline void mcuReset();
};

#endif // AVR8TIMERCOUNTER2_H
