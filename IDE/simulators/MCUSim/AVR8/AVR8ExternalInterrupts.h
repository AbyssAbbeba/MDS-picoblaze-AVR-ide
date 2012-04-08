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
 * @file AVR8ExternalInterrupts.h
 */

#ifndef AVR8EXTERNALINTERRUPTS_H
#define AVR8EXTERNALINTERRUPTS_H

class AVR8DataMemory;
class AVR8IO;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8ExternalInterrupts
 */
class AVR8ExternalInterrupts : public MCUSim::Subsys {
public:
	AVR8ExternalInterrupts() {};

	struct Config {
		bool m_enabled;
	};

	Config m_config;

	AVR8ExternalInterrupts * link(
		MCUSim::EventLogger	* eventLogger,
		AVR8DataMemory		* dataMemory,
		AVR8IO			* io);

	void reset(MCUSim::ResetMode mode);
	bool enabled() {
		return m_config.m_enabled;
	}

	// This has to be called even in a sleep mode (watchdog clock).
	// This method cannot read I/O backwards, it has to be called upon any change on I/O.
	void clockCycle();

protected:
	AVR8DataMemory * m_dataMemory;
	AVR8IO * m_io;

	bool m_int0prev;
	bool m_int1prev;

	inline void mcuReset();
};

#endif // AVR8EXTERNALINTERRUPTS_H
