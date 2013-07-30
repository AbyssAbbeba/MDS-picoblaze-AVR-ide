// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>, (C) 2013
 * @ingroup PicoBlaze
 * @file PicoBlazeSim.h
 */
// =============================================================================

#ifndef PICOBLAZESIM_H
#define PICOBLAZESIM_H

// Forward declarations
class PicoBlazeConfig;
class PicoBlazeInstructionSet;
class PicoBlazeInterruptController;
class PicoBlazeDataMemory;
class PicoBlazeRegisters;
class PicoBlazeProgramMemory;
class PicoBlazeIO;
class PicoBlazeClockControl;
class PicoBlazeStack;

#include "../MCUSim.h"

#include <vector>

/**
 * @brief
 * @ingroup PicoBlaze
 * @class PicoBlazeSim
 */
class PicoBlazeSim : public MCUSim
{
    ////    Friend classes    ////
    friend class PicoBlazeConfig;

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        PicoBlazeSim();

        /**
         * @brief
         */
        ~PicoBlazeSim();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        MCUSimClock::ClockSource & getClockSource();

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
        MCUSimSubsys * getSubsys ( MCUSimSubsys::SubsysId id );

        /**
         * @brief
         * @return
         */
        MCUSimConfig & getConfig();

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
            return MD_NORMAL;
        }

        /**
         * @brief
         * @return
         */
        MCUSimEventLogger * getLog()
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
        PicoBlazeConfig * m_config;

        /// @name PicoBlaze simulator subsystems
        //@{
            ///
            PicoBlazeIO * m_io;

            ///
            PicoBlazeStack * m_stack;

            ///
            PicoBlazeDataMemory * m_dataMemory;

            ///
            PicoBlazeRegisters * m_registers;

            ///
            PicoBlazeClockControl * m_clockControl;

            ///
            PicoBlazeProgramMemory * m_programMemory;

            ///
            PicoBlazeInterruptController * m_interruptController;

            ///
            PicoBlazeInstructionSet * m_instructionSet;
        //@}

        /**
         * @brief
         */
        MCUSimEventLogger * m_eventLogger;

        /**
         * @brief
         */
        float m_clockPeriod;

        /**
         * @brief
         */
        float m_time;

    ////    Private Attributes    ////
    private:
        /**
         * @brief
         */
        std::vector<MCUSimSubsys*> m_subSystems;
};

#endif // PICOBLAZESIM_H
