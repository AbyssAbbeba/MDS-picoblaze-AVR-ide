// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2012 Moravia Microsystems, s.r.o.
 *
 * @authors Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup AVR8
 * @file AVR8IO.h
 */
// =============================================================================

// #define NDEBUG

#ifndef AVR8IO_H
#define AVR8IO_H

// Forward declarations
class AVR8DataMemory;

#include "../MCUSim.h"
#include "AVR8PinNames.h"

#include <cstdlib>
#include <cassert>

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8IO
 */
class AVR8IO : public MCUSim::IO
{
    ////    Public Static Constants    ////
    public:
        /// @brief
        static const unsigned int NUMBER_OF_PORTS = 4;

        /// @brief
        static const unsigned int NUMBER_OF_PINS = AVR8PinNames::PIN__MAX__;//0 + ( NUMBER_OF_PORTS * 8 ); // TODO: write something better here

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
            SimFloatType m_pullUpresistance; // 20-50k ohms

            ///
            AVR8PinNames::PIN m_specFuncMap [ AVR8PinNames::SPF__MAX__ ];

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
        AVR8IO();

        /**
         * @brief
         */
        ~AVR8IO();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] dataMemory
         * @return
         */
        AVR8IO * link ( MCUSim::EventLogger * eventLogger,
                        AVR8DataMemory * dataMemory );

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
        bool getLog ( AVR8PinNames::SPF pin );

        /**
         * @brief
         * @param[in] pin
         * @return
         */
        bool getLog ( AVR8PinNames::PIN pin );

        /**
         * @brief
         * @param[in] pin
         * @param[in] val
         */
        void setLog ( AVR8PinNames::SPF pin,
                      bool val );

        /**
         * @brief
         * @param[in] pin
         * @param[in] val
         */
        void setLog ( AVR8PinNames::PIN pin,
                      bool val );

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] pin
         * @return
         */
        SimFloatType getVoltage ( AVR8PinNames::SPF pin ) const
        {
            return getVoltage(m_config.m_specFuncMap[pin]);
        }

        /**
         * @brief
         * @param[in] pin
         * @return
         */
        SimFloatType getVoltage ( AVR8PinNames::PIN pin ) const
        {
            assert(AVR8PinNames::PIN_NC != pin);
            return m_lowLevelInterface[II_REAL_VOLTAGE][pin];
        }

        /**
         * @brief
         * @param[in] pin
         * @param[in] volts
         */
        void setVoltage ( AVR8PinNames::SPF pin,
                          SimFloatType volts )
        {
            setVoltage(m_config.m_specFuncMap[pin], volts);
        }

        /**
         * @brief
         * @param[in] pin
         * @param[in] volts
         */
        void setVoltage ( AVR8PinNames::PIN pin,
                          SimFloatType volts )
        {
            assert(AVR8PinNames::PIN_NC != pin);
            m_lowLevelInterface[II_VOLTAGE_INT][pin] = volts;
        }

//         /**
//          * @brief
//          * @param[in] pin
//          * @return
//          */
//         SimFloatType getResistance ( PIN pin ) const
//         {
//             assert(AVR8PinNames::PIN_NC != pin);
//             return m_lowLevelInterface[II_RESISTANCE][pin];
//         }

        /**
         * @brief
         * @param[in] pin
         * @param[in] ohms
         */
        void setResistance ( AVR8PinNames::SPF pin,
                             SimFloatType ohms )
        {
            setResistance(m_config.m_specFuncMap[pin], ohms);
        }

        /**
         * @brief
         * @param[in] pin
         * @param[in] ohms
         */
        void setResistance ( AVR8PinNames::PIN pin, SimFloatType ohms )
        {
            assert(AVR8PinNames::PIN_NC != pin);
            m_lowLevelInterface[II_RESISTANCE][pin] = ohms;
        }

//         /**
//          * @brief
//          * @param[in] pin
//          */
//         void setAsOutput ( PIN pin )
//         {
//             assert(AVR8PinNames::PIN_NC != pin);
//             m_lowLevelInterface[II_RESISTANCE][pin] = 0;
//         }
//
//         /**
//          * @brief
//          * @param[in] pin
//          */
//         void setAsInput ( PIN pin )
//         {
//             assert(AVR8PinNames::PIN_NC != pin);
//             m_lowLevelInterface[II_RESISTANCE][pin] = INFINITY;
//         }

        /**
         * @brief
         * @param[in] pin
         * @param[in] mode
         */
        void setPinMode ( AVR8PinNames::SPF pin,
                          PINMD mode )
        {
            setPinMode(m_config.m_specFuncMap[pin], mode);
        }

        /**
         * @brief
         * @param[in] pin
         * @param[in] mode
         */
        void setPinMode ( AVR8PinNames::PIN pin,
                          PINMD mode )
        {
            assert(AVR8PinNames::PIN_NC != pin);

            // TODO: Certainly this is not complete.

            m_pinMode[pin] = mode;
        }

        /**
         * @brief
         * @param[in] pin
         * @return
         */
        PINMD getPinMode ( AVR8PinNames::SPF pin ) const
        {
            return getPinMode(m_config.m_specFuncMap[pin]);
        }

        /**
         * @brief
         * @param[in] pin
         * @return
         */
        PINMD getPinMode ( AVR8PinNames::PIN pin ) const
        {
            assert(AVR8PinNames::PIN_NC != pin);
            return m_pinMode[pin];
        }

        /**
         * @brief
         * @param[in] voltage
         */
        void setSourceVoltage ( SimFloatType voltage )
        {
            m_sourceVoltage = voltage;
            m_logThreshold0 = m_sourceVoltage / 2;
            m_logThreshold1 = m_logThreshold0;
        }

        /**
         * @brief
         * @return
         */
        SimFloatType getSourceVoltage() const
        {
            return m_sourceVoltage;
        }

        /**
         * @brief
         * @return
         */
        unsigned int getNumberOfPins() const
        {
            return NUMBER_OF_PINS;
        }

        /**
         * @brief
         * @return
         */
        SimFloatType ** getLowLevelInterface()
        {
            return m_lowLevelInterface;
        }

        /**
         * @brief
         * @param[in] pin
         */
        void setEnabled ( bool flag )
        {
            m_enabled = flag;
        }

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] pinIdx
         * @param[in] portIndex
         * @param[in] ddrAddr
         */
        inline void portSetDirection ( unsigned int pinIdx,
                                       int portIndex,
                                       const unsigned int ddrAddr );

        /**
         * @brief
         * @param[in] pinIdx
         * @param[in] inRegAddr
         * @param[in] outRegAddr
         */
        inline void portReadWrite ( unsigned int pinIdx,
                                    const unsigned int inRegAddr,
                                    const unsigned int outRegAddr );

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

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        Config m_config;

    ////    Protected Attributes    ////
    protected:
        /// @name AVR8 simulator subsystems
        //@{
            /// @brief
            AVR8DataMemory * m_dataMemory;
        //@}

        /**
         * @brief
         */
        SimFloatType m_logThreshold0;

        /**
         * @brief
         */
        SimFloatType m_logThreshold1;

        /**
         * @brief
         */
        unsigned int m_lastDdr [ NUMBER_OF_PORTS ];

        /**
         * @brief
         */
        PINMD m_pinMode [ NUMBER_OF_PINS ];

        /**
         * @brief
         */
        bool m_PullUpDisabled;

        /**
         * @brief
         */
        bool m_enabled; // by default this is disabled
};

#endif // AVR8IO_H
