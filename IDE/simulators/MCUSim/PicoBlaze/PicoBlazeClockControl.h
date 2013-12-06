// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>, (C) 2013
 * @ingroup PicoBlaze
 * @file PicoBlazeClockControl.h
 */
// =============================================================================

#ifndef PICOBLAZECLOCKCONTROL_H
#define PICOBLAZECLOCKCONTROL_H

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup PicoBlaze
 * @class PicoBlazeClockControl
 */
class PicoBlazeClockControl : public MCUSimClock
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Event
        {
            EVENT_CLK_ERR_INCOMPATIBLE_SOURCE,  ///<
            EVENT_CLK__MAX__                    ///<
        };

        /**
         * @brief
         */
        struct Config
        {
        };

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        PicoBlazeClockControl * link ( MCUSimEventLogger * eventLogger );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSimBase::ResetMode mode );

        /**
         * @brief
         * @return
         */
        float getFrequency();

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
};

#endif // PICOBLAZECLOCKCONTROL_H
