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
 * @file MCUSimClock.h
 */
// =============================================================================

#ifndef MCUSIMCLOCK_H
#define MCUSIMCLOCK_H

#include "MCUSimSubsys.h"

/**
 * @brief
 * @ingroup MCUSim
 * @class MCUSimClock
 */
class MCUSimClock : public MCUSimSubsys
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        class ClockSource
        {
            ////    Public Datatypes    ////
            public:
                /**
                 * @brief
                 */
                enum Type
                {
                    TYPE_NONE,              ///<
                    TYPE_RC,                ///<
                    TYPE_CERAMIC_RES,       ///<
                    TYPE_LOW_FREQ_CRYSTAL,  ///<
                    TYPE_CRYSTAL,           ///<
                    TYPE_EXTERNAL           ///<
                };

            ////    Constructors and Destructors    ////
            public:
                /**
                 * @brief
                 */
                ClockSource() : m_type ( TYPE_NONE ) {};

            ////    Inline Public Operations    ////
            public:
                /**
                 * @brief
                 * @return
                 */
                Type getType() const
                {
                    return m_type;
                }

                /**
                 * @brief
                 * @return
                 */
                float getResistance() const
                {
                    return m_resistance;
                }

                /**
                 * @brief
                 * @return
                 */
                float getCapacity() const
                {
                    return m_capacity;
                }

                /**
                 * @brief
                 * @return
                 */
                float getFrequency() const
                {
                    return m_frequency;
                }

            ////    Protected Attributes    ////
            protected:
                /**
                 * @brief
                 */
                Type m_type;

                /**
                 * @brief
                 */
                float m_resistance;

                /**
                 * @brief
                 */
                float m_capacity;

                /**
                 * @brief
                 */
                float m_frequency;
        };

        /**
         * @brief
         */
        class ClockSourceSpec : public ClockSource
        {
            ////    Constructors and Destructors    ////
            private:
                /**
                 * @brief
                 */
                ClockSourceSpec();

            ////    Inline Public Operations    ////
            public:
                /**
                 * @brief
                 */
                void setInternalClockSource()
                {
                    m_type = TYPE_NONE;
                }

                /**
                 * @brief
                 * @param[in] resistance
                 * @param[in] capacity
                 */
                void setRcOscilator ( float resistance,
                                      float capacity )
                {
                    m_type = TYPE_RC;
                    m_resistance = resistance;
                    m_capacity = capacity;
                }

                /**
                 * @brief
                 * @param[in] resFrequency
                 * @param[in] condCapacity
                 */
                void setCeramicResonator ( float resFrequency,
                                           float condCapacity = 0 )
                {
                    m_type = TYPE_RC;
                    m_frequency = resFrequency;
                    m_capacity = condCapacity;
                }

                /**
                 * @brief
                 * @param[in] crystFrequency
                 * @param[in] condCapacity
                 */
                void setLowFreqCrystal ( float crystFrequency,
                                         float condCapacity = 0 )
                {
                    m_type = TYPE_LOW_FREQ_CRYSTAL;
                    m_frequency = crystFrequency;
                    m_capacity = condCapacity;
                }

                /**
                 * @brief
                 * @param[in] crystFrequency
                 * @param[in] condCapacity
                 */
                void setCrystal ( float crystFrequency,
                                  float condCapacity = 0 )
                {
                    m_type = TYPE_CRYSTAL;
                    m_frequency = crystFrequency;
                    m_capacity = condCapacity;
                }

                /**
                 * @brief
                 * @param[in] frequency
                 */
                void setExternalClockSource ( float frequency )
                {
                    m_type = TYPE_EXTERNAL;
                    m_frequency = frequency;
                }
        };

    ////    Constructors and Destructors    ////
    protected:
        /**
         * @brief
         */
        MCUSimClock() {};

        /**
         * @brief
         * @param[in,out] eventLogger
         */
        MCUSimClock ( MCUSimEventLogger * eventLogger ) : MCUSimSubsys(eventLogger, ID_CLK_CONTROL) {};

    ////    Inline Protected Operations    ////
    protected:
        /**
         * @brief
         * @param[in,out] eventLogger
         */
        void link ( MCUSimEventLogger * eventLogger )
        {
            MCUSimSubsys::link(eventLogger, ID_CLK_CONTROL);
        }
};

#endif // MCUSIMCLOCK_H
