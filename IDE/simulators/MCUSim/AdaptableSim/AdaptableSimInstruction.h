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

#include "AdaptableSimOperationID.h"

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
        struct FixedOperands
        {
            uint64_t m_data;

            bool use ( unsigned int operand ) const { return (bool) ( m_data >> ( 24 + operand ) ); }
            unsigned int value ( unsigned int operand ) const { return ( 0xffff & ( m_data >> ( 16 * operand )) ); }
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
                F_PARITY      = 6,
                F_FLAG        = 7
            };

            /**
             * @brief
             */
            enum AddressingMode
            {
                A_IMMEDIATE = 0,
                A_REG_DIR   = 1,
                A_REG_INDR  = 2,
                A_DATA_DIR  = 3,
                A_DATA_INDR = 4,
                A_PROGRAM   = 5,
                A_PORT      = 6
                // Max. (=upper bound) = 15
            };

            ///
            uint64_t m_data;

            bool nextN          () const { return bool ( 0b1 & (m_data >> 38) ); }
            bool nextP          () const { return bool ( 0b1 & (m_data >> 37) ); }
            bool discartResult  () const { return bool ( 0b1 & (m_data >> 36) ); }
            bool separateResult () const { return bool ( 0b1 & (m_data >> 35) ); }
            bool ignoreCarry    () const { return bool ( 0b1 & (m_data >> 34) ); }
            bool ignoreZero     () const { return bool ( 0b1 & (m_data >> 39) ); }
            bool evenParity     () const { return bool ( 0b1 & (m_data >> 33) ); }

            Condition condition () const
            {
                return Condition ( 0b11 & (m_data >> 31) );
            }

            ProcFlag conditionFlag () const
            {
                return ProcFlag ( 0b111 & (m_data >> 28) );
            }

            FlagAttr flagAttr ( ProcFlag flag ) const
            {
                return FlagAttr ( 0b11 & ( m_data >> ( 12 + 2 * int(flag) ) ) );
            }

            AddressingMode addressingMode ( unsigned int operand ) const
            {
                return AddressingMode ( 0b1111 & ( m_data >> ( operand * 4 ) ) );
            }
        };

    ////    Friends    ////
    friend std::ostream & operator << ( std::ostream & out,
                                        const AdaptableSimInstruction & instruction );

    friend std::ostream & operator << ( std::ostream & out,
                                        AdaptableSimInstruction::OperParam operParam );

    ////    Public Attributes    ////
    public:
        ///
        unsigned int m_code;

        ///
        unsigned int m_mask;

        ///
        unsigned short m_id;

        ///
        unsigned short m_timeP;

        ///
        unsigned short m_timeN;

        ///
        unsigned short m_nextP;

        ///
        unsigned short m_nextN;

        ///
        AdaptableSimOperationID::ID m_operation;

        ///
        OperParam m_parameters;

        ///
        FixedOperands m_fixedOperands;

        ///
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
