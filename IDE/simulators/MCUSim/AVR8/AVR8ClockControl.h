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
 * @file AVR8ClockControl.h
 */
// =============================================================================

#ifndef AVR8CLOCKCONTROL_H
#define AVR8CLOCKCONTROL_H

// Forward declarations
class AVR8FusesAndLocks;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8ClockControl
 */
class AVR8ClockControl : public MCUSim::Clock
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Event
        {
            EVENT_CLK_WRN_SOURCE_NOT_USED,      ///<
            EVENT_CLK_WRN_CAPACITY_TOO_LOW,     ///<
            EVENT_CLK_WRN_ATYPICAL_FREQ,        ///<
            EVENT_CLK_WRN_ATYPICAL_CAP,         ///<

            EVENT_CLK_ERR_SOURCE_USELESS,       ///<
            EVENT_CLK_ERR_INCOMPATIBLE_SOURCE,  ///<
            EVENT_CLK_ERR_OUT_OF_FREQ_RANGE,    ///<

            EVENT_CLK__MAX__                    ///<
        };

        /**
         * @brief
         */
        struct StartUpTime
        {
            ///
            StartUpTime()
            {
                m_startUpTime = -1;
                m_addtionalDelay = -1;
            }

            int m_startUpTime;          ///<
            float m_addtionalDelay;     ///<
        };

        /**
         * @brief
         */
        struct Config
        {
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        AVR8ClockControl() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] fusesAndLocks
         * @return
         */
        AVR8ClockControl * link ( MCUSim::EventLogger * eventLogger,
                                  AVR8FusesAndLocks   * fusesAndLocks );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSim::ResetMode mode );

        /**
         * @brief
         * @return
         */
        float getFrequency();

        /**
         * @brief
         * @return
         */
        StartUpTime startUpTime();

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @return
         */
        inline unsigned int getClksel() const;

        /**
         * @brief
         * @return
         */
        inline unsigned int getSut() const;

        /**
         * @brief
         * @param[in] clksel
         * @return
         */
        inline float freqHighFreqCrystal ( const unsigned int clksel );

        /**
         * @brief
         * @return
         */
        inline float freqLowFreqCrystal();

        /**
         * @brief
         * @param[in] clksel
         * @return
         */
        inline float freqExternalRC ( const unsigned int clksel );

        /**
         * @brief
         * @param[in] clksel
         * @return
         */
        inline float freqInternalRC ( const unsigned int clksel );

        /**
         * @brief
         * @return
         */
        inline float freqExternalSource();

        /**
         * @brief
         * @param[in] sut
         * @return
         */
        inline StartUpTime startUpExternalSource ( const unsigned int sut );

        /**
         * @brief
         * @param[in] sut
         * @return
         */
        inline StartUpTime startUpInternalRC ( const unsigned int sut );

        /**
         * @brief
         * @param[in] sut
         * @return
         */
        inline StartUpTime startUpExternalRC ( const unsigned int sut );

        /**
         * @brief
         * @param[in] sut
         * @return
         */
        inline StartUpTime startUpLowFreqCrystal ( const unsigned int sut );

        /**
         * @brief
         * @param[in] clksel
         * @param[in] sut
         * @return
         */
        inline StartUpTime startUpHighFreqCrystal ( const unsigned int clksel,
                                                    const unsigned int sut );

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        Config m_config;

        /**
         * @brief
         */
        ClockSource m_clockSource;

    ////    Protected Attributes    ////
    protected:
        /// @name AVR8 simulator subsystems
        //@{
            /// @brief
            AVR8FusesAndLocks * m_fusesAndLocks;
        //@}
};

#endif // AVR8CLOCKCONTROL_H
