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
 * @ingroup MCUSim
 * @file MCUSimIO.h
 */
// =============================================================================

#ifndef MCUSIMIO_H
#define MCUSIMIO_H

#include "MCUSimSubsys.h"

/*
 * NOTE:
 * Voltages and currents can be anything except for NaN, since NaN means that state of the pin is a "mystery",
 * Values of resistance:
 *   - equal to +INFINITY  : purely input pin
 *   - lower than 0        : the pin has a very special function, and therefore must be excluded from nodal analysis
 *   - equal to -INFINITY  : (reserved for future use)
 *   - not a number (NaN)  : pin is not implemented!
 *   - otherwise           : normal pin
 */

/*
 * NOTE:
 * math.h defines also these macros:
 *   - INFINITY
 *   - NAN
 *
 * And don't check whether NAN macro has been defined (it's a GNU extension)!
 */

/**
 * @brief
 * @ingroup MCUSim
 * @class MCUSimIO
 */
class MCUSimIO : public MCUSimSubsys
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        #ifdef SIM_IO_QUADRUPLE_PRECISION
            typedef long double SimFloatType;
        #elseifdef SIM_IO_DOUBLE_PRECISION
            typedef double SimFloatType;
        #else
            typedef float SimFloatType;
        #endif

        /**
         * @brief
         */
        enum Event
        {
            EVENT_IO_INDETERMINABLE_LOG, ///<
            EVENT_IO__MAX__              ///<
        };

        /**
         * @brief
         */
        enum InterfaceIndex
        {
            II_VOLTAGE_INT  = 0, ///<
            II_REAL_VOLTAGE = 1, ///<
            II_CURRENT      = 2, ///<
            II_RESISTANCE   = 3, ///<
            II__MAX__            ///<
        };

    ////    Constructors and Destructors    ////
    protected:
        /**
         * @brief
         */
        MCUSimIO() {};

        /**
         * @brief
         * @param[in,out] eventLogger
         */
        MCUSimIO ( MCUSimEventLogger * eventLogger ) : MCUSimSubsys(eventLogger, ID_IO) {}

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        virtual unsigned int getNumberOfPins() const = 0;

        /**
         * @brief
         * @param[in] voltage
         */
        virtual void setSourceVoltage ( SimFloatType voltage ) = 0;

        /**
         * @brief
         * @return
         */
        virtual SimFloatType getSourceVoltage() const = 0;

        /**
         * @brief
         * @return
         */
        virtual SimFloatType ** getLowLevelInterface() = 0;

        /**
         * @brief
         * @param[in,out] out
         * @param[in] eventId
         * @param[in] locationOrReason
         * @param[in] detail
         */
        virtual void eventToString ( std::ostream & out,
                                     int eventId,
                                     int locationOrReason,
                                     int detail ) override;

    ////    Inline Protected Operations    ////
    protected:
        /**
         * @brief
         * @param[in,out] eventLogger
         */
        void link ( MCUSimEventLogger * eventLogger )
        {
            MCUSimSubsys::link(eventLogger, ID_IO);
        }

    ////    Protected Attributes    ////
    protected:
        /**
         * @brief
         */
        SimFloatType m_sourceVoltage;

        // idx =  |n       ... |23  16|15   8|7    0|
        //        |special ... |PORT_C|PORT_B|PORT_A|
        /**
         * @brief
         */
        SimFloatType ** m_lowLevelInterface;
};

#endif // MCUSIMIO_H
