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
 * @file AVR8Sim.h
 */

#ifndef AVR8SIM_H
#define AVR8SIM_H

class AVR8Config;
class AVR8InstructionSet;
class AVR8ProgramMemory;
class AVR8DataMemory;
class AVR8FusesAndLocks;
class AVR8InterruptController;
class AVR8TimerCounter0;
class AVR8TimerCounter1;
class AVR8TimerCounter2;
class AVR8IO;
class AVR8ExternalInterrupts;
class AVR8DataEEPROM;
class AVR8BootLoader;
class AVR8WatchdogTimer;
class AVR8SystemControl;
class AVR8ClockControl;
class AVR8Spi;
class AVR8Usart;
class AVR8Twi;
class AVR8Adc;
class AVR8AnalogComparator;
class AVR8Isp;
class AVR8ParallelProg;

#include "../MCUSim.h"
#include "AVR8RegNames.h"

#include <vector>

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8Sim
 */
class AVR8Sim : public MCUSim {
	friend class AVR8Config;

public:
	AVR8Sim();
	~AVR8Sim();

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
	enum HaltMode {
		HALTM_NONE,
		HALTM_BREAK,
		HALTM_PROG
	};

	Clock::ClockSource & getClockSource();

	void reset(ResetMode mode);
	float cycles2time(int numOfCycles) {
		return ( numOfCycles * m_clockPeriod );
	}
	int executeInstruction();
	int timeStep(float timeStep);

	Subsys * getSubsys(Subsys::SubsysId id);
	Config & getConfig();
	EventLogger * getLog() {
		return m_eventLogger;
	}

	Family family() const;
	Arch arch() const;
	Mode mode() const {
		return m_processorMode;
	}
	const char * name() const;

	SleepMode sleepMode() const {
		return m_sleepMode;
	}

// 	#ifdef SWIG DEBUG
		AVR8ProgramMemory & getProgMem();
// 	#endif

protected:
	AVR8Config * m_config;

	AVR8InstructionSet * m_instructionSet;
	AVR8ProgramMemory * m_programMemory;
	AVR8DataMemory * m_dataMemory;
	AVR8FusesAndLocks * m_fusesAndLocks;
	AVR8InterruptController * m_interrupts;
	AVR8TimerCounter0 * m_timerCounter0;
	AVR8TimerCounter1 * m_timerCounter1;
	AVR8TimerCounter2 * m_timerCounter2;
	AVR8IO * m_io;
	AVR8ExternalInterrupts * m_externalInterrupts;
	AVR8DataEEPROM * m_dataEEPROM;
	AVR8BootLoader * m_bootLoader;
	AVR8WatchdogTimer * m_watchdogTimer;
	AVR8SystemControl * m_systemControl;
	AVR8ClockControl * m_clockControl;
	AVR8Spi * m_spi;
	AVR8Usart * m_usart;
	AVR8Twi * m_twi;
	AVR8Adc * m_adc;
	AVR8AnalogComparator * m_acomp;
	AVR8Isp * m_isp;
	AVR8ParallelProg * m_pprog;

	EventLogger * m_eventLogger;

	Mode m_processorMode;
	HaltMode m_haltMode;
	SleepMode m_sleepMode;

	float m_clockPeriod;

	float m_time;
	int m_clockCycles;

private:
	std::vector<Subsys*> m_subSystems;

	inline void deleteSubSystems();
	inline void checkSubSystems() const;
	inline void regSubSys(Subsys * subSystem);

	inline void resetToInitialValues();
	inline void loadConfig();
	inline void mcuReset();
};

#endif // AVR8SIM_H
