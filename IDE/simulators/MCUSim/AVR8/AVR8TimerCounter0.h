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
 * @file AVR8TimerCounter0.h
 */

#ifndef AVR8TIMERCOUNTER0_H
#define AVR8TIMERCOUNTER0_H

class AVR8DataMemory;
class AVR8IO;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8TimerCounter0
 */
class AVR8TimerCounter0 : public MCUSim::Subsys {
public:
	AVR8TimerCounter0() {};

	struct Config {
		bool m_enabled;
	};

	Config m_config;

	AVR8TimerCounter0 * link(
		MCUSim::EventLogger	* eventLogger,
		MCUSim::Subsys::SubsysId  subsysId,
		AVR8DataMemory		* dataMemory,
		AVR8IO			* m_io);

	void reset(MCUSim::ResetMode mode);
	bool enabled() {
		return m_config.m_enabled;
	}

	void clockCycles(unsigned int numberOf);

	unsigned int getPrescalerVal() const {
		return m_prescaler;
	}

protected:
	AVR8DataMemory * m_dataMemory;
	AVR8IO * m_io;

	static const unsigned int COUNTER_DELAY = 3;

	unsigned int m_incrementWithDelay[COUNTER_DELAY];
	unsigned int m_iwdIndexOut;
	unsigned int m_iwdIndexIn;
	unsigned int m_clockSource;
	unsigned int m_prescaler;
	bool m_t0Log;

	inline void incrementWithDelay(unsigned int number);
	inline unsigned int incrementPrescaler(unsigned int number, unsigned int max);
	inline void incrementTimer(unsigned int number);
	inline void resetPrescalerOnCond();
	inline void clearDelayArray();
	inline void sampleT0();

	inline void resetToInitialValues();
	inline void mcuReset();

	inline void determinateClockSource();
};

#endif // AVR8TIMERCOUNTER0_H
