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

#include "AVR8InterruptController.h"
#include "AVR8InstructionSet.h"
#include "AVR8DataMemory.h"

AVR8InterruptController::AVR8InterruptController(
		MCUSim::EventLogger * eventLogger,
		AVR8InstructionSet * instructionSet,
		AVR8DataMemory * dataMemory)
		:
		MCUSim::Subsys(eventLogger, ID_INTERRUPTS),
		m_instructionSet(instructionSet),
		m_dataMemory(dataMemory)
{
}

int AVR8InterruptController::autoInterrupt() {
	if ( false == m_dataMemory->readBit(AVR8RegNames::SREG, AVR8RegNames::SREG_I) ) {
		return 0;
	}

	if ( INTVEC_NONE != m_interruptToExecute ) {
		return executeInterrupt(m_interruptToExecute);
	}

	m_interruptToExecute = detectPendingInterrupt();
	return 0;
}

void AVR8InterruptController::reti() {
	// Set Global Interrupt Enable Flag in SREG
	m_dataMemory->writeBit(AVR8RegNames::SREG, AVR8RegNames::SREG_I);

	m_actInterruptCounter--;
}


inline int AVR8InterruptController::executeInterrupt(AVR8InterruptController::InterruptVector vector) {
	// Treat RESET as a special case
	if ( (true == m_config.m_possibleInterrupts[INTVEC_RESET]) && (INTVEC_RESET == vector) ) {
		return -1;
	}

	// Clear Global Interrupt Enable Flag in SREG
	m_dataMemory->writeBit(AVR8RegNames::SREG, AVR8RegNames::SREG_I);

	m_actInterruptCounter++;

	unsigned int pc = m_instructionSet->getProgramCounter();
	m_dataMemory->pushOnStack(pc & 0xff);
	pc >>= 8;
	m_dataMemory->pushOnStack(pc & 0xff);

	m_instructionSet->setProgramCounter((unsigned int)vector);

	return 3; // <-- This is probably specific to AVR with 16b PC, e.g. ATmega8A
}

inline AVR8InterruptController::InterruptVector AVR8InterruptController::detectPendingInterrupt() {

	// External Interrupt Request 0
	if (
		// Does the interrupt exist?
		( true == m_config.m_possibleInterrupts[INTVEC_INT0] )
			&&
		// Is it enabled?
		( true == m_dataMemory->readBit(AVR8RegNames::GICR, AVR8RegNames::GICR_INT0) )
			&&
		// Is it requested?
		( true == m_dataMemory->readBit(AVR8RegNames::GIFR, AVR8RegNames::GIFR_INTF0) )
	) {
		return INTVEC_INT0;
	}

	// External Interrupt Request 1
	if (
		// Does the interrupt exist?
		( true == m_config.m_possibleInterrupts[INTVEC_INT1] )
			&&
		// Is it enabled?
		( true == m_dataMemory->readBit(AVR8RegNames::GICR, AVR8RegNames::GICR_INT1) )
			&&
		// Is it requested?
		( true == m_dataMemory->readBit(AVR8RegNames::GIFR, AVR8RegNames::GIFR_INTF1) )
	) {
		return INTVEC_INT1;
	}

	// Timer/Counter2 Compare Match
	if (
		// Does the interrupt exist?
		( true == m_config.m_possibleInterrupts[INTVEC_TIMER2_COMP] )
			&&
		// Is it enabled?
		( true == m_dataMemory->readBit(AVR8RegNames::TIMSK, AVR8RegNames::TIMSK_OCIE2) )
			&&
		// Is it requested?
		( true == m_dataMemory->readBit(AVR8RegNames::TIFR, AVR8RegNames::TIFR_OCF2) )
	) {
		return INTVEC_TIMER2_COMP;
	}

	// Timer/Counter2 Overflow
	if (
		// Does the interrupt exist?
		( true == m_config.m_possibleInterrupts[INTVEC_TIMER2_OVF] )
			&&
		// Is it enabled?
		( true == m_dataMemory->readBit(AVR8RegNames::TIMSK, AVR8RegNames::TIMSK_TOIE2) )
			&&
		// Is it requested?
		( true == m_dataMemory->readBit(AVR8RegNames::TIFR, AVR8RegNames::TIFR_TOV2) )
	) {
		return INTVEC_TIMER2_OVF;
	}

	// Timer/Counter1 Capture Event
	if (
		// Does the interrupt exist?
		( true == m_config.m_possibleInterrupts[INTVEC_TIMER1_CAPT] )
			&&
		// Is it enabled?
		( true == m_dataMemory->readBit(AVR8RegNames::TIMSK, AVR8RegNames::TIMSK_TICIE1) )
			&&
		// Is it requested?
		( true == m_dataMemory->readBit(AVR8RegNames::TIFR, AVR8RegNames::TIFR_ICF1) )
	) {
		return INTVEC_TIMER1_CAPT;
	}

	// Timer/Counter1 Compare Match A
	if (
		// Does the interrupt exist?
		( true == m_config.m_possibleInterrupts[INTVEC_TIMER1_COMPA] )
			&&
		// Is it enabled?
		( true == m_dataMemory->readBit(AVR8RegNames::TIMSK, AVR8RegNames::TIMSK_OCIE1A) )
			&&
		// Is it requested?
		( true == m_dataMemory->readBit(AVR8RegNames::TIFR, AVR8RegNames::TIFR_OCF1A) )
	) {
		return INTVEC_TIMER1_COMPA;
	}
	
	// Timer/Counter1 Compare Match B
	if (
		// Does the interrupt exist?
		( true == m_config.m_possibleInterrupts[INTVEC_TIMER1_COMPB] )
			&&
		// Is it enabled?
		( true == m_dataMemory->readBit(AVR8RegNames::TIMSK, AVR8RegNames::TIMSK_OCIE1B) )
			&&
		// Is it requested?
		( true == m_dataMemory->readBit(AVR8RegNames::TIFR, AVR8RegNames::TIFR_OCF1B) )
	) {
		return INTVEC_TIMER1_COMPB;
	}
	
	// Timer/Counter1 Overflow
	if (
		// Does the interrupt exist?
		( true == m_config.m_possibleInterrupts[INTVEC_TIMER1_OVF] )
			&&
		// Is it enabled?
		( true == m_dataMemory->readBit(AVR8RegNames::TIMSK, AVR8RegNames::TIMSK_TOIE1) )
			&&
		// Is it requested?
		( true == m_dataMemory->readBit(AVR8RegNames::TIFR, AVR8RegNames::TIFR_TOV1) )
	) {
		return INTVEC_TIMER1_OVF;
	}

	// Timer/Counter0 Overflow
	if (
		// Does the interrupt exist?
		( true == m_config.m_possibleInterrupts[INTVEC_TIMER0_OVF] )
			&&
		// Is it enabled?
		( true == m_dataMemory->readBit(AVR8RegNames::TIMSK, AVR8RegNames::TIMSK_TOIE0) )
			&&
		// Is it requested?
		( true == m_dataMemory->readBit(AVR8RegNames::TIFR, AVR8RegNames::TIFR_TOV0) )
	) {
		return INTVEC_TIMER0_OVF;
	}

// 		INTVEC_SPI_STC		= 0x00A, ///< Serial Transfer Complete
// 		INTVEC_USART_RXC	= 0x00B, ///< USART, Rx Complete
// 		INTVEC_USART_UDRE	= 0x00C, ///< USART Data Register Empty
// 		INTVEC_USART_TXC	= 0x00D, ///< USART, Tx Complete
// 		INTVEC_ADC		= 0x00E, ///< ADC Conversion Complete
// 		INTVEC_EE_RDY		= 0x00F, ///< EEPROM Ready
// 		INTVEC_ANA_COMP		= 0x010, ///< Analog Comparator
// 		INTVEC_TWI		= 0x011, ///< Two-wire Serial Interface
// 		INTVEC_SPM_RDY		= 0x012, ///< Store Program Memory Ready
// 
// 	if ( true == m_config.m_possibleInterrupts[INTVEC_SPI_STC] ) {
// 	}
// 	if ( true == m_config.m_possibleInterrupts[INTVEC_USART_RXC] ) {
// 	}
// 	if ( true == m_config.m_possibleInterrupts[INTVEC_USART_UDRE] ) {
// 	}
// 	if ( true == m_config.m_possibleInterrupts[INTVEC_USART_TXC] ) {
// 	}
// 	if ( true == m_config.m_possibleInterrupts[INTVEC_ADC] ) {
// 	}
// 	if ( true == m_config.m_possibleInterrupts[INTVEC_EE_RDY] ) {
// 	}
// 	if ( true == m_config.m_possibleInterrupts[INTVEC_ANA_COMP] ) {
// 	}
// 	if ( true == m_config.m_possibleInterrupts[INTVEC_TWI] ) {
// 	}
// 	if ( true == m_config.m_possibleInterrupts[INTVEC_SPM_RDY] ) {
// 	}

	return INTVEC_NONE;
}

void AVR8InterruptController::reset(MCUSim::Subsys::SubsysResetMode mode) {
	switch ( mode ) {
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

inline void AVR8InterruptController::mcuReset() {
	m_actInterruptCounter = 0;
	m_interruptToExecute = INTVEC_NONE;
}

inline void AVR8InterruptController::resetToInitialValues() {
}
