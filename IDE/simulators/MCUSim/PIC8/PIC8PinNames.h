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
 * @file PIC8PinNames.h
 */

#ifdef PIC8PINNAMES_H
#define PIC8PINNAMES_H

namespace PIC8PinNames {
    /// @brief Real port pins
    enum PIN {
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

        PIN_MCLR,       ///<
        PIN_OSC1,       ///<
        PIN_OSC2,       ///<
        PIN_VSS,        ///<
        PIN_VDD,        ///<

        PIN__MAX__
    }

    /// @brief Special Pin Functions
    enum SPF {
        SPF_CLKIN,      ///<
        SPF_CLKOUT,     ///<
        SPF_T0CKI,      ///<
        SPF_INT,        ///<

        SPF__MAX__
    }
};

#endif // PIC8PINNAMES_H
