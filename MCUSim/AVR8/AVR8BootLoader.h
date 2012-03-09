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

#ifndef AVR8BOOTLOADER_H
#define AVR8BOOTLOADER_H

class AVR8ProgramMemory;
class AVR8DataMemory;
class AVR8FusesAndLocks;
class AVR8DataEEPROM;
class AVR8InstructionSet;

#include "../MCUSim.h"
#include "AVR8Sim.h"

#include <cstddef>
#include <cstdint>

class AVR8BootLoader : public MCUSim::Subsys {

public:
	AVR8BootLoader();
	~AVR8BootLoader();

	enum Event {
		EVENT_BOOT_WR_ACCESS_NOT_ALLOWED,
		EVENT_BOOT_RD_ACCESS_NOT_ALLOWED,
		EVENT_BOOT_INVALID_CR_CHAGE,
		EVENT_BOOT_INVALID_PAGE_ADDR,
		EVENT_BOOT_WR_BUSY,
		EVENT_BOOT_OVERLOAD, // practically that means that the EEPROM write is in progress while attempting to use SPM
		EVENT_BOOT_NO_OPER_SELECTED,
		EVENT_BOOT_ADDR_OUT_OF_RANGE,

		EVENT_BOOT_ERR_INVALID_ADDR,
		EVENT_BOOT_WRN_INVALID_ADDR // This is only a warning
	};

	struct Config {
		float m_minProgTime; // 3.7e-3
		float m_maxProgTime; // 4.5e-3
		unsigned int m_pageSize; // 32 WORDS on ATmega8
		int m_bootResetAddress[4];
		unsigned int m_rwwSectionSize; // number of PAGES
	};

	Config m_config;

	AVR8BootLoader * link(
		MCUSim::EventLogger	* eventLogger,
		AVR8ProgramMemory	* programMemory,
		AVR8DataMemory		* dataMemory,
		AVR8FusesAndLocks	* fusesAndLocks,
		AVR8DataEEPROM		* dataEEPROM,
		AVR8InstructionSet	* instructionSet,
		AVR8Sim::HaltMode	* haltMode);

	void reset(MCUSim::ResetMode mode);
	void timeStep(float timeStep, unsigned int clockCycles = 0);

	unsigned int spmWrite(unsigned int addr, unsigned int val);
	unsigned int lpmRead(unsigned int addr);
	unsigned int getBootAddress() const;
	inline bool inUse(unsigned int addr) const;

protected:
	enum SpmMode {
		SPMMD_NONE,	///< None
		SPMMD_BUFFER,	///< Write to the buffer
		SPMMD_PGERS,	///< Page Erase
		SPMMD_PGWRT,	///< Page Write
		SPMMD_BLBSET,	///< Boot Lock Bit Set
		SPMMD_RWWSRE	///< Read-While-Write Section Read Enable

	};
	SpmMode m_spmMode;
	unsigned int m_spmcrLast;
	int m_cr_timer;
	uint32_t * m_writeBuffer;
	float m_progTimer;
	bool m_writeInProgress;
	bool m_rwwSectionBusy;

	AVR8ProgramMemory * m_programMemory;
	AVR8DataMemory * m_dataMemory;
	AVR8FusesAndLocks & m_fusesAndLocks;
	AVR8DataEEPROM * m_dataEEPROM;
    	AVR8InstructionSet * m_instructionSet;
	AVR8Sim::HaltMode & m_haltMode;

	inline void resetToInitialValues();
	inline void loadConfig();
	inline void mcuReset();

	inline SpmMode determinateSpmMode(unsigned int spmcr) const;
	inline bool isInApplicationSection(unsigned int addr) const;
	inline bool isInRWWSection(unsigned int addr) const;
	inline bool isReadAllowed(unsigned int addr) const;
	inline bool isWriteAllowed(unsigned int addr) const;

	inline void manageProgTimer(const float timeStep);
	inline void manageContRegTimer(const unsigned int clockCycles);
	inline void normalizeControlReg(const unsigned int clockCycles);

	inline void pageErase(unsigned int addr);
	inline void setLockBits(unsigned int addr, unsigned int val);
	inline void pageWrite(unsigned int addr);
	inline float randomTimeInRange() const;
};


// -----------------------------------------------------------------------------
// INLINE FUNCTIONS
// -----------------------------------------------------------------------------

inline bool AVR8BootLoader::inUse(unsigned int addr) const {
	return ( (true == m_rwwSectionBusy) && (true == isInRWWSection(addr)) );
}

inline bool AVR8BootLoader::isInRWWSection(unsigned int addr) const {
	return ( addr < (m_config.m_pageSize * m_config.m_rwwSectionSize) );
}

#endif // AVR8BOOTLOADER_H
