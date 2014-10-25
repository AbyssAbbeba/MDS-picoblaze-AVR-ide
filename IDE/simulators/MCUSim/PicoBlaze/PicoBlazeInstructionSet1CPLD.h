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
 * @file PicoBlazeInstructionSet1CPLD.h
 */
// =============================================================================

#ifndef PICOBLAZEINSTRUCTIONSET1CPLD_H
#define PICOBLAZEINSTRUCTIONSET1CPLD_H

#include "PicoBlazeInstructionSet.h"

/**
 * @brief
 * @ingroup PicoBlaze
 * @class PicoBlazeInstructionSet1CPLD
 */
class PicoBlazeInstructionSet1CPLD : public PicoBlazeInstructionSet
{
    ////    Public Static Constants    ////
    public:
        /**
         * @brief
         */
        static void ( PicoBlazeInstructionSet1CPLD ::* const m_opCodeDispatchTable [ 32 ] ) (const unsigned int opCode);

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
         * Possible instructions:
         * - SLA sX : 10 1000 xxxx 0000 0000
         * - SLX sX : 10 1000 xxxx 0000 0010
         * - RL sX  : 10 1000 xxxx 0000 0100
         * - SL0 sX : 10 1000 xxxx 0000 0110
         * - SL1 sX : 10 1000 xxxx 0000 0111
         * - SRA sX : 10 1000 xxxx 0000 1000
         * - SRX sX : 10 1000 xxxx 0000 1010
         * - RR sX  : 10 1000 xxxx 0000 1100
         * - SR0 sX : 10 1000 xxxx 0000 1110
         * - SR1 sX : 10 1000 xxxx 0000 1111
         */
        void inst_10100 ( const unsigned int opCode );

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
                void inst_ADD_sx_kk   ( const unsigned int opCode );
                void inst_ADD_sx_sy   ( const unsigned int opCode );
                void inst_ADDCY_sx_kk ( const unsigned int opCode );
                void inst_ADDCY_sx_sy ( const unsigned int opCode );
                void inst_SUB_sx_kk   ( const unsigned int opCode );
                void inst_SUB_sx_sy   ( const unsigned int opCode );
                void inst_SUBCY_sx_kk ( const unsigned int opCode );
                void inst_SUBCY_sx_sy ( const unsigned int opCode );
            //@}

            /// @name Interrupt Group
            //@{
                void inst_RETURNI   ( const unsigned int opCode );
                void inst_INTERRUPT ( const unsigned int opCode );
            //@}

            /// @name Logical Group
            //@{
                void inst_LOAD_sx_kk ( const unsigned int opCode );
                void inst_LOAD_sx_sy ( const unsigned int opCode );
                void inst_AND_sx_kk  ( const unsigned int opCode );
                void inst_AND_sx_sy  ( const unsigned int opCode );
                void inst_OR_sx_kk   ( const unsigned int opCode );
                void inst_OR_sx_sy   ( const unsigned int opCode );
                void inst_XOR_sx_kk  ( const unsigned int opCode );
                void inst_XOR_sx_sy  ( const unsigned int opCode );
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

#endif // PICOBLAZEINSTRUCTIONSET1CPLD_H
