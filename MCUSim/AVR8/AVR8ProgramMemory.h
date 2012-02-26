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
#include "AVR8FusesAndLocks.h"

#include <cstddef>
#include <cstdint>

class AVR8ProgramMemory : public MCUSim::Memory {

public:
	AVR8ProgramMemory(MCUSim::EventLogger * eventLogger, AVR8FusesAndLocks & fuses);
	~AVR8ProgramMemory();

	struct Config {
		int m_undefinedValue; // -1 means random
		unsigned int m_size;

		unsigned int m_pageSize; // 32 WORDS on ATmega8
		int bootResetAddress[4];
		unsigned int rwwSectionSize; // number of PAGES
	};

	Config m_config;

	MCUSim::RetCode directRead(unsigned int addr, unsigned int & data) const;
	MCUSim::RetCode directWrite(unsigned int addr, unsigned int data);
	void resize(unsigned int newSize);
	void reset(SubsysResetMode mode);

	inline unsigned int size() const;
	inline unsigned int readRaw(unsigned int addr);
	inline unsigned int read(unsigned int addr);
	inline void write(unsigned int addr, unsigned int val);
	template<unsigned int sizeBits> unsigned int getUndefVal() const;
	inline unsigned int getBootSectionAddress() const;

protected:
	uint32_t * m_memory;
	unsigned int m_size;
	AVR8FusesAndLocks & m_fusesAndLocks;

	inline void resetToInitialValues();
	inline void loadConfig();

private:
	AVR8ProgramMemory();
};


// -----------------------------------------------------------------------------
// INLINE FUNCTIONS
// -----------------------------------------------------------------------------

inline unsigned int AVR8ProgramMemory::size() const {
	return m_size;
}

inline unsigned int AVR8ProgramMemory::readRaw(unsigned int addr) {
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

inline unsigned int AVR8ProgramMemory::read(unsigned int addr) {
	return (readRaw(addr) & 0x0ffff);
}

inline void AVR8ProgramMemory::write(unsigned int addr, unsigned int val) {
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

inline unsigned int AVR8ProgramMemory::getBootSectionAddress() const {
	if ( true == m_fusesAndLocks[AVR8FusesAndLocks::FUSE_BOOTSZ1] ) {
		if ( true == m_fusesAndLocks[AVR8FusesAndLocks::FUSE_BOOTSZ0] ) {
			return m_config.bootResetAddress[3]; // BOOTSZ1 = 1, BOOTSZ0 = 1
		} else {
			return m_config.bootResetAddress[2]; // BOOTSZ1 = 1, BOOTSZ0 = 0
		}
	} else {
		if ( true == m_fusesAndLocks[AVR8FusesAndLocks::FUSE_BOOTSZ0] ) {
			return m_config.bootResetAddress[1]; // BOOTSZ1 = 0, BOOTSZ0 = 1
		} else {
			return m_config.bootResetAddress[0]; // BOOTSZ1 = 0, BOOTSZ0 = 0
		}
	}
}

#endif // AVR8PROGRAMMEMORY_H
