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
 * @file AVR8Isp.h
 */

#ifndef AVR8ISP_H
#define AVR8ISP_H

class AVR8ProgramMemory;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8Isp
 */
class AVR8Isp : public MCUSim::Subsys {
public:
	AVR8Isp() {};

	struct Config {
		bool m_enabled;
	};

	Config m_config;

	AVR8Isp * link(
		MCUSim::EventLogger	* eventLogger,
		AVR8ProgramMemory	* programMemory);

	void reset(MCUSim::ResetMode mode);
	bool enabled() {
		return m_config.m_enabled;
	}

// 	void clockCycles(unsigned int numberOf);
// 	void timeStep(float timeStep, unsigned int clockCycles = 0);

protected:
	AVR8ProgramMemory * m_programMemory;

	inline void resetToInitialValues();
	inline void mcuReset();
};

#endif // AVR8ISP_H
