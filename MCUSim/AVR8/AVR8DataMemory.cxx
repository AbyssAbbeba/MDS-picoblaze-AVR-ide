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

#include <cstdlib>
#include <cstring>

AVR8DataMemory::AVR8DataMemory(MCUSim::EventLogger * eventLogger)
	: Memory(eventLogger, MCUSim::Memory::SP_CODE),
	  m_memory(NULL),
	  m_size(0),
	  m_mem2size(0),
	  m_mem2sizes(NULL)
{
}

AVR8DataMemory::~AVR8DataMemory() {
	if ( NULL != m_memory )
	{
		delete m_memory;
	}
	if ( NULL != m_memory2 ) {
		delete[] m_memory2;
	}
	if ( NULL != m_mem2sizes ) {
		delete m_mem2sizes;
	}
}

MCUSim::RetCode AVR8DataMemory::directRead(unsigned int addr, unsigned int & data) const {
	unsigned int addrVariant = (addr >> 24);
	addr &= ((1 << 24) - 1);

	if ( addr >= m_size ) {
		return MCUSim::RC_ADDR_OUT_OF_RANGE;
	}

	register unsigned int storedData = m_memory[addr];

	if ( storedData & MFLAG_VIRTUAL ) {
		if ( 0 == addrVariant ) {
			// Determinate address variant, somehow ... ??
		}

		addr = m_memory[addr] & 0xffff;

		if ( addr >= m_mem2size ) {
			return MCUSim::RC_ADDR_OUT_OF_RANGE;
		}
		if ( addrVariant >= m_mem2sizes[addr] ) {
			return MCUSim::RC_ADDR_OUT_OF_RANGE;
		}
		storedData = m_memory2[addr][addrVariant];
	}

	storedData &= 0xff;
	data = storedData;

	return MCUSim::RC_OK;
}

MCUSim::RetCode AVR8DataMemory::directWrite(unsigned int addr, unsigned int data) {
	unsigned int addrVariant = (addr >> 24);
	addr &= ((1 << 24) - 1);

	if ( addr >= m_size ) {
		return MCUSim::RC_ADDR_OUT_OF_RANGE;
	}

	data &= 0xff;

	register unsigned int storedData = m_memory[addr];

	if ( storedData & MFLAG_VIRTUAL ) {
		if ( 0 == addrVariant ) {
			// Determinate address variant, somehow ... ??
		}

		addr = (storedData & 0xffff);

		if ( addr >= m_mem2size ) {
			return MCUSim::RC_ADDR_OUT_OF_RANGE;
		}
		if ( addrVariant >= m_mem2sizes[addr] ) {
			return MCUSim::RC_ADDR_OUT_OF_RANGE;
		}

		m_memory2[addr][addrVariant] = data;
	} else {
		m_memory[addr] = data;
	}

	return MCUSim::RC_OK;
}

void AVR8DataMemory::resize(unsigned int newSize) {
	uint32_t * memoryOrig = m_memory;
	m_memory = new uint32_t[newSize];

	unsigned int sizeToCopy = ( m_size <= newSize ) ? m_size : newSize;
	for ( unsigned int i = 0; i < sizeToCopy; i++ ) {
		m_memory[i] = memoryOrig[i];
	}

	for ( unsigned int i = sizeToCopy; i < newSize; i++ ) {
		m_memory[i] = getUndefVal<8>();
	}

	if ( NULL != memoryOrig ) {
		delete memoryOrig;
	}
	m_size = newSize;
}

void AVR8DataMemory::reset(MCUSim::Subsys::SubsysResetMode mode) {
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

inline void AVR8DataMemory::loadConfig() {
	// Set memory size
	if ( 0 == m_config.m_sramSize ) {
		// Device with no SRAM
		resize(m_config.m_regFileSize);
	} else {
		resize(32 + m_config.m_sramSize);
	}

	// Intialize IO memory, i.e. set memory cell flags
	for ( unsigned int i = 0, addr = 32; i < m_config.m_ioRegSize; i++, addr++ ) {
		m_memory[addr] &= 0xff; // Preserve only the register content, not configuration
		m_memory[addr] |= (m_config.m_ioRegInitValues[i] & 0xffffff00);
	}


	if ( 0 == m_config.m_mem2size ) {
		if ( NULL != m_memory2 ) {
			delete[] m_memory2;
		}
	} else {
		uint32_t ** memory2Orig = m_memory2;
		m_memory2 = new uint32_t * [m_config.m_mem2size];

		for ( unsigned int i = 0; i < m_config.m_mem2size; i++ ) {
			m_memory2[i] = new uint32_t [ m_config.m_mem2sizes[i] ];

			for ( unsigned int j = 0; i < m_config.m_mem2sizes[i]; j++ ) {
				if (
					( NULL != memory2Orig )
						&&
					( NULL != m_mem2sizes )
						&&
					( i < m_mem2size )
						&&
					( j < m_mem2sizes[i] )
				   )
				{
					m_memory2[i][j] = memory2Orig[i][j];
				}

				m_memory2[i][j] &= 0xff; // Preserve only the memory cell content, not its configuration
				m_memory2[i][j] |= (m_config.m_ioMem2InitValues[i][j] & 0xffffff00);
			}
		}

		if ( NULL != memory2Orig ) {
			delete[] memory2Orig;
		}
		if ( NULL != m_mem2sizes ) {
			delete m_mem2sizes;
		}

		m_mem2size = m_config.m_mem2size;
		m_mem2sizes = new unsigned int [m_config.m_mem2size];
		memcpy(m_mem2sizes, m_config.m_mem2sizes, m_config.m_mem2size);
	}
}
inline void AVR8DataMemory::resetToInitialValues() {
	// Initialize CPU register file, set all registers to zero
	for ( unsigned int i = 0; i < m_config.m_regFileSize; i++ ) {
		m_memory[i] = MFLAG_DEFAULT;
	}

	// Intialize IO memory, i.e. set memory cell flags
	mcuReset();

	// Initialize internal SRAM, set all registers to zero
	for ( unsigned int i = 0; i < m_config.m_regFileSize; i++ ) {
		m_memory[i] = MFLAG_DEFAULT;
	}
}

inline void AVR8DataMemory::mcuReset() {
	// (re-)Intialize IO memory, i.e. set memory cell flags
	for ( unsigned int i = 0, addr = 32; i < m_config.m_ioRegSize; i++, addr++ ) {
		m_memory[addr] &= 0xffffff00; // Preserve only the register configuration, not its content
		m_memory[addr] |= (m_config.m_ioRegInitValues[i] & 0xff);

		// Generate random values according to the default value initialization mask
		if ( 0 != m_config.m_ioRegRandomInit[i] ) {
			m_memory[addr] ^= ( getUndefVal<8>() & m_config.m_ioRegRandomInit[i] );
		}
	}

	for ( unsigned int i = 0; i < m_mem2size; i++ ) {
		for ( unsigned int j = 0; j < m_mem2sizes[i]; j++ ) {
			m_memory2[i][j] &= 0xffffff00; // Preserve only the register configuration, not its content
			m_memory2[i][j] |= (m_config.m_ioMem2InitValues[i][j] & 0xff);
		}
	}
}

template<unsigned int sizeBits>
unsigned int AVR8DataMemory::getUndefVal() const {
	if ( -1 == m_config.m_undefinedValue ) {
		// Generate random value
		return ( (unsigned int)random() & ((1 << sizeBits) - 1) );
	} else {
		// Return predefined value
		return ( m_config.m_undefinedValue & ((1 << sizeBits) - 1) );
	}
}
