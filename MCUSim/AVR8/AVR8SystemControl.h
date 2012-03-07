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


#ifndef AVR8SYSTEMCONTROL_H
#define AVR8SYSTEMCONTROL_H

class AVR8DataMemory;
class AVR8InterruptController;
class AVR8WatchdogTimer;
class AVR8IO;
class AVR8FusesAndLocks;

#include "../MCUSim.h"

class AVR8SystemControl : public MCUSim::Subsys {
public:
	AVR8SystemControl();
	AVR8SystemControl(
		MCUSim::EventLogger	* eventLogger,
		AVR8DataMemory		* dataMemory,
		AVR8InterruptController	* interruptController,
		AVR8WatchdogTimer	* watchdogTimer,
		AVR8IO			* io,
		AVR8FusesAndLocks	* fusesAndLocks);

	enum Event {
		EVENT_EXT_RESET_INVOKED,
		EVENT_EXT_RESET_START,
		EVENT_EXT_RESET_CANCELED,
		EVENT_EXT_RESET_UNSTABLE,

		EVENT_BOD_RESET_TMR_START,
		EVENT_BOD_RESET_BEGIN,
		EVENT_BOD_RESET_END,

		EVENT_POWER_ON_RESET_BEGIN,
		EVENT_POWER_ON_RESET_END,

		EVENT__MAX__
	};

	struct Config {
		float m_resetTresholdLow;
		float m_resetTresholdHigh;
		float m_minResetPulseWidth;

		float m_powerOnResetTresholdFalling;
		float m_powerOnResetTresholdRaising;

		float m_tBOD; // t_BOD: Minimum low voltage period for Brown-out Detection (see manual for details).
		float m_brownOutTreshold[2]; // [0] for bodlevel 0, [1] for bodlevel 1

		bool m_hasPowerOnReset;
		bool m_hasBrownOutReset;
		bool m_hasExternalReset;
		bool m_hasWatchDog;
	};

	Config m_config;

	void reset(SubsysResetMode mode);
	void timeStep(float timeStep, unsigned int clockCycles);

	void watchDogReset();

protected:
	AVR8DataMemory * m_dataMemory;
	AVR8InterruptController * m_interruptController;
	AVR8WatchdogTimer * m_watchdogTimer;
	AVR8IO * m_io;
	AVR8FusesAndLocks & m_fusesAndLocks;

	float m_rstTime;
	float m_brownOutTimer;

	bool m_resetInProgress;
	bool m_powerOnReset;
	bool m_brownOutReset;

	inline void handleExternalReset(const float timeStep);
	inline void handleBrownOutReset(const float timeStep);
	inline void handlePowerOnReset();
	
	inline void mcuReset();
};

#endif // AVR8SYSTEMCONTROL_H
