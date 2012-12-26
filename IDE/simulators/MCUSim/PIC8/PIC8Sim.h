// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2012 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
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
class PIC8InterruptCtrl;
class PIC8DataEEPROM;
class PIC8WatchDogTimer;
class PIC8Timer0;

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
        ~PIC8Sim();

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
        EventLogger * getLog()
        {
            return m_eventLogger;
        }

        /**
         * @brief
         * @param[in] numOfCycles
         * @return
         */
        float cycles2time ( int numOfCycles )
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
        PIC8Config * m_config;

        /// @name PIC8 simulator subsystems
        //@{
            PIC8InstructionSet * m_instructionSet; ///<
            PIC8ProgramMemory * m_programMemory;   ///<
            PIC8DataMemory * m_dataMemory;         ///<
            PIC8ConfigWord * m_configWord;         ///<
            PIC8IO * m_io;                         ///<
            PIC8ClockControl * m_clockControl;     ///<
            PIC8WatchDogTimer * m_watchDogTimer;   ///<
            PIC8Stack * m_stack;                   ///<
            PIC8InterruptCtrl * m_interruptCtrl;   ///<
            PIC8DataEEPROM * m_dataEEPROM;         ///<
            PIC8Timer0 * m_timer0;                 ///<
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

#endif // PIC8SIM_H
