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

#ifndef AVR8PARALELPROG_H
#define AVR8PARALELPROG_H

class AVR8ProgramMemory;

#include "../MCUSim.h"

class AVR8ParalelProg : public MCUSim::Subsys {
public:
	AVR8ParalelProg() {};

	struct Config {
	};

	Config m_config;

	AVR8ParalelProg * link(
		MCUSim::EventLogger	* eventLogger,
		AVR8ProgramMemory	* programMemory);

	void reset(MCUSim::ResetMode mode);

// 	void clockCycles(unsigned int numberOf);
// 	void timeStep(float timeStep, unsigned int clockCycles = 0);

protected:
	AVR8ProgramMemory * m_programMemory;

	inline void resetToInitialValues();
	inline void mcuReset();
};

#endif // AVR8PARALELPROG_H
