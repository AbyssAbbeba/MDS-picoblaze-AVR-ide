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

#ifndef AVR8PROGRAMMEMORY_H
#define AVR8PROGRAMMEMORY_H

#include "../MCUSim.h"

#include <cstddef>
#include <cstdint>

class AVR8ProgramMemory : public MCUSim::Memory {

public:
	AVR8ProgramMemory(MCUSim::EventLogger * eventLogger);
	~AVR8ProgramMemory();

	MCUSim::RetCode directRead(unsigned int addr, unsigned int data) const;
	MCUSim::RetCode directWrite(unsigned int addr, unsigned int & data);
	void resize(unsigned int newSize);
	void reset(SubsysResetMode mode);

	unsigned int size() {
		return m_size;
	}

	// This method should run as fast as possible. It has to be defined in the interface because it is mend to be inline function, and it must be usable everywhere where this header file is included.
	unsigned int read(unsigned int addr) {
		if ( addr >= m_size ) {
			logEvent(EVENT_MEM_ERR_RD_NOT_IMPLEMENTED, addr);
			return getUndefVal(16);
		}

		int result = m_memory[addr];
		if ( result & MFLAG_UNDEFINED ) {
			logEvent(EVENT_MEM_WRN_RD_UNDEFINED, addr);
		}

		result &= 0x0ffff;
		return result;
	}

	// This method should run as fast as possible. It has to be defined in the interface because it is mend to be inline function, and it must be usable everywhere where this header file is included.
	unsigned int readRaw(unsigned int addr) {
		if ( addr >= m_size ) {
			if ( 0 == m_size ) {
				logEvent(EVENT_MEM_ERR_RD_NONEXISTENT, addr);
			} else {
				logEvent(EVENT_MEM_ERR_RD_NOT_IMPLEMENTED, addr);
			}
			return getUndefVal(16);
		}

		int result = m_memory[addr];
		if ( result & MFLAG_UNDEFINED ) {
			logEvent(EVENT_MEM_WRN_RD_UNDEFINED, addr);
		}

		return result;
	}

	// This method should run as fast as possible.
	void write(unsigned int addr, unsigned int val) {
		if ( addr >= m_size ) {
			if ( 0 == m_size ) {
				logEvent(EVENT_MEM_ERR_WR_NONEXISTENT, addr);
			} else {
				logEvent(EVENT_MEM_ERR_WR_NOT_IMPLEMENTED, addr);
			}
			return;
		}

		m_memory[addr] &= (0xff000000 ^ MFLAG_UNDEFINED);
		m_memory[addr] |= val;

		logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, addr);
	}

protected:
	unsigned int * m_memory;
	unsigned int m_size;
	MCUSim::EventLogger * m_eventLogger;

private:
	AVR8ProgramMemory();
};

#endif // AVR8PROGRAMMEMORY_H
