// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup AdaptableSim
 * @file AdaptableSimOperations.cxx
 */
// =============================================================================

#include "AdaptableSimOperations.h"

bool AdaptableSimOperations::operationSwitch ( AdaptableSimInstruction::OperationID operation,
                                               unsigned int operands [ AdaptableSimInstruction::OPERANDS_MAX ],
                                               AdaptableSimInstruction::OperParam parameters )
{
    if ( AdaptableSimInstruction::OperParam::C_NONE != parameters.condition() )
    {
        bool flag = m_statusFlags->getFlag(parameters.conditionFlag());
        if ( AdaptableSimInstruction::OperParam::C_POSITIVE == parameters.condition() )
        {
            if ( false == flag )
            {
                return false;
            }
        }
        else if ( true == flag )
        {
            return false;
        }
    }

    switch ( operation )
    {
        case AdaptableSimInstruction::OP_NONE:
            break;
        case AdaptableSimInstruction::OP_ABS_JUMP:
            instAbsoluteJump(parameters, operands[0]);
            break;
        case AdaptableSimInstruction::OP_ABS_CALL:
            instAbsoluteCall(parameters, operands[0]);
            break;
        case AdaptableSimInstruction::OP_REL_JUMP:
            instRelativeJump(parameters, operands[0]);
            break;
        case AdaptableSimInstruction::OP_REL_CALL:
            instRelativeCall(parameters, operands[0]);
            break;
        case AdaptableSimInstruction::OP_OFS_JUMP:
            instOffsetJump(parameters, operands[0], operands[1]);
            break;
        case AdaptableSimInstruction::OP_OFS_CALL:
            instOffsetCall(parameters, operands[0], operands[1]);
            break;
        case AdaptableSimInstruction::OP_IDX_JUMP:
            instIndexJump(parameters, operands[0], operands[1], operands[2]);
            break;
        case AdaptableSimInstruction::OP_IDX_CALL:
            instIndexCall(parameters, operands[0], operands[1], operands[2]);
            break;
        case AdaptableSimInstruction::OP_RETURN:
            instReturn();
            break;
        case AdaptableSimInstruction::OP_BIT_TEST:
            instBitTest(parameters, operands[0], operands[1]);
            break;
        case AdaptableSimInstruction::OP_ISR_RETURN:
            instIsrReturn();
            break;
        case AdaptableSimInstruction::OP_SET_BANK:
            instSetBank(operands[0]);
            break;
        case AdaptableSimInstruction::OP_MOVE:
            instMove(parameters, operands[0], operands[1]);
            break;
        case AdaptableSimInstruction::OP_CB_MOVE:
            instCbMove(parameters, operands[0], operands[1]);
            break;
        case AdaptableSimInstruction::OP_MOVE_BIT:
            instMoveBit(parameters, operands[0], operands[1], operands[1]);
            break;
        case AdaptableSimInstruction::OP_CB_MOVE_BIT:
            instCbMoveBit(parameters, operands[0], operands[1], operands[1]);
            break;
        case AdaptableSimInstruction::OP_SWAP:
            instSwap(parameters, operands[0], operands[1]);
            break;
        case AdaptableSimInstruction::OP_CB_SWAP:
            instCbSwap(parameters, operands[0], operands[1]);
            break;
        case AdaptableSimInstruction::OP_SWAP_BIT:
            instSwapBit(parameters, operands[0], operands[1], operands[1]);
            break;
        case AdaptableSimInstruction::OP_CB_SWAP_BIT:
            instCbSwapBit(parameters, operands[0], operands[1], operands[1]);
            break;
        case AdaptableSimInstruction::OP_ADD:
            instAdd(parameters, operands[0], operands[1], operands[2]);
            break;
        case AdaptableSimInstruction::OP_SUB:
            instSub(parameters, operands[0], operands[1], operands[2]);
            break;
        case AdaptableSimInstruction::OP_AND:
            instAnd(parameters, operands[0], operands[1], operands[2]);
            break;
        case AdaptableSimInstruction::OP_OR:
            instOr(parameters, operands[0], operands[1], operands[2]);
            break;
        case AdaptableSimInstruction::OP_XOR:
            instXor(parameters, operands[0], operands[1], operands[2]);
            break;
        case AdaptableSimInstruction::OP_CPL:
            instCpl(parameters, operands[0]);
            break;
        case AdaptableSimInstruction::OP_SHIFT_LEFT_0:
            instShiftLeft0(parameters, operands[0]);
            break;
        case AdaptableSimInstruction::OP_SHIFT_RIGHT_0:
            instShiftRight0(parameters, operands[0]);
            break;
        case AdaptableSimInstruction::OP_SHIFT_LEFT_1:
            instShiftLeft1(parameters, operands[0]);
            break;
        case AdaptableSimInstruction::OP_SHIFT_RIGHT_1:
            instShiftRight1(parameters, operands[0]);
            break;
        case AdaptableSimInstruction::OP_SHIFT_LEFT_R:
            instShiftLeftR(parameters, operands[0]);
            break;
        case AdaptableSimInstruction::OP_SHIFT_RIGHT_R:
            instShiftRightR(parameters, operands[0]);
            break;
        case AdaptableSimInstruction::OP_ROTATE_LEFT:
            instRotateLeft(parameters, operands[0]);
            break;
        case AdaptableSimInstruction::OP_ROTATE_RIGHT:
            instRotateRight(parameters, operands[0]);
            break;
        case AdaptableSimInstruction::OP_ROTATE_LEFT_C:
            instRotateLeftC(parameters, operands[0]);
            break;
        case AdaptableSimInstruction::OP_ROTATE_RIGHT_C:
            instRotateRightC(parameters, operands[0]);
            break;
    }

    for ( int  i = 0; i < 8; i++ )
    {
        switch ( parameters.flagAttr( AdaptableSimInstruction::OperParam::ProcFlag(i) ) )
        {
            case AdaptableSimInstruction::OperParam::P_SET:
                m_statusFlags->setFlag(AdaptableSimInstruction::OperParam::ProcFlag(i), true);
                break;
            case AdaptableSimInstruction::OperParam::P_CLEAR:
                m_statusFlags->setFlag(AdaptableSimInstruction::OperParam::ProcFlag(i), false);
                break;
            default:
                break;
        }
    }

    return true;
}

inline void AdaptableSimOperations::instAbsoluteJump(AdaptableSimInstruction::OperParam parameters,unsigned int opr0)
{
}

inline void AdaptableSimOperations::instAbsoluteCall(AdaptableSimInstruction::OperParam parameters,unsigned int opr0)
{
}

inline void AdaptableSimOperations::instRelativeJump(AdaptableSimInstruction::OperParam parameters,unsigned int opr0)
{
}

inline void AdaptableSimOperations::instRelativeCall(AdaptableSimInstruction::OperParam parameters,unsigned int opr0)
{
}

inline void AdaptableSimOperations::instOffsetJump(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1)
{
}

inline void AdaptableSimOperations::instOffsetCall(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1)
{
}

inline void AdaptableSimOperations::instIndexJump(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1,unsigned int opr2)
{
}

inline void AdaptableSimOperations::instIndexCall(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1,unsigned int opr2)
{
}

inline void AdaptableSimOperations::instReturn()
{
}

inline void AdaptableSimOperations::instBitTest(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1)
{

}

inline void AdaptableSimOperations::instIsrReturn()
{
}

inline void AdaptableSimOperations::instSetBank(unsigned int opr0)
{
}

inline void AdaptableSimOperations::instMove(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1)
{
}

inline void AdaptableSimOperations::instCbMove(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1)
{
}

inline void AdaptableSimOperations::instMoveBit(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1,unsigned int opr2)
{
}

inline void AdaptableSimOperations::instCbMoveBit(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1,unsigned int opr2)
{
}

inline void AdaptableSimOperations::instSwap (AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1)
{
}

inline void AdaptableSimOperations::instCbSwap (AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1)
{
}

inline void AdaptableSimOperations::instSwapBit (AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1,unsigned int opr2)
{
}

inline void AdaptableSimOperations::instCbSwapBit(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1,unsigned int opr2)
{
}

inline void AdaptableSimOperations::instAdd(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1,unsigned int opr2)
{
}

inline void AdaptableSimOperations::instSub(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1,unsigned int opr2)
{
}

inline void AdaptableSimOperations::instAnd(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1,unsigned int opr2)
{
}

inline void AdaptableSimOperations::instOr(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1,unsigned int opr2)
{
}

inline void AdaptableSimOperations::instXor(AdaptableSimInstruction::OperParam parameters,unsigned int opr0,unsigned int opr1,unsigned int opr2)
{
}

inline void AdaptableSimOperations::instCpl(AdaptableSimInstruction::OperParam parameters,unsigned int opr0)
{
}

inline void AdaptableSimOperations::instShiftLeft0(AdaptableSimInstruction::OperParam parameters,unsigned int opr0)
{
}

inline void AdaptableSimOperations::instShiftRight0(AdaptableSimInstruction::OperParam parameters,unsigned int opr0)
{
}

inline void AdaptableSimOperations::instShiftLeft1(AdaptableSimInstruction::OperParam parameters,unsigned int opr0)
{
}

inline void AdaptableSimOperations::instShiftRight1(AdaptableSimInstruction::OperParam parameters,unsigned int opr0)
{
}

inline void AdaptableSimOperations::instShiftLeftR(AdaptableSimInstruction::OperParam parameters,unsigned int opr0)
{
}

inline void AdaptableSimOperations::instShiftRightR(AdaptableSimInstruction::OperParam parameters,unsigned int opr0)
{
}

inline void AdaptableSimOperations::instRotateLeft(AdaptableSimInstruction::OperParam parameters,unsigned int opr0)
{
}

inline void AdaptableSimOperations::instRotateRight(AdaptableSimInstruction::OperParam parameters,unsigned int opr0)
{
}

inline void AdaptableSimOperations::instRotateLeftC(AdaptableSimInstruction::OperParam parameters,unsigned int opr0)
{
}

inline void AdaptableSimOperations::instRotateRightC(AdaptableSimInstruction::OperParam parameters,unsigned int opr0)
{
}
