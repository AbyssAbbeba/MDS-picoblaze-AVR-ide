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


#ifndef AVR8EXTERNALINTERRUPTS_H
#define AVR8EXTERNALINTERRUPTS_H

#include "../MCUSim.h"

class AVR8DataMemory;
class AVR8IO;

class AVR8ExternalInterrupts : public MCUSim::Subsys {
public:
	AVR8ExternalInterrupts(MCUSim::EventLogger * eventLogger, AVR8DataMemory * dataMemory, AVR8IO * io);

	struct Config {
	};

	Config m_config;

	void reset(SubsysResetMode mode);

	// This has to be called even in a sleep mode (watchdog clock).
	// This method cannot read I/O backwards, it has to be called upon any change on I/O.
	void clockCycle();

protected:
	AVR8DataMemory * m_dataMemory;
	AVR8IO * m_io;

	bool m_int0prev;
	bool m_int1prev;

	inline void mcuReset();

private:
	AVR8ExternalInterrupts();
};

#endif // AVR8EXTERNALINTERRUPTS_H
