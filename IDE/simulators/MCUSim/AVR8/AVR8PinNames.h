// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup AVR8
 * @file AVR8PinNames.h
 */
// =============================================================================

#ifndef AVR8PINNAMES_H
#define AVR8PINNAMES_H

// NOTE: written for ATmega8

/**
 * @brief
 * @ingroup AVR8
 * @namespace AVR8PinNames
 */
namespace AVR8PinNames
{
    ////    Datatypes    ////

    /// @brief Real port pins
    enum PIN
    {
        PIN_NC = -1,    ///< Not connected

        PIN_PA0,        ///< Bit 0 in port A
        PIN_PA1,        ///<   .    .    .
        PIN_PA2,        ///<   .    .    .
        PIN_PA3,        ///<   .    .    .
        PIN_PA4,        ///<   .    .    .
        PIN_PA5,        ///<   .    .    .
        PIN_PA6,        ///<   .    .    .
        PIN_PA7,        ///< Bit 7 in port A

        PIN_PB0,        ///< Bit 0 in port A
        PIN_PB1,        ///<   .    .    .
        PIN_PB2,        ///<   .    .    .
        PIN_PB3,        ///<   .    .    .
        PIN_PB4,        ///<   .    .    .
        PIN_PB5,        ///<   .    .    .
        PIN_PB6,        ///<   .    .    .
        PIN_PB7,        ///< Bit 7 in port A

        PIN_PC0,        ///< Bit 0 in port A
        PIN_PC1,        ///<   .    .    .
        PIN_PC2,        ///<   .    .    .
        PIN_PC3,        ///<   .    .    .
        PIN_PC4,        ///<   .    .    .
        PIN_PC5,        ///<   .    .    .
        PIN_PC6,        ///<   .    .    .
        PIN_PC7,        ///< Bit 7 in port A

        PIN_PD0,        ///< Bit 0 in port A
        PIN_PD1,        ///<   .    .    .
        PIN_PD2,        ///<   .    .    .
        PIN_PD3,        ///<   .    .    .
        PIN_PD4,        ///<   .    .    .
        PIN_PD5,        ///<   .    .    .
        PIN_PD6,        ///<   .    .    .
        PIN_PD7,        ///< Bit 7 in port A

        PIN_VCC,        ///< TODO:
        PIN_GND,        ///< TODO:
        PIN_AREF,       ///< TODO:
        PIN_AVCC,       ///< TODO:

        PIN__MAX__      ///< Number of pins which the simulated device might possibly have
    };

    /// @brief Special Pin Functions
    enum SPF
    {
        SPF_T0,         ///<
        SPF_INT0,       ///<
        SPF_INT1,       ///<

        SPF_RESET,      ///<
        SPF_RXD,        ///<
        SPF_TXD,        ///<
        SPF_XCK,        ///<
        SPF_XTAL1,      ///<
        SPF_TOSC1,      ///<
        SPF_XTAL2,      ///<
        SPF_TOSC2,      ///<
        SPF_T1,         ///<
        SPF_AIN0,       ///<
        SPF_AIN1,       ///<
        SPF_ICP1,       ///<
        SPF_ADC5,       ///<
        SPF_SCL,        ///<
        SPF_ADC4,       ///<
        SPF_SDA,        ///<
        SPF_ADC3,       ///<
        SPF_ADC2,       ///<
        SPF_ADC1,       ///<
        SPF_ADC0,       ///<
        SPF_SCK,        ///<
        SPF_MISO,       ///<
        SPF_MOSI,       ///<
        SPF_OC2,        ///<
        SPF_SS,         ///<
        SPF_OC1B,       ///<
        SPF_OC1A,       ///<

        SPF__MAX__      ///<
    };
};

#endif // AVR8PINNAMES_H
