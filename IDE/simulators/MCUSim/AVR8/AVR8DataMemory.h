/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @ingroup AVR8
 * @file AVR8DataMemory.h
 */

#ifndef AVR8DATAMEMORY_H
#define AVR8DATAMEMORY_H

class DataFile;

#include "AVR8RegNames.h"
#include "../MCUSim.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8DataMemory
 */
class AVR8DataMemory : public MCUSim::Memory {

public:
	AVR8DataMemory();
	~AVR8DataMemory();

	enum Event {
		EVENT_DMEM_STACK_OVERFLOW = EVENT_MEM__MAX__,
		EVENT_DMEM_STACK_UNDERFLOW,
	};

	struct Config {
		Config();
		~Config();

		int m_undefinedValue; // -1 means random
		unsigned int m_regFileSize;
		unsigned int m_sramSize;
		unsigned int m_ioRegSize;

		uint32_t * m_ioRegInitValues; // |8-bit: FLAGS|8-bit: readable|8-bit writable|8-bit: value|
		uint8_t * m_ioRegRandomInit; // |8-bit: bit mask, log. 1 means random value, 0 means keep initial value|

		unsigned int m_mem2size;
		unsigned int * m_mem2sizes;

		uint32_t ** m_ioMem2InitValues;

		unsigned int m_spWidth;
		int m_spMax;
	};

	Config m_config;

	AVR8DataMemory * link(MCUSim::EventLogger * eventLogger);

	MCUSim::RetCode directRead(unsigned int addr, unsigned int & data) const;
	MCUSim::RetCode directWrite(unsigned int addr, unsigned int data);
	void resize(unsigned int newSize);
	unsigned int size() const {
		return m_size;
	}
	void reset(MCUSim::ResetMode mode);

	void loadDataFile(const DataFile * file);
	void storeInDataFile(DataFile * file) const;

	/// @warning Do not use this method unless you REALLY KNOW WHAT YOU ARE DOING!
	inline bool readBitFast(unsigned int regAddr, unsigned int bitMask);
	/// @warning Do not use this method unless you REALLY KNOW WHAT YOU ARE DOING!
	inline void setBitFast(unsigned int regAddr, unsigned int bitMask);
	/// @warning Do not use this method unless you REALLY KNOW WHAT YOU ARE DOING!
	inline void clearBitFast(unsigned int regAddr, unsigned int bitMask);
	/// @warning Do not use this method unless you REALLY KNOW WHAT YOU ARE DOING!
	inline unsigned int readFast(unsigned int addr);
	/// @warning Do not use this method unless you REALLY KNOW WHAT YOU ARE DOING!
	inline void writeFast(unsigned int addr, unsigned int val);

	inline unsigned int read(uint32_t addr);
	inline void write(uint32_t addr, unsigned int val);
	inline void pushOnStack(const unsigned int value);
	inline unsigned int popFromStack();

	inline unsigned int getUndefVal() const;

protected:
	uint32_t * m_memory;
	uint32_t ** m_memory2;
	unsigned int m_size;

	inline void resetToInitialValues();
	inline void loadConfig();
	inline void mcuReset();
};

// -----------------------------------------------------------------------------
// INLINE FUNCTIONS
// -----------------------------------------------------------------------------

inline bool AVR8DataMemory::readBitFast(unsigned int regAddr, unsigned int bitMask) {
	return bool(m_memory[regAddr] & bitMask);
}

inline void AVR8DataMemory::setBitFast(unsigned int regAddr, unsigned int bitMask) {
	m_memory[regAddr] |= bitMask;
	logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, regAddr);
}

inline void AVR8DataMemory::clearBitFast(unsigned int regAddr, unsigned int bitMask) {
	m_memory[regAddr] &= ~bitMask;
	logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, regAddr);
}

inline unsigned int AVR8DataMemory::readFast(unsigned int addr) {
	return (m_memory[addr] & 0xff);
}

inline void AVR8DataMemory::writeFast(unsigned int addr, unsigned int val) {
	m_memory[addr] &= (0xffffff00 ^ (MFLAG_UNDEFINED | MFLAG_DEFAULT));
	m_memory[addr] |= val;
	logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, addr);
}

inline void AVR8DataMemory::write(uint32_t addr, unsigned int val) {
	unsigned int addrVariant = (addr >> 24);
	addr &= ((1 << 24) - 1);

	if ( addr >= m_size ) {
		if ( 0 == m_size ) {
			logEvent(EVENT_MEM_ERR_WR_NONEXISTENT, addr);
		} else {
			logEvent(EVENT_MEM_ERR_WR_NOT_IMPLEMENTED, addr);
		}
		return;
	}

	register uint32_t result = m_memory[addr];

	if ( result & MFLAG_VIRTUAL ) {
		if ( 0 == addrVariant ) {
			// Determinate address variant, somehow ... ??
		}

		addr = (result & 0xffff);

		if ( addr >= m_config.m_mem2size ) {
			logEvent(EVENT_MEM_ERR_WR_NOT_IMPLEMENTED, addr);
			return;
		}
		if ( addrVariant >= m_config.m_mem2sizes[addr] ) {
			logEvent(EVENT_MEM_ERR_WR_NOT_IMPLEMENTED, addr);
			return;
		}
		result = m_memory2[addr][addrVariant];
	}

	if ( 0 == (result & 0xffff00) ) {
		logEvent(EVENT_MEM_ERR_WR_NOT_IMPLEMENTED, addr);
		return;
	}
	if ( 0 == (result & 0xff00) ) {
		logEvent(EVENT_MEM_WRN_WR_READ_ONLY, addr);
		return;
	}

	if ( result & MFLAG_RESERVED ) {
		logEvent(EVENT_MEM_WRN_WR_RESERVED_WRITTEN, addr);
	}

	const uint32_t valueOrig = (result & 0xff);
	val &= ((result & 0xff00) >> 8);

	result &= (0xffffff00 ^ (MFLAG_UNDEFINED | MFLAG_DEFAULT));
	result |= val;

	if ( 0 != addrVariant ) {
		m_memory2[addr][addrVariant] = result;
	} else {
		m_memory[addr] = result;
	}

	logEvent(EVENT_MEM_INF_WR_VAL_WRITTEN, addr);

	if ( valueOrig != val) {
		logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, addr);
	}
}

inline void AVR8DataMemory::pushOnStack(const unsigned int value) {
	int sp = 0;	// Stack pointer (at most 16b)

	// Load stack pointer
	if ( m_config.m_spWidth > 8 ) {
		sp = read(AVR8RegNames::SPH);	// Stack Pointer High Register
		sp <<= 8;
	}
	sp |= read(AVR8RegNames::SPL);		// Stack Pointer Low Register

	// Push the value onto stack, post decrement scheme
	write(sp--, value);

	// Check stack pointer for allowed range
	while ( sp < 0 ) {
		sp += m_config.m_spMax;
		logEvent(EVENT_DMEM_STACK_UNDERFLOW);
	}
	while ( sp > m_config.m_spMax ) {
		sp -= m_config.m_spMax;
		logEvent(EVENT_DMEM_STACK_OVERFLOW);
	}

	// Store stack pointer
	write(AVR8RegNames::SPL, sp & 0xff);	// Stack Pointer Low Register
	if ( m_config.m_spWidth > 8 ) {
		sp &= 0xff00;
		sp >>= 8;
		write(AVR8RegNames::SPH, sp);	// Stack Pointer High Register
	}
}

inline unsigned int AVR8DataMemory::popFromStack() {
	int sp = 0;	// Stack pointer (at most 16b)

	// Load stack pointer
	if ( m_config.m_spWidth > 8 ) {
		sp = read(AVR8RegNames::SPH);	// Stack Pointer High Register
		sp <<= 8;
	}
	sp |= read(AVR8RegNames::SPL);		// Stack Pointer Low Register

	// Push the value onto stack, post decrement scheme
	unsigned int value = read(++sp);

	// Check stack pointer for allowed range
	while ( sp < 0 ) {
		sp += m_config.m_spMax;
		logEvent(EVENT_DMEM_STACK_UNDERFLOW);
	}
	while ( sp > m_config.m_spMax ) {
		sp -= m_config.m_spMax;
		logEvent(EVENT_DMEM_STACK_OVERFLOW);
	}

	// Store stack pointer
	write(AVR8RegNames::SPL, sp & 0xff);	// Stack Pointer Low Register
	if ( m_config.m_spWidth > 8 ) {
		sp &= 0xff00;
		sp >>= 8;
		write(AVR8RegNames::SPH, sp);	// Stack Pointer High Register
	}

	return value;
}

inline unsigned int AVR8DataMemory::read(uint32_t addr) {
	unsigned int addrVariant = (addr >> 24);
	addr &= ((1 << 24) - 1);

	if ( addr >= m_size ) {
		if ( 0 == m_size ) {
			logEvent(EVENT_MEM_ERR_RD_NONEXISTENT, addr);
		} else {
			logEvent(EVENT_MEM_ERR_RD_NOT_IMPLEMENTED, addr);
		}
		return getUndefVal();
	}

	register uint32_t result = m_memory[addr];

	if ( result & MFLAG_VIRTUAL ) {
		if ( 0 == addrVariant ) {
			// Determinate address variant, somehow ... ??
		}

		addr = (result & 0xffff);

		if ( addr >= m_config.m_mem2size ) {
			logEvent(EVENT_MEM_ERR_WR_NOT_IMPLEMENTED, addr);
			return getUndefVal();
		}
		if ( addrVariant >= m_config.m_mem2sizes[addr] ) {
			logEvent(EVENT_MEM_ERR_WR_NOT_IMPLEMENTED, addr);
			return getUndefVal();
		}

		result = m_memory2[addr][addrVariant];
	}

	if ( 0 == (result & 0xffff00) ) {
		logEvent(EVENT_MEM_ERR_RD_NOT_IMPLEMENTED, addr);
	}
	if ( 0 == (result & 0xff0000) ) {
		logEvent(EVENT_MEM_WRN_RD_WRITE_ONLY, addr);
	}

	if ( result & MFLAG_RESERVED ) {
		logEvent(EVENT_MEM_WRN_RD_RESERVED_READ, addr);
	}
	if ( result & MFLAG_UNDEFINED ) {
		logEvent(EVENT_MEM_WRN_RD_UNDEFINED, addr);
	}
	if ( result & MFLAG_DEFAULT ) {
		logEvent(EVENT_MEM_WRN_RD_DEFAULT, addr);
	}

	logEvent(EVENT_MEM_INF_WR_VAL_READ, addr);

	if ( 0 != (result & 0xff0000) ) {
		result ^= ((result >> 16) & 0xff) & getUndefVal();
	}

	result &= 0x0ff;
	return result;
}

inline unsigned int AVR8DataMemory::getUndefVal() const {
	if ( -1 == m_config.m_undefinedValue ) {
		// Generate random value
		return ( (unsigned int)rand() & ((1 << 8) - 1) );
	} else {
		// Return predefined value
		return ( m_config.m_undefinedValue & ((1 << 8) - 1) );
	}
}

#endif // AVR8DATAMEMORY_H
