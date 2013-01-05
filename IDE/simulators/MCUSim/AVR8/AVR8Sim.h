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
 * @file AVR8Sim.h
 */
// =============================================================================

#ifndef AVR8SIM_H
#define AVR8SIM_H

// Forward declarations
class AVR8Config;
class AVR8InstructionSet;
class AVR8ProgramMemory;
class AVR8DataMemory;
class AVR8FusesAndLocks;
class AVR8InterruptController;
class AVR8TimerCounter0;
class AVR8TimerCounter1;
class AVR8TimerCounter2;
class AVR8IO;
class AVR8ExternalInterrupts;
class AVR8DataEEPROM;
class AVR8BootLoader;
class AVR8WatchdogTimer;
class AVR8SystemControl;
class AVR8ClockControl;
class AVR8Spi;
class AVR8Usart;
class AVR8Twi;
class AVR8Adc;
class AVR8AnalogComparator;
class AVR8Isp;
class AVR8ParallelProg;

#include "../MCUSim.h"
#include "AVR8RegNames.h"

#include <vector>

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8Sim
 */
class AVR8Sim : public MCUSim
{
    friend class AVR8Config;

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum SleepMode
        {
            /// MCU is not in a sleep mode
            SLEEPMD_NONE            = -1,

            /// SM2=0, SM1=0, SM0=0, mode: Idle
            SLEEPMD_IDLE            = 0,

            /// SM2=0, SM1=0, SM0=1, mode: ADC Noise Reduction
            SLEEPMD_ADC_NR          = (AVR8RegNames::MCUCR_SM0),

            /// SM2=0, SM1=1, SM0=0, mode: Power-down
            SLEEPMD_POWER_DOWN      = (AVR8RegNames::MCUCR_SM1),

            /// SM2=0, SM1=1, SM0=1, mode: Power-save
            SLEEPMD_POWER_SAVE      = (AVR8RegNames::MCUCR_SM1 | AVR8RegNames::MCUCR_SM0),

            /// SM2=1, SM1=0, SM0=0, mode: (Reserved)
            SLEEPMD_RESERVED0       = (AVR8RegNames::MCUCR_SM2),

            /// SM2=1, SM1=0, SM0=1, mode: (Reserved)
            SLEEPMD_RESERVED1       = (AVR8RegNames::MCUCR_SM2 | AVR8RegNames::MCUCR_SM0),

            /// SM2=1, SM1=1, SM0=0, mode: Standby
            SLEEPMD_STANDBY         = (AVR8RegNames::MCUCR_SM2 | AVR8RegNames::MCUCR_SM1),

            /// Invalid value in SM2:SM0 bits
            SLEEPMD_INVALID         = (AVR8RegNames::MCUCR_SM2 | AVR8RegNames::MCUCR_SM1 | AVR8RegNames::MCUCR_SM0)
        };

        /**
         * @brief
         */
        enum HaltMode
        {
            HALTM_NONE,  ///<
            HALTM_BREAK, ///<
            HALTM_PROG   ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        AVR8Sim();

        /**
         * @brief
         */
        ~AVR8Sim();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        Clock::ClockSource & getClockSource();

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( ResetMode mode );

        /**
         * @brief
         * @return
         */
        int executeInstruction();

        /**
         * @brief
         * @param[in] timeStep
         * @return
         */
        int timeStep ( float timeStep );

        /**
         * @brief
         * @param[in] id
         * @return
         */
        Subsys * getSubsys ( Subsys::SubsysId id );

        /**
         * @brief
         * @return
         */
        Config & getConfig();

        /**
         * @brief
         * @return
         */
        Family family() const;

        /**
         * @brief
         * @return
         */
        Arch arch() const;

        /**
         * @brief
         * @return
         */
        const char * name() const;

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] numOfCycles
         * @return
         */
        float cycles2time ( int numOfCycles )
        {
            return ( numOfCycles * m_clockPeriod );
        }

        /**
         * @brief
         * @return
         */
        EventLogger * getLog()
        {
            return m_eventLogger;
        }

        /**
         * @brief
         * @return
         */
        Mode mode() const
        {
            return m_processorMode;
        }

        /**
         * @brief
         * @return
         */
        SleepMode sleepMode() const
        {
            return m_sleepMode;
        }

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         */
        inline void deleteSubSystems();

        /**
         * @brief
         */
        inline void checkSubSystems() const;

        /**
         * @brief
         * @param[in,out] subSystem
         */
        inline void regSubSys ( Subsys * subSystem );

        /**
         * @brief
         */
        inline void resetToInitialValues();

        /**
         * @brief
         */
        inline void loadConfig();

        /**
         * @brief
         */
        inline void mcuReset();

    ////    Protected Attributes    ////
    protected:
        /**
         * @brief
         */
        AVR8Config * m_config;

        /// @name AVR8 simulator subsystems
        //@{
            ///
            AVR8InstructionSet * m_instructionSet;

            ///
            AVR8ProgramMemory * m_programMemory;

            ///
            AVR8DataMemory * m_dataMemory;

            ///
            AVR8FusesAndLocks * m_fusesAndLocks;

            ///
            AVR8InterruptController * m_interrupts;

            ///
            AVR8TimerCounter0 * m_timerCounter0;

            ///
            AVR8TimerCounter1 * m_timerCounter1;

            ///
            AVR8TimerCounter2 * m_timerCounter2;

            ///
            AVR8IO * m_io;

            ///
            AVR8ExternalInterrupts * m_externalInterrupts;

            ///
            AVR8DataEEPROM * m_dataEEPROM;

            ///
            AVR8BootLoader * m_bootLoader;

            ///
            AVR8WatchdogTimer * m_watchdogTimer;

            ///
            AVR8SystemControl * m_systemControl;

            ///
            AVR8ClockControl * m_clockControl;

            ///
            AVR8Spi * m_spi;

            ///
            AVR8Usart * m_usart;

            ///
            AVR8Twi * m_twi;

            ///
            AVR8Adc * m_adc;

            ///
            AVR8AnalogComparator * m_acomp;

            ///
            AVR8Isp * m_isp;

            ///
            AVR8ParallelProg * m_pprog;
        //@}

        /**
         * @brief
         */
        EventLogger * m_eventLogger;

        /**
         * @brief
         */
        Mode m_processorMode;

        /**
         * @brief
         */
        HaltMode m_haltMode;

        /**
         * @brief
         */
        SleepMode m_sleepMode;

        /**
         * @brief
         */
        float m_clockPeriod;

        /**
         * @brief
         */
        float m_time;

        /**
         * @brief
         */
        int m_clockCycles;

    ////    Private Attributes    ////
    private:
        /**
         * @brief
         */
        std::vector<Subsys*> m_subSystems;
};

#endif // AVR8SIM_H
