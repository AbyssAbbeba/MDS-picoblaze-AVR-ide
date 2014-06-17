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
 * @file AdaptableSimInstructionSet.h
 */
// =============================================================================

#ifndef AdaptableSimINSTRUCTIONSET_H
#define AdaptableSimINSTRUCTIONSET_H

// Forward declarations
class AdaptableSimIO;
class AdaptableSimStack;
class AdaptableSimRegisters;
class AdaptableSimDataMemory;
class AdaptableSimProgramMemory;
class AdaptableSimInterruptController;

#include "../MCUSim.h"

#include "AdaptableSimInsNames.h"
#include "AdaptableSimStatusFlags.h"

#include <cstdint>

/**
 * @brief
 * @ingroup AdaptableSim
 * @class AdaptableSimInstructionSet
 */
class AdaptableSimInstructionSet : public MCUSimCPU
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct Config
        {
            ///
            Config()
            {
                m_ignoreUndefinedOpCodes = true;
            }

            ///
            MCUSim::Family m_dev;

            ///
            int m_pcMax;

            ///
            unsigned int m_interruptVector;

            ///
            bool m_ignoreUndefinedOpCodes;

            ///
            uint8_t m_hwbuild;
        };

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] io
         * @param[in,out] stack
         * @param[in,out] registers
         * @param[in,out] dataMemory
         * @param[in,out] statusFlags
         * @param[in,out] programMemory
         * @param[in,out] interruptController
         * @return
         */
        AdaptableSimInstructionSet * link ( MCUSimEventLogger            * eventLogger,
                                         AdaptableSimIO                  * io,
                                         AdaptableSimStack               * stack,
                                         AdaptableSimRegisters           * registers,
                                         AdaptableSimDataMemory          * dataMemory,
                                         AdaptableSimStatusFlags         * statusFlags,
                                         AdaptableSimProgramMemory       * programMemory,
                                         AdaptableSimInterruptController * interruptController );

        /**
         * @brief
         * @return
         */
        int execInstruction();

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSimBase::ResetMode mode ) override;

        /**
         * @brief Request program interrupt.
         */
        void irq();

    ////    Inline Protected Operations    ////
    protected:
        /**
         * @brief
         * @param[in] val
         * @return New value the program counter (PC)
         */
        inline int incrPc ( const int val = 1 );

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        AdaptableSimStatusFlagsBase * getStatusFlags()
        {
            return m_statusFlags;
        }

        /**
         * @brief
         * @return
         */
        virtual unsigned int getProgramCounter() const override
        {
            return ( unsigned int ) m_pc;
        }

        /**
         * @brief
         * @param[in] newPc
         */
        virtual void setProgramCounter ( unsigned int newPc ) override
        {
            m_pc = int ( newPc );
            logEvent ( EVENT_CPU_PC_CHANGED, m_pc );
        }

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         */
        inline void mcuReset();

        /**
         * @brief
         */
        inline void resetToInitialValues();

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        Config m_config;

    ////    Protected Attributes    ////
    protected:
        ///
        int m_pc;

        ///
        int m_actSubprogCounter;

        ///
        unsigned int m_instructionCounter [ AdaptableSimInsNames::INS__MAX__ ];

        ///
        AdaptableSimInsNames::Instructions m_lastInstruction;

        /// @name AdaptableSim simulator subsystems
        //@{
            ///
            AdaptableSimIO * m_io;

            ///
            AdaptableSimStack * m_stack;

            ///
            AdaptableSimRegisters * m_registers;

            ///
            AdaptableSimDataMemory * m_dataMemory;

            ///
            AdaptableSimProgramMemory * m_programMemory;

            ///
            AdaptableSimInterruptController * m_interruptController;

            ///
            AdaptableSimStatusFlags * m_statusFlags;
        //@}
};

// -----------------------------------------------------------------------------
// Inline Function Definitions
// -----------------------------------------------------------------------------

inline int AdaptableSimInstructionSet::incrPc ( const int val )
{
    m_pc += val;
    while ( m_pc >= m_config.m_pcMax )
    {
        m_pc -= m_config.m_pcMax;
        logEvent ( MCUSimEventLogger::FLAG_HI_PRIO, EVENT_CPU_PC_OVERFLOW );
    }
    while ( m_pc < 0 )
    {
        m_pc += m_config.m_pcMax;
        logEvent ( MCUSimEventLogger::FLAG_HI_PRIO, EVENT_CPU_PC_UNDERFLOW );
    }
    logEvent ( EVENT_CPU_PC_CHANGED, m_pc );
    return m_pc;
}

#endif // AdaptableSimINSTRUCTIONSET_H
