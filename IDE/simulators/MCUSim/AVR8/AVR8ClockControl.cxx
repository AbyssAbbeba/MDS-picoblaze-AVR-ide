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

#include "AVR8ClockControl.h"
#include "AVR8FusesAndLocks.h"

AVR8ClockControl * AVR8ClockControl::link(
		MCUSim::EventLogger	* eventLogger,
		AVR8FusesAndLocks	* fusesAndLocks
) {
	Clock::link(eventLogger);

	m_fusesAndLocks = fusesAndLocks;

	return this;
}

float AVR8ClockControl::getFrequency() {
	const unsigned int clksel = getClksel();

	switch ( clksel ) {
		/*
		 * External Clock
		 */
		case 0b0000:
			return freqExternalSource();
			
		/*
		 * Calibrated Internal RC Oscillator
		 */
		case 0b0001:
		case 0b0010:
		case 0b0011:
		case 0b0100:
			return freqInternalRC(clksel);

		/*
		 * External RC Oscillator
		 */
		case 0b0101:
		case 0b0110:
		case 0b0111:
		case 0b1000:
			return freqExternalRC(clksel);

		/*
		 * External Low-frequency Crystal
		 */
		case 0b1001:
			return freqLowFreqCrystal();

		/*
		 * External Crystal/Ceramic Resonator
		 */
		case 0b1010:
		case 0b1011:
		case 0b1100:
		case 0b1101:
		case 0b1110:
		case 0b1111:
			return freqHighFreqCrystal(clksel);

		/*
		 * Unknown setting
		 */
		default:
			return 0;
	}
}

inline float AVR8ClockControl::freqHighFreqCrystal(const unsigned int clksel) {
	/*
	 *  Table: Crystal Oscillator Operating Modes:
	 * ╔═══════╤═════════════╤══════════════════════╤══════════════════════════════════╗
	 * ║ CKOPT │  CKSEL3:1   │ Frequency Range(MHz) │ Recommended Range for Capacitors ║
	 * ╟───────┼─────────────┼──────────────────────┼──────────────────────────────────╢
	 * ║   1   │     101     │      0.4 - 0.9       │               -                  ║
	 * ║   1   │     110     │      0.9 - 3.0       │            12 - 22               ║
	 * ║   1   │     111     │      3.0 - 8.0       │            12 - 22               ║
	 * ║   0   │ 101,110,111 │      1.0 ≤           │            12 - 22               ║
	 * ╚═══════╧═════════════╧══════════════════════╧══════════════════════════════════╝
	 */

	if (
		( ClockSource::TYPE_CRYSTAL != m_clockSource.getType() )
			&&
		( ClockSource::TYPE_CERAMIC_RES != m_clockSource.getType() )
	) {
		logEvent(EVENT_CLK_ERR_INCOMPATIBLE_SOURCE);
		return 0;
	}

	bool checkCapycityRange = true;

	if ( false == (*m_fusesAndLocks)[AVR8FusesAndLocks::FUSE_CKOPT] ) {
		// CKOPT = 1

		float freqRange[2];

		switch ( clksel & 0b0110 ) {
			case 0b01:
				if ( ClockSource::TYPE_CERAMIC_RES != m_clockSource.getType() ) {
					logEvent(EVENT_CLK_ERR_INCOMPATIBLE_SOURCE);
					return 0;
				}
				checkCapycityRange = false;
				freqRange[0] = 0.4e6;
				freqRange[1] = 0.9e6;
				break;

			case 0b10:
				freqRange[0] = 0.9e6;
				freqRange[1] = 3.0e6;
				break;

			case 0b11:
				freqRange[0] = 3.0e6;
				freqRange[1] = 8.0e6;
				break;
			default:
				return 0;
		}

		if (
			( m_clockSource.getFrequency() < freqRange[0] )
				||
			( m_clockSource.getFrequency() > freqRange[1] )
		) {
			logEvent(EVENT_CLK_ERR_OUT_OF_FREQ_RANGE);
			return 0;
		}
	} else {
		// CKOPT = 0
		if ( m_clockSource.getFrequency() < 1e6 ) {
			logEvent(EVENT_CLK_ERR_OUT_OF_FREQ_RANGE);
			return 0;
		}
	}

	if ( true == checkCapycityRange ) {
		if (
			( m_clockSource.getCapacity() < 12e-12 )
				||
			( m_clockSource.getCapacity() > 22e-12 )

		) {
			logEvent(EVENT_CLK_WRN_ATYPICAL_CAP);
		}
	}

	return m_clockSource.getFrequency();
}

inline float AVR8ClockControl::freqLowFreqCrystal() {

	if ( ClockSource::TYPE_LOW_FREQ_CRYSTAL != m_clockSource.getType() ) {
		logEvent(EVENT_CLK_ERR_INCOMPATIBLE_SOURCE);
		return 0;
	}

	if ( 4e5 < m_clockSource.getFrequency() ) {
		logEvent(EVENT_CLK_ERR_OUT_OF_FREQ_RANGE);
		return 0;
	} else if ( 32768 != m_clockSource.getFrequency() ) {
		logEvent(EVENT_CLK_WRN_ATYPICAL_FREQ);
	}

	float capacity = m_clockSource.getCapacity();

	/*
	 * By programming the CKOPT Fuse, the user can enable an internal 36 pF capacitors.
	 */
	if ( true == (*m_fusesAndLocks)[AVR8FusesAndLocks::FUSE_CKOPT] ) {
		capacity += 36e-12;
	}

	if ( 36e-12 != capacity ) {
		logEvent(EVENT_CLK_WRN_ATYPICAL_CAP);
	}

	return m_clockSource.getFrequency();
}

inline float AVR8ClockControl::freqExternalRC(const unsigned int clksel) {
	/*
	 *  Table: External RC Oscillator Operating Modes:
	 * ╔═══════════════════════╤══════════╗
	 * ║ Frequency Range (MHz) │ CKSEL3:0 ║
	 * ╟───────────────────────┼──────────╢
	 * ║      0.1 - 0.9        │   0101   ║
	 * ║      0.9 - 3.0        │   0110   ║
	 * ║      3.0 - 8.0        │   0111   ║
	 * ║      8.0 - 12.0       │   1000   ║
	 * ╚═══════════════════════╧══════════╝
	 */

	if ( ClockSource::TYPE_RC != m_clockSource.getType() ) {
		logEvent(EVENT_CLK_ERR_INCOMPATIBLE_SOURCE);
		return 0;
	}

	float capacity = m_clockSource.getCapacity();
	float freqRange[2];

	switch ( clksel ) {
		case 0b0101:
			freqRange[0] = 0.1e6;
			freqRange[1] = 0.9e6;
			break;
		case 0b0110:
			freqRange[0] = 0.9e6;
			freqRange[1] = 3.0e6;
			break;
		case 0b0111:
			freqRange[0] = 3.0e6;
			freqRange[1] = 8.0e6;
			break;
		case 0b1000:
			freqRange[0] = 8.0e6;
			freqRange[1] = 12.0e6;
			break;
	}

	/*
	 * By programming the CKOPT Fuse, the user can enable an internal 36 pF capacitor
	 * between XTAL1 and GND, thereby removing the need for an external capacitor.
	 */
	if ( true == (*m_fusesAndLocks)[AVR8FusesAndLocks::FUSE_CKOPT] ) {
		capacity += 36e-12;
	}

	// C should be at least 22pF
	if ( capacity < 22e-12 ) {
		logEvent(EVENT_CLK_WRN_CAPACITY_TOO_LOW);
	}

	if ( (0 == m_clockSource.getResistance()) || (0 == capacity) ) {
		logEvent(EVENT_CLK_ERR_SOURCE_USELESS);
		return 0;
	}

	// f = 1/(3RC)
	float frequency = ( 1.0 / 3 * m_clockSource.getResistance() * capacity );

	if ( (frequency < freqRange[0]) || (frequency > freqRange[1]) ) {
		logEvent(EVENT_CLK_ERR_OUT_OF_FREQ_RANGE);
		return 0;
	}

	return frequency;
}

inline float AVR8ClockControl::freqInternalRC(const unsigned int clksel) {
	/*
	 *  Table: Internal Calibrated RC Oscillator Operating Modes:
	 * ╔═════════════════════════╤══════════╗
	 * ║ Nominal Frequency (MHz) │ CKSEL3:0 ║
	 * ╟─────────────────────────┼──────────╢
	 * ║          1.0            │   0001   ║
	 * ║          2.0            │   0010   ║
	 * ║          4.0            │   0011   ║
	 * ║          8.0            │   0100   ║
	 * ╚═════════════════════════╧══════════╝
	 */

	if ( ClockSource::TYPE_NONE != m_clockSource.getType() ) {
		logEvent(EVENT_CLK_WRN_SOURCE_NOT_USED);
	}

	// TODO: Implement handling of the calibration byte
	switch ( clksel ) {
		case 0b0001:	// 1.0 MHz
			return 1.0;
		case 0b0010:	// 2.0 MHz
			return 2.0;
		case 0b0011:	// 4.0 MHz
			return 4.0;
		case 0b0100:	// 8.0 MHz
			return 8.0;
		default:	// Unknown setting
			return 0;
	}
}

inline float AVR8ClockControl::freqExternalSource() {
	if ( ClockSource::TYPE_EXTERNAL != m_clockSource.getType() ) {
		logEvent(EVENT_CLK_ERR_INCOMPATIBLE_SOURCE);
		return 0;
	}

	return m_clockSource.getFrequency();
}

inline unsigned int AVR8ClockControl::getClksel() const {
	/*
	 *  Table: Device Clocking Options Select:
	 * ╔════════════════════════════════════╤═══════════════╗
	 * ║ Device Clocking Option             │   CKSEL3:0    ║
	 * ╟────────────────────────────────────┼───────────────╢
	 * ║ External Crystal/Ceramic Resonator │  1111 - 1010  ║
	 * ║ External Low-frequency Crystal     │  1001         ║
	 * ║ External RC Oscillator             │  1000 - 0101  ║
	 * ║ Calibrated Internal RC Oscillator  │  0100 - 0001  ║
	 * ║ External Clock                     │  0000         ║
	 * ╚════════════════════════════════════╧═══════════════╝
	 */

	return (
		( ( true == (*m_fusesAndLocks)[AVR8FusesAndLocks::FUSE_CKSEL3] ) ? 0 : 0x8 ) |
		( ( true == (*m_fusesAndLocks)[AVR8FusesAndLocks::FUSE_CKSEL2] ) ? 0 : 0x4 ) |
		( ( true == (*m_fusesAndLocks)[AVR8FusesAndLocks::FUSE_CKSEL1] ) ? 0 : 0x2 ) |
		( ( true == (*m_fusesAndLocks)[AVR8FusesAndLocks::FUSE_CKSEL0] ) ? 0 : 0x1 )
	);
}

inline unsigned int AVR8ClockControl::getSut() const {
	return (
		( ( true == (*m_fusesAndLocks)[AVR8FusesAndLocks::FUSE_SUT1] ) ? 0 : 0x2 ) |
		( ( true == (*m_fusesAndLocks)[AVR8FusesAndLocks::FUSE_SUT0] ) ? 0 : 0x1 )
	);
}

AVR8ClockControl::StartUpTime AVR8ClockControl::startUpTime() {

	const unsigned int clksel = getClksel();
	const unsigned int sut    = getSut();

	switch ( clksel ) {
		/*
		 * External Clock
		 */
		case 0b0000:
			return startUpExternalSource(sut);
			
		/*
		 * Calibrated Internal RC Oscillator
		 */
		case 0b0001:
		case 0b0010:
		case 0b0011:
		case 0b0100:
			return startUpInternalRC(sut);

		/*
		 * External RC Oscillator
		 */
		case 0b0101:
		case 0b0110:
		case 0b0111:
		case 0b1000:
			return startUpExternalRC(sut);

		/*
		 * External Low-frequency Crystal
		 */
		case 0b1001:
			return startUpLowFreqCrystal(sut);

		/*
		 * External Crystal/Ceramic Resonator
		 */
		case 0b1010:
		case 0b1011:
		case 0b1100:
		case 0b1101:
		case 0b1110:
		case 0b1111:
			return startUpHighFreqCrystal(clksel, sut);

		/*
		 * Unknown setting
		 */
		default:
			return StartUpTime();
	}
}

inline AVR8ClockControl::StartUpTime AVR8ClockControl::startUpExternalSource(const unsigned int sut) {
	/*
	 * Table: Start-up Times for the External Clock Selection
	  * ╔════════╤═══════════════════════════╤══════════════════╗
	  * ║ SUT1:0 │    Start-up Time from     │ Additional Delay ║
	  * ║        │ Power-down and Power-save │   from Reset     ║
	  * ╟────────┼───────────────────────────┼──────────────────╢
	  * ║   00   │            6 CK           │         -        ║
	  * ║   01   │            6 CK           │      4.1 ms      ║
	  * ║   10   │            6 CK           │       65 ms      ║
	  * ║   11   │        (reserved)         │    (reserved)    ║
	  * ╚════════╧═══════════════════════════╧══════════════════╝
	  */
	StartUpTime result;

	switch ( sut ) {
		case 0b00:
			result.m_startUpTime	= 6;
			result.m_addtionalDelay	= 0;
			break;
		case 0b01:
			result.m_startUpTime	= 6;
			result.m_addtionalDelay	= 4.1e-3;
			break;
		case 0b10:
			result.m_startUpTime	= 6;
			result.m_addtionalDelay	= 65e-3;
			break;
		case 0b11:
			// Reserved
			break;
	}

	return result;
}

inline AVR8ClockControl::StartUpTime AVR8ClockControl::startUpInternalRC(const unsigned int sut) {
	/*
	 * Table: Start-up Times for the Internal Calibrated RC Oscillator Clock Selection
	  * ╔════════╤═══════════════════════════╤══════════════════╗
	  * ║ SUT1:0 │    Start-up Time from     │ Additional Delay ║
	  * ║        │ Power-down and Power-save │   from Reset     ║
	  * ╟────────┼───────────────────────────┼──────────────────╢
	  * ║   00   │            6 CK           │         -        ║
	  * ║   01   │            6 CK           │      4.1 ms      ║
	  * ║   10   │            6 CK           │       65 ms      ║
	  * ║   11   │        (reserved)         │    (reserved)    ║
	  * ╚════════╧═══════════════════════════╧══════════════════╝
	  */
	StartUpTime result;

	switch ( sut ) {
		case 0b00:
			result.m_startUpTime	= 6;
			result.m_addtionalDelay	= 0;
			break;
		case 0b01:
			result.m_startUpTime	= 6;
			result.m_addtionalDelay	= 4.1e-3;
			break;
		case 0b10:
			result.m_startUpTime	= 6;
			result.m_addtionalDelay	= 65e-3;
			break;
		case 0b11:
			// Reserved
			break;
	}

	return result;
}

inline AVR8ClockControl::StartUpTime AVR8ClockControl::startUpExternalRC(const unsigned int sut) {
	/*
	 * Table: Start-up Times for the External RC Oscillator Clock Selection
	  * ╔════════╤═══════════════════════════╤══════════════════╗
	  * ║ SUT1:0 │    Start-up Time from     │ Additional Delay ║
	  * ║        │ Power-down and Power-save │   from Reset     ║
	  * ╟────────┼───────────────────────────┼──────────────────╢
	  * ║   00   │           18 CK           │         -        ║
	  * ║   01   │           18 CK           │      4.1 ms      ║
	  * ║   10   │           18 CK           │       65 ms      ║
	  * ║   11   │            6 CK           │      4.1 ms      ║
	  * ╚════════╧═══════════════════════════╧══════════════════╝
	  */
	StartUpTime result;

	switch ( sut ) {
		case 0b00:
			result.m_startUpTime	= 18;
			result.m_addtionalDelay	= 0;
			break;
		case 0b01:
			result.m_startUpTime	= 18;
			result.m_addtionalDelay	= 4.1e-3;
			break;
		case 0b10:
			result.m_startUpTime	= 18;
			result.m_addtionalDelay	= 65e-3;
			break;
		case 0b11:
			result.m_startUpTime	= 6;
			result.m_addtionalDelay	= 4.1e-3;
			break;
	}

	return result;
}

inline AVR8ClockControl::StartUpTime AVR8ClockControl::startUpLowFreqCrystal(const unsigned int sut) {
	/*
	 * Table: Start-up Times for the Low-frequency Crystal Oscillator Clock Selection
	  * ╔════════╤═══════════════════════════╤══════════════════╗
	  * ║ SUT1:0 │    Start-up Time from     │ Additional Delay ║
	  * ║        │ Power-down and Power-save │   from Reset     ║
	  * ╟────────┼───────────────────────────┼──────────────────╢
	  * ║   00   │           1K CK           │      4.1 ms      ║
	  * ║   01   │           1K CK           │       65 ms      ║
	  * ║   10   │          32K CK           │       65 ms      ║
	  * ║   11   │        (reserved)         │    (reserved)    ║
	  * ╚════════╧═══════════════════════════╧══════════════════╝
	  */
	StartUpTime result;

	switch ( sut ) {
		case 0b00:
			result.m_startUpTime	= 1000;
			result.m_addtionalDelay	= 4.1e-3;
			break;
		case 0b01:
			result.m_startUpTime	= 1000;
			result.m_addtionalDelay	= 65e-3;
			break;
		case 0b10:
			result.m_startUpTime	= 32000;
			result.m_addtionalDelay	= 65e-3;
			break;
		case 0b11:
			// Reserved
			break;
	}

	return result;
}

inline AVR8ClockControl::StartUpTime AVR8ClockControl::startUpHighFreqCrystal(const unsigned int clksel, const unsigned int sut) {
	 /*
	  * Table: Start-up Times for the Crystal Oscillator Clock Selection
	  * ╔════════╤════════╤═══════════════════════════╤══════════════════╗
	  * ║ CKSEL0 │ SUT1:0 │    Start-up Time from     │ Additional Delay ║
	  * ║        │        │ Power-down and Power-save │   from Reset     ║
	  * ╟────────┼────────┼───────────────────────────┼──────────────────╢
	  * ║   0    │   00   │          258 CK           │       4.1 ms     ║
	  * ║   0    │   01   │          258 CK           │        65 ms     ║
	  * ║   0    │   10   │           1K CK           │        –         ║
	  * ║   0    │   11   │           1K CK           │       4.1 ms     ║
	  * ║   1    │   00   │           1K CK           │        65 ms     ║
	  * ║   1    │   01   │          16K CK           │        –         ║
	  * ║   1    │   10   │          16K CK           │       4.1 ms     ║
	  * ║   1    │   11   │          16K CK           │        65 ms     ║
	  * ╚════════╧════════╧═══════════════════════════╧══════════════════╝
	  */

	StartUpTime result;

	if ( 0 == (clksel & 0x01) ) {
		// CKSEL0 = 0
		switch ( sut ) {
			case 0b00:
				result.m_startUpTime	= 258;
				result.m_addtionalDelay	= 4.1e-3;
				break;
			case 0b01:
				result.m_startUpTime	= 258;
				result.m_addtionalDelay	= 65e-3;
				break;
			case 0b10:
				result.m_startUpTime	= 1000;
				result.m_addtionalDelay	= 0;
				break;
			case 0b11:
				result.m_startUpTime	= 1000;
				result.m_addtionalDelay	= 4.1e-3;
				break;
		}
	} else {
		// CKSEL0 = 1
		switch ( sut ) {
			case 0b00:
				result.m_startUpTime	= 1000;
				result.m_addtionalDelay	= 65e-3;
				break;
			case 0b01:
				result.m_startUpTime	= 16000;
				result.m_addtionalDelay	= 0;
				break;
			case 0b10:
				result.m_startUpTime	= 16000;
				result.m_addtionalDelay	= 4.1e-3;
				break;
			case 0b11:
				result.m_startUpTime	= 16000;
				result.m_addtionalDelay	= 65e-3;
				break;
		}
	}

	return result;
}

void AVR8ClockControl::reset(MCUSim::ResetMode) {
}
