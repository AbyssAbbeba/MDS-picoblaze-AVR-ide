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


#ifndef AVR8IO_H
#define AVR8IO_H

class AVR8DataMemory;

#include "../MCUSim.h"

class AVR8IO : public MCUSim::IO {
public:
	AVR8IO(MCUSim::EventLogger * eventLogger, AVR8DataMemory * dataMemory);

	enum PIN {
		PIN_T0, // TODO: Assign some reasonable value here.
		PIN_INT0, // TODO: Assign some reasonable value here.
		PIN_INT1, // TODO: Assign some reasonable value here.

		PIN__MAX__
	};

	struct Config {
	};

	Config m_config;

	void reset(SubsysResetMode mode);

	void timeStep(float timeStep);			// <-- Call this first,
	void clockCycles(unsigned int numberOf);	// <-- then call this (always).

	bool getLog(PIN pin) const {
		// TODO: Not implemented yet!
	}

protected:
	AVR8DataMemory * m_dataMemory;

	inline void resetToInitialValues();
	inline void mcuReset();

private:
	AVR8IO();
};

#endif // AVR8IO_H
