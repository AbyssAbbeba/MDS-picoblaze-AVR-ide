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

#include "AVR8FusesAndLocks.h"

#include <cstdlib>

AVR8FusesAndLocks * AVR8FusesAndLocks::link(MCUSim::EventLogger * eventLogger) {
	Subsys::link(eventLogger, ID_FUSES);

	return this;
}

void AVR8FusesAndLocks::setLockBits(unsigned char lb) {
	unsigned char mask = 0x1;
	for ( int i = 0; i < 8; i++ ) {
		if ( lb & mask ) {
			m_lockBits[i] = false;
		} else {
			m_lockBits[i] = true;
		}
		mask <<= 1;
	}

	logEvent(EVENT_LOCK_BITS_SET);
}

unsigned char AVR8FusesAndLocks::operator[] (Bytes byte) const {
	unsigned char result = 0;
	unsigned char mask = 0x01;
	const bool * array;
	int idx;

	switch ( byte ) {
		case BYTE_FUSES_HIGH:
			array = m_fuses;
			idx = 8;
			break;
		case BYTE_FUSES_LOW:
			array = m_fuses;
			idx = 0;
			break;
		case BYTE_LOCKS_LOW:
			array = m_lockBits;
			idx = 0;
			break;
		default:
			// TODO: add some reasonable error handling here
			return 0;
	}

	for ( int i = 0; i < 8; i++ ) {
		if ( false == array[idx++] ) {
			result |= mask;
		}
		mask <<= 1;
	}

	return result;
}

void AVR8FusesAndLocks::reset(MCUSim::ResetMode mode) {
	switch ( mode ) {
		case MCUSim::RSTMD_NEW_CONFIG:
			loadConfig();
			break;
		case MCUSim::RSTMD_INITIAL_VALUES:
			resetToInitialValues();
			break;
		default:
			// Irrelevant requests are silently ignored
			break;
	}
}

inline void AVR8FusesAndLocks::loadConfig() {
	// Set to default values
	unsigned char mask = 0x01;
	for ( int i = 0; i < 16; i++ ) {
		if ( mask & m_config.m_defaultFuses ) {
			m_fuses[i] = true;
		} else {
			m_fuses[i] = false;
		}
		mask <<= 1;
	}
	mask = 0x01;
	for ( int i = 0; i < 8; i++ ) {
		if ( mask & m_config.m_defaultLockBits ) {
			m_lockBits[i] = true;
		} else {
			m_lockBits[i] = false;
		}
		mask <<= 1;
	}
}
	
inline void AVR8FusesAndLocks::resetToInitialValues() {
	for ( int i = 0; i < FUSE__MAX__; i++ ) {
		m_fuses[i] = false;
	}
}

unsigned int AVR8FusesAndLocks::getUndefVal() const {
	if ( -1 == m_config.m_undefinedValue ) {
		// Generate random value
		return ( (unsigned int)rand() & ((1 << 8) - 1) );
	} else {
		// Return predefined value
		return ( m_config.m_undefinedValue & ((1 << 8) - 1) );
	}
}
