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
 * @file AdaptableSimInstruction.h
 */
// =============================================================================

#ifndef ADAPTABLESIMINSTRUCTION_H
#define ADAPTABLESIMINSTRUCTION_H

// Standard header files.
#include <vector>
#include <cstdint>
#include <ostream>

/**
 * @brief
 * @ingroup AdaptableSim
 * @class AdaptableSimInstruction
 */
class AdaptableSimInstruction
{
    ////    Public Static Constants    ////
    public:
        /// @brief
        static const unsigned int OPERANDS_MAX = 3;

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum OperationID
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
            OP_ROTATE_LEFT,     ///<
            OP_ROTATE_RIGHT,    ///<
            OP_ROTATE_LEFT_C,   ///<
            OP_ROTATE_RIGHT_C   ///<
        };

        /**
         * @brief
         */
        struct FixedOperands
        {
            uint64_t m_data;

            bool use ( unsigned int operand ) const { return (bool) ( m_data >> ( 24 + operand ) ); }
            unsigned int value ( unsigned int operand ) const { return ( 0xf & ( m_data >> ( 16 * operand )) ); }
        };

        /**
         * @brief
         */
        struct OperParam
        {
            /**
             * @brief
             */
            enum Condition
            {
                C_NONE     = 0,
                C_POSITIVE = 1,
                C_NEGATIVE = 2
            };

            /**
             * @brief
             */
            enum FlagAttr
            {
                P_KEEP   = 0,
                P_SET    = 1,
                P_CLEAR  = 2,
                P_AFFECT = 3
            };

            /**
             * @brief
             */
            enum ProcFlag
            {
                F_ZERO        = 0,
                F_CARRY       = 1,
                F_OVERFLOW    = 2,
                F_NEGATIVE    = 3,
                F_HALF_CARRY  = 4,
                F_INTR_ENABLE = 5,
                F_FLAG_0      = 6,
                F_FLAG_1      = 7
            };

            /**
             * @brief
             */
            enum AddressingMode
            {
                A_REG_DIR   = 0,
                A_REG_INDR  = 1,
                A_DATA_DIR  = 2,
                A_DATA_INDR = 3,
                A_PROGRAM   = 4,
                A_PORT      = 5,
                // Max. (=upper bound) = 15
            };

            ///
            uint64_t m_data;

            int oprSize ( unsigned int operand ) const { return (int) ( 0b1111 & (m_data >> ( 39 + 4 * operand )) ); }

            bool nextN          () const { return bool ( 0b1 & (m_data >> 38) ); }
            bool nextP          () const { return bool ( 0b1 & (m_data >> 37) ); }
            bool discartResult  () const { return bool ( 0b1 & (m_data >> 36) ); }
            bool separateResult () const { return bool ( 0b1 & (m_data >> 35) ); }
            bool ignoreCarry    () const { return bool ( 0b1 & (m_data >> 34) ); }
            bool evenParity     () const { return bool ( 0b1 & (m_data >> 33) ); }

            Condition condition () const
            {
                return Condition ( 0b11 & (m_data >> 31) );
            }

            ProcFlag conditionFlag () const
            {
                return ProcFlag ( 0b111 & (m_data >> 28) );
            }

            FlagAttr flagAttr ( ProcFlag flag )
            {
                return FlagAttr ( 0b11 & ( m_data >> ( 12 + 2 * int(flag) ) ) );
            }

            AddressingMode addressingMode ( unsigned int operand )
            {
                return AddressingMode ( 0b1111 & ( m_data >> ( operand * 4 ) ) );
            }
        };

    ////    Friends    ////
    friend std::ostream & operator << ( std::ostream & out,
                                        const AdaptableSimInstruction & instruction );

    friend std::ostream & operator << ( std::ostream & out,
                                        AdaptableSimInstruction::OperationID operationID );

    friend std::ostream & operator << ( std::ostream & out,
                                        AdaptableSimInstruction::OperParam operParam );

    ////    Public Attributes    ////
    public:
        unsigned int m_code;
        unsigned int m_mask;
        unsigned short m_id;
        unsigned short m_timeP;
        unsigned short m_timeN;
        unsigned short m_nextP;
        unsigned short m_nextN;

        OperationID m_operation;
        OperParam m_parameters;
        FixedOperands m_fixedOperands;

        std::vector<unsigned char> m_permutation [ OPERANDS_MAX ];
};

/// @name Tracing operators
//@{
    /**
     * @brief
     * @param[in,out] out
     * @param[in] instruction
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const AdaptableSimInstruction & instruction );

    /**
     * @brief
     * @param[in,out] out
     * @param[in] operationID
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 AdaptableSimInstruction::OperationID operationID );

    /**
     * @brief
     * @param[in,out] out
     * @param[in] operParam
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 AdaptableSimInstruction::OperParam operParam );

    /**
     * @brief
     * @param[in,out] out
     * @param[in] condition
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 AdaptableSimInstruction::OperParam::Condition condition );

    /**
     * @brief
     * @param[in,out] out
     * @param[in] attribute
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 AdaptableSimInstruction::OperParam::FlagAttr attribute );

    /**
     * @brief
     * @param[in,out] out
     * @param[in] flag
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 AdaptableSimInstruction::OperParam::ProcFlag flag );

    /**
     * @brief
     * @param[in,out] out
     * @param[in] mode
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 AdaptableSimInstruction::OperParam::AddressingMode mode );

    /**
     * @brief
     * @param[in,out] out
     * @param[in] fixedOperands
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 AdaptableSimInstruction::FixedOperands fixedOperands );
//@}

#endif // ADAPTABLESIMINSTRUCTION_H