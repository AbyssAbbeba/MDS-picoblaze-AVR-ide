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
 * @file AVR8DataEEPROM.h
 */

#ifndef AVR8DATAEEPROM_H
#define AVR8DATAEEPROM_H

class DataFile;
class AVR8DataMemory;
class AVR8InterruptController;

#include "../MCUSim.h"

#include <cstddef>
#include <cstdint>

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8DataEEPROM
 */
class AVR8DataEEPROM : public MCUSim::Memory {
public:
	AVR8DataEEPROM();

	enum Event {
		EVENT_EEPROM_INVALID_CR_CHAGE = EVENT_MEM__MAX__, ///<

		EVENT_EEPROM__MAX__
	};

	struct Config {
		unsigned int m_addrRegWidth;
		unsigned int m_size;
		unsigned int m_writeTime; // 8.448 (in seconds!)
		int m_undefinedValue;
	};

	Config m_config;

	AVR8DataEEPROM * link(
		MCUSim::EventLogger	* eventLogger,
		AVR8DataMemory		* dataMemory,
		AVR8InterruptController	* interruptControllers);
	~AVR8DataEEPROM();

	void reset(MCUSim::ResetMode mode);
	void loadDataFile(const DataFile * file);
	void storeInDataFile(DataFile * file) const;

	MCUSim::RetCode directRead(unsigned int addr, unsigned int & data) const;
	MCUSim::RetCode directWrite(unsigned int addr, unsigned int data);
	void resize(unsigned int newSize);
	unsigned int size() const {
		return m_config.m_size;
	}
	bool writeInProgress() const {
		return m_writeInProgress;
	}

	unsigned int timeStep(float timeStep, unsigned int clockCycles = 0);	// <-- This has to be called even in a sleep mode

protected:
	AVR8DataMemory * m_dataMemory;
	AVR8InterruptController * m_interruptController;

	uint32_t * m_memory;
	unsigned int m_eearLast;
	unsigned int m_eecrLast;
	bool m_writeInProgress;
	int m_eecr_timer;

	static const int WB_ADDR = 0;
	static const int WB_DATA = 1;
	unsigned int m_writeBuffer[2];
	float m_writeTimer;

	inline unsigned int readEecr(const unsigned int clockCycles);
	inline void writeByte(float timeStep, unsigned int eecr);
	inline unsigned int readByte();
	unsigned int getUndefVal() const;

	inline void loadConfig();
	inline void resetToInitialValues();
	inline void mcuReset();
};

#endif // AVR8DATAEEPROM_H
