// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup PIC8
 * @file PIC8PinNames.h
 */
// =============================================================================

#ifndef PIC8PINNAMES_H
#define PIC8PINNAMES_H

/**
 * @brief
 * @ingroup PIC8
 * @namespace PIC8PinNames
 */
namespace PIC8PinNames
{
    ////    Datatypes    ////

    /**
     * @brief Real port pins
     */
    enum PIN
    {
        PIN_NC = -1,    ///< Not connected

        PIN_RA0,        ///<
        PIN_RA1,        ///<
        PIN_RA2,        ///<
        PIN_RA3,        ///<
        PIN_RA4,        ///<
        PIN_RA5,        ///<
        PIN_RA6,        ///<
        PIN_RA7,        ///<

        PIN_RB0,        ///<
        PIN_RB1,        ///<
        PIN_RB2,        ///<
        PIN_RB3,        ///<
        PIN_RB4,        ///<
        PIN_RB5,        ///<
        PIN_RB6,        ///<
        PIN_RB7,        ///<

        PIN_RC0,        ///<
        PIN_RC1,        ///<
        PIN_RC2,        ///<
        PIN_RC3,        ///<
        PIN_RC4,        ///<
        PIN_RC5,        ///<
        PIN_RC6,        ///<
        PIN_RC7,        ///<

        PIN_RD0,        ///<
        PIN_RD1,        ///<
        PIN_RD2,        ///<
        PIN_RD3,        ///<
        PIN_RD4,        ///<
        PIN_RD5,        ///<
        PIN_RD6,        ///<
        PIN_RD7,        ///<

        PIN_MCLR,       ///<
        PIN_OSC1,       ///<
        PIN_OSC2,       ///<
        PIN_VSS,        ///<
        PIN_VDD,        ///<

        PIN__MAX__      ///<
    };

    /**
     * @brief Special Pin Functions
     */
    enum SPF
    {
        SPF_CLKIN,      ///<
        SPF_CLKOUT,     ///<
        SPF_T0CKI,      ///<
        SPF_INT,        ///<
        SPF_MCLR,       ///<

        SPF_CLOCK,      ///<
        SPF_DATA,       ///<

        SPF__MAX__      ///<
    };
};

#endif // PIC8PINNAMES_H
