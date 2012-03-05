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

class AVR8DataMemory;
class AVR8InterruptController;
class AVR8FusesAndLocks;

#include "../MCUSim.h"

class AVR8WatchdogTimer : public MCUSim::Subsys {
public:
	AVR8WatchdogTimer(
		MCUSim::EventLogger	* eventLogger,
		AVR8DataMemory		* dataMemory,
		AVR8InterruptController	* interruptController,
		AVR8FusesAndLocks	& fuses);

	enum Event {
		EVENT_WDT_INVALID_CR_CHAGE,
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

protected:
	AVR8DataMemory * m_dataMemory;
	AVR8InterruptController * m_interruptController;
	AVR8FusesAndLocks & m_fusesAndLocks;

	float m_time; // in 1µs
	unsigned int m_prescaler;
	unsigned int m_wdtcrLast;
	int m_wdce_timer;

	inline unsigned int readWdtcr(const unsigned int clockCycles);
	inline void mcuReset();

private:
	AVR8WatchdogTimer();
};

#endif // AVR8WATCHDOGTIMER_H
