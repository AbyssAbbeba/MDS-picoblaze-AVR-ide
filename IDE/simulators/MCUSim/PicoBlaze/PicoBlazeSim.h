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
 * @file PicoBlazeSim.h
 */
// =============================================================================

#ifndef PICOBLAZESIM_H
#define PICOBLAZESIM_H

// Forward declarations
class PicoBlazeIO;
class PicoBlazeStack;
class PicoBlazeConfig;
class PicoBlazeRegisters;
class PicoBlazeDataMemory;
class PicoBlazeStatusFlags;
class PicoBlazeClockControl;
class PicoBlazeProgramMemory;
class PicoBlazeInstructionSet;
class PicoBlazeInterruptController;

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
        virtual ~PicoBlazeSim();

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
            return MD_NORMAL;
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
         * @param[in] subSystem
         */
        inline void unregSubSys ( const MCUSimSubsys * subSystem );

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

            ///
            PicoBlazeStatusFlags * m_statusFlags;
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
