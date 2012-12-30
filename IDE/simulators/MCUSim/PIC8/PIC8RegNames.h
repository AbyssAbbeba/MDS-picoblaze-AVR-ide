// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2012 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 * @ingroup PIC8
 * @file PIC8RegNames.h
 */
// =============================================================================

#ifndef PIC8REGNAMES_H
#define PIC8REGNAMES_H

/**
 * @brief
 * @ingroup PIC8
 * @namespace PIC8RegNames
 */
namespace PIC8RegNames
{
    ////    Datatypes    ////

    /**
     * @brief
     */
    enum SFR
    {
        /// @name Bank 0
        //@{
            INDF         = 0,     ///<
            TMR0         = -0x01, ///<
            PCL          = -0x02, ///<
            STATUS       = -0x03, ///<
            FSR          = -0x04, ///<
            PORTA        = -0x05, ///<
            PORTB        = -0x06, ///<
            // ---       = -0x07, ///<
            EEDATA       = -0x08, ///<
            EEADR        = -0x09, ///<
            PCLATH       = -0x0A, ///<
            INTCON       = -0x0B, ///< Interrupt control register
        //@}

        /// @name Bank 1
        //@{
            // (INDF)    = -0x80, ///<
            OPTION_REG   = -0x81, ///<
            // (PCL)     = -0x82, ///<
            // (STATUS)  = -0x83, ///<
            // (FSR)     = -0x84, ///<
            TRISA        = -0x85, ///< PORTA Data Direction Register
            TRISB        = -0x86, ///< PORTB Data Direction Register
            // ---       = -0x87, ///<
            EECON1       = -0x88, ///<
            EECON2       = -0x89, ///<
            // (PCLATH)  = -0x0A, ///<
            // (INTCON)  = -0x0B, ///<
        //@}

        // Added only for compatibility with more advanced device
        PORTC,
        PORTD,
        TRISC,
        TRISD,

        SFR__MAX__                ///<
    };

    /**
     * @brief
     */
    enum SFB
    {
        /// @name Bank 0
        //@{
            /// @name Status register
            //@{
                STATUS_IRP         = 0x80, ///< TODO: write description here
                STATUS_RP1         = 0x40, ///< RP1: Register Bank Select bits (used for direct addressing)
                STATUS_RP0         = 0x20, ///< RP0: Register Bank Select bits (used for direct addressing)
                STATUS_TO          = 0x10, ///< Time-out bit
                STATUS_PD          = 0x08, ///< Power-down bit
                STATUS_Z           = 0x04, ///< Zero bit
                STATUS_DC          = 0x02, ///< Digit carry/borrow bit
                STATUS_C           = 0x01, ///< Carry/borrow bit
            //@}

            /// @name Port A register
            //@{
                PORTA_RA4          = 0x10, ///<
                PORTA_T0CKI        = 0x10, ///<
                PORTA_RA3          = 0x08, ///<
                PORTA_RA2          = 0x04, ///<
                PORTA_RA1          = 0x02, ///<
                PORTA_RA0          = 0x01, ///<
            //@}

            /// @name Port B register
            //@{
                PORTB_RB7          = 0x80, ///<
                PORTB_RB6          = 0x40, ///<
                PORTB_RB5          = 0x20, ///<
                PORTB_RB4          = 0x10, ///<
                PORTB_RB3          = 0x08, ///<
                PORTB_RB2          = 0x04, ///<
                PORTB_RB1          = 0x02, ///<
                PORTB_RB0          = 0x01, ///<
                PORTB_INT          = 0x01, ///<
            //@}

            /// @name Interrupt control register
            //@{
                INTCON_GIE         = 0x80, ///< Global Interrupt Enable bit
                INTCON_EEIE        = 0x40, ///< EE Write Complete Interrupt Enable bit
                INTCON_T0IE        = 0x20, ///< TMR0 Overflow Interrupt Enable bit
                INTCON_INTE        = 0x10, ///< RB0/INT External Interrupt Enable bit
                INTCON_RBIE        = 0x08, ///< RB Port Change Interrupt Enable bit
                INTCON_T0IF        = 0x04, ///< TMR0 Overflow Interrupt Flag bit
                INTCON_INTF        = 0x02, ///< RB0/INT External Interrupt Flag bit
                INTCON_RBIF        = 0x01, ///< RB Port Change Interrupt Flag bit
            //@}
        //@}

        /// @name Bank 1
        //@{
            /// @name
            //@{
                OPTION_REG_RBPU    = 0x80, ///<
                OPTION_REG_INTEDG  = 0x40, ///<
                OPTION_REG_T0CS    = 0x20, ///<
                OPTION_REG_T0SE    = 0x10, ///<
                OPTION_REG_PSA     = 0x08, ///<
                OPTION_REG_PS2     = 0x04, ///<
                OPTION_REG_PS1     = 0x02, ///<
                OPTION_REG_PS0     = 0x01, ///<
            //@}

            /// @name Data eeprom memory control register
            //@{
                EECON1_EEIF        = 0x10, ///< EEPROM Write Operation Interrupt Flag bit
                EECON1_WRERR       = 0x08, ///< EEPROM Error Flag bit
                EECON1_WREN        = 0x04, ///< EEPROM Write Enable bit
                EECON1_WR          = 0x02, ///< Write Control bit
                EECON1_RD          = 0x01, ///< Read Control bit
            //@}
        //@}

        SFB__MAX__                         ///<
    };
};

#endif // PIC8REGNAMES_H
