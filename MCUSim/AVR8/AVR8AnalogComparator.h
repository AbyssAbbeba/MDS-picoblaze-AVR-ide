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

#ifndef AVR8ANALOGCOMPARATOR_H
#define AVR8ANALOGCOMPARATOR_H

class AVR8DataMemory;

#include "../MCUSim.h"

class AVR8AnalogComparator : public MCUSim::Subsys {
public:
	AVR8AnalogComparator() {};

	struct Config {
	};

	Config m_config;

	AVR8AnalogComparator * link(
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

#endif // AVR8ANALOGCOMPARATOR_H
