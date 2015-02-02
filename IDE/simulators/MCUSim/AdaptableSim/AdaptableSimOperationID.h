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
 * @file AdaptableSimOperationID.h
 */
// =============================================================================

#ifndef ADAPTABLESIMOPERATIONID_H
#define ADAPTABLESIMOPERATIONID_H

// Standard header files.
#include <ostream>

/**
 * @brief
 * @ingroup AdaptableSim
 * @namespace AdaptableSimOperationID
 */
namespace AdaptableSimOperationID
{
    ////    Datatypes    ////

    /**
     * @brief
     */
    enum ID
    {
        OP_NONE,            ///<

        OP_ABS_JUMP,        ///<
        OP_ABS_CALL,        ///<
        OP_REL_JUMP,        ///<
        OP_REL_CALL,        ///<
        OP_OFS_JUMP,        ///<
        OP_OFS_CALL,        ///<
        OP_IDX_JUMP,        ///<
        OP_IDX_CALL,        ///<
        OP_RETURN,          ///<
        OP_ISR_RETURN,      ///<
        OP_SET_BANK,        ///<
        OP_MOVE,            ///<
        OP_CB_MOVE,         ///<
        OP_MOVE_BIT,        ///<
        OP_CB_MOVE_BIT,     ///<
        OP_SWAP,            ///<
        OP_CB_SWAP,         ///<
        OP_SWAP_BIT,        ///<
        OP_CB_SWAP_BIT,     ///<
        OP_CPL,             ///<
        OP_BIT_TEST,        ///<
        OP_ADD,             ///<
        OP_SUB,             ///<
        OP_AND,             ///<
        OP_OR,              ///<
        OP_XOR,             ///<
        OP_SHIFT_LEFT_0,    ///<
        OP_SHIFT_RIGHT_0,   ///<
        OP_SHIFT_LEFT_1,    ///<
        OP_SHIFT_RIGHT_1,   ///<
        OP_SHIFT_LEFT_R,    ///<
        OP_SHIFT_RIGHT_R,   ///<
        OP_SHIFT_LEFT_C,    ///<
        OP_SHIFT_RIGHT_C,   ///<
        OP_ROTATE_LEFT,     ///<
        OP_ROTATE_RIGHT,    ///<
    };
}

/// @name Tracing operators
//@{
    /**
     * @brief
     * @param[in,out] out
     * @param[in] operationID
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 AdaptableSimOperationID::ID operationID );
//@}

#endif // ADAPTABLESIMOPERATIONID_H
