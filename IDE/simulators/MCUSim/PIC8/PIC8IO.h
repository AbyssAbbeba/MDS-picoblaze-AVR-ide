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
 * @file PIC8IO.h
 */

// #define NDEBUG

#ifndef PIC8IO_H
#define PIC8IO_H

class PIC8DataMemory;

#include "../MCUSim.h"
#include "PIC8PinNames.h"

#include <cstdlib>
#include <cassert>

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8IO
 */
class PIC8IO : public MCUSim::IO {
public:
	PIC8IO();
	~PIC8IO();

// 	static const unsigned int NUMBER_OF_PORTS = 4;
// 	static const unsigned int NUMBER_OF_PINS = PIC8PinNames::PIN__MAX__;//0 + ( NUMBER_OF_PORTS * 8 ); // TODO: write something better here
// 
// 	enum PINMD {
// 		PINMD_GENERAL_IO	= 0x0,
// 		PINMD_INPUT_OVERRIDE	= 0x1,
// 		PINMD_OUTPUT_OVERRIDE	= 0x2,
// 		PINMD_IO_OVERRIDE	= 0x3, ///< ( PINMD_INPUT_OVERRIDE | PINMD_OUTPUT_OVERRIDE )
// 		PINMD_NOT_IMPLEMENTED	= 0x4
// 	};
// 
// 	enum RandValMode {
// 		RVM_RANDOM,
// 		RVM_HIGH,
// 		RVM_LOW
// 	};
// 
// 	struct Config {
// 		Config() {
// 			m_random = RVM_RANDOM;
// 		}
// 
// 		RandValMode m_random;
// 		SimFloatType m_pullUpresistance; // 20-50k ohms
// 		PIC8PinNames::PIN m_specFuncMap[PIC8PinNames::SPF__MAX__];
// 		bool m_hasPort[NUMBER_OF_PORTS];
// 		bool m_availablePins[NUMBER_OF_PINS];
// 	};
// 
// 	Config m_config;

	PIC8IO * link(MCUSim::EventLogger * eventLogger, PIC8DataMemory * dataMemory);
	void reset(MCUSim::ResetMode mode);

	void clockCycles();

// 	bool getLog(PIC8PinNames::SPF pin) {
// 		return getLog(m_config.m_specFuncMap[pin]);
// 	}
// 	bool getLog(PIC8PinNames::PIN pin) {
// 		assert(PIC8PinNames::PIN_NC != pin);
// 
// 		if ( m_lowLevelInterface[II_REAL_VOLTAGE][pin] > m_logThreshold1 ) {
// 			return true;
// 		} else if ( m_lowLevelInterface[II_REAL_VOLTAGE][pin] < m_logThreshold0 ) {
// 			return false;
// 		} else {
// // 			logEvent(EVENT_IO_INDETERMINABLE_LOG, pin);
// // 			switch ( m_config.m_random ) {
// // 				case RVM_RANDOM:
// // 					return ( ( rand() > (RAND_MAX / 2) ) ? true : false );
// // 				case RVM_HIGH:
// // 					return true;
// // 				case RVM_LOW:
// // 					return false;
// // 			}
// 		}
// 
// 		assert(0);
// 		return false; // <-- Program flow control MUST NEVER reach this.
// 	}
// 	void setLog(PIC8PinNames::SPF pin, bool val) {
// 		setLog(m_config.m_specFuncMap[pin], val);
// 	}
// 	void setLog(PIC8PinNames::PIN pin, bool val) {
// 		assert(PIC8PinNames::PIN_NC != pin);
// 
// 		if ( true == val ) {
// 			m_lowLevelInterface[II_VOLTAGE_INT][pin] = m_sourceVoltage;
// 		} else {
// 			m_lowLevelInterface[II_VOLTAGE_INT][pin] = 0;
// 		}
// 	}
// 	SimFloatType getVoltage(PIC8PinNames::SPF pin) const {
// 		return getVoltage(m_config.m_specFuncMap[pin]);
// 	}
// 	SimFloatType getVoltage(PIC8PinNames::PIN pin) const {
// 		assert(PIC8PinNames::PIN_NC != pin);
// 
// 		return m_lowLevelInterface[II_REAL_VOLTAGE][pin];
// 	}
// 	void setVoltage(PIC8PinNames::SPF pin, SimFloatType volts) {
// 		setVoltage(m_config.m_specFuncMap[pin], volts);
// 	}
// 	void setVoltage(PIC8PinNames::PIN pin, SimFloatType volts) {
// 		assert(PIC8PinNames::PIN_NC != pin);
// 
// 		m_lowLevelInterface[II_VOLTAGE_INT][pin] = volts;
// 	}
// // 	SimFloatType getResistance(PIN pin) const {
// // 		assert(PIC8PinNames::PIN_NC != pin);
// //
// // 		return m_lowLevelInterface[II_RESISTANCE][pin];
// // 	}
// 	void setResistance(PIC8PinNames::SPF pin, SimFloatType ohms) {
// 		setResistance(m_config.m_specFuncMap[pin], ohms);
// 	}
// 	void setResistance(PIC8PinNames::PIN pin, SimFloatType ohms) {
// 		assert(PIC8PinNames::PIN_NC != pin);
// 
// 		m_lowLevelInterface[II_RESISTANCE][pin] = ohms;
// 	}
// // 	void setAsOutput(PIN pin) {
// // 		assert(PIC8PinNames::PIN_NC != pin);
// //
// // 		m_lowLevelInterface[II_RESISTANCE][pin] = 0;
// // 	}
// // 	void setAsInput(PIN pin) {
// // 		assert(PIC8PinNames::PIN_NC != pin);
// //
// // 		m_lowLevelInterface[II_RESISTANCE][pin] = INFINITY;
// // 	}
// 	void setPinMode(PIC8PinNames::SPF pin, PINMD mode) {
// 		setPinMode(m_config.m_specFuncMap[pin], mode);
// 	}
// 	void setPinMode(PIC8PinNames::PIN pin, PINMD mode) {
// 		assert(PIC8PinNames::PIN_NC != pin);
// 
// 		// TODO: Certainly this is not complete.
// 
// 		m_pinMode[pin] = mode;
// 	}
// 	PINMD getPinMode(PIC8PinNames::SPF pin) const {
// 		return getPinMode(m_config.m_specFuncMap[pin]);
// 	}
// 	PINMD getPinMode(PIC8PinNames::PIN pin) const {
// 		assert(PIC8PinNames::PIN_NC != pin);
// 
// 		return m_pinMode[pin];
// 	}
// 
// 	void setSourceVoltage(SimFloatType voltage) {
// 		m_sourceVoltage = voltage;
// 		m_logThreshold0 = m_sourceVoltage / 2;
// 		m_logThreshold1 = m_logThreshold0;
// 	}
// 	SimFloatType getSourceVoltage() const {
// 		return m_sourceVoltage;
// 	}
// 	unsigned int getNumberOfPins() const {
// 		return NUMBER_OF_PINS;
// 	}
// 	SimFloatType ** getLowLevelInterface() {
// 		return m_lowLevelInterface;
// 	}
// 	void setEnabled(bool flag) {
// 		m_enabled = flag;
// 	}

protected:
// 	PIC8DataMemory * m_dataMemory;
// 
// 	// Move to the base class
// 	SimFloatType m_sourceVoltage;
// 
// 	SimFloatType m_logThreshold0;
// 	SimFloatType m_logThreshold1;
// 
// 	// idx =  |n       ... |23  16|15   8|7    0|
// 	//        |special ... |PORT_C|PORT_B|PORT_A|
// 	SimFloatType ** m_lowLevelInterface;
// 	unsigned int m_lastDdr[NUMBER_OF_PORTS];
// 	PINMD m_pinMode[NUMBER_OF_PINS];
// 	bool m_PullUpDisabled;
// 	bool m_enabled; // by default this is disabled
// 
// 	inline void portSetDirection(unsigned int pinIdx, int portIndex, const unsigned int ddrAddr);
// 	inline void portReadWrite(unsigned int pinIdx, const unsigned int inRegAddr, const unsigned int outRegAddr);
// 
// 	inline void loadConfig();
// 	inline void resetToInitialValues();
// 	inline void mcuReset();
};

#endif // PIC8IO_H
