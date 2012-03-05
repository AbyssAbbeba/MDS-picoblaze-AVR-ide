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

//          ╭────────────╮
// PIN █──┬─┤ RESISTANCE ├─────▎← VOLTAGE_INT
//        │ ╰────────────╯
//        ╰────────────────────▎→ REAL_VOLTAGE
//

#include "AVR8IO.h"
#include "AVR8DataMemory.h"

#include <math.h>

#ifndef NAN
  #error 'NAN' macro is not defined
#endif

AVR8IO::AVR8IO(
		MCUSim::EventLogger	* eventLogger,
		AVR8DataMemory		* dataMemory)
		 :
		MCUSim::IO(eventLogger),
		m_dataMemory(dataMemory)
{
	m_lowLevelInterface = new SimFloatType* [II__MAX__];
	for ( int i = 0; i < II__MAX__; i++ ) {
		m_lowLevelInterface[i] = new SimFloatType[NUMBER_OF_PINS];
	}

	m_enabled = false;
}

AVR8IO::~AVR8IO() {
	if ( NULL != m_lowLevelInterface ) {
		delete[] m_lowLevelInterface;
	}
}

void AVR8IO::clockCycles() {
	if ( false == m_enabled ) {
		return;
	}

	/*
	 * - Read port states and set new ones.
	 * - Adjust port configurations to the I/O flags DDRx registers
	 */
	m_PullUpDisabled = m_dataMemory->readBitFast(AVR8RegNames::SFIOR, AVR8RegNames::SFIOR_PUD);
	if ( true == m_config.m_hasPort[0] ) {
		portSetDirection(AVR8PinNames::PIN_PA0, 0, AVR8RegNames::DDRA);
		portReadWrite(AVR8PinNames::PIN_PA0, AVR8RegNames::PINA, AVR8RegNames::PORTA);
	}
	if ( true == m_config.m_hasPort[1] ) {
		portSetDirection(AVR8PinNames::PIN_PB0, 1, AVR8RegNames::DDRB);
		portReadWrite(AVR8PinNames::PIN_PB0, AVR8RegNames::PINB, AVR8RegNames::PORTB);
	}
	if ( true == m_config.m_hasPort[2] ) {
		portSetDirection(AVR8PinNames::PIN_PC0, 2, AVR8RegNames::DDRC);
		portReadWrite(AVR8PinNames::PIN_PC0, AVR8RegNames::PINC, AVR8RegNames::PORTC);
	}
	if ( true == m_config.m_hasPort[3] ) {
		portSetDirection(AVR8PinNames::PIN_PD0, 3, AVR8RegNames::DDRD);
		portReadWrite(AVR8PinNames::PIN_PD0, AVR8RegNames::PIND, AVR8RegNames::PORTD);
	}

}

inline void AVR8IO::portSetDirection(unsigned int pinIdx, int portIndex, const unsigned int ddrAddr) {
	const unsigned int ddr = m_dataMemory->readFast(ddrAddr);
	if ( ddr == m_lastDdr[portIndex]) {
		return;
	}
	m_lastDdr[portIndex] = ddr;

	for ( int i = 0x1; i < 0x100; i <<= 1, pinIdx++ ) {
		if ( ( PINMD_NOT_IMPLEMENTED | PINMD_OUTPUT_OVERRIDE ) & m_pinMode[pinIdx] ) {
			continue;
		}

		SimFloatType resistance;

		if ( ddr & i ) {
			// Output
			resistance = 0;
		} else {
			// Input
			if ( true == m_PullUpDisabled ) {
				resistance = INFINITY;
			} else {
				resistance = m_config.m_pullUpresistance;
			}
		}
		setResistance((AVR8PinNames::PIN)pinIdx, resistance);
	}
}

// This takes into account input and output override, and the input synchronizer.
inline void AVR8IO::portReadWrite(unsigned int pinIdx, const unsigned int inRegAddr, const unsigned int outRegAddr) {
	unsigned int in = 0;
	unsigned int out = m_dataMemory->readFast(outRegAddr);

	for ( int i = 0x1; i < 0x100; i <<= 1, pinIdx++ ) {
		if ( PINMD_NOT_IMPLEMENTED & m_pinMode[pinIdx] ) {
			continue;
		}

		// Read pin state
		if ( !(PINMD_INPUT_OVERRIDE & m_pinMode[pinIdx]) ) {
			if ( true == getLog ( (AVR8PinNames::PIN)pinIdx ) ) {
				in |= i;
			}
		}

		// Set new pin state
		if ( !(PINMD_OUTPUT_OVERRIDE & m_pinMode[pinIdx]) ) {
			setLog ( (AVR8PinNames::PIN)pinIdx, bool(out & i) );
		}
	}

	m_dataMemory->writeFast(inRegAddr, in);
}

void AVR8IO::reset(MCUSim::Subsys::SubsysResetMode mode) {
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

inline void AVR8IO::loadConfig() {
	for ( unsigned int i = 0; i < NUMBER_OF_PINS; i++ ) {
		if ( true == m_config.m_availablePins[i] ) {
			m_pinMode[i] = PINMD_NOT_IMPLEMENTED;
		} else {
			m_pinMode[i] = PINMD_GENERAL_IO;
		}
	}
}

inline void AVR8IO::resetToInitialValues() {
	// TODO: Not implemented yet!
}

inline void AVR8IO::mcuReset() {
	for ( unsigned int i = 0; i < NUMBER_OF_PINS; i++ ) {
		if ( PINMD_NOT_IMPLEMENTED & m_pinMode[i] ) {
			m_lowLevelInterface[II_VOLTAGE_INT][i]	= 0;
			m_lowLevelInterface[II_REAL_VOLTAGE][i]	= 0;
			m_lowLevelInterface[II_CURRENT][i]	= 0;
			m_lowLevelInterface[II_RESISTANCE][i]	= NAN; // pin is not implemented
		} else {
			for ( int j = 0; j < II__MAX__; j++ ) {
				m_lowLevelInterface[j][i] = 0;
			}
			m_pinMode[i] = PINMD_GENERAL_IO;
		}
	}
	for ( unsigned int i = 0; i < NUMBER_OF_PORTS; i++ ) {
		m_lastDdr[i] = (unsigned int) -1;
	}
}
