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
 * @ingroup PIC8
 * @file PIC8IO.h
 */
// =============================================================================

// #define NDEBUG

#ifndef PIC8IO_H
#define PIC8IO_H

// Forward declarations
class PIC8DataMemory;

#include "../MCUSim.h"
#include "PIC8PinNames.h"

#include <cstdlib>
#include <cassert>
#include <cmath>

#ifndef NAN
  #error 'NAN' macro is not defined
#endif

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8IO
 */
class PIC8IO : public MCUSim::IO
{
    ////    Public Static Constants    ////
    public:
        /// @brief
        static const unsigned int NUMBER_OF_PORTS = 4; // RA, RB, RC, RD

        /// @brief
        static const unsigned int NUMBER_OF_PINS = PIC8PinNames::PIN__MAX__; // TODO: write something better here

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum PINMD
        {
            PINMD_GENERAL_IO        = 0x0, ///<
            PINMD_INPUT_OVERRIDE    = 0x1, ///<
            PINMD_OUTPUT_OVERRIDE   = 0x2, ///<
            PINMD_IO_OVERRIDE       = 0x3, ///< ( PINMD_INPUT_OVERRIDE | PINMD_OUTPUT_OVERRIDE )
            PINMD_NOT_IMPLEMENTED   = 0x4  ///<
        };

        /**
         * @brief
         */
        enum RandValMode
        {
            RVM_RANDOM, ///<
            RVM_HIGH,   ///<
            RVM_LOW     ///<
        };

        /**
         * @brief
         */
        struct Config
        {
            /// @brief
            Config()
            {
                m_random = RVM_RANDOM;
            }

            ///
            RandValMode m_random;

            ///
            PIC8PinNames::PIN m_specFuncMap [ PIC8PinNames::SPF__MAX__ ];

            ///
            bool m_hasPort [ NUMBER_OF_PORTS ];

            ///
            bool m_availablePins [ NUMBER_OF_PINS ];
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        PIC8IO();

        /**
         * @brief
         */
        ~PIC8IO();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] dataMemory
         * @return
         */
        PIC8IO * link ( MCUSim::EventLogger * eventLogger,
                        PIC8DataMemory * dataMemory );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSim::ResetMode mode );

        /**
         * @brief
         */
        void clockCycles();

        /**
         * @brief
         * @param[in] pin
         * @return
         */
        bool getLog ( PIC8PinNames::SPF pin );

        /**
         * @brief
         * @param[in] pin
         * @return
         */
        bool getLog ( PIC8PinNames::PIN pin );

        /**
         * @brief
         * @param[in] pin
         * @param[in] val
         */
        void setLog ( PIC8PinNames::SPF pin,
                      bool val );

        /**
         * @brief
         * @param[in] pin
         * @param[in] val
         */
        void setLog ( PIC8PinNames::PIN pin,
                      bool val );

    ////    Inline Public Operations    ////
    public:
        SimFloatType getVoltage ( PIC8PinNames::SPF pin ) const
        {
            return getVoltage(m_config.m_specFuncMap[pin]);
        }

        SimFloatType getVoltage ( PIC8PinNames::PIN pin ) const
        {
            assert(PIC8PinNames::PIN_NC != pin);
            return m_lowLevelInterface[II_REAL_VOLTAGE][pin];
        }

        void setVoltage ( PIC8PinNames::SPF pin,
                          SimFloatType volts )
        {
            setVoltage(m_config.m_specFuncMap[pin], volts);
        }

        void setVoltage ( PIC8PinNames::PIN pin,
                          SimFloatType volts )
        {
            assert(PIC8PinNames::PIN_NC != pin);
            m_lowLevelInterface[II_VOLTAGE_INT][pin] = volts;
        }

        SimFloatType getResistance ( PIC8PinNames::PIN pin ) const
        {
            assert(PIC8PinNames::PIN_NC != pin);
            return m_lowLevelInterface[II_RESISTANCE][pin];
        }

        void setResistance ( PIC8PinNames::SPF pin,
                             SimFloatType ohms )
        {
            setResistance(m_config.m_specFuncMap[pin], ohms);
        }

        void setResistance ( PIC8PinNames::PIN pin,
                             SimFloatType ohms )
        {
            assert(PIC8PinNames::PIN_NC != pin);
            m_lowLevelInterface[II_RESISTANCE][pin] = ohms;
        }

        void setAsOutput ( PIC8PinNames::PIN pin )
        {
            assert(PIC8PinNames::PIN_NC != pin);
            m_lowLevelInterface[II_RESISTANCE][pin] = 0;
        }

        void setAsInput ( PIC8PinNames::PIN pin )
        {
            assert(PIC8PinNames::PIN_NC != pin);
            m_lowLevelInterface[II_RESISTANCE][pin] = INFINITY;
        }

        void setPinMode ( PIC8PinNames::SPF pin,
                          PINMD mode )
        {
            setPinMode(m_config.m_specFuncMap[pin], mode);
        }

        void setPinMode ( PIC8PinNames::PIN pin,
                          PINMD mode )
        {
            assert(PIC8PinNames::PIN_NC != pin);

            // TODO: Certainly this is not complete.
            m_pinMode[pin] = mode;
        }

        PINMD getPinMode ( PIC8PinNames::SPF pin ) const
        {
            return getPinMode(m_config.m_specFuncMap[pin]);
        }

        PINMD getPinMode ( PIC8PinNames::PIN pin ) const
        {
            assert(PIC8PinNames::PIN_NC != pin);
            return m_pinMode[pin];
        }

        void setSourceVoltage ( SimFloatType voltage )
        {
            m_sourceVoltage = voltage;
            m_logThreshold0 = m_sourceVoltage / 2;
            m_logThreshold1 = m_logThreshold0;
        }

        SimFloatType getSourceVoltage() const
        {
                return m_sourceVoltage;
        }

        unsigned int getNumberOfPins() const
        {
                return NUMBER_OF_PINS;
        }

        SimFloatType ** getLowLevelInterface()
        {
                return m_lowLevelInterface;
        }

        void setEnabled ( bool flag )
        {
            m_enabled = flag;
        }

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         */
        inline void loadConfig();

        /**
         * @brief
         */
        inline void resetToInitialValues();

        /**
         * @brief
         */
        inline void mcuReset();

        /**
         * @brief
         * @param[in] pinIdx
         * @param[in] trisRegAddr
         */
        inline void portSetDirection ( unsigned int pinIdx,
                                       unsigned int trisRegAddr );

        /**
         * @brief
         * @param[in] pinIdx
         * @param[in] trisRegAddr
         * @param[in] portRegAddr
         */
        inline void portReadWrite ( unsigned int pinIdx,
                                    unsigned int trisRegAddr,
                                    unsigned int portRegAddr );

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        Config m_config;

    ////    Protected Attributes    ////
    protected:
        /// @name PIC8 simulator subsystems
        //@{
            PIC8DataMemory * m_dataMemory;
        //@}

        SimFloatType m_logThreshold0;
        SimFloatType m_logThreshold1;

        // idx =  |n       ... |23  16|15   8|7    0|
        //        |special ... |PORT_C|PORT_B|PORT_A|
        PINMD m_pinMode [ NUMBER_OF_PINS ];
        bool m_enabled; // by default this is disabled
};

#endif // PIC8IO_H
