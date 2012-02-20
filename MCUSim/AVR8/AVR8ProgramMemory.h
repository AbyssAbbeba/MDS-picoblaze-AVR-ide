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

class AVR8Fuses;

#include <cstddef>
#include <cstdint>

class AVR8ProgramMemory : public MCUSim::Memory {

public:
	AVR8ProgramMemory(MCUSim::EventLogger * eventLogger, AVR8Fuses & fuses);
	~AVR8ProgramMemory();

	struct Config {
		int m_undefinedValue; // -1 means random
		int bootResetAddress[4];
		unsigned int m_size;
	};

	Config m_config;

	MCUSim::RetCode directRead(unsigned int addr, unsigned int & data) const;
	MCUSim::RetCode directWrite(unsigned int addr, unsigned int data);
	void resize(unsigned int newSize);
	void reset(SubsysResetMode mode);

	unsigned int size() {
		return m_size;
	}

	unsigned int readRaw(unsigned int addr) {
		if ( addr >= m_size ) {
			if ( 0 == m_size ) {
				logEvent(EVENT_MEM_ERR_RD_NONEXISTENT, addr);
			} else {
				logEvent(EVENT_MEM_ERR_RD_NOT_IMPLEMENTED, addr);
			}
			return getUndefVal<16>();
		}

		int result = m_memory[addr];
		if ( result & MFLAG_UNDEFINED ) {
			logEvent(EVENT_MEM_WRN_RD_UNDEFINED, addr);
		}

		return result;
	}

	unsigned int read(unsigned int addr) {
		return (readRaw(addr) & 0x0ffff);
	}

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

	template<unsigned int sizeBits> unsigned int getUndefVal() const;

	unsigned int getBootSectionAddress() const;

protected:
	uint32_t * m_memory;
	unsigned int m_size;
	AVR8Fuses & m_fuses;

	inline void resetToInitialValues();
	inline void loadConfig();

private:
	AVR8ProgramMemory();
};

#endif // AVR8PROGRAMMEMORY_H
