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
 * @file AVR8Twi.h
 */

#ifndef AVR8TWI_H
#define AVR8TWI_H

class AVR8DataMemory;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8Twi
 */
class AVR8Twi : public MCUSim::Subsys {
public:
	AVR8Twi() {};

	struct Config {
	};

	Config m_config;

	AVR8Twi * link(
		MCUSim::EventLogger	* eventLogger,
		AVR8DataMemory		* dataMemory);

	void reset(MCUSim::ResetMode mode);

// 	void clockCycles(unsigned int numberOf);
// 	void timeStep(float timeStep, unsigned int clockCycles = 0);

protected:
	AVR8DataMemory * m_dataMemory;

	inline void resetToInitialValues();
	inline void mcuReset();
};

#endif // AVR8TWI_H
