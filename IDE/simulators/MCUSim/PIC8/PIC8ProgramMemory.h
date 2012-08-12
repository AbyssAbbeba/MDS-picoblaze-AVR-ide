/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @ingroup PIC8
 * @file PIC8ProgramMemory.h
 */

#ifndef PIC8PROGRAMMEMORY_H
#define PIC8PROGRAMMEMORY_H

class DataFile;

#include "../MCUSim.h"

#include <cstddef>
#include <cstdint>

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8ProgramMemory
 */
class PIC8ProgramMemory : public MCUSim::Memory {

public:
	PIC8ProgramMemory();
	~PIC8ProgramMemory();

	struct Config {
		Config() {
			m_undefinedValue = -1;
		}
		int m_undefinedValue; // -1 means random
		unsigned int m_size;
	};

	Config m_config;

	PIC8ProgramMemory * link(MCUSim::EventLogger * eventLogger);
	void reset(MCUSim::ResetMode mode);

	MCUSim::RetCode directRead(unsigned int addr, unsigned int & data) const;
	MCUSim::RetCode directWrite(unsigned int addr, unsigned int data);
	void resize(unsigned int newSize);

	void loadDataFile(const DataFile * file);
	void storeInDataFile(DataFile * file) const;

	inline unsigned int size() const;
	inline unsigned int readRaw(unsigned int addr);
	inline unsigned int read(unsigned int addr);
	inline void write(unsigned int addr, unsigned int val);

	unsigned int getUndefVal() const;

protected:
	uint32_t * m_memory;
	unsigned int m_size;

	inline void resetToInitialValues();
	inline void loadConfig();
	inline void mcuReset();
};


// -----------------------------------------------------------------------------
// INLINE FUNCTIONS
// -----------------------------------------------------------------------------

inline unsigned int PIC8ProgramMemory::size() const {
	return m_size;
}

inline unsigned int PIC8ProgramMemory::readRaw(unsigned int addr) {
	if ( addr >= m_size ) {
		if ( 0 == m_size ) {
			logEvent(EVENT_MEM_ERR_RD_NONEXISTENT, addr);
		} else {
			logEvent(EVENT_MEM_ERR_RD_NOT_IMPLEMENTED, addr);
		}
		return getUndefVal();
	}

	int result = m_memory[addr];
	if ( result & MFLAG_UNDEFINED ) {
		logEvent(EVENT_MEM_WRN_RD_UNDEFINED, addr);
	}

	return result;
}

inline unsigned int PIC8ProgramMemory::read(unsigned int addr) {
	return (readRaw(addr) & 0x0ffff);
}

inline void PIC8ProgramMemory::write(unsigned int addr, unsigned int val) {
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

#endif // PIC8PROGRAMMEMORY_H
