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
 * @ingroup PicoBlaze
 * @file PicoBlazeInstructionSet.h
 */
// =============================================================================

#ifndef PICOBLAZEINSTRUCTIONSET_H
#define PICOBLAZEINSTRUCTIONSET_H

// Forward declarations
class PicoBlazeIO;
class PicoBlazeStack;
class PicoBlazeRegisters;
class PicoBlazeRegisters;
class PicoBlazeDataMemory;
class PicoBlazeProgramMemory;
class PicoBlazeInterruptController;

#include "../MCUSim.h"

#include "PicoBlazeInsNames.h"
#include "PicoBlazeStatusFlags.h"

#include <cstdint>

/**
 * @brief
 * @ingroup PicoBlaze
 * @class PicoBlazeInstructionSet
 */
class PicoBlazeInstructionSet : public MCUSimCPU
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
        PicoBlazeInstructionSet * link ( MCUSimEventLogger            * eventLogger,
                                         PicoBlazeIO                  * io,
                                         PicoBlazeStack               * stack,
                                         PicoBlazeRegisters           * registers,
                                         PicoBlazeDataMemory          * dataMemory,
                                         PicoBlazeStatusFlags         * statusFlags,
                                         PicoBlazeProgramMemory       * programMemory,
                                         PicoBlazeInterruptController * interruptController );

        /**
         * @brief
         * @param[in] obj
         */
        void adapt ( const PicoBlazeInstructionSet * obj );

        /**
         * @brief
         * @return
         */
        virtual bool isValid() const = 0;

        /**
         * @brief
         * @return
         */
        virtual int execInstruction() = 0;

        /**
         * @brief
         * @param[in] mode
         */
        virtual void reset ( MCUSimBase::ResetMode mode ) override;

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
        PicoBlazeStatusFlagsBase * getStatusFlags()
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
//             logEvent ( EVENT_CPU_PC_CHANGED, m_pc );
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
        unsigned int m_instructionCounter [ PicoBlazeInsNames::INS__MAX__ ];

        ///
        PicoBlazeInsNames::Instructions m_lastInstruction;

        /// @name PicoBlaze simulator subsystems
        //@{
            ///
            PicoBlazeIO * m_io;

            ///
            PicoBlazeStack * m_stack;

            ///
            PicoBlazeRegisters * m_registers;

            ///
            PicoBlazeDataMemory * m_dataMemory;

            ///
            PicoBlazeProgramMemory * m_programMemory;

            ///
            PicoBlazeInterruptController * m_interruptController;

            ///
            PicoBlazeStatusFlags * m_statusFlags;
        //@}
};

// -----------------------------------------------------------------------------
// Inline Function Definitions
// -----------------------------------------------------------------------------

inline int PicoBlazeInstructionSet::incrPc ( const int val )
{
    m_pc += val;
    while ( m_pc >= m_config.m_pcMax )
    {
        m_pc -= m_config.m_pcMax;
        logEvent ( MCUSimEventLogger::FLAG_HI_PRIO, EVENT_CPU_PC_OVERFLOW, m_pc );
    }
    while ( m_pc < 0 )
    {
        m_pc += m_config.m_pcMax;
        logEvent ( MCUSimEventLogger::FLAG_HI_PRIO, EVENT_CPU_PC_UNDERFLOW, m_pc );
    }
//     logEvent ( EVENT_CPU_PC_CHANGED, m_pc );
    return m_pc;
}

#endif // PICOBLAZEINSTRUCTIONSET_H
