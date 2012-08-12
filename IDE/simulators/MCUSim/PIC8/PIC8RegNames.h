/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @ingroup PIC8
 * @file PIC8RegNames.h
 */

#ifdef PIC8REGNAMES_H
#define PIC8REGNAMES_H

namespace PIC8RegNames {
	enum SFR {
		/* Bank 0 */
		INDF		= 0x00,
		TMR0		= 0x01,
		PCL		= 0x02,
		STATUS		= 0x03,
		FSR		= 0x04,
		PORTA		= 0x05,
		PORTB		= 0x06,
		// (0x07 Unimplemented location)
		EEDATA		= 0x08,
		EEADR		= 0x09,
		PCLATH		= 0x0A,
		INTCON		= 0x0B,

		/* Bank 1 */
		INDF		= 0x80,
		OPTION_REG	= 0x81,
		PCL		= 0x82,
		STATUS		= 0x83,
		FSR		= 0x84,
		TRISA		= 0x85,
		TRISB		= 0x86,
		// (0x87 Unimplemented location)
		EECON1		= 0x88,
		EECON2		= 0x89,
		PCLATH		= 0x0A,
		INTCON		= 0x0B
	}

	enum SFB {
		/* Bank 0 */
		STATUS_IRP		= 7,
		STATUS_RP1		= 6,
		STATUS_RP0		= 5,
		STATUS_TO		= 4,
		STATUS_PD		= 3,
		STATUS_Z		= 2,
		STATUS_DC		= 1,
		STATUS_C		= 1,

		PORTA_RA4		= 4,
		PORTA_T0CKI		= 4,
		PORTA_RA3		= 3,
		PORTA_RA2		= 2,
		PORTA_RA1		= 1,
		PORTA_RA0		= 0,

		PORTB_RB7		= 7,
		PORTB_RB6		= 6,
		PORTB_RB5		= 5,
		PORTB_RB4		= 4,
		PORTB_RB3		= 3,
		PORTB_RB2		= 2,
		PORTB_RB1		= 1,
		PORTB_RB0		= 0,
		PORTB_INT		= 0,

		INTCON_GIE		= 7,
		INTCON_EEIE		= 6,
		INTCON_T0IE		= 5,
		INTCON_INTE		= 4,
		INTCON_RBIE		= 3,
		INTCON_T0IF		= 2,
		INTCON_INTF		= 1,
		INTCON_RBIF		= 0,

		/* Bank 1 */
		OPTION_REG_RBPU		= 7,
		OPTION_REG_INTEDG	= 6,
		OPTION_REG_T0CS		= 5,
		OPTION_REG_T0SE		= 4,
		OPTION_REG_PSA		= 3,
		OPTION_REG_PS2		= 2,
		OPTION_REG_PS1		= 1,
		OPTION_REG_PS0		= 0,

		STATUS_IRP		= 7,
		STATUS_RP1		= 6,
		STATUS_RP0		= 5,
		STATUS_TO		= 4,
		STATUS_PD		= 3,
		STATUS_Z		= 2,
		STATUS_DC		= 1,
		STATUS_C		= 0,
		
		EECON1_EEIF		= 4,
		EECON1_WRERR		= 3,
		EECON1_WREN		= 2,
		EECON1_WR		= 1,
		EECON1_RD		= 0,
		
		INTCON_GIE		= 7,
		INTCON_EEIE		= 6,
		INTCON_T0IE		= 5,
		INTCON_INTE		= 4,
		INTCON_RBIE		= 3,
		INTCON_T0IF		= 2,
		INTCON_INTF		= 1,
		INTCON_RBIF		= 0
	}
};

#endif // PIC8REGNAMES_H
