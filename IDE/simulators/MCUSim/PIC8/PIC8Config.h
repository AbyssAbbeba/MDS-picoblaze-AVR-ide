/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @ingroup PIC8
 * @file PIC8Config.h
 */

#ifndef PIC8CONFIG_H
#define PIC8CONFIG_H

#include "../MCUSim.h"
#include "PIC8Sim.h"
#include "PIC8InstructionSet.h"
#include "PIC8ProgramMemory.h"
#include "PIC8DataMemory.h"
#include "PIC8ConfigWord.h"
#include "PIC8IO.h"
#include "PIC8Stack.h"
#include "PIC8ClockControl.h"

#include <string>

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8Config
 */
class PIC8Config : public MCUSim::Config {
public:
	PIC8Config();

	PIC8InstructionSet::Config	* m_configInstructionSet;
	PIC8ProgramMemory::Config	* m_configProgramMemory;
	PIC8DataMemory::Config		* m_configDataMemory;
	PIC8ConfigWord::Config		* m_configConfigWord;
	PIC8IO::Config			* m_configIO;
	PIC8ClockControl::Config	* m_configClockControl;
	PIC8Stack::Config		* m_configStack;

	MCUSim::Family m_family;
	std::string m_name;

	void link(PIC8Sim * system);
	bool linked() const {
		return isLinked;
	}

private:
	bool isLinked;
};

#endif // PIC8CONFIG_H
