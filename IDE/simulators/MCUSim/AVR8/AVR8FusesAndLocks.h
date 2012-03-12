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
 * @file AVR8FusesAndLocks.h
 */

#ifndef AVR8FUSESANDLOCKS_H
#define AVR8FUSESANDLOCKS_H

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8FusesAndLocks
 */
class AVR8FusesAndLocks : public MCUSim::Subsys {
public:
	AVR8FusesAndLocks() {};

	enum Event {
		EVENT_LOCK_BITS_SET
	};

	// Fuse High and Low Byte:
	// -----------------------
	enum Fuses {
		// Bit		Bit #
		FUSE_CKSEL0	=  0, ///< Select Clock source, default: 1 (unprogrammed)
		FUSE_CKSEL1	=  1, ///< Select Clock source, default: 0 (programmed)
		FUSE_CKSEL2	=  2, ///< Select Clock source, default: 0 (programmed)
		FUSE_CKSEL3	=  3, ///< Select Clock source, default: 0 (programmed)
		FUSE_SUT0	=  4, ///< Select start-up time, default: 0 (programmed)
		FUSE_SUT1	=  5, ///< Select start-up time, default: 1 (unprogrammed)
		FUSE_BODEN	=  6, ///< Brown out detector enable, default: 1 (unprogrammed, BOD disabled)
		FUSE_BODLEVEL	=  7, ///< Brown out detector trigger level, default: 1 (unprogrammed)

		FUSE_BOOTRST	=  8, ///< Select Reset Vector, default: 1 (unprogrammed)
		FUSE_BOOTSZ0	=  9, ///< Select Boot Size, default: 0 (programmed)
		FUSE_BOOTSZ1	= 10, ///< Select Boot Size, default: 0 (programmed)
		FUSE_EESAVE	= 11, ///< EEPROM memory is preserved through the Chip Erase, default: 1 (unprogrammed, EEPROM not preserved)
		FUSE_CKOPT	= 12, ///< Oscillator options, default: 1 (unprogrammed)
		FUSE_SPIEN	= 13, ///< Enable Serial Program and Data Downloading, default: 0 (programmed, SPI prog. enabled)
		FUSE_WDTON	= 14, ///< WDT always on, default: 1 (unprogrammed, WDT enabled by WDTCR)
		FUSE_RSTDISBL	= 15, ///< Select if PC6 is I/O pin or RESET pin, default: 1 (unprogrammed, PC6 is RESET-pin)

		FUSE_JTAGEN, // TODO: Assign some reasonable value
		FUSE_OCDEN, // TODO: Assign some reasonable value

		FUSE__MAX__
	};

	enum LockBits {
		LB_LB1		= 0, ///< Lock bit, default: 1 (unprogrammed)
		LB_LB2		= 1, ///< Lock bit, default: 1 (unprogrammed)
		LB_BLB01	= 2, ///< Boot lock bit, default: 1 (unprogrammed)
		LB_BLB02	= 3, ///< Boot lock bit, default: 1 (unprogrammed)
		LB_BLB11	= 4, ///< Boot lock bit, default: 1 (unprogrammed)
		LB_BLB12	= 5, ///< Boot lock bit, default: 1 (unprogrammed)
		LB_RESERVED0	= 6, ///< Reserved
		LB_RESERVED1	= 7, ///< Reserved

		LB__MAX__
	};

	enum Bytes {
		BYTE_FUSES_LOW,
		BYTE_FUSES_HIGH,
		BYTE_LOCKS_LOW
	};

	struct Config {
		int m_undefinedValue; // -1 means random
	};

	Config m_config;

	bool operator[] (Fuses fuse) const {
		return m_fuses[fuse];
	}
	void setFuse(Fuses fuse, bool value) {
		m_fuses[fuse] = value;
	}

	bool operator[] (LockBits lb) const {
		return m_lockBits[lb];
	}
	void setLockBit(LockBits lb, bool value) {
		m_lockBits[lb] = value;
	}

	void setLockBits(unsigned char lb);
	unsigned char operator[] (Bytes byte) const;

	template<unsigned int sizeBits> unsigned int getUndefVal() const;

	AVR8FusesAndLocks * link(MCUSim::EventLogger * eventLogger);
	void reset(MCUSim::ResetMode mode);

protected:
	// False means unprogrammed (1), true means programmed (0); it's confusing, isn't it? But that's how the manual describes it...
	bool m_fuses[FUSE__MAX__];
	bool m_lockBits[LB__MAX__];

private:
	inline void resetToInitialValues();
};

#endif // AVR8FUSESANDLOCKS_H
