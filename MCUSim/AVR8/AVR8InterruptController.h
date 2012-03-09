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

// TABLE OF CURRENTLY SUPPORTED INTERRUPTS:
// ----------------------------------------
// 1	0x000	RESET		External Pin, Power-on Reset, Brown-out Reset, and Watchdog Reset
// 2	0x001	INT0		External Interrupt Request 0
// 3	0x002	INT1		External Interrupt Request 1
// 4	0x003	TIMER2 COMP	Timer/Counter2 Compare Match
// 5	0x004	TIMER2 OVF	Timer/Counter2 Overflow
// 6	0x005	TIMER1 CAPT	Timer/Counter1 Capture Event
// 7	0x006	TIMER1 COMPA	Timer/Counter1 Compare Match A
// 8	0x007	TIMER1 COMPB	Timer/Counter1 Compare Match B
// 9	0x008	TIMER1 OVF	Timer/Counter1 Overflow
// 10	0x009	TIMER0 OVF	Timer/Counter0 Overflow
// 11	0x00A	SPI, STC	Serial Transfer Complete
// 12	0x00B	USART, RXC	USART, Rx Complete
// 13	0x00C	USART, UDRE	USART Data Register Empty
// 14	0x00D	USART, TXC	USART, Tx Complete
// 15	0x00E	ADC		ADC Conversion Complete
// 16	0x00F	EE_RDY		EEPROM Ready
// 17	0x010	ANA_COMP	Analog Comparator
// 18	0x011	TWI		Two-wire Serial Interface
// 19	0x012	SPM_RDY		Store Program Memory Ready

#ifndef AVR8INTERRUPTCONTROLLER_H
#define AVR8INTERRUPTCONTROLLER_H

class AVR8InstructionSet;
class AVR8ProgramMemory;
class AVR8DataMemory;
class AVR8FusesAndLocks;
class AVR8BootLoader;

#include "../MCUSim.h"
#include "AVR8Sim.h"

class AVR8InterruptController : public MCUSim::Subsys {
public:
	AVR8InterruptController();

	enum Event {
		EVENT_INT_ENTERING_INTERRUPT,
		EVENT_INT_INTERRUPT_CANCELED,
		EVENT_INT_LEAVING_INTERRUPT,

		EVENT_INT__MAX__
	};

	enum InterruptVector {
		INTVEC_NONE		= -1,    ///<

		INTVEC_RESET		= 0x000, ///< External Pin, Power-on Reset, Brown-out Reset, and Watchdog Reset
		INTVEC_INT0		= 0x001, ///< External Interrupt Request 0
		INTVEC_INT1		= 0x002, ///< External Interrupt Request 1
		INTVEC_TIMER2_COMP	= 0x003, ///< Timer/Counter2 Compare Match
		INTVEC_TIMER2_OVF	= 0x004, ///< Timer/Counter2 Overflow
		INTVEC_TIMER1_CAPT	= 0x005, ///< Timer/Counter1 Capture Event
		INTVEC_TIMER1_COMPA	= 0x006, ///< Timer/Counter1 Compare Match A
		INTVEC_TIMER1_COMPB	= 0x007, ///< Timer/Counter1 Compare Match B
		INTVEC_TIMER1_OVF	= 0x008, ///< Timer/Counter1 Overflow
		INTVEC_TIMER0_OVF	= 0x009, ///< Timer/Counter0 Overflow
		INTVEC_SPI_STC		= 0x00A, ///< Serial Transfer Complete
		INTVEC_USART_RXC	= 0x00B, ///< USART, Rx Complete
		INTVEC_USART_UDRE	= 0x00C, ///< USART Data Register Empty
		INTVEC_USART_TXC	= 0x00D, ///< USART, Tx Complete
		INTVEC_ADC		= 0x00E, ///< ADC Conversion Complete
		INTVEC_EE_RDY		= 0x00F, ///< EEPROM Ready
		INTVEC_ANA_COMP		= 0x010, ///< Analog Comparator
		INTVEC_TWI		= 0x011, ///< Two-wire Serial Interface
		INTVEC_SPM_RDY		= 0x012, ///< Store Program Memory Ready

		INTVEC__MAX__
	};

	enum InterruptGroup {
		INTGRP_EXTERNAL = 0,
		INTGRP_TIMER012,
		INTGRP_UART,

		INTGRP__MAX__
	};

	struct Config {
		bool m_possibleInterrupts[INTVEC__MAX__];
		bool m_interruptGroups[INTGRP__MAX__];
	};

	Config m_config;

	AVR8InterruptController * link(
		MCUSim::EventLogger	* eventLogger,
		AVR8InstructionSet	* instructionSet,
		AVR8ProgramMemory	* programMemory,
		AVR8DataMemory		* dataMemory,
		AVR8FusesAndLocks	* fusesAndLocks,
		AVR8Sim::SleepMode	* sleepMode,
		AVR8BootLoader		* bootLoader);

	void reset(MCUSim::ResetMode mode);

	/**
	 * @return -1: system reset requested; 0: do nothing; N: interrupt executed, takes N cycles
	 */
	int autoInterrupt();
	void reti();
	void genIntReq(InterruptVector interrupt);

	bool interrupted() {
		if ( 0 == m_actInterruptCounter ) {
			return false;
		} else {
			return true;
		}
	}

protected:
	AVR8InstructionSet * m_instructionSet;
	AVR8ProgramMemory * m_programMemory;
	AVR8DataMemory * m_dataMemory;
	AVR8FusesAndLocks & m_fusesAndLocks;
	AVR8Sim::SleepMode & m_sleepMode;
	AVR8BootLoader * m_bootLoader;

	int m_actInterruptCounter;
	int m_interruptCounter[INTVEC__MAX__];
	bool m_intReqWithoutFlag[INTVEC__MAX__];
	InterruptVector m_interruptToExecute;
	int m_interruptFlagToClear[2];

	inline void resetToInitialValues();
	inline void mcuReset();

	inline int executeInterrupt(InterruptVector vector);
	inline InterruptVector detectPendingInterrupt();
	inline bool confirmInterrupt(InterruptVector vector);
};

#endif // AVR8INTERRUPTCONTROLLER_H
