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
 * @ingroup AVR8
 * @file AVR8WatchdogTimer.h
 */
// =============================================================================

#ifndef AVR8WATCHDOGTIMER_H
#define AVR8WATCHDOGTIMER_H

// Forward declarations
class AVR8DataMemory;
class AVR8InterruptController;
class AVR8FusesAndLocks;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8WatchdogTimer
 */
class AVR8WatchdogTimer : public MCUSimSubsys
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Event
        {
            EVENT_WDT_INVALID_CR_CHAGE,  ///<
            EVENT_WDT_PRESCALER_CHANGED, ///<
            EVENT_WDT_RESET,             ///<

            EVENT_WDT__MAX__             ///<
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
        AVR8WatchdogTimer() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] dataMemory
         * @param[in,out] interruptController
         * @param[in,out] fusesAndLocks
         * @return
         */
        AVR8WatchdogTimer * link ( MCUSimEventLogger     * eventLogger,
                                   AVR8DataMemory          * dataMemory,
                                   AVR8InterruptController * interruptController,
                                   AVR8FusesAndLocks       * fusesAndLocks );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSimBase::ResetMode mode );

        /**
         * @brief
         * @param[in] timeStep
         * @param[in] clockCycles
         */
        void timeStep ( float timeStep,
                        unsigned int clockCycles = 0 );    // <-- This has to be called even in a sleep mode

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         */
        void wdr()
        {
            reset(MCUSim::RSTMD_MCU_RESET);
        };

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] clockCycles
         * @return
         */
        inline unsigned int readWdtcr ( const unsigned int clockCycles );

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
            ///
            AVR8DataMemory * m_dataMemory;

            ///
            AVR8InterruptController * m_interruptController;

            ///
            AVR8FusesAndLocks * m_fusesAndLocks;
        //@}

        /**
         * @brief
         */
        float m_time; // in 1µs

        /**
         * @brief
         */
        unsigned int m_prescaler;

        /**
         * @brief
         */
        unsigned int m_wdtcrLast;

        /**
         * @brief
         */
        int m_wdce_timer;
};

#endif // AVR8WATCHDOGTIMER_H
