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

#ifndef AVR8REGNAMES_H
#define AVR8REGNAMES_H

// ATmega8

namespace AVR8RegNames {
	enum CPURegisters {
		/// @name CPU register file
		//@{
		R0	= 0x00,		R1	= 0x01,
		R2	= 0x02,		R3	= 0x03,
		R4	= 0x04,		R5	= 0x05,
		R6	= 0x06,		R7	= 0x07,
		R8	= 0x08,		R9	= 0x09,
		R10	= 0x0A,		R11	= 0x0B,
		R12	= 0x0C,		R13	= 0x0D,
		R14	= 0x0E,		R15	= 0x0F,
		R16	= 0x10,		R17	= 0x11,
		R18	= 0x12,		R19	= 0x13,
		R20	= 0x14,		R21	= 0x15,
		R22	= 0x16,		R23	= 0x17,
		R24	= 0x18,		R25	= 0x19,
		R26	= 0x1A,		R27	= 0x1B,
		R28	= 0x1C,		R29	= 0x1D,
		R30	= 0x1E,		R31	= 0x1F,
		//@}

		/// @name 16-bit registers X, Y, and Z
		//@{
		XL	= R26,		XH	= R27,
		YL	= R28,		YH	= R29,
		ZL	= R30,		ZH	= R31
		//@}
	};

	enum SFR {
		 // TODO: find it, and fill it in here
		RAMPD = 0,
		RAMPZ = 0,

		SREG	= 0x5F,		///< The AVR Status Register
		SPH	= 0x5E,		///< Stack Pointer High Register
		SPL	= 0x5D,		///< Stack Pointer Low Register
		OCR0	= 0x5C,		///< Output Compare Register
		EIND	= 0x5C,		///< Extended Indirect Register
		GICR	= 0x5B,		///< General Interrupt Control Register
		GIFR	= 0x5A,		///< General Interrupt Flag Register
		TIMSK	= 0x59,		///< Timer/Counter Interrupt Mask Register
		TIFR	= 0x58,		///< Timer/Counter Interrupt Flag Register
		SPMCR	= 0x57,		///< Store Program Memory Control Register
		TWCR	= 0x56,		///< TWI Control Register
		MCUCR	= 0x55,		///< MCU Control Register
		MCUCSR	= 0x54,		///< MCU Control and Status Register
		TCCR0	= 0x53,		///< Timer/Counter Control Register
		TCNT0	= 0x52,		///< Timer/Counter Register
		OSCCAL	= 0x51,		///< Oscillator Calibration Register
		SFIOR	= 0x50,		///< Special Function IO Register
		TCCR1A	= 0x4F,		///< Timer/Counter 1 Control Register A
		TCCR1B	= 0x4E,		///< Timer/Counter 1 Control Register B
		TCNT1H	= 0x4D,		///< Timer/Counter 1
		TCNT1L	= 0x4C,		///< Timer/Counter 1
		OCR1AH	= 0x4B,		///< Output Compare Register 1 A
		OCR1AL	= 0x4A,		///< Output Compare Register 1 A
		OCR1BH	= 0x49,		///< Output Compare Register 1 B
		OCR1BL	= 0x48,		///< Output Compare Register 1 B
		ICR1H	= 0x47,		///< Input Capture Register 1
		ICR1L	= 0x46,		///< Input Capture Register 1
		TCCR2	= 0x45,		///< Timer/Counter Control Register
		TCNT2	= 0x44,		///< Timer/Counter Register
		OCR2	= 0x43,		///< Output Compare Register
		ASSR	= 0x42,		///< Asynchronous Status Register
		WDTCR	= 0x41,		///< Watchdog Timer Control Register
		UBRRH	= 0x40 | 1<<24,	///< USART Baud Rate Register
		UCSRC	= 0x40 | 2<<24,	///< USART Control and Status Register C
		EEARH	= 0x3F,		///< EEPROM Address Register
		EEARL	= 0x3E,		///< EEPROM Address Register
		EEDR	= 0x3D,		///< EEPROM Data Register
		EECR	= 0x3C,		///< EEPROM Control Register
		PORTA	= 0x3B,		///< Port A Data Register
		DDRA	= 0x3A,		///< Port A Data Direction Register
		PINA	= 0x39,		///< Port A Input Pins Address
		PORTB	= 0x38,		///< Port B Data Register
		DDRB	= 0x37,		///< Port B Data Direction Register
		PINB	= 0x36,		///< Port B Input Pins Address
		PORTC	= 0x35,		///< Port C Data Register
		DDRC	= 0x34,		///< Port C Data Direction Register
		PINC	= 0x33,		///< Port C Input Pins Address
		PORTD	= 0x32,		///< Port D Data Register
		DDRD	= 0x31,		///< Port D Data Direction Register
		PIND	= 0x30,		///< Port D Input Pins Address
		SPDR	= 0x2F,		///< SPI Data Register
		SPSR	= 0x2E,		///< SPI Status Register
		SPCR	= 0x2D,		///< SPI Control Register
		UDR	= 0x2C,		///< USART I/O Data Register
		UCSRA	= 0x2B,		///< USART Control and Status Register A
		UCSRB	= 0x2A,		///< USART Control and Status Register B
		UBRRL	= 0x29,		///< USART Baud Rate Register
		ACSR	= 0x28,		///< Analog Comparator Control and Status Register
		ADMUX	= 0x27,		///< ADC Multiplexer Selection Register
		ADCSRA	= 0x26,		///< ADC Control and Status Register A
		ADCH	= 0x25,		///< The ADC Data Register
		ADCL	= 0x24,		///< The ADC Data Register
		TWDR	= 0x23,		///< TWI Data Register
		TWAR	= 0x22,		///< TWI (Slave) Address Register
		TWSR	= 0x21,		///< TWI Status Register
		TWBR	= 0x20		///< TWI Bit Rate Register
	};

	enum SFB {
		/// @name SREG (0x5F): The AVR Status Register
		//@{
		SREG_C		= 0x01, ///< Carry Flag
		SREG_Z		= 0x02, ///< Zero Flag
		SREG_N		= 0x04, ///< Negative Flag
		SREG_V		= 0x08, ///< Two's Complement Overflow Flag
		SREG_S		= 0x10, ///< Sign Bit, S = N ⊕ V
		SREG_H		= 0x20, ///< Half Carry Flag
		SREG_T		= 0x40, ///< Bit Copy Storage
		SREG_I		= 0x80, ///< Global Interrupt Enable
		//@}

		/// @name SPH (0x5E): Stack Pointer High Register
		//@{
		SPH_SP8		= 0x01, ///<
		SPH_SP9		= 0x02, ///<
		SPH_SP10	= 0x04, ///<
		//SPH_		= 0x08, ///<
		//SPH_		= 0x10, ///<
		//SPH_		= 0x20, ///<
		//SPH_		= 0x40, ///<
		//SPH_		= 0x80, ///<
		//@}

		/// @name SPL (0x5D): Stack Pointer Low Register
		//@{
		SPL_SP0		= 0x01, ///<
		SPL_SP1		= 0x02, ///<
		SPL_SP2		= 0x04, ///<
		SPL_SP3		= 0x08, ///<
		SPL_SP4		= 0x10, ///<
		SPL_SP5		= 0x20, ///<
		SPL_SP6		= 0x40, ///<
		SPL_SP7		= 0x80, ///<
		//@}

		/// @name EIND (0x5C): Extended Indirect Register
		//@{
		EIND_EIND0	= 0x01, ///<
		EIND_EIND1	= 0x02, ///<
		EIND_EIND2	= 0x04, ///<
		EIND_EIND3	= 0x08, ///<
		EIND_EIND4	= 0x10, ///<
		EIND_EIND5	= 0x20, ///<
		EIND_EIND6	= 0x40, ///<
		EIND_EIND7	= 0x80, ///<
		//@}

		/// @name GICR (0x5B): General Interrupt Control Register
		//@{
		GICR_IVCE	= 0x01, ///<
		GICR_IVSEL	= 0x02, ///< Interrupt Vector Select
		//GICR_		= 0x04, ///<
		//GICR_		= 0x08, ///<
		//GICR_		= 0x10, ///<
		GICR_INT2	= 0x20, ///<
		GICR_INT0	= 0x40, ///< External Interrupt Request 0 Enable
		GICR_INT1	= 0x80, ///< External Interrupt Request 1 Enable
		//@}

		/// @name GIFR (0x5A): General Interrupt Flag Register
		//@{
		//GIFR_		= 0x01, ///<
		//GIFR_		= 0x02, ///<
		//GIFR_		= 0x04, ///<
		//GIFR_		= 0x08, ///<
		//GIFR_		= 0x10, ///<
		GIFR_INTF2	= 0x20, ///<
		GIFR_INTF0	= 0x40, ///< External Interrupt Flag 0
		GIFR_INTF1	= 0x80, ///< External Interrupt Flag 1
		//@}

		/// @name TIMSK (0x59): Timer/Counter Interrupt Mask Register
		//@{
		TIMSK_TOIE0	= 0x01, ///< Timer/Counter0 Overflow Interrupt Enable
		TIMSK_OCIE0	= 0x02, ///<
		TIMSK_TOIE1	= 0x04, ///< Timer/Counter1, Overflow Interrupt Enable
		TIMSK_OCIE1B	= 0x08, ///< Timer/Counter1, Output Compare B Match Interrupt Enable
		TIMSK_OCIE1A	= 0x10, ///< Timer/Counter1, Output Compare A Match Interrupt Enable
		TIMSK_TICIE1	= 0x20, ///< Timer/Counter1, Input Capture Interrupt Enable
		TIMSK_TOIE2	= 0x40, ///< Timer/Counter2 Overflow Interrupt Enable
		TIMSK_OCIE2	= 0x80, ///< Timer/Counter2 Output Compare Match Interrupt Enable
		//@}

		/// @name TIFR (0x58): Timer/Counter Interrupt Flag Register
		//@{
		TIFR_TOV0	= 0x01, ///< Timer/Counter0 Overflow Flag
		TIFR_OCF0	= 0x02, ///<
		TIFR_TOV1	= 0x04, ///< Timer/Counter1, Overflow Flag
		TIFR_OCF1B	= 0x08, ///< Timer/Counter1, Output Compare B Match Flag
		TIFR_OCF1A	= 0x10, ///< Timer/Counter1, Output Compare A Match Flag
		TIFR_ICF1	= 0x20, ///< Timer/Counter1, Input Capture Flag
		TIFR_TOV2	= 0x40, ///< Timer/Counter2 Overflow Flag
		TIFR_OCF2	= 0x80, ///< Output Compare Flag 2
		//@}

		/// @name SPMCR (0x57): Store Program Memory Control Register
		//@{
		SPMCR_SPMEN	= 0x01, ///< Store Program Memory Enable
		SPMCR_PGERS	= 0x02, ///< Page Erase
		SPMCR_PGWRT	= 0x04, ///< Page Write
		SPMCR_BLBSET	= 0x08, ///< Boot Lock Bit Set
		SPMCR_RWWSRE	= 0x10, ///< Read-While-Write Section Read Enable
		//SPMCR_	= 0x20, ///<
		SPMCR_RWWSB	= 0x40, ///< Read-While-Write Section Busy
		SPMCR_SPMIE	= 0x80, ///< SPM Interrupt Enable
		//@}

		/// @name TWCR (0x56): TWI Control Register
		//@{
		TWCR_TWIE	= 0x01, ///< TWI Interrupt Enable
		//TWCR_		= 0x02, ///<
		TWCR_TWEN	= 0x04, ///<
		TWCR_TWWC	= 0x08, ///<
		TWCR_TWSTO	= 0x10, ///<
		TWCR_TWSTA	= 0x20, ///<
		TWCR_TWEA	= 0x40, ///<
		TWCR_TWINT	= 0x80, ///< TWI Interrupt Flag
		//@}

		/// @name MCUCR (0x55): MCU Control Register
		//@{
		MCUCR_ISC00	= 0x01, ///< Interrupt Sense Control 1 Bit 0
		MCUCR_ISC01	= 0x02, ///< Interrupt Sense Control 1 Bit 1
		MCUCR_ISC10	= 0x04, ///< Interrupt Sense Control 1 Bit 2
		MCUCR_ISC11	= 0x08, ///< Interrupt Sense Control 1 Bit 3
		MCUCR_SM0	= 0x10, ///< Sleep Mode Select Bit 0
		MCUCR_SM1	= 0x20, ///< Sleep Mode Select Bit 1
		MCUCR_SM2	= 0x40, ///< Sleep Mode Select Bit 2
		MCUCR_SE	= 0x80, ///< Sleep Enable
		//@}

		/// @name MCUCSR (0x54): MCU Control and Status Register
		//@{
		MCUCSR_PORF	= 0x01, ///< Power-on Reset Flag
		MCUCSR_EXTRF	= 0x02, ///< External Reset Flag
		MCUCSR_BORF	= 0x04, ///< Brown-out Reset Flag
		MCUCSR_WDRF	= 0x08, ///< Watchdog Reset Flag
		//MCUCSR_JTRF	= 0x10, ///<
		//MCUCSR_	= 0x20, ///<
		//MCUCSR_ISC2	= 0x40, ///<
		//MCUCSR_JTD	= 0x80, ///<
		//@}

		/// @name TCCR0 (0x53): Timer/Counter Control Register
		//@{
		TCCR0_CS00	= 0x01, ///<  Clock Select bit 0
		TCCR0_CS01	= 0x02, ///<  Clock Select bit 1
		TCCR0_CS02	= 0x04, ///<  Clock Select bit 2
		//TCCR0_	= 0x08, ///<
		//TCCR0_	= 0x10, ///<
		//TCCR0_	= 0x20, ///<
		//TCCR0_	= 0x40, ///<
		//TCCR0_	= 0x80, ///<
		//@}

		/// @name SFIOR (0x50): Special Function IO Register
		//@{
		SFIOR_PSR10	= 0x01, ///< Prescaler Reset Timer/Counter1 and Timer/Counter0
		SFIOR_PSR2	= 0x02, ///<
		SFIOR_PUD	= 0x04, ///< Pull-up Disable
		SFIOR_ACME	= 0x08, ///<
		//SFIOR_	= 0x10, ///<
		SFIOR_ADTS0	= 0x20, ///<
		SFIOR_ADTS1	= 0x40, ///<
		SFIOR_ADTS2	= 0x80, ///<
		//@}

		/// @name TCCR1A (0x4F): Timer/Counter 1 Control Register A
		//@{
		TCCR1A_WGM10	= 0x01, ///<
		TCCR1A_WGM11	= 0x02, ///<
		TCCR1A_FOC1B	= 0x04, ///<
		TCCR1A_FOC1A	= 0x08, ///<
		TCCR1A_COM1B0	= 0x10, ///<
		TCCR1A_COM1B1	= 0x20, ///<
		TCCR1A_COM1A0	= 0x40, ///<
		TCCR1A_COM1A1	= 0x80, ///<
		//@}

		/// @name TCCR1B (0x4E): Timer/Counter 1 Control Register B
		//@{
		TCCR1B_CS10	= 0x01, ///<
		TCCR1B_CS11	= 0x02, ///<
		TCCR1B_CS12	= 0x04, ///<
		TCCR1B_WGM12	= 0x08, ///<
		TCCR1B_WGM13	= 0x10, ///<
		//TCCR1B_	= 0x20, ///<
		TCCR1B_ICES1	= 0x40, ///<
		TCCR1B_ICNC1	= 0x80, ///<
		//@}

		/// @name TCCR2 (0x45): Timer/Counter Control Register
		//@{
		TCCR2_CS20	= 0x01, ///<
		TCCR2_CS21	= 0x02, ///<
		TCCR2_CS22	= 0x04, ///<
		TCCR2_WGM21	= 0x08, ///<
		TCCR2_COM20	= 0x10, ///<
		TCCR2_COM21	= 0x20, ///<
		TCCR2_WGM20	= 0x40, ///<
		TCCR2_FOC2	= 0x80, ///<
		//@}

		/// @name ASSR (0x42): Asynchronous Status Register
		//@{
		ASSR_TCR2UB	= 0x01, ///<
		ASSR_OCR2UB	= 0x02, ///<
		ASSR_TCN2UB	= 0x04, ///<
		ASSR_AS2	= 0x08, ///<
		//ASSR_		= 0x10, ///<
		//ASSR_		= 0x20, ///<
		//ASSR_		= 0x40, ///<
		//ASSR_		= 0x80, ///<
		//@}

		/// @name WDTCR (0x41): Watchdog Timer Control Register
		//@{
		WDTCR_WDP0	= 0x01, ///< Watchdog Timer Prescaler 0
		WDTCR_WDP1	= 0x02, ///< Watchdog Timer Prescaler 1
		WDTCR_WDP2	= 0x04, ///< Watchdog Timer Prescaler 2
		WDTCR_WDE	= 0x08, ///< Watchdog Enable
		WDTCR_WDCE	= 0x10, ///< Watchdog Change Enable
		//WDTCR_	= 0x20, ///<
		//WDTCR_	= 0x40, ///<
		//WDTCR_	= 0x80, ///<
		//@}

		/// @name UCSRC (0x40): USART Control and Status Register C
		//@{
		UCSRC_UCPOL	= 0x01, ///<
		UCSRC_UCSZ0	= 0x02, ///<
		UCSRC_UCSZ1	= 0x04, ///<
		UCSRC_USBS	= 0x08, ///<
		UCSRC_UPM0	= 0x10, ///<
		UCSRC_UPM1	= 0x20, ///<
		UCSRC_UMSEL	= 0x40, ///<
		UCSRC_URSEL	= 0x80, ///<
		//@}

		/// @name XXYY (0x3C): EEPROM Control Register
		//@{
		EECR_EERE	= 0x01, ///< EEPROM Read Enable
		EECR_EEWE	= 0x02, ///< EEPROM Write Enable
		EECR_EEMWE	= 0x04, ///< EEPROM Master Write Enable
		EECR_EERIE	= 0x08, ///< EEPROM Ready Interrupt Enable
		//EECR_	= 0x10, ///<
		//EECR_	= 0x20, ///<
		//EECR_	= 0x40, ///<
		//EECR_	= 0x80, ///<
		//@}

		/// @name PORTA (0x3B): Port A Data Register
		//@{
		PORTA_PORTA0	= 0x01, ///< Bit 0 in port A
		PORTA_PORTA1	= 0x02, ///<  .    .     .
		PORTA_PORTA2	= 0x04, ///<  .    .     .
		PORTA_PORTA3	= 0x08, ///<  .    .     .
		PORTA_PORTA4	= 0x10, ///<  .    .     .
		PORTA_PORTA5	= 0x20, ///<  .    .     .
		PORTA_PORTA6	= 0x40, ///<  .    .     .
		PORTA_PORTA7	= 0x80, ///< Bit 7 in port A
		//@}

		/// @name XXYY (0x3A): Port A Data Direction Register
		//@{
		DDRA_DDA0	= 0x01, ///< Data direction flag for bit 0 in port A
		DDRA_DDA1	= 0x02, ///<          .        .         .
		DDRA_DDA2	= 0x04, ///<          .        .         .
		DDRA_DDA3	= 0x08, ///<          .        .         .
		DDRA_DDA4	= 0x10, ///<          .        .         .
		DDRA_DDA5	= 0x20, ///<          .        .         .
		DDRA_DDA6	= 0x40, ///<          .        .         .
		DDRA_DDA7	= 0x80, ///< Data direction flag for bit 7 in port A
		//@}

		/// @name XXYY (0x39): Port A Input Pins
		//@{
		PINA_PINA0	= 0x01, ///< Input read from bit 0 in port A
		PINA_PINA1	= 0x02, ///<       .        .         .
		PINA_PINA2	= 0x04, ///<       .        .         .
		PINA_PINA3	= 0x08, ///<       .        .         .
		PINA_PINA4	= 0x10, ///<       .        .         .
		PINA_PINA5	= 0x20, ///<       .        .         .
		PINA_PINA6	= 0x40, ///<       .        .         .
		PINA_PINA7	= 0x80, ///< Input read from bit 7 in port A
		//@}

		/// @name PORTB (0x38): Port B Data Register
		//@{
		PORTB_PORTB0	= 0x01, ///< Bit 0 in port B
		PORTB_PORTB1	= 0x02, ///<  .    .     .
		PORTB_PORTB2	= 0x04, ///<  .    .     .
		PORTB_PORTB3	= 0x08, ///<  .    .     .
		PORTB_PORTB4	= 0x10, ///<  .    .     .
		PORTB_PORTB5	= 0x20, ///<  .    .     .
		PORTB_PORTB6	= 0x40, ///<  .    .     .
		PORTB_PORTB7	= 0x80, ///< Bit 7 in port B
		//@}

		/// @name XXYY (0x37): Port B Data Direction Register
		//@{
		DDRB_DDB0	= 0x01, ///< Data direction flag for bit 0 in port B
		DDRB_DDB1	= 0x02, ///<          .        .         .
		DDRB_DDB2	= 0x04, ///<          .        .         .
		DDRB_DDB3	= 0x08, ///<          .        .         .
		DDRB_DDB4	= 0x10, ///<          .        .         .
		DDRB_DDB5	= 0x20, ///<          .        .         .
		DDRB_DDB6	= 0x40, ///<          .        .         .
		DDRB_DDB7	= 0x80, ///< Data direction flag for bit 7 in port B
		//@}

		/// @name PINB (0x36): Port B Input Pins
		//@{
		PINB_PINB0	= 0x01, ///< Input read from bit 0 in port B
		PINB_PINB1	= 0x02, ///<       .        .         .
		PINB_PINB2	= 0x04, ///<       .        .         .
		PINB_PINB3	= 0x08, ///<       .        .         .
		PINB_PINB4	= 0x10, ///<       .        .         .
		PINB_PINB5	= 0x20, ///<       .        .         .
		PINB_PINB6	= 0x40, ///<       .        .         .
		PINB_PINB7	= 0x80, ///< Input read from bit 7 in port B
		//@}

		/// @name PORTC (0x35): Port C Data Register
		//@{
		PORTC_PORTC0	= 0x01, ///< Bit 0 in port C
		PORTC_PORTC1	= 0x02, ///<  .    .     .
		PORTC_PORTC2	= 0x04, ///<  .    .     .
		PORTC_PORTC3	= 0x08, ///<  .    .     .
		PORTC_PORTC4	= 0x10, ///<  .    .     .
		PORTC_PORTC5	= 0x20, ///<  .    .     .
		PORTC_PORTC6	= 0x40, ///<  .    .     .
		PORTC_PORTC7	= 0x80, ///< Bit 7 in port C
		//@}

		/// @name DDRC (0x34): Port C Data Direction Register
		//@{
		DDRC_DDC0	= 0x01, ///< Data direction flag for bit 0 in port C
		DDRC_DDC1	= 0x02, ///<          .        .         .
		DDRC_DDC2	= 0x04, ///<          .        .         .
		DDRC_DDC3	= 0x08, ///<          .        .         .
		DDRC_DDC4	= 0x10, ///<          .        .         .
		DDRC_DDC5	= 0x20, ///<          .        .         .
		DDRC_DDC6	= 0x40, ///<          .        .         .
		DDRC_DDC7	= 0x80, ///< Data direction flag for bit 7 in port C
		//@}

		/// @name PINC (0x33): Port C Input Pins
		//@{
		PINC_PINC0	= 0x01, ///< Input read from bit 0 in port C
		PINC_PINC1	= 0x02, ///<       .        .         .
		PINC_PINC2	= 0x04, ///<       .        .         .
		PINC_PINC3	= 0x08, ///<       .        .         .
		PINC_PINC4	= 0x10, ///<       .        .         .
		PINC_PINC5	= 0x20, ///<       .        .         .
		PINC_PINC6	= 0x40, ///<       .        .         .
		PINC_PINC7	= 0x80, ///< Input read from bit 7 in port C
		//@}

		/// @name PORTD (0x32): Port D Data Register
		//@{
		PORTD_PORTD0	= 0x01, ///< Bit 0 in port D
		PORTD_PORTD1	= 0x02, ///<  .    .     .
		PORTD_PORTD2	= 0x04, ///<  .    .     .
		PORTD_PORTD3	= 0x08, ///<  .    .     .
		PORTD_PORTD4	= 0x10, ///<  .    .     .
		PORTD_PORTD5	= 0x20, ///<  .    .     .
		PORTD_PORTD6	= 0x40, ///<  .    .     .
		PORTD_PORTD7	= 0x80, ///< Bit 7 in port D
		//@}

		/// @name DDRD (0x31): Port D Data Direction Register
		//@{
		DDRD_DDD0	= 0x01, ///< Data direction flag for bit 0 in port D
		DDRD_DDD1	= 0x02, ///<          .        .         .
		DDRD_DDD2	= 0x04, ///<          .        .         .
		DDRD_DDD3	= 0x08, ///<          .        .         .
		DDRD_DDD4	= 0x10, ///<          .        .         .
		DDRD_DDD5	= 0x20, ///<          .        .         .
		DDRD_DDD6	= 0x40, ///<          .        .         .
		DDRD_DDD7	= 0x80, ///< Data direction flag for bit 7 in port D
		//@}

		/// @name PIND (0x30): Port D Input Pins
		//@{
		PIND_PIND0	= 0x01, ///< Input read from bit 0 in port D
		PIND_PIND1	= 0x02, ///<       .        .         .
		PIND_PIND2	= 0x04, ///<       .        .         .
		PIND_PIND3	= 0x08, ///<       .        .         .
		PIND_PIND4	= 0x10, ///<       .        .         .
		PIND_PIND5	= 0x20, ///<       .        .         .
		PIND_PIND6	= 0x40, ///<       .        .         .
		PIND_PIND7	= 0x80, ///< Input read from bit 7 in port D
		//@}

		/// @name SPSR (0x2E): SPI Status Register
		//@{
		SPSR_SPI2X	= 0x01, ///<
		//SPSR_		= 0x02, ///<
		//SPSR_		= 0x04, ///<
		//SPSR_		= 0x08, ///<
		//SPSR_		= 0x10, ///<
		//SPSR_		= 0x20, ///<
		SPSR_WCOL	= 0x40, ///<
		SPSR_SPIF	= 0x80, ///< SPI Interrupt Flag
		//@}

		/// @name SPCR (0x2D): SPI Control Register
		//@{
		SPCR_SPR0	= 0x01, ///<
		SPCR_SPR1	= 0x02, ///<
		SPCR_CPHA	= 0x04, ///<
		SPCR_CPOL	= 0x08, ///<
		SPCR_MSTR	= 0x10, ///<
		SPCR_DORD	= 0x20, ///<
		SPCR_SPE	= 0x40, ///<
		SPCR_SPIE	= 0x80, ///< SPI Interrupt Enable
		//@}

		/// @name UCSRA (0x2B): USART Control and Status Register A
		//@{
		UCSRA_MPCM	= 0x01, ///<
		UCSRA_U2X	= 0x02, ///<
		UCSRA_PE	= 0x04, ///<
		UCSRA_DOR	= 0x08, ///<
		UCSRA_FE	= 0x10, ///<
		UCSRA_UDRE	= 0x20, ///< USART Data Register Empty
		UCSRA_TXC	= 0x40, ///< USART Transmit Complete
		UCSRA_RXC	= 0x80, ///< USART Receive Complete
		//@}

		/// @name UCSRB (0x2A): USART Control and Status Register B
		//@{
		UCSRB_TXB8	= 0x01, ///<
		UCSRB_RXB8	= 0x02, ///<
		UCSRB_UCSZ2	= 0x04, ///<
		UCSRB_TXEN	= 0x08, ///<
		UCSRB_RXEN	= 0x10, ///<
		UCSRB_UDRIE	= 0x20, ///< USART Data Register Empty Interrupt Enable
		UCSRB_TXCIE	= 0x40, ///< TX Complete Interrupt Enable
		UCSRB_RXCIE	= 0x80, ///< RX Complete Interrupt Enable
		//@}


		/// @name ACSR (0x28): Analog Comparator Control and Status Register
		//@{
		ACSR_ACIS0	= 0x01, ///<
		ACSR_ACIS1	= 0x02, ///<
		ACSR_ACIC	= 0x04, ///<
		ACSR_ACIE	= 0x08, ///< Analog Comparator Interrupt Enable
		ACSR_ACI	= 0x10, ///< Analog Comparator Interrupt Flag
		ACSR_ACO	= 0x20, ///<
		ACSR_ACBG	= 0x40, ///<
		ACSR_ACD	= 0x80, ///<
		//@}

		/// @name ADMUX (0x27): ADC Multiplexer Selection Register
		//@{
		ADMUX_MUX0	= 0x01, ///<
		ADMUX_MUX1	= 0x02, ///<
		ADMUX_MUX2	= 0x04, ///<
		ADMUX_MUX3	= 0x08, ///<
		ADMUX_MUX4	= 0x10, ///<
		ADMUX_ADLAR	= 0x20, ///<
		ADMUX_REFS0	= 0x40, ///<
		ADMUX_REFS1	= 0x80, ///<
		//@}

		/// @name ADCSRA (0x26): ADC Control and Status Register A
		//@{
		ADCSRA_ADPS0	= 0x01, ///<
		ADCSRA_ADPS1	= 0x02, ///<
		ADCSRA_ADPS2	= 0x04, ///<
		ADCSRA_ADIE	= 0x08, ///< ADC Interrupt Enable
		ADCSRA_ADIF	= 0x10, ///< ADC Interrupt Flag
		ADCSRA_ADFR	= 0x20, ///<
		ADCSRA_ADSC	= 0x40, ///<
		ADCSRA_ADEN	= 0x80, ///<
		//@}

		/// @name TWAR (0x22): TWI (Slave) Address Register
		//@{
		TWAR_TWGCE	= 0x01, ///<
		TWAR_TWA0	= 0x02, ///<
		TWAR_TWA1	= 0x04, ///<
		TWAR_TWA2	= 0x08, ///<
		TWAR_TWA3	= 0x10, ///<
		TWAR_TWA4	= 0x20, ///<
		TWAR_TWA5	= 0x40, ///<
		TWAR_TWA6	= 0x80  ///<
		//@}
	};
};

#endif // AVR8REGNAMES_H
