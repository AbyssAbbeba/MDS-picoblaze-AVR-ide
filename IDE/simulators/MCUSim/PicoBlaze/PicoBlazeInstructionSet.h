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
 * @file PicoBlazeInstructionSet.h
 */
// =============================================================================

#ifndef PICOBLAZEINSTRUCTIONSET_H
#define PICOBLAZEINSTRUCTIONSET_H

// Forward declarations
class PicoBlazeIO;
class PicoBlazeStack;
class PicoBlazeRegisters;
class PicoBlazeDataMemory;
class PicoBlazeProgramMemory;
class PicoBlazeInterruptController;

#include "../MCUSim.h"
#include "PicoBlazeInsNames.h"

#include <cstdint>

/**
 * @brief
 * @ingroup PicoBlaze
 * @class PicoBlazeInstructionSet
 */
class PicoBlazeInstructionSet : public MCUSimCPU
{
    ////    Public Static Constants    ////
    public:
        /// @brief
        static const unsigned int INTERRUPT_VECTOR = 0x3ff;

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
                m_pcMax = 1024; // <- default value for KCPSM3
            }

            ///
            bool m_ignoreUndefinedOpCodes;

            ///
            int m_pcMax;
        };

        /**
         * @brief
         */
        struct StatusFlags
        {
            StatusFlags();

            void reset();
            void interrupt();
            void returni();

            /// Interrupt enable flag
            bool m_inte;

            /// 
            bool m_carry;

            /// 
            bool m_zero;

            ///
            bool m_preCarry;

            ///
            bool m_preZero;

            ///
            int m_interrupted;
        };

    ////    Static Public Constants    ////
    public:
        /**
         * @brief
         */
        static void ( PicoBlazeInstructionSet :: * const m_opCodeDispatchTable [ 32 ] ) ( const unsigned int opCode );

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        PicoBlazeInstructionSet() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] io
         * @param[in,out] stack
         * @param[in,out] registers
         * @param[in,out] dataMemory
         * @param[in,out] programMemory
         * @param[in,out] interruptController
         * @return
         */
        PicoBlazeInstructionSet * link ( MCUSimEventLogger          * eventLogger,
                                         PicoBlazeIO                  * io,
                                         PicoBlazeStack               * stack,
                                         PicoBlazeRegisters           * registers,
                                         PicoBlazeDataMemory          * dataMemory,
                                         PicoBlazeProgramMemory       * programMemory,
                                         PicoBlazeInterruptController * interruptController );

        /**
         * @brief
         * @return
         */
        int execInstruction();

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSimBase::ResetMode mode );

        /**
         * @brief Request program interrupt.
         */
        void irq();

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        StatusFlags getStatusFlags()
        {
            return m_statusFlags;
        }

        /**
         * @brief
         * @param[in] newFlags
         */
        void setStatusFlags ( StatusFlags newFlags )
        {
            m_statusFlags = newFlags;
        }

        /**
         * @brief
         * @return
         */
        unsigned int getProgramCounter() const
        {
            return ( unsigned int ) m_pc;
        }

        /**
         * @brief
         * @param[in] newPc
         */
        void setProgramCounter ( unsigned int newPc )
        {
            m_pc = int ( newPc );
            logEvent ( EVENT_CPU_PC_CHANGED, m_pc );
        }

    ////    Instruction Operations - Public Operations and Inline Public Operations    ////
    public:
        /**
         * @brief
         *
         * Possible instuctions:
         * - SRA Sx : 10 0000 xxxx 0000 1000
         * - SRX Sx : 10 0000 xxxx 0000 1010
         * - RR Sx  : 10 0000 xxxx 0000 1100
         * - SR0 Sx : 10 0000 xxxx 0000 1110
         * - SR1 Sx : 10 0000 xxxx 0000 1111
         * - SLA Sx : 10 0000 xxxx 0000 0000
         * - RL Sx  : 10 0000 xxxx 0000 0010
         * - SLX Sx : 10 0000 xxxx 0000 0100
         * - SL0 Sx : 10 0000 xxxx 0000 0110
         * - SL1 Sx : 10 0000 xxxx 0000 0111
         */
        void inst_10000 ( const unsigned int opCode );

        /**
         * @brief
         *
         * Possible instuctions:
         * - ENABLE INTERRUPT  : 11 1110 0000 0000 0001
         * - DISABLE INTERRUPT : 11 1110 0000 0000 0000
         */
        void inst_11110 ( const unsigned int opCode );

        /// @name Processor Instructions
        //@{
            /**
             * @brief Invalid instruction.
             * @param[in] opCode
             * @return
             *
             * @warning Invocation of this method is an error condition.
             */
            void inst__ ( const unsigned int opCode );

            /// @name Program Control Group
            //@{
                void inst_JUMP   ( const unsigned int opCode );
                void inst_CALL   ( const unsigned int opCode );
                void inst_RETURN ( const unsigned int opCode );
            //@}

            /// @name Arithmetic Group
            //@{
                void inst_ADD     ( const unsigned int opCode );
                void inst_ADDCY   ( const unsigned int opCode );
                void inst_SUB     ( const unsigned int opCode );
                void inst_SUBCY   ( const unsigned int opCode );
                void inst_COMPARE ( const unsigned int opCode );
            //@}

            /// @name Interrupt Group
            //@{
                void inst_RETURNI            ( const unsigned int opCode );
                inline void inst_ENABLE_INT  ( const unsigned int opCode );
                inline void inst_DISABLE_INT ( const unsigned int opCode );
            //@}

            /// @name Logical Group
            //@{
                void inst_LOAD ( const unsigned int opCode );
                void inst_AND  ( const unsigned int opCode );
                void inst_OR   ( const unsigned int opCode );
                void inst_XOR  ( const unsigned int opCode );
                void inst_TEST ( const unsigned int opCode );
            //@}

            /// @name Storage Group
            //@{
                void inst_STORE ( const unsigned int opCode );
                void inst_FETCH ( const unsigned int opCode );
            //@}

            /// @name Shift and Rotate Group
            //@{
                inline void inst_SR0 ( const unsigned int opCode );
                inline void inst_SR1 ( const unsigned int opCode );
                inline void inst_SRX ( const unsigned int opCode );
                inline void inst_SRA ( const unsigned int opCode );
                inline void inst_RR  ( const unsigned int opCode );

                inline void inst_SL0 ( const unsigned int opCode );
                inline void inst_SL1 ( const unsigned int opCode );
                inline void inst_SLX ( const unsigned int opCode );
                inline void inst_SLA ( const unsigned int opCode );
                inline void inst_RL  ( const unsigned int opCode );
            //@}

            /// @name Input/Output Group
            //@{
                void inst_INPUT  ( const unsigned int opCode );
                void inst_OUTPUT ( const unsigned int opCode );
            //@}
        //@}

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] val
         * @return New value the program counter (PC)
         */
        inline int incrPc ( const int val = 1 );

        /**
         * @brief
         */
        inline void mcuReset();

        /**
         * @brief
         */
        inline void resetToInitialValues();

        /**
         * @brief
         * @param[in] instName
         */
        inline void instructionEnter ( PicoBlazeInsNames::Instructions instName );

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        Config m_config;

    ////    Protected Attributes    ////
    protected:
        ///
        StatusFlags m_statusFlags;

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
        //@}
};

#endif // PICOBLAZEINSTRUCTIONSET_H
