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

#include "AVR8ConfigWord.h"

#include <cstdlib>

AVR8ConfigWord * AVR8ConfigWord::link(MCUSim::EventLogger * eventLogger) {
	Subsys::link(eventLogger, ID_FUSES);
	return this;
}

void AVR8ConfigWord::reset(MCUSim::ResetMode mode) {
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

inline void AVR8ConfigWord::loadConfig() {
	// Set to default values
	unsigned char mask = 0x01;
	for ( int i = 0; i < CFGW__MAX__; i++ ) {
		if ( mask & m_config.m_defaultCfgWord ) {
			m_cfgWord[i] = true;
		} else {
			m_cfgWord[i] = false;
		}
		mask <<= 1;
	}
}

inline void AVR8ConfigWord::resetToInitialValues() {
	for ( int i = 0; i < CFGW__MAX__; i++ ) {
		m_cfgWord[i] = false;
	}
}
