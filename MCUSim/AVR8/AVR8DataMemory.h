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

#ifndef AVR8DATAMEMORY_H
#define AVR8DATAMEMORY_H

#include "AVR8RegNames.h"
#include "../MCUSim.h"

#include <cstddef>
#include <cstdint>

class AVR8DataMemory : public MCUSim::Memory {

public:
	AVR8DataMemory(MCUSim::EventLogger * eventLogger);
	~AVR8DataMemory();

	MCUSim::RetCode directRead(unsigned int addr, unsigned int data) const;
	MCUSim::RetCode directWrite(unsigned int addr, unsigned int & data);
	void resize(unsigned int newSize);
	unsigned int size() {
		return m_size;
	}
	void reset(SubsysResetMode mode);

	// This method should run as fast as possible. It has to be defined in the interface because it is mend to be inline function, and it must be usable everywhere where this header file is included.
	unsigned int read(unsigned int addr) {
		// TODO: Add address variant handling.

		if ( addr >= m_size ) {
			if ( 0 == m_size ) {
				logEvent(EVENT_MEM_ERR_RD_NONEXISTENT, addr);
			} else {
				logEvent(EVENT_MEM_ERR_RD_NOT_IMPLEMENTED, addr);
			}
			return getUndefVal(8);
		}

		int result = m_memory[addr];
		if ( result & MFLAG_UNDEFINED ) {
			logEvent(EVENT_MEM_WRN_RD_UNDEFINED, addr);
		}
		if ( result & MFLAG_DEFAULT ) {
			logEvent(EVENT_MEM_WRN_RD_DEFAULT, addr);
		}

// 		logEvent(EVENT_MEM_INF_WR_VAL_READ, addr);

		result &= 0x0ff;
		return result;
	}

	bool readBit(unsigned int regAddr, unsigned int bitMask) {
		return (read(regAddr) & bitMask);
	}

	void writeBit(unsigned int regAddr, unsigned int bitMask) {
		write(regAddr, (read(regAddr) | bitMask));
	}

	// This method should run as fast as possible. It has to be defined in the interface because it is mend to be inline function, and it must be usable everywhere where this header file is included.
	void write(unsigned int addr, unsigned int val) {
		// TODO: Add address variant handling.

		if ( addr >= m_size ) {
			if ( 0 == m_size ) {
				logEvent(EVENT_MEM_ERR_WR_NONEXISTENT, addr);
			} else {
				logEvent(EVENT_MEM_ERR_WR_NOT_IMPLEMENTED, addr);
			}
			return;
		}

		const unsigned int valueOrig = m_memory[addr] & 0xff;

		m_memory[addr] &= (0xff000000 ^ MFLAG_UNDEFINED ^ MFLAG_DEFAULT);
		m_memory[addr] |= val;

// 		logEvent(EVENT_MEM_INF_WR_VAL_WRITTEN, addr);

		if ( valueOrig != val) {
			logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, addr);
		}
	}

protected:
	unsigned int * m_memory;
	unsigned int m_size;
	MCUSim::EventLogger * m_eventLogger;

private:
	AVR8DataMemory();
};

#endif // AVR8DATAMEMORY_H
