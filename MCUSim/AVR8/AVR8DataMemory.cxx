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

#include "AVR8DataMemory.h"

AVR8DataMemory::AVR8DataMemory(MCUSim::EventLogger * eventLogger) : Memory(eventLogger, MCUSim::Memory::SP_CODE), m_memory(NULL), m_size(0) {
}

AVR8DataMemory::~AVR8DataMemory() {
	if ( NULL != m_memory )
	{
		delete m_memory;
	}
}

MCUSim::RetCode AVR8DataMemory::directRead(unsigned int addr, unsigned int data) const {
	return MCUSim::RC_UNKNOWN_ERROR;
}

MCUSim::RetCode AVR8DataMemory::directWrite(unsigned int addr, unsigned int & data) {
	return MCUSim::RC_UNKNOWN_ERROR;
}

void AVR8DataMemory::resize(unsigned int newSize) {
	unsigned int * memoryOrig = m_memory;
	m_memory = new unsigned int[newSize];

	unsigned int sizeToCopy = ( m_size <= newSize ) ? m_size : newSize;
	for ( unsigned int i = 0; i < sizeToCopy; i++ ) {
		m_memory[i] = memoryOrig[i];
	}

	for ( unsigned int i = sizeToCopy; i < newSize; i++ ) {
		m_memory[i] = getUndefVal(8);
	}

	if ( NULL != memoryOrig ) {
		delete memoryOrig;
	}
	m_size = newSize;
}

void AVR8DataMemory::reset(MCUSim::Subsys::SubsysResetMode mode) {
}
