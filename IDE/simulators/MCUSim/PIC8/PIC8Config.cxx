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

#include "PIC8Config.h"

PIC8Config::PIC8Config() : MCUSim::Config(MCUSim::ARCH_PIC8) {
	isLinked = false;

	m_configIO		= NULL;
	m_configDataMemory	= NULL;
	m_configClockControl	= NULL;
	m_configInstructionSet	= NULL;
	m_configProgramMemory	= NULL;
	m_configWord		= NULL;
	m_configStack		= NULL;
}

void PIC8Config::link(PIC8Sim * system) {
	m_configIO			= & ( system -> m_io			-> m_config );
	m_configDataMemory		= & ( system -> m_dataMemory		-> m_config );
	m_configClockControl		= & ( system -> m_clockControl		-> m_config );
	m_configInstructionSet		= & ( system -> m_instructionSet	-> m_config );
	m_configProgramMemory		= & ( system -> m_programMemory		-> m_config );
	m_configConfigWord		= & ( system -> m_configWord		-> m_config );
	m_configStack			= & ( system -> m_stack );
}
