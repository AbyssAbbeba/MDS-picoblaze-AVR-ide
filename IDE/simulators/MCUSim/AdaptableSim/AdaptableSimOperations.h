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
 * @ingroup AdaptableSim
 * @file AdaptableSimOperations.h
 */
// =============================================================================

#ifndef ADAPTABLESIMOPERATIONS_H
#define ADAPTABLESIMOPERATIONS_H

#include "AdaptableSimInstructionSet.h"

class AdaptableSimOperations : public AdaptableSimInstructionSet
{
    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] operation
         * @param[in] operands
         * @param[in] parameters
         * @return
         */
        virtual bool operationSwitch ( AdaptableSimInstruction::OperationID operation,
                                       unsigned int operands [ AdaptableSimInstruction::OPERANDS_MAX ],
                                       AdaptableSimInstruction::OperParam parameters ) final override;

    ////    Inline Private Operations    ////
    private:
        /// @name Control Flow Operations
        //@{
            inline void instAbsoluteJump(AdaptableSimInstruction::OperParam parameters,unsigned int opr0);
            inline void instAbsoluteCall(AdaptableSimInstruction::OperParam parameters,unsigned int opr0);
            inline void instRelativeJump(AdaptableSimInstruction::OperParam parameters,unsigned int opr0);
            inline void instRelativeCall(AdaptableSimInstruction::OperParam parameters,unsigned int opr0);
            inline void instOffsetJump(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1);
            inline void instOffsetCall(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1);
            inline void instIndexJump(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1,unsigned int opr2);
            inline void instIndexCall(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1,unsigned int opr2);
            inline void instReturn();
            inline void instBitTest(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1);
        //@}

        /// @name Data Flow Operations
        //@{
            inline void instMove(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1);
            inline void instCbMove(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1);
            inline void instMoveBit(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1,unsigned int opr2);
            inline void instCbMoveBit(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1,unsigned int opr2);
            inline void instSwap (AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1);
            inline void instCbSwap (AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1);
            inline void instSwapBit (AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1,unsigned int opr2);
            inline void instCbSwapBit(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1,unsigned int opr2);
        //@}

        /// @name Interrupt Request and Service Operations
        //@{
            inline void instIsrReturn();
        //@}

        /// @name Arithmetic Operations
        //@{
            inline void instAdd(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1,unsigned int opr2);
            inline void instSub(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1,unsigned int opr2);
        //@}

        /// @name Logical Operations
        //@{
            inline void instAnd(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1,unsigned int opr2);
            inline void instOr(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1,unsigned int opr2);
            inline void instXor(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1,unsigned int opr2);
            inline void instCpl(AdaptableSimInstruction::OperParam parameters,unsigned int opr0);
        //@}

        /// @name Shift and Rotate Operations
        //@{
            inline void instShiftLeft0(AdaptableSimInstruction::OperParam parameters,unsigned int opr0);
            inline void instShiftRight0(AdaptableSimInstruction::OperParam parameters,unsigned int opr0);
            inline void instShiftLeft1(AdaptableSimInstruction::OperParam parameters,unsigned int opr0);
            inline void instShiftRight1(AdaptableSimInstruction::OperParam parameters,unsigned int opr0);
            inline void instShiftLeftR(AdaptableSimInstruction::OperParam parameters,unsigned int opr0);
            inline void instShiftRightR(AdaptableSimInstruction::OperParam parameters,unsigned int opr0);
            inline void instRotateLeft(AdaptableSimInstruction::OperParam parameters,unsigned int opr0);
            inline void instRotateRight(AdaptableSimInstruction::OperParam parameters,unsigned int opr0);
            inline void instRotateLeftC(AdaptableSimInstruction::OperParam parameters,unsigned int opr0);
            inline void instRotateRightC(AdaptableSimInstruction::OperParam parameters,unsigned int opr0);
        //@}

        /// @name System Control Operations
        //@{
            inline void instSetBank(unsigned int opr0);
        //@}
};

#endif // ADAPTABLESIMOPERATIONS_H
