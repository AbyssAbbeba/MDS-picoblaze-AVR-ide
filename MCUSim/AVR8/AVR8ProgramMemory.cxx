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

#include "AVR8ProgramMemory.h"
#include "AVR8DataMemory.h"
#include "AVR8DataEEPROM.h"
#include "AVR8InstructionSet.h"

#include <cstdlib>

AVR8ProgramMemory::AVR8ProgramMemory(
	MCUSim::EventLogger * eventLogger,
	AVR8BootLoader * bootLoader)
	 :
	Memory(eventLogger, MCUSim::Memory::SP_CODE),
	m_bootLoader(bootLoader),
	m_memory(NULL),
	m_size(0)
{
}

AVR8ProgramMemory::~AVR8ProgramMemory() {
	if ( NULL != m_memory ) {
		delete m_memory;
	}
}

MCUSim::RetCode AVR8ProgramMemory::directRead(unsigned int addr, unsigned int & data) const {
	if ( addr >= m_size ) {
		return MCUSim::RC_ADDR_OUT_OF_RANGE;
	}
	data = m_memory[addr];

	if ( MFLAG_UNDEFINED & data ) {
		data &= 0xffff;
		return MCUSim::RC_NONDETERMINISTIC;
	} else {
		data &= 0xffff;
		return MCUSim::RC_OK;
	}
}

MCUSim::RetCode AVR8ProgramMemory::directWrite(unsigned int addr, unsigned int data) {
	if ( addr >= m_size ) {
		return MCUSim::RC_ADDR_OUT_OF_RANGE;
	}

	m_memory[addr] = (data & 0xffff);
	return MCUSim::RC_OK;
}

void AVR8ProgramMemory::resize(unsigned int newSize) {
	unsigned int * memoryOrig = m_memory;
	m_memory = new unsigned int[newSize];

	unsigned int sizeToCopy = ( m_size <= newSize ) ? m_size : newSize;
	for ( unsigned int i = 0; i < sizeToCopy; i++ ) {
		m_memory[i] = memoryOrig[i];
	}

	for ( unsigned int i = sizeToCopy; i < newSize; i++ ) {
		m_memory[i] = (getUndefVal<16>() | MFLAG_UNDEFINED);
	}

	if ( NULL != memoryOrig ) {
		delete memoryOrig;
	}
	m_size = newSize;
}

void AVR8ProgramMemory::reset(MCUSim::Subsys::SubsysResetMode mode) {
	switch ( mode ) {
		case RSTMD_NEW_CONFIG:
			loadConfig();
			break;
		case RSTMD_INITIAL_VALUES:
			resetToInitialValues();
			break;
		case RSTMD_MCU_RESET:
			mcuReset();
			break;
		default:
			// Irrelevant requests are silently ignored
			break;
	}
}

inline void AVR8ProgramMemory::loadConfig() {
	resize(m_config.m_size);
}

inline void AVR8ProgramMemory::mcuReset() {
}

inline void AVR8ProgramMemory::resetToInitialValues() {
	for ( unsigned int i = 0; i < m_size; i++ ) {
		m_memory[i] = (0xff | MFLAG_UNDEFINED);
	}
}

template<unsigned int sizeBits>
unsigned int AVR8ProgramMemory::getUndefVal() const {
	if ( -1 == m_config.m_undefinedValue ) {
		// Generate random value
		return ((unsigned int)rand() & ((1 << sizeBits) - 1));
	} else {
		// Return predefined value
		return ( m_config.m_undefinedValue & ((1 << sizeBits) - 1) );
	}
}
