/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 * @ingroup PIC8
 * @file PIC8RegNames.h
 */


#ifndef PIC8REGNAMES_H
#define PIC8REGNAMES_H


/**
 * @brief
 * @ingroup PIC8
 * @namespace PIC8RegNames
 */
namespace PIC8RegNames {
	// ---BANK0---
	// TMR0 - 8-bit Real-Time Clock/Counter
	// PCL - Low Order 8 bits of Program Counter
	// SREG (STATUS) (<7:0> - IPR, RP1, RP0, TO, PD, Z, DC, C)
	// FSR - Indirect Data Memory Address Pointer 0
	// PORTA (<4:0> - RA4/T0CKI, RA3, RA2, RA1, RA0)
	// PORTB (<7:0> - RB7, RB6, RB5, RB4, RB3, RB2, RB1, RB0/INT)
	// EEDATA - EEPROM Data Register
	// EEADR - EEPROM Address Register
	// PCLATH (<4:0>) - Write Buffer for upper 5 bits of Program Counter
	// INTCON (<7:0> - GIE, EEIE, T0IE, INTE, RBIE, T0IF, INTF, RBIF)
	// ---End of BANK0---
	enum BANK0 {
		TMR0	= 0x01,
		PCL	= 0x02,
		SREG	= 0x03,
		FSR	= 0x04,
		PORTA	= 0x05,
		PORTB	= 0x06,
		EEDATA	= 0x08,
		EEADR	= 0x09,
		PCLATH	= 0xA0,
		INTCON	= 0x0B
	};
	// ---BANK1---
	// OPTREG (OPTION_REG) - (<7:0> - RBPU, INTEDG, T0CS, T0SE, PSA, PS2, PS1, PS0)
	// PCL - same as in BANK0
	// SREG (STATUS) - same as in BANK0
	// FSR - same as in BANK0
	// TRISA (<4:0>) - PORTA Data Direction Register
	// TRISB - PORTB Data Direction Register
	// EECON1 (<4:0> - EEIF, WRERR, WREN, WR, RD)
	// EECON2 - EEPROM Control Register 2 (not physical)
	// PCLATH - same as in BANK0
	// INTCON - same as in BANK0
	// ---End of BANK1---
	enum BANK1 {
		OPTREG	= 0x081,
		TRISA	= 0x085,
		TRISB	= 0x086,
		EECON1	= 0x088,
		EECON2	= 0x089
	};
	enum BITS {
		// SREG (0x03/0x83)
		SREG_C	= 0x01,
		SREG_DC	= 0x02,
		SREG_Z	= 0x04,
		SREG_PD	= 0x08,
		SREG_TO	= 0x010,
		SREG_RP0 = 0x020,
		SREG_RP1 = 0x040,
		SREG_IRP = 0x080,

		// PORTA (0x05)
		PORTA_RA0	= 0x01,
		PORTA_RA1	= 0x02,
		PORTA_RA2	= 0x04,
		PORTA_RA3	= 0x08,
		PORTA_RA4_T0CKI	= 0x010,

		// PORTB (0x06)

		// PCLATH (0x0A/0x08A)

		// INCON (0x0B, 0x08B)

		// OPTREG (0x081)

		// TRISA (0x085)

		// EECON1 (0x088)

		// EECON2 (0x089)
	}
	// Working Register?

}


#endif //PIC8REGNAMES_H
