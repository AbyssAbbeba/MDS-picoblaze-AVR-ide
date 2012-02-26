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

#ifndef AVR8FUSESANDLOCKS_H
#define AVR8FUSESANDLOCKS_H

#include "../MCUSim.h"

class AVR8FusesAndLocks : public MCUSim::Subsys {
public:

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
		// LB_	= 6, ///< Reserved
		// LB_	= 7, ///< Reserved

		LB__MAX__
	};

	AVR8FusesAndLocks(MCUSim::EventLogger * eventLogger);

	bool operator[] (Fuses fuse) {
		return m_fuses[fuse];
	}
	void setFuse(Fuses fuse, bool value) {
		m_fuses[fuse] = value;
	}

	bool operator[] (LockBits lb) {
		return m_lockBits[lb];
	}
	void setLockBit(LockBits lb, bool value) {
		m_lockBits[lb] = value;
	}

	void reset(SubsysResetMode mode);

protected:
	// False means unprogrammed (1), true means programmed (0); it's confusing, isn't it? But that's how the manual describes it...
	bool m_fuses[FUSE__MAX__];
	bool m_lockBits[LB__MAX__];

private:
	AVR8FusesAndLocks();
	inline void resetToInitialValues();
};

#endif // AVR8FUSESANDLOCKS_H
