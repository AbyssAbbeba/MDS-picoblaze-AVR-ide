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

#include "AVR8Config.h"

AVR8Config::AVR8Config() : MCUSim::Config(MCUSim::ARCH_AVR8) {
	isLinked = false;

	m_configIO			= NULL;
	m_configBootLoader		= NULL;
	m_configDataMemory		= NULL;
	m_configDataEEPROM		= NULL;
	m_configClockControl		= NULL;
	m_configInstructionSet		= NULL;
	m_configProgramMemory		= NULL;
	m_configFusesAndLocks		= NULL;
	m_configTimerCounter0		= NULL;
	m_configTimerCounter1		= NULL;
	m_configTimerCounter2		= NULL;
	m_configWatchdogTimer		= NULL;
	m_configSystemControl		= NULL;
	m_configExternalInterrupts	= NULL;
	m_configInterruptController	= NULL;
	m_configSpi			= NULL;
	m_configUsart			= NULL;
	m_configTwi			= NULL;
	m_configAdc			= NULL;
	m_configAcomp			= NULL;
	m_configIsp			= NULL;
	m_configPprog			= NULL;
}

void AVR8Config::link(AVR8Sim * system) {
	m_configIO			= & ( system -> m_io			-> m_config );
	m_configBootLoader		= & ( system -> m_bootLoader		-> m_config );
	m_configDataMemory		= & ( system -> m_dataMemory		-> m_config );
	m_configDataEEPROM		= & ( system -> m_dataEEPROM		-> m_config );
	m_configClockControl		= & ( system -> m_clockControl		-> m_config );
	m_configInstructionSet		= & ( system -> m_instructionSet	-> m_config );
	m_configProgramMemory		= & ( system -> m_programMemory		-> m_config );
	m_configFusesAndLocks		= & ( system -> m_fusesAndLocks		-> m_config );
	m_configTimerCounter0		= & ( system -> m_timerCounter0		-> m_config );
	m_configTimerCounter1		= & ( system -> m_timerCounter1		-> m_config );
	m_configTimerCounter2		= & ( system -> m_timerCounter2		-> m_config );
	m_configWatchdogTimer		= & ( system -> m_watchdogTimer		-> m_config );
	m_configSystemControl		= & ( system -> m_systemControl		-> m_config );
	m_configExternalInterrupts	= & ( system -> m_externalInterrupts	-> m_config );
	m_configInterruptController	= & ( system -> m_interrupts		-> m_config );
	m_configSpi			= & ( system -> m_spi			-> m_config );
	m_configUsart			= & ( system -> m_usart			-> m_config );
	m_configTwi			= & ( system -> m_twi			-> m_config );
	m_configAdc			= & ( system -> m_adc			-> m_config );
	m_configAcomp			= & ( system -> m_acomp			-> m_config );
	m_configIsp			= & ( system -> m_isp			-> m_config );
	m_configPprog			= & ( system -> m_pprog			-> m_config );
}
