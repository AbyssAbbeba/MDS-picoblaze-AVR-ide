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
 * @file PIC8Sim.h
 */
// =============================================================================

#ifndef PIC8SIM_H
#define PIC8SIM_H

// Forward declarations
class PIC8InstructionSet;
class PIC8Config;
class PIC8DataMemory;
class PIC8ProgramMemory;
class PIC8ConfigWord;
class PIC8IO;
class PIC8ClockControl;
class PIC8Stack;
class PIC8InterruptController;
class PIC8DataEEPROM;
class PIC8WatchDogTimer;
class PIC8TimerCounter0;
class PIC8Timer0WdtPrescaller;
class PIC8ISP;
class PIC8ExternalInterrupts;

#include "../MCUSim.h"

#include <vector>

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8Sim
 */
class PIC8Sim : public MCUSim
{
    ////    Friend classes    ////
    friend class PIC8Config;

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        PIC8Sim();

        /**
         * @brief
         */
        virtual ~PIC8Sim();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        virtual MCUSimClock::ClockSource & getClockSource() override;

        /**
         * @brief
         * @param[in] mode
         */
        virtual void reset ( ResetMode mode ) override;

        /**
         * @brief
         * @return
         */
        virtual int executeInstruction() override;

        /**
         * @brief
         * @param[in] timeStep
         * @return
         */
        virtual int timeStep ( float timeStep ) override;

        /**
         * @brief
         * @param[in] id
         * @return
         */
        virtual MCUSimSubsys * getSubsys ( MCUSimSubsys::SubsysId id ) override;

        /**
         * @brief
         * @return
         */
        virtual MCUSimConfig & getConfig() override;

        /**
         * @brief
         * @return
         */
        virtual Family family() const override;

        /**
         * @brief
         * @return
         */
        virtual Arch arch() const override;

        /**
         * @brief
         * @return
         */
        virtual const char * name() const override;

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        virtual Mode mode() const override
        {
            return m_processorMode;
        }

        /**
         * @brief
         * @return
         */
        virtual MCUSimEventLogger * getLog() override
        {
            return m_eventLogger;
        }

        /**
         * @brief
         * @param[in] numOfCycles
         * @return
         */
        virtual float cycles2time ( int numOfCycles ) override
        {
            return ( numOfCycles * m_clockPeriod );
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
        inline void regSubSys ( MCUSimSubsys * subSystem );

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
        PIC8Config * m_config;

        /// @name PIC8 simulator subsystems
        //@{
            ///
            PIC8IO * m_io;

            ///
            PIC8Stack * m_stack;

            ///
            PIC8TimerCounter0 * m_timerCounter0;

            ///
            PIC8DataEEPROM * m_dataEEPROM;

            ///
            PIC8DataMemory * m_dataMemory;

            ///
            PIC8ConfigWord * m_configWord;

            ///
            PIC8ClockControl * m_clockControl;

            ///
            PIC8ProgramMemory * m_programMemory;

            ///
            PIC8WatchDogTimer * m_watchDogTimer;

            ///
            PIC8InterruptController * m_interruptController;

            ///
            PIC8InstructionSet * m_instructionSet;

            ///
            PIC8Timer0WdtPrescaller * m_timer0WdtPrescaller;

            ///
            PIC8ISP * m_isp;

            ///
            PIC8ExternalInterrupts * m_externalInterrupts;
        //@}

        /**
         * @brief
         */
        MCUSimEventLogger * m_eventLogger;

        /**
         * @brief
         */
        Mode m_processorMode;

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
        std::vector<MCUSimSubsys*> m_subSystems;
};

#endif // PIC8SIM_H
