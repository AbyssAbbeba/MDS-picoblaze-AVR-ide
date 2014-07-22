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
 * @file PicoBlazeInstructionSet3.h
 */
// =============================================================================

#ifndef PICOBLAZEINSTRUCTIONSET3_H
#define PICOBLAZEINSTRUCTIONSET3_H

#include "PicoBlazeInstructionSet.h"

/**
 * @brief
 * @ingroup PicoBlaze
 * @class PicoBlazeInstructionSet3
 */
class PicoBlazeInstructionSet3 : public PicoBlazeInstructionSet
{
    ////    Public Static Constants    ////
    public:
        /**
         * @brief
         */
        static void ( PicoBlazeInstructionSet3 :: * const m_opCodeDispatchTable [ 32 ] ) ( const unsigned int opCode );

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        virtual int execInstruction() override;

        /**
         * @brief
         * @return
         */
        virtual bool isValid() const override;

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
         * @param[in] instName
         */
        inline void instructionEnter ( PicoBlazeInsNames::Instructions instName );
};

#endif // PICOBLAZEINSTRUCTIONSET3_H
