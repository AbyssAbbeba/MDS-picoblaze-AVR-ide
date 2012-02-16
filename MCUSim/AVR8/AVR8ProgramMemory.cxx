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

AVR8ProgramMemory::AVR8ProgramMemory(MCUSim::EventLogger * eventLogger) : Memory(eventLogger, MCUSim::Memory::SP_CODE), m_memory(NULL), m_size(0) {
}

AVR8ProgramMemory::~AVR8ProgramMemory() {
	if ( NULL != m_memory ) {
		delete m_memory;
	}
}

MCUSim::RetCode AVR8ProgramMemory::directRead(unsigned int addr, unsigned int data) const {
	// TODO: Implement
	return MCUSim::RC_UNKNOWN_ERROR;
}

MCUSim::RetCode AVR8ProgramMemory::directWrite(unsigned int addr, unsigned int & data) {
	// TODO: Implement
	return MCUSim::RC_UNKNOWN_ERROR;
}

void AVR8ProgramMemory::resize(unsigned int newSize) {
	unsigned int * memoryOrig = m_memory;
	m_memory = new unsigned int[newSize];

	unsigned int sizeToCopy = ( m_size <= newSize ) ? m_size : newSize;
	for ( unsigned int i = 0; i < sizeToCopy; i++ ) {
		m_memory[i] = memoryOrig[i];
	}

	for ( unsigned int i = sizeToCopy; i < newSize; i++ ) {
		m_memory[i] = getUndefVal(16) | MFLAG_UNDEFINED;
	}

	if ( NULL != memoryOrig ) {
		delete memoryOrig;
	}
	m_size = newSize;
}

void AVR8ProgramMemory::reset(MCUSim::Subsys::SubsysResetMode mode) {
	// TODO: Implement
}
