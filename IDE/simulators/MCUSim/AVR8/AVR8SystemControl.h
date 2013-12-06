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
 * @file AVR8SystemControl.h
 */
// =============================================================================

#ifndef AVR8SYSTEMCONTROL_H
#define AVR8SYSTEMCONTROL_H

// Forward declarations
class AVR8DataMemory;
class AVR8InterruptController;
class AVR8WatchdogTimer;
class AVR8IO;
class AVR8FusesAndLocks;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8SystemControl
 */
class AVR8SystemControl : public MCUSimSubsys
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Event
        {
            EVENT_EXT_RESET_INVOKED,    ///<
            EVENT_EXT_RESET_START,      ///<
            EVENT_EXT_RESET_CANCELED,   ///<
            EVENT_EXT_RESET_UNSTABLE,   ///<

            EVENT_BOD_RESET_TMR_START,  ///<
            EVENT_BOD_RESET_BEGIN,      ///<
            EVENT_BOD_RESET_END,        ///<

            EVENT_POWER_ON_RESET_BEGIN, ///<
            EVENT_POWER_ON_RESET_END,   ///<

            EVENT__MAX__                ///<
        };

        /**
         * @brief
         */
        struct Config
        {
            ///
            float m_resetTresholdLow;

            ///
            float m_resetTresholdHigh;

            ///
            float m_minResetPulseWidth;

            ///
            float m_powerOnResetTresholdFalling;

            ///
            float m_powerOnResetTresholdRaising;

            ///
            float m_tBOD; // t_BOD: Minimum low voltage period for Brown-out Detection (see manual for details).

            ///
            float m_brownOutTreshold [ 2 ]; // [0] for bodlevel 0, [1] for bodlevel 1

            ///
            bool m_hasPowerOnReset;

            ///
            bool m_hasBrownOutReset;

            ///
            bool m_hasExternalReset;

            ///
            bool m_hasWatchDog;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        AVR8SystemControl() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] dataMemory
         * @param[in,out] interruptController
         * @param[in,out] watchdogTimer
         * @param[in,out] io
         * @param[in,out] fusesAndLocks
         * @return
         */
        AVR8SystemControl * link ( MCUSimEventLogger     * eventLogger,
                                   AVR8DataMemory          * dataMemory,
                                   AVR8InterruptController * interruptController,
                                   AVR8WatchdogTimer       * watchdogTimer,
                                   AVR8IO                  * io,
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
                        unsigned int clockCycles = 0 );

        /**
         * @brief
         */
        void watchDogReset();

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] timeStep
         */
        inline void handleExternalReset ( const float timeStep );

        /**
         * @brief
         * @param[in] timeStep
         */
        inline void handleBrownOutReset ( const float timeStep );

        /**
         * @brief
         */
        inline void handlePowerOnReset();

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
            AVR8WatchdogTimer * m_watchdogTimer;

            ///
            AVR8IO * m_io;

            ///
            AVR8FusesAndLocks * m_fusesAndLocks;
        //@}

        /**
         * @brief
         */
        float m_rstTime;

        /**
         * @brief
         */
        float m_brownOutTimer;

        /**
         * @brief
         */
        bool m_resetInProgress;

        /**
         * @brief
         */
        bool m_powerOnReset;

        /**
         * @brief
         */
        bool m_brownOutReset;
};

#endif // AVR8SYSTEMCONTROL_H
