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
 * @file AdaptableSimSim.h
 */
// =============================================================================

#ifndef AdaptableSimSIM_H
#define AdaptableSimSIM_H

// Forward declarations
class AdaptableSimIO;
class AdaptableSimStack;
class AdaptableSimConfig;
class AdaptableSimRegisters;
class AdaptableSimDataMemory;
class AdaptableSimStatusFlags;
class AdaptableSimClockControl;
class AdaptableSimProgramMemory;
class AdaptableSimInstructionSet;
class AdaptableSimInterruptController;

#include "../MCUSim.h"

#include <vector>

/**
 * @brief
 * @ingroup AdaptableSim
 * @class AdaptableSimSim
 */
class AdaptableSimSim : public MCUSim
{
    ////    Friend classes    ////
    friend class AdaptableSimConfig;

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        AdaptableSimSim();

        /**
         * @brief
         */
        virtual ~AdaptableSimSim();

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
        AdaptableSimConfig * m_config;

        /// @name AdaptableSim simulator subsystems
        //@{
            ///
            AdaptableSimIO * m_io;

            ///
            AdaptableSimStack * m_stack;

            ///
            AdaptableSimDataMemory * m_dataMemory;

            ///
            AdaptableSimRegisters * m_registers;

            ///
            AdaptableSimClockControl * m_clockControl;

            ///
            AdaptableSimProgramMemory * m_programMemory;

            ///
            AdaptableSimInterruptController * m_interruptController;

            ///
            AdaptableSimInstructionSet * m_instructionSet;

            ///
            AdaptableSimStatusFlags * m_statusFlags;
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

#endif // AdaptableSimSIM_H
