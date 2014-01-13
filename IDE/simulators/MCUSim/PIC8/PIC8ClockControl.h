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
 * @file PIC8ClockControl.h
 */
// =============================================================================

#ifndef PIC8CLOCKCONTROL_H
#define PIC8CLOCKCONTROL_H

// Forward declarations
class PIC8ConfigWord;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8ClockControl
 */
class PIC8ClockControl : public MCUSimClock
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
            EVENT_CLK_WRN_RESISTANCE_TOO_LOW,   ///<
            EVENT_CLK_WRN_RESISTANCE_TOO_HIGH,  ///<
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
            }

            int m_startUpTime;          ///<
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
        PIC8ClockControl() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] fusesAndLocks
         * @return
         */
        PIC8ClockControl * link ( MCUSimEventLogger * eventLogger,
                                  PIC8ConfigWord      * configWord );

        /**
         * @brief
         * @param[in] mode
         */
        virtual void reset ( MCUSimBase::ResetMode mode ) override;

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
        inline unsigned int getOscSel() const;

        /**
         * @brief
         * @return
         */
        inline float freqRC();

        /**
         * @brief
         * @return
         */
        inline float freqHS();

        /**
         * @brief
         * @return
         */
        inline float freqXT();

        /**
         * @brief
         * @return
         */
        inline float freqLP();

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
        /// @name PIC8 simulator subsystems
        //@{
            /// @brief
            PIC8ConfigWord * m_configWord;
        //@}
};

#endif // PIC8CLOCKCONTROL_H
