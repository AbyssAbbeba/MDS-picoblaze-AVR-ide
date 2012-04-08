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
 * @file AVR8Adc.h
 */

#ifndef AVR8ADC_H
#define AVR8ADC_H

class AVR8DataMemory;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8Adc
 */
class AVR8Adc : public MCUSim::Subsys {
public:
	AVR8Adc() {};

	struct Config {
		bool m_enabled;
	};

	Config m_config;

	AVR8Adc * link(
		MCUSim::EventLogger	* eventLogger,
		AVR8DataMemory		* dataMemory);

	void reset(MCUSim::ResetMode mode);
	bool enabled() {
		return m_config.m_enabled;
	}

// 	void clockCycles(unsigned int numberOf);
// 	void timeStep(float timeStep, unsigned int clockCycles = 0);

protected:
	AVR8DataMemory * m_dataMemory;

	inline void resetToInitialValues();
	inline void mcuReset();
};

#endif // AVR8ADC_H
