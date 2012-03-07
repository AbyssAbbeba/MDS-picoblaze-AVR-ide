/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 *
 */

#include "AVR8Sim.h"

#include "AVR8InstructionSet.h"
#include "AVR8ProgramMemory.h"
#include "AVR8DataMemory.h"
#include "AVR8FusesAndLocks.h"
#include "AVR8InterruptController.h"
#include "AVR8TimerCounter0.h"
#include "AVR8IO.h"
#include "AVR8ExternalInterrupts.h"
#include "AVR8DataEEPROM.h"
#include "AVR8BootLoader.h"
#include "AVR8WatchdogTimer.h"
#include "AVR8SystemControl.h"
#include "AVR8ClockControl.h"

#include <cassert>

AVR8Sim::AVR8Sim() : MCUSim() {
	m_eventLogger		= new EventLogger;

	m_interrupts		= new AVR8InterruptController();
	m_fusesAndLocks		= new AVR8FusesAndLocks();
	m_programMemory		= new AVR8ProgramMemory();
	m_dataMemory		= new AVR8DataMemory();
	m_dataEEPROM		= new AVR8DataEEPROM();
	m_bootLoader		= new AVR8BootLoader();
	m_instructionSet	= new AVR8InstructionSet();
	m_timerCounter0		= new AVR8TimerCounter0();
	m_io			= new AVR8IO();
	m_externalInterrupts	= new AVR8ExternalInterrupts();
	m_watchdogTimer		= new AVR8WatchdogTimer();
	m_systemControl		= new AVR8SystemControl();
	m_clockControl		= new AVR8ClockControl();

// 	m_interrupts->setup(m_eventLogger, m_instructionSet, m_programMemory, m_dataMemory, m_fusesAndLocks, &m_sleepMode, m_bootLoader);
// 	*m_fusesAndLocks		= AVR8FusesAndLocks(m_eventLogger);
// 	*m_programMemory		= AVR8ProgramMemory(m_eventLogger, m_bootLoader);
// 	*m_dataMemory		= AVR8DataMemory(m_eventLogger);
// 	*m_dataEEPROM		= AVR8DataEEPROM(m_eventLogger, m_dataMemory, m_interrupts);
// 	*m_bootLoader		= AVR8BootLoader(m_eventLogger, m_programMemory, m_dataMemory, m_fusesAndLocks, m_dataEEPROM, m_instructionSet, m_haltMode);
// 	*m_instructionSet	= AVR8InstructionSet(m_eventLogger, m_programMemory, m_dataMemory, &m_processorMode, &m_sleepMode, m_fusesAndLocks, m_interrupts, m_systemControl, &m_haltMode, m_bootLoader);
// 	*m_timerCounter0		= AVR8TimerCounter0(m_eventLogger, Subsys::ID_COUNTER_0, m_dataMemory, m_io);
// 	*m_io			= AVR8IO(m_eventLogger, m_dataMemory);
// 	*m_externalInterrupts	= AVR8ExternalInterrupts(m_eventLogger, m_dataMemory, m_io);
// 	*m_watchdogTimer		= AVR8WatchdogTimer(m_eventLogger, m_dataMemory, m_interrupts, m_fusesAndLocks);
// 	*m_systemControl		= AVR8SystemControl(m_eventLogger, m_dataMemory, m_interrupts, m_watchdogTimer, m_io, m_fusesAndLocks);
// 	*m_clockControl		= AVR8ClockControl(m_eventLogger, *m_fusesAndLocks);

	checkSubSystems();
}

AVR8Sim::~AVR8Sim() {
	deleteSubSystems();
}

inline void AVR8Sim::deleteSubSystems() {
	for (
		std::vector<Subsys*>::iterator i = m_subSystems.begin();
		i != m_subSystems.end();
		i++
	) {
		delete *i;
	}
}

inline void AVR8Sim::checkSubSystems() const {
	for (
		std::vector<Subsys*>::const_iterator i = m_subSystems.begin();
		i != m_subSystems.end();
		i++
	) {
		assert ( Subsys::ID_INVALID != (*i)->getId() );
	}
}

inline void AVR8Sim::regSubSys(Subsys * subSystem) {
	m_subSystems.push_back(subSystem);
}

MCUSim::Subsys * AVR8Sim::getSubsys(Subsys::SubsysId id) {
	switch ( id ) {
		case Subsys::ID_MEM_CODE:	return m_programMemory;
		case Subsys::ID_MEM_DATA:	return m_dataMemory;
		case Subsys::ID_MEM_EEPROM:	return m_dataEEPROM;

		case Subsys::ID_CPU:		return m_instructionSet;
		case Subsys::ID_FUSES:		return m_fusesAndLocks;
		case Subsys::ID_INTERRUPTS:	return m_interrupts;
		case Subsys::ID_EXT_INT:	return m_externalInterrupts;
		case Subsys::ID_WATCHDOG:	return m_watchdogTimer;
		case Subsys::ID_IO:		return m_io;
		case Subsys::ID_BOOT_LOADER:	return m_bootLoader;
		case Subsys::ID_SYS_CONTROL:	return m_systemControl;
		case Subsys::ID_CLK_CONTROL:	return m_clockControl;
		case Subsys::ID_COUNTER_0:	return m_timerCounter0;

		default:
			return NULL;
	}
}

MCUSim::RetCode AVR8Sim::setConfig(Config * newConfig) {
}

MCUSim::EventLogger * AVR8Sim::getLog() {
	return m_eventLogger;
}

MCUSim::Arch AVR8Sim::arch() {
	return m_arch;
}

MCUSim::Mode AVR8Sim::mode() {
	return m_processorMode;
}

float AVR8Sim::cycles2time(int numOfCycles) {

}

int AVR8Sim::executeInstruction() {

}

int AVR8Sim::timeStep(float time) {
	
}


