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
 * @ingroup PIC8
 * @file PIC8WatchDogTimer.h
 */
// =============================================================================

#ifndef PIC8WATCHDOGTIMER_H
#define PIC8WATCHDOGTIMER_H

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8WatchDogTimer
 */
class PIC8WatchDogTimer : public MCUSim::Subsys
{
    ////    Public Datatypes    ////
    public:
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
        PIC8WatchDogTimer();

        /**
         * @brief
         */
        ~PIC8WatchDogTimer();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @return
         */
        PIC8WatchDogTimer * link ( MCUSim::EventLogger * eventLogger );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSim::ResetMode mode );

        /**
         * @brief
         */
        void watchDogReset();

        /**
         * @brief
         */
        void watchDogPrescalerReset();

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        Config m_config;
};

#endif // PIC8WATCHDOGTIMER_H
