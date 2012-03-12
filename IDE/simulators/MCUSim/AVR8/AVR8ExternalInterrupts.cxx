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

#include "AVR8ExternalInterrupts.h"
#include "AVR8DataMemory.h"
#include "AVR8IO.h"

AVR8ExternalInterrupts * AVR8ExternalInterrupts::link(
		MCUSim::EventLogger	* eventLogger,
		AVR8DataMemory		* dataMemory,
		AVR8IO			* io
) {
	Subsys::link(eventLogger, ID_EXT_INT);

	m_dataMemory = dataMemory;
	m_io = io;

	return this;
}

void AVR8ExternalInterrupts::clockCycle() {
	using namespace AVR8RegNames;

	// Interrupt n Sense Control:
	// --------------------------
	// ISCn1	ISCn0	Description
	// 0	0	The low level of INTn generates an interrupt request.
	// 0	1	Any logical change on INTn generates an interrupt request.
	// 1	0	The falling edge of INTn generates an interrupt request.
	// 1	1	The rising edge of INTn generates an interrupt request.

	const unsigned int mcucr = m_dataMemory->readFast(MCUCR);
	const bool int0 = m_io->getLog(AVR8PinNames::SPF_INT0);
	const bool int1 = m_io->getLog(AVR8PinNames::SPF_INT1);

	// External interrupt 0
	switch ( mcucr & ( MCUCR_ISC01 | MCUCR_ISC00 ) ) {
		case 0:
			if ( true == int0 ) {
				m_dataMemory->setBitFast(GIFR, GIFR_INTF0);
			}
			break;
		case MCUCR_ISC00:
			if ( m_int0prev != int0 ) {
				m_dataMemory->setBitFast(GIFR, GIFR_INTF0);
			}
			break;
		case MCUCR_ISC01:
			if ( (true == m_int0prev) && (false == int0) ) {
				m_dataMemory->setBitFast(GIFR, GIFR_INTF0);
			}
			break;
		case (MCUCR_ISC01 | MCUCR_ISC00):
			if ( (false == m_int0prev) && (true == int0) ) {
				m_dataMemory->setBitFast(GIFR, GIFR_INTF0);
			}
			break;
	}

	// External interrupt 1
	switch ( mcucr & ( MCUCR_ISC11 | MCUCR_ISC10 ) ) {
		case 0:
			if ( true == int1 ) {
				m_dataMemory->setBitFast(GIFR, GIFR_INTF1);
			}
			break;
		case MCUCR_ISC10:
			if ( m_int1prev != int1 ) {
				m_dataMemory->setBitFast(GIFR, GIFR_INTF1);
			}
			break;
		case MCUCR_ISC11:
			if ( (true == m_int1prev) && (false == int1) ) {
				m_dataMemory->setBitFast(GIFR, GIFR_INTF1);
			}
			break;
		case (MCUCR_ISC11 | MCUCR_ISC10):
			if ( (false == m_int1prev) && (true == int1) ) {
				m_dataMemory->setBitFast(GIFR, GIFR_INTF1);
			}
			break;
	}

	m_int0prev = int0;
	m_int1prev = int1;
}

void AVR8ExternalInterrupts::reset(MCUSim::ResetMode mode) {
	switch ( mode ) {
		case MCUSim::RSTMD_MCU_RESET:
			mcuReset();
			break;
		default:
			// Irrelevant requests are silently ignored
			break;
	}
}

inline void AVR8ExternalInterrupts::mcuReset() {
	m_int0prev = m_io->getLog(AVR8PinNames::SPF_INT0);
	m_int1prev = m_io->getLog(AVR8PinNames::SPF_INT1);
}
