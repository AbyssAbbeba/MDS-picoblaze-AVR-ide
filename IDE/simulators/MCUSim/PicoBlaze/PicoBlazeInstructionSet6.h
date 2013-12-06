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
 * @file PicoBlazeInstructionSet6.h
 */
// =============================================================================

#ifndef PICOBLAZEINSTRUCTIONSET6_H
#define PICOBLAZEINSTRUCTIONSET6_H

#include "PicoBlazeInstructionSet.h"

/**
 * @brief
 * @ingroup PicoBlaze
 * @class PicoBlazeInstructionSet6
 */
class PicoBlazeInstructionSet6 : public PicoBlazeInstructionSet
{
    ////    Public Static Constants    ////
    public:
        /**
         * @brief
         */
        static void ( PicoBlazeInstructionSet6 :: * const m_opCodeDispatchTable [ 64 ] ) ( const unsigned int opCode );

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
         * - SLA sX     : 0x14-00
         * - RL sX      : 0x14-02
         * - SRX sX     : 0x14-04
         * - SL0 sX     : 0x14-06
         * - SL1 sX     : 0x14-07
         * - SRA sX     : 0x14-08
         * - SLX sX     : 0x14-0A
         * - RR sX      : 0x14-0C
         * - SR0 sX     : 0x14-0E
         * - SR1 sX     : 0x14-0F
         * - HWBUILD sX : 0x14-80
         */
        void inst_0x14 ( const unsigned int opCode );

        /**
         * @brief
         *
         * Possible instuctions:
         * - DISABLE INTERRUPT : 0x28000
         * - ENABLE INTERRUPT  : 0x28001
         */
        void inst_0x28 ( const unsigned int opCode );

        /**
         * @brief
         *
         * Possible instuctions:
         * - RETURNI DISABLE : 0x29000
         * - RETURNI ENABLE  : 0x29001
         */
        void inst_0x29 ( const unsigned int opCode );

        /**
         * @brief
         *
         * Possible instuctions:
         * - REGBANK A : 0x37000
         * - REGBANK B : 0x37001
         */
        void inst_0x37 ( const unsigned int opCode );

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

            /// @name Register loading
            //@{
                void inst_LOAD_sX_sY ( const unsigned int opCode );
                void inst_LOAD_sX_kk ( const unsigned int opCode );
                void inst_STAR_sX_sY ( const unsigned int opCode );
            //@}

            /// @name Logical
            //@{
                void inst_AND_sX_sY ( const unsigned int opCode );
                void inst_AND_sX_kk ( const unsigned int opCode );
                void inst_OR_sX_sY  ( const unsigned int opCode );
                void inst_OR_sX_kk  ( const unsigned int opCode );
                void inst_XOR_sX_sY ( const unsigned int opCode );
                void inst_XOR_sX_kk ( const unsigned int opCode );
            //@}

            /// @name Arithmetic
            //@{
                void inst_ADD_sX_sY   ( const unsigned int opCode );
                void inst_ADD_sX_kk   ( const unsigned int opCode );
                void inst_ADDCY_sX_sY ( const unsigned int opCode );
                void inst_ADDCY_sX_kk ( const unsigned int opCode );
                void inst_SUB_sX_sY   ( const unsigned int opCode );
                void inst_SUB_sX_kk   ( const unsigned int opCode );
                void inst_SUBCY_sX_sY ( const unsigned int opCode );
                void inst_SUBCY_sX_kk ( const unsigned int opCode );
            //@}

            /// @name Test and Compare
            //@{
                void inst_TEST_sX_sY      ( const unsigned int opCode );
                void inst_TEST_sX_kk      ( const unsigned int opCode );
                void inst_TESTCY_sX_sY    ( const unsigned int opCode );
                void inst_TESTCY_sX_kk    ( const unsigned int opCode );
                void inst_COMPARE_sX_sY   ( const unsigned int opCode );
                void inst_COMPARE_sX_kk   ( const unsigned int opCode );
                void inst_COMPARECY_sX_sY ( const unsigned int opCode );
                void inst_COMPARECY_sX_kk ( const unsigned int opCode );
            //@}

            /// @name Shift and Rotate
            //@{
                inline void inst_SR0_sX ( const unsigned int opCode );
                inline void inst_SR1_sX ( const unsigned int opCode );
                inline void inst_SRX_sX ( const unsigned int opCode );
                inline void inst_SRA_sX ( const unsigned int opCode );
                inline void inst_RR_sX  ( const unsigned int opCode );

                inline void inst_SL0_sX ( const unsigned int opCode );
                inline void inst_SL1_sX ( const unsigned int opCode );
                inline void inst_SLX_sX ( const unsigned int opCode );
                inline void inst_SLA_sX ( const unsigned int opCode );
                inline void inst_RL_sX  ( const unsigned int opCode );
            //@}

            /// @name Register Bank Selection
            //@{
                inline void inst_REGBANK_A ( const unsigned int opCode );
                inline void inst_REGBANK_B ( const unsigned int opCode );
            //@}

            /// @name Input and Output
            //@{
                void inst_INPUT_sX_sY  ( const unsigned int opCode );
                void inst_INPUT_sX_pp  ( const unsigned int opCode );
                void inst_OUTPUT_sX_sY ( const unsigned int opCode );
                void inst_OUTPUT_sX_pp ( const unsigned int opCode );
                void inst_OUTPUTK_kk_p ( const unsigned int opCode );
            //@}

            /// @name Scratch Pad Memory
            //@{
                void inst_STORE_sX_sY ( const unsigned int opCode );
                void inst_STORE_sX_ss ( const unsigned int opCode );
                void inst_FETCH_sX_sY ( const unsigned int opCode );
                void inst_FETCH_sX_ss ( const unsigned int opCode );
            //@}

            /// @name Interrupt Handling
            //@{
                inline void inst_DISABLE_INTERRUPT ( const unsigned int opCode );
                inline void inst_ENABLE_INTERRUPT  ( const unsigned int opCode );
                inline void inst_RETURNI_DISABLE   ( const unsigned int opCode );
                inline void inst_RETURNI_ENABLE    ( const unsigned int opCode );
            //@}

            /// @name Jump
            //@{
                void inst_JUMP_aaa    ( const unsigned int opCode );
                void inst_JUMP_Z_aaa  ( const unsigned int opCode );
                void inst_JUMP_NZ_aaa ( const unsigned int opCode );
                void inst_JUMP_C_aaa  ( const unsigned int opCode );
                void inst_JUMP_NC_aaa ( const unsigned int opCode );
                void inst_JUMP_sX_sY  ( const unsigned int opCode );
            //@}

            /// @name Subroutines
            //@{
                void inst_CALL_aaa     ( const unsigned int opCode );
                void inst_CALL_Z_aaa   ( const unsigned int opCode );
                void inst_CALL_NZ_aaa  ( const unsigned int opCode );
                void inst_CALL_C_aaa   ( const unsigned int opCode );
                void inst_CALL_NC_aaa  ( const unsigned int opCode );
                void inst_CALL_sX_sY   ( const unsigned int opCode );
                void inst_RETURN       ( const unsigned int opCode );
                void inst_RETURN_Z     ( const unsigned int opCode );
                void inst_RETURN_NZ    ( const unsigned int opCode );
                void inst_RETURN_C     ( const unsigned int opCode );
                void inst_RETURN_NC    ( const unsigned int opCode );
                void inst_LD_RET_sX_sY ( const unsigned int opCode );
            //@}

            /// @name Version Control
            //@{
                inline void inst_HWBUILD_sX ( const unsigned int opCode );
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

#endif // PICOBLAZEINSTRUCTIONSET6_H
