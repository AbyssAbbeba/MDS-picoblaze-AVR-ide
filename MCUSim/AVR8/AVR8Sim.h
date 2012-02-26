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
class AVR8FusesAndLocks;
class AVR8InterruptControler;
class AVR8TimerCounter0;
class AVR8IO;
class AVR8ExternalInterrupts;
class AVR8WatchdogTimer;
class AVR8DataEEPROM;

#include "../MCUSim.h"
#include "AVR8RegNames.h"

class AVR8Sim : public MCUSim {

public:
	AVR8Sim();

	enum SleepMode {
		/// MCU is not in a sleep mode
		SLEEPMD_NONE		= -1,

		/// SM2=0, SM1=0, SM0=0, mode: Idle
		SLEEPMD_IDLE		= 0,
		
		/// SM2=0, SM1=0, SM0=1, mode: ADC Noise Reduction
		SLEEPMD_ADC_NR		= (AVR8RegNames::MCUCR_SM0),
		
		/// SM2=0, SM1=1, SM0=0, mode: Power-down
		SLEEPMD_POWER_DOWN	= (AVR8RegNames::MCUCR_SM1),
		
		/// SM2=0, SM1=1, SM0=1, mode: Power-save
		SLEEPMD_POWER_SAVE	= (AVR8RegNames::MCUCR_SM1 | AVR8RegNames::MCUCR_SM0),
		
		/// SM2=1, SM1=0, SM0=0, mode: (Reserved)
		SLEEPMD_RESERVED0	= (AVR8RegNames::MCUCR_SM2),
		
		/// SM2=1, SM1=0, SM0=1, mode: (Reserved)
		SLEEPMD_RESERVED1	= (AVR8RegNames::MCUCR_SM2 | AVR8RegNames::MCUCR_SM0),
		
		/// SM2=1, SM1=1, SM0=0, mode: Standby
		SLEEPMD_STANDBY		= (AVR8RegNames::MCUCR_SM2 | AVR8RegNames::MCUCR_SM1),
		
		/// Invalid value in SM2:SM0 bits
		SLEEPMD_INVALID		= (AVR8RegNames::MCUCR_SM2 | AVR8RegNames::MCUCR_SM1 | AVR8RegNames::MCUCR_SM0)
	};

	Subsys * getSubsys(Subsys::SubsysId id);
	RetCode setConfig(Config * newConfig);
	EventLogger * getLog();
	Arch arch();
	Mode mode();

	SleepMode sleepMode() const {
		return m_sleepMode;
	}

protected:
	AVR8InstructionSet * m_instructionSet;
	AVR8ProgramMemory * m_programMemory;
	AVR8DataMemory * m_dataMemory;
	AVR8FusesAndLocks * m_fusesAndLocks;
	AVR8InterruptControler * m_interrupts;
	AVR8TimerCounter0 * m_timerCounter0;
	AVR8IO * m_io;
	AVR8ExternalInterrupts * m_externalInterrupts;
	AVR8WatchdogTimer * m_watchdogTimer;
	AVR8DataEEPROM * m_dataEEPROM;

	EventLogger * m_eventLogger;

	Mode m_processorMode;
	SleepMode m_sleepMode;
};

#endif // AVR8SIM_H
