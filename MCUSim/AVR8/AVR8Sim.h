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

#ifndef AVR8SIM_H
#define AVR8SIM_H

class AVR8InstructionSet;
class AVR8ProgramMemory;
class AVR8DataMemory;
class AVR8Fuses;
class AVR8InterruptControler;

#include "../MCUSim.h"

class AVR8Sim : public MCUSim {

public:
	AVR8Sim();

	Subsys * getSubsys(Subsys::SubsysId id);
	RetCode setConfig(Config * newConfig);
	EventLogger * getLog();
	Arch arch();
	Mode mode();

protected:
	AVR8InstructionSet * m_cpuCore;
	AVR8ProgramMemory * m_programMemory;
	AVR8DataMemory * m_dataMemory;
	AVR8Fuses * m_fuses;
	AVR8InterruptControler * m_interrupts;

	EventLogger * m_eventLogger;
	Mode m_processorMode;
};

#endif // AVR8SIM_H
