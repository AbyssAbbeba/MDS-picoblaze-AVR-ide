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
 * @ingroup AdaptableSim
 * @file AdaptableSimClockControl.h
 */
// =============================================================================

#ifndef AdaptableSimCLOCKCONTROL_H
#define AdaptableSimCLOCKCONTROL_H

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup AdaptableSim
 * @class AdaptableSimClockControl
 */
class AdaptableSimClockControl : public MCUSimClock
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
        AdaptableSimClockControl * link ( MCUSimEventLogger * eventLogger );

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

#endif // AdaptableSimCLOCKCONTROL_H
