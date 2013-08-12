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
 * @file PicoBlazeInstructionSet2.h
 */
// =============================================================================

#ifndef PICOBLAZEINSTRUCTIONSET2_H
#define PICOBLAZEINSTRUCTIONSET2_H

#include "PicoBlazeInstructionSet.h"

/**
 * @brief
 * @ingroup PicoBlaze
 * @class PicoBlazeInstructionSet2
 */
class PicoBlazeInstructionSet2 : public PicoBlazeInstructionSet
{
    ////    Public Static Constants    ////
    public:
        /**
         * @brief
         */
        static void ( PicoBlazeInstructionSet2 :: * const m_opCodeDispatchTable [ 32 ] ) ( const unsigned int opCode );

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        int execInstruction();

        /**
         * @brief
         * @return
         */
        bool isValid() const;

    ////    Instruction Operations - Public Operations and Inline Public Operations    ////
    public:
        /**
         * @brief
         *
         * Possible instuctions:
         * - SRA Sx : 10 1000 xxxx 0000 1000
         * - SRX Sx : 10 1000 xxxx 0000 1010
         * - RR Sx  : 10 1000 xxxx 0000 1100
         * - SR0 Sx : 10 1000 xxxx 0000 1110
         * - SR1 Sx : 10 1000 xxxx 0000 1111
         * - SLA Sx : 10 1000 xxxx 0000 0000
         * - RL Sx  : 10 1000 xxxx 0000 0010
         * - SLX Sx : 10 1000 xxxx 0000 0100
         * - SL0 Sx : 10 1000 xxxx 0000 0110
         * - SL1 Sx : 10 1000 xxxx 0000 0111
         */
        void inst_10100 ( const unsigned int opCode );

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

#endif // PICOBLAZEINSTRUCTIONSET2_H
