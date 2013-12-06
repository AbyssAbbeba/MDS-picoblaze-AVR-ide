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
 * @file PicoBlazeInstructionSet1.h
 */
// =============================================================================

#ifndef PICOBLAZEINSTRUCTIONSET1_H
#define PICOBLAZEINSTRUCTIONSET1_H

#include "PicoBlazeInstructionSet.h"

/**
 * @brief
 * @ingroup PicoBlaze
 * @class PicoBlazeInstructionSet1
 */
class PicoBlazeInstructionSet1 : public PicoBlazeInstructionSet
{
    ////    Public Static Constants    ////
    public:
        /**
         * @brief
         */
        static void ( PicoBlazeInstructionSet1 :: * const m_opCodeDispatchTable [ 16 ] ) ( const unsigned int opCode );

        /**
         * @brief
         */
        static void ( PicoBlazeInstructionSet1 :: * const m_inst_1100_DTable [ 8 ] ) ( const unsigned int opCode );

        /**
         * @brief
         */
        static void ( PicoBlazeInstructionSet1 :: * const m_inst_1101_DTable [ 16 ] ) ( const unsigned int opCode );

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
         * - DISABLE INTERRUPT : 1000 0000 0001 0000
         * - ENABLE INTERRUPT  : 1000 0000 0011 0000
         * - RETURNI DISABLE   : 1000 0000 1101 0000
         * - RETURNI ENABLE    : 1000 0000 1111 0000
         * - JUMP aaa          : 1000 --01 aaaa aaaa
         * - CALL aaa          : 1000 --11 aaaa aaaa
         * - RETURN            : 1000 --00 1000 0000
         */
        void inst_1000 ( const unsigned int opCode );

        /**
         * @brief
         *
         * Possible instuctions:
         * - JUMP cnd, aaa : 1001 cc01 aaaa aaaa
         * - CALL cnd, aaa : 1001 cc11 aaaa aaaa
         * - RETURN cnd    : 1001 cc00 1000 0000
         */
        void inst_1001 ( const unsigned int opCode );

        /**
         * @brief
         *
         * Possible instuctions:
         * - LOAD  sX, sY : 1100 xxxx yyyy 0000
         * - AND   sX, sY : 1100 xxxx yyyy 0001
         * - OR    sX, sY : 1100 xxxx yyyy 0010
         * - XOR   sX, sY : 1100 xxxx yyyy 0011
         * - ADD   sX, sY : 1100 xxxx yyyy 0100
         * - ADDCY sX, sY : 1100 xxxx yyyy 0101
         * - SUB   sX, sY : 1100 xxxx yyyy 0110
         * - SUBCY sX, sY : 1100 xxxx yyyy 0111
         */
        void inst_1100 ( const unsigned int opCode );

        /**
         * @brief
         *
         * Possible instuctions:
         * - SLA sX : 1101 xxxx 0000 0000
         * - RL  sX : 1101 xxxx 0000 0010
         * - SLX sX : 1101 xxxx 0000 0100
         * - SL0 sX : 1101 xxxx 0000 0110
         * - SL1 sX : 1101 xxxx 0000 0111
         * - SRA sX : 1101 xxxx 0000 1000
         * - SRX sX : 1101 xxxx 0000 1010
         * - RR  sX : 1101 xxxx 0000 1100
         * - SR0 sX : 1101 xxxx 0000 1110
         * - SR1 sX : 1101 xxxx 0000 1111
         */
        void inst_1101 ( const unsigned int opCode );

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
                inline void inst_JUMP   ( const unsigned int opCode );
                inline void inst_CALL   ( const unsigned int opCode );
                inline void inst_RETURN ( const unsigned int opCode );
            //@}

            /// @name Arithmetic Group
            //@{
                void inst_ADD_sx_sy   ( const unsigned int opCode );
                void inst_ADD_sx_kk   ( const unsigned int opCode );
                void inst_ADDCY_sx_sy ( const unsigned int opCode );
                void inst_ADDCY_sx_kk ( const unsigned int opCode );
                void inst_SUB_sx_sy   ( const unsigned int opCode );
                void inst_SUB_sx_kk   ( const unsigned int opCode );
                void inst_SUBCY_sx_sy ( const unsigned int opCode );
                void inst_SUBCY_sx_kk ( const unsigned int opCode );
            //@}

            /// @name Interrupt Group
            //@{
                inline void inst_RETURNI_ENA ( const unsigned int opCode );
                inline void inst_RETURNI_DIS ( const unsigned int opCode );
                inline void inst_ENABLE_INT  ( const unsigned int opCode );
                inline void inst_DISABLE_INT ( const unsigned int opCode );
                inline void inst_RETURNI_aux ();
            //@}

            /// @name Logical Group
            //@{
                void inst_LOAD_sx_sy ( const unsigned int opCode );
                void inst_LOAD_sx_kk ( const unsigned int opCode );
                void inst_AND_sx_sy  ( const unsigned int opCode );
                void inst_AND_sx_kk  ( const unsigned int opCode );
                void inst_OR_sx_sy   ( const unsigned int opCode );
                void inst_OR_sx_kk   ( const unsigned int opCode );
                void inst_XOR_sx_sy  ( const unsigned int opCode );
                void inst_XOR_sx_kk  ( const unsigned int opCode );
            //@}

            /// @name Shift and Rotate Group
            //@{
                void inst_SR0_sx ( const unsigned int opCode );
                void inst_SR1_sx ( const unsigned int opCode );
                void inst_SRX_sx ( const unsigned int opCode );
                void inst_SRA_sx ( const unsigned int opCode );
                void inst_RR_sx  ( const unsigned int opCode );

                void inst_SL0_sx ( const unsigned int opCode );
                void inst_SL1_sx ( const unsigned int opCode );
                void inst_SLX_sx ( const unsigned int opCode );
                void inst_SLA_sx ( const unsigned int opCode );
                void inst_RL_sx  ( const unsigned int opCode );
            //@}

            /// @name Input/Output Group
            //@{
                void inst_INPUT_sx_pp  ( const unsigned int opCode );
                void inst_INPUT_sx_sy  ( const unsigned int opCode );
                void inst_OUTPUT_sx_pp ( const unsigned int opCode );
                void inst_OUTPUT_sx_sy ( const unsigned int opCode );
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

#endif // PICOBLAZEINSTRUCTIONSET1_H
