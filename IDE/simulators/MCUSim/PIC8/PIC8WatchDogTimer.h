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
 * @ingroup PIC8
 * @file PIC8WatchDogTimer.h
 */
// =============================================================================

#ifndef PIC8WATCHDOGTIMER_H
#define PIC8WATCHDOGTIMER_H

class PIC8Timer0WdtPrescaller;
class PIC8DataMemory;
class PIC8ConfigWord;
class PIC8InterruptController;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8WatchDogTimer
 */
class PIC8WatchDogTimer : public MCUSim::Subsys
{
    ////    Public Static Constants    ////
    public:
        /// @brief
        constexpr static float NOMINAL_TIME_OUT = 18e3;

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Event
        {
            EVENT_WDT_RESET,             ///<

            EVENT_WDT__MAX__             ///<
        };

        /**
         * @brief
         */
        struct Config
        {
            ///
            bool m_enabled;
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
        PIC8WatchDogTimer * link ( MCUSim::EventLogger     * eventLogger,
                                   PIC8Timer0WdtPrescaller * timer0WdtPrescaller,
                                   PIC8DataMemory          * dataMemory,
                                   PIC8ConfigWord          * configWord,
                                   PIC8InterruptController * interruptCtrl );

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

        /**
         * @brief
         * @param[in] timeStep
         * @param[in] clockCycles
         */
        void timeStep ( float timeStep,
                        unsigned int clockCycles );

    ////    Inline Private Operations    ////
    private:
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

    ////    Private Attributes    ////
    private:
        /// @name PIC8 simulator subsystems
        //@{
            ///
            PIC8Timer0WdtPrescaller * m_timer0WdtPrescaller;

            ///
            PIC8DataMemory * m_dataMemory;

            ///
            PIC8ConfigWord * m_configWord;

            ///
            PIC8InterruptController * m_interruptController;
        //@}

        ///
        float m_time;
};

#endif // PIC8WATCHDOGTIMER_H
