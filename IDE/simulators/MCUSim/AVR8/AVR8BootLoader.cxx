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

#include "AVR8BootLoader.h"
#include "AVR8ProgramMemory.h"
#include "AVR8DataMemory.h"
#include "AVR8FusesAndLocks.h"
#include "AVR8DataEEPROM.h"
#include "AVR8InstructionSet.h"

#include <cstdlib>

AVR8BootLoader::AVR8BootLoader() {
	m_writeBuffer = NULL;
}

AVR8BootLoader * AVR8BootLoader::link(
	MCUSim::EventLogger	* eventLogger,
	AVR8ProgramMemory	* programMemory,
	AVR8DataMemory		* dataMemory,
	AVR8FusesAndLocks	* fusesAndLocks,
	AVR8DataEEPROM		* dataEEPROM,
    	AVR8InstructionSet	* instructionSet,
	AVR8Sim::HaltMode	* haltMode
) {
	Subsys::link(eventLogger, ID_BOOT_LOADER);

	m_programMemory	= programMemory;
	m_dataMemory	= dataMemory;
	m_fusesAndLocks	= fusesAndLocks;
	m_dataEEPROM	= dataEEPROM;
	m_instructionSet= instructionSet;
	m_haltMode	= haltMode;

	return this;
}

AVR8BootLoader::~AVR8BootLoader() {
	if ( NULL != m_writeBuffer ) {
		delete[] m_writeBuffer;
	}
}

inline AVR8BootLoader::SpmMode AVR8BootLoader::determinateSpmMode(unsigned int spmcr) const {
	SpmMode mode = SPMMD_NONE;

	/*
	 * Writing any other combination than “10001”, “01001”, “00101”, “00011” or “00001” in the lower
	 * five bits will have no effect.
	 */
	if ( AVR8RegNames::SPMCR_PGERS & spmcr ) {
		mode = SPMMD_PGERS;
	}
	if ( AVR8RegNames::SPMCR_PGWRT & spmcr ) {
		if ( SPMMD_NONE != mode ) {
			return SPMMD_NONE;
		}
		mode = SPMMD_PGWRT;
	}
	if ( AVR8RegNames::SPMCR_BLBSET & spmcr ) {
		if ( SPMMD_NONE != mode ) {
			return SPMMD_NONE;
		}
		mode = SPMMD_BLBSET;
	}
	if ( AVR8RegNames::SPMCR_RWWSRE & spmcr ) {
		if ( SPMMD_NONE != mode ) {
			return SPMMD_NONE;
		}
		mode = SPMMD_RWWSRE;
	}

	if ( SPMMD_NONE == mode ) {
		mode = SPMMD_BUFFER;
	}

	return mode;
}

inline void AVR8BootLoader::normalizeControlReg(const unsigned int clockCycles) {
	using namespace AVR8RegNames;

	unsigned int spmcr = m_dataMemory->readFast(SPMCR);
	spmcr &= ( SPMCR_SPMEN | SPMCR_PGERS | SPMCR_PGWRT | SPMCR_BLBSET | SPMCR_RWWSRE );

	if ( m_spmcrLast == spmcr ) {
		// No change means nothing to do here
		return;
	}

	if ( (SPMCR_SPMEN & spmcr) && (false == m_writeInProgress) ) {
		// Accept the change
		m_cr_timer = 4 + clockCycles;
		m_spmcrLast = spmcr;
		m_spmMode = determinateSpmMode(spmcr);
		if ( SPMMD_NONE == m_spmMode ) {
			logEvent(EVENT_BOOT_INVALID_CR_CHAGE, AVR8RegNames::SPMCR, m_dataMemory->readFast(SPMCR));
		}
	} else {
		// Refuse the change
		spmcr = m_dataMemory->readFast(SPMCR);
		logEvent(EVENT_BOOT_INVALID_CR_CHAGE, SPMCR, spmcr);
		spmcr &= ( 0xff ^ ( SPMCR_SPMEN | SPMCR_PGERS | SPMCR_PGWRT | SPMCR_BLBSET | SPMCR_RWWSRE ) );
		spmcr |= m_spmcrLast;
		m_dataMemory->writeFast(SPMCR, spmcr);
	}
}
// Manage the timer which keeps the SPMEN, PGERS, PGWRT, BLBSET, and RWWSRE flag set for 4 clock cycles.
inline void AVR8BootLoader::manageContRegTimer(const unsigned int clockCycles) {
	using namespace AVR8RegNames;

	if ( 0 == m_cr_timer ) {
		return;
	}

	// Decrement the timer
	m_cr_timer -= clockCycles;
	if ( m_cr_timer < 0 ) {
		m_cr_timer = 0;
	}

	if ( (0 == m_cr_timer) && (false == m_writeInProgress) ) {
		// Clear the the flags (after 4 clock cycles)
		unsigned int spmcr = m_dataMemory->readFast(SPMCR);
		spmcr &= ( 0xff ^ ( SPMCR_SPMEN | SPMCR_PGERS | SPMCR_PGWRT | SPMCR_BLBSET | SPMCR_RWWSRE ) );
		m_dataMemory->writeFast(SPMCR, spmcr);
		m_spmcrLast = spmcr;
		m_spmMode = SPMMD_NONE;
	}
}
inline void AVR8BootLoader::manageProgTimer(const float timeStep) {
	using namespace AVR8RegNames;

	if ( false == m_writeInProgress ) {
		return;
	}

	m_progTimer -= timeStep;

	if ( m_progTimer <= 0 ) {
		m_writeInProgress = false;

		unsigned int spmcr = m_dataMemory->readFast(SPMCR);
		spmcr &= ( 0xff ^ ( SPMCR_SPMEN | SPMCR_PGERS | SPMCR_PGWRT | SPMCR_BLBSET | SPMCR_RWWSRE ) );
		m_dataMemory->writeFast(SPMCR, spmcr);
	}
}

void AVR8BootLoader::timeStep(float timeStep, unsigned int clockCycles) {
	normalizeControlReg(clockCycles);
	manageContRegTimer(clockCycles);
	manageProgTimer(timeStep);
}

inline float AVR8BootLoader::randomTimeInRange() const {
	if ( m_config.m_minProgTime == m_config.m_maxProgTime ) {
		return m_config.m_maxProgTime;
	} else {
		/*                   random
		 * result = min + ──────────── * ( max - min )
		 *                 random_MAX
		 */
		return m_config.m_minProgTime + ((rand() * 1.0 / RAND_MAX) * (m_config.m_maxProgTime - m_config.m_minProgTime));
	}
}

inline void AVR8BootLoader::pageErase(unsigned int addr) {
	if ( 0 != (addr % m_config.m_pageSize) ) {
		logEvent(EVENT_BOOT_INVALID_PAGE_ADDR, addr);
		m_rwwSectionBusy = false;
		return;
	}
	if ( addr >= m_programMemory->size() ) {
		logEvent(EVENT_BOOT_ADDR_OUT_OF_RANGE, addr);
		m_rwwSectionBusy = false;
		return;
	}
	if ( true == m_dataEEPROM->writeInProgress() ) {
		logEvent(EVENT_BOOT_OVERLOAD);
	}

	if ( isInRWWSection(addr) ) {
		m_rwwSectionBusy = true;
	}
	m_writeInProgress = true;
	m_progTimer = randomTimeInRange();
	*m_haltMode = AVR8Sim::HALTM_PROG;

	for ( unsigned int i = 0; i < m_config.m_pageSize; i++ ) {
		m_programMemory->write(addr++, 0xffff);
	}
}

inline void AVR8BootLoader::pageWrite(unsigned int addr) {
	if ( 0 != (addr % m_config.m_pageSize) ) {
		logEvent(EVENT_BOOT_INVALID_PAGE_ADDR, addr);
		m_rwwSectionBusy = false;
		return;
	}
	if ( true == m_dataEEPROM->writeInProgress() ) {
		logEvent(EVENT_BOOT_OVERLOAD);
	}

	if ( isInRWWSection(addr) ) {
		m_rwwSectionBusy = true;
	}
	m_writeInProgress = true;
	m_progTimer = randomTimeInRange();
	*m_haltMode = AVR8Sim::HALTM_PROG;

	for ( unsigned int i = 0; i < m_config.m_pageSize; i++ ) {
		m_programMemory->write(addr++, m_writeBuffer[i]);
	}
}

inline void AVR8BootLoader::setLockBits(unsigned int addr, unsigned int val) {
	using namespace AVR8RegNames;

	if ( 0x0001 != addr ) {
		logEvent(EVENT_BOOT_WRN_INVALID_ADDR, addr);
	}

	m_fusesAndLocks->setLockBits( (unsigned char)(val & 0xff) );

	unsigned int spmcr = m_dataMemory->readFast(SPMCR);
	spmcr &= ( 0xff ^ ( SPMCR_SPMEN | SPMCR_PGERS | SPMCR_PGWRT | SPMCR_BLBSET | SPMCR_RWWSRE ) );
	m_dataMemory->writeFast(SPMCR, spmcr);
}

unsigned int AVR8BootLoader::spmWrite(unsigned int addr, unsigned int val) {
	using namespace AVR8RegNames;

	if (
		( true == isInApplicationSection(m_instructionSet->getProgramCounter()) )
			||
		( false == isWriteAllowed(addr) )
	) {
		logEvent(EVENT_BOOT_WR_ACCESS_NOT_ALLOWED, addr);
		return 2;
	}

	if ( true == m_writeInProgress ) {
		logEvent(EVENT_BOOT_WR_BUSY, addr);
		return 2;
	}

	unsigned int spmcr = m_dataMemory->readFast(SPMCR);
	spmcr &= ( 0xff ^ SPMCR_RWWSB );
	m_cr_timer = 0;

	switch ( m_spmMode ) {
		case SPMMD_NONE:	// None
			logEvent(EVENT_BOOT_NO_OPER_SELECTED, addr, val);
			break;
		case SPMMD_BUFFER:	// Write to the buffer
			m_writeBuffer[addr % m_config.m_pageSize] = val;
			m_rwwSectionBusy = false;
			break;
		case SPMMD_PGERS:	// Page Erase
			pageErase(addr);
			break;
		case SPMMD_PGWRT:	// Page Write
			pageWrite(addr);
			break;
		case SPMMD_BLBSET:	// Boot Lock Bit Set
			setLockBits(addr, val);
			break;
		case SPMMD_RWWSRE:	// Read-While-Write Section Read Enable
			m_rwwSectionBusy = false;
			break;
	}

	if ( true == m_rwwSectionBusy ) {
		spmcr |= SPMCR_RWWSB;
	}
	m_dataMemory->writeFast(SPMCR, spmcr);
	return 2;
}

unsigned int AVR8BootLoader::lpmRead(unsigned int addr) {
	using namespace AVR8RegNames;

	if ( SPMMD_BLBSET == m_spmMode ) {
		unsigned int spmcr = m_dataMemory->readFast(SPMCR);
		spmcr &= ( 0xff ^ ( SPMCR_SPMEN | SPMCR_PGERS | SPMCR_PGWRT | SPMCR_BLBSET | SPMCR_RWWSRE ) );
		m_dataMemory->writeFast(SPMCR, spmcr);

		switch ( addr ) {
			case 0x0000: // Read fuse low bits
				return (unsigned int)((*m_fusesAndLocks)[AVR8FusesAndLocks::BYTE_FUSES_LOW]);
			case 0x0001: // Read lock bits
				return (unsigned int)((*m_fusesAndLocks)[AVR8FusesAndLocks::BYTE_LOCKS_LOW]);
			case 0x0003: // Read fuse high bits
				return (unsigned int)((*m_fusesAndLocks)[AVR8FusesAndLocks::BYTE_FUSES_HIGH]);
			default:
				logEvent(EVENT_BOOT_ERR_INVALID_ADDR, addr, SPMMD_BLBSET);
				return m_fusesAndLocks->getUndefVal();
		}
	}

	if ( true == isReadAllowed(addr) ) {
		return m_programMemory->read(addr);
	} else {
		logEvent(EVENT_BOOT_RD_ACCESS_NOT_ALLOWED, addr);
		return m_fusesAndLocks->getUndefVal();
	}
}

inline bool AVR8BootLoader::isWriteAllowed(unsigned int addr) const {
	if ( true == isInApplicationSection(addr) ) {
		// Write to the application section section
		if ( true == (*m_fusesAndLocks)[AVR8FusesAndLocks::LB_BLB01] ) {
			// Lock bit BLB01 programmed -> NOT ALLOWED
			return false;
		} else {
			// Lock bit BLB01 unprogrammed -> allowed
			return true;
		}
	} else {
		// Write to the boot loader section section
		if ( true == (*m_fusesAndLocks)[AVR8FusesAndLocks::LB_BLB11] ) {
			// Lock bit BLB11 programmed -> NOT ALLOWED
			return false;
		} else {
			// Lock bit BLB11 unprogrammed -> allowed
			return true;
		}
	}
}

inline bool AVR8BootLoader::isReadAllowed(unsigned int addr) const {
	if ( true == isInApplicationSection(m_instructionSet->getProgramCounter()) ) {
		// Executing from the application section.
		if ( true == isInApplicationSection(addr) ) {
			// Read from the application section section is always allowed while executing from it.
			return true;
		} else {
			// Attempting to read from the boot loader.
			if ( true == (*m_fusesAndLocks)[AVR8FusesAndLocks::LB_BLB12] ) {
				// Lock bit BLB12 programmed -> NOT ALLOWED
				return false;
			} else {
				// Lock bit BLB12 unprogrammed -> allowed
				return true;
			}
		}
	} else {
		// Executing from the boot loader section.
		if ( true == isInApplicationSection(addr) ) {
			// Attempting to read from the application section.
			if ( true == (*m_fusesAndLocks)[AVR8FusesAndLocks::LB_BLB02] ) {
				// Lock bit BLB02 programmed -> NOT ALLOWED
				return false;
			} else {
				// Lock bit BLB02 unprogrammed -> allowed
				return true;
			}
		} else {
			// Read from the boot loader section is always allowed while executing from it.
			return true;
		}
	}
}

inline bool AVR8BootLoader::isInApplicationSection(unsigned int addr) const {
	if ( addr < getBootAddress() ) {
		return true;
	} else {
		return false;
	}
}

unsigned int AVR8BootLoader::getBootAddress() const {
	if ( false == (*m_fusesAndLocks)[AVR8FusesAndLocks::FUSE_BOOTSZ1] ) {
		if ( false == (*m_fusesAndLocks)[AVR8FusesAndLocks::FUSE_BOOTSZ0] ) {
			return m_config.m_bootResetAddress[3]; // BOOTSZ1 = 1, BOOTSZ0 = 1
		} else {
			return m_config.m_bootResetAddress[2]; // BOOTSZ1 = 1, BOOTSZ0 = 0
		}
	} else {
		if ( false == (*m_fusesAndLocks)[AVR8FusesAndLocks::FUSE_BOOTSZ0] ) {
			return m_config.m_bootResetAddress[1]; // BOOTSZ1 = 0, BOOTSZ0 = 1
		} else {
			return m_config.m_bootResetAddress[0]; // BOOTSZ1 = 0, BOOTSZ0 = 0
		}
	}
}

void AVR8BootLoader::reset(MCUSim::ResetMode mode) {
	switch ( mode ) {
		case MCUSim::RSTMD_NEW_CONFIG:
			loadConfig();
			break;
		case MCUSim::RSTMD_INITIAL_VALUES:
			resetToInitialValues();
			break;
		case MCUSim::RSTMD_MCU_RESET:
			mcuReset();
			break;
		default:
			// Irrelevant requests are silently ignored
			break;
	}
}

inline void AVR8BootLoader::loadConfig() {
	if ( NULL != m_writeBuffer ) {
		delete[] m_writeBuffer;
	}
	m_writeBuffer = new uint32_t[m_config.m_pageSize];
}

inline void AVR8BootLoader::mcuReset() {
	m_spmcrLast = m_dataMemory->readFast(AVR8RegNames::SPMCR);
	m_cr_timer = 0;
	m_spmMode = SPMMD_NONE;
	m_writeInProgress = false;
	m_rwwSectionBusy = m_dataMemory->readBitFast(AVR8RegNames::SPMCR, AVR8RegNames::SPMCR_RWWSB);
	m_progTimer = 0;
}

inline void AVR8BootLoader::resetToInitialValues() {
	for ( unsigned int i = 0; i < m_config.m_pageSize; i++ ) {
		m_writeBuffer[i] = m_programMemory->getUndefVal();
	}
}
