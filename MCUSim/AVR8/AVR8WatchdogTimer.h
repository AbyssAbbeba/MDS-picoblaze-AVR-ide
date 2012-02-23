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


#ifndef AVR8WATCHDOGTIMER_H
#define AVR8WATCHDOGTIMER_H

#include "../MCUSim.h"

class AVR8DataMemory;
class AVR8Fuses;

class AVR8WatchdogTimer : public MCUSim::Subsys {
public:
	AVR8WatchdogTimer(MCUSim::EventLogger * eventLogger, AVR8DataMemory * dataMemory, AVR8Fuses & fuses);

	enum Event {
		EVENT_WDT_PRESCALER_CHANGED,
		EVENT_WDT_RESET,

		EVENT_WDT__MAX__
	};

	struct Config {
	};

	Config m_config;

	void reset(SubsysResetMode mode);

	void timeStep(float timeStep, unsigned int clockCycles = 0);	// <-- This has to be called even in a sleep mode
	void wdr() {
		reset(RSTMD_MCU_RESET);
	};
	bool resetFlagRdClr() {
		bool result = m_resetFlag;
		m_resetFlag = false;
		return result;
	}

protected:
	AVR8DataMemory * m_dataMemory;
	AVR8Fuses & m_fuses;

	bool m_resetFlag;
	float m_time; // in 1µs
	unsigned int m_prescaler;
	unsigned int m_wdtcrLast;
	int m_wdce_timer;

	inline void mcuReset();

private:
	AVR8WatchdogTimer();
};

#endif // AVR8WATCHDOGTIMER_H
