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
 * @ingroup AdjSimProcDef
 * @file AdjSimProcDef.h
 */
// =============================================================================

#ifndef ADJSIMPROCDEF_H
#define ADJSIMPROCDEF_H

// Standard header files.
#include <vector>
#include <string>
#include <iostream>

#include "simulators/MCUSim/AdaptableSim/AdaptableSimOperationID.h"

/**
 * @brief
 * @ingroup AdjSimProcDef
 * @class AdjSimProcDef
 */
class AdjSimProcDef
{
    ////    Friends    ////
    friend std::ostream & operator << ( std::ostream & out,
                                        const AdjSimProcDef & definition );

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct Instruction
        {
            enum ProcessorFlag
            {
                FLAG_ZERO,
                FLAG_CARRY,
                FLAG_OVERFLOW,
                FLAG_NEGATIVE,
                FLAG_HALFLAG_CARRY,
                FLAG_INTR_ENABLE,
                FLAG_PARITY,
                FLAG_FLAG,

                FLAG__MAX__
            };

            enum OpCodeBit
            {
                OCB_ZERO,
                OCB_ONE,
                OCB_DONT_CARE
            };

            std::string m_mnemonic;
            std::vector<OpCodeBit> m_opCode;

            struct Next
            {
                int m_positive; ///< -1 means not set
                int m_negative; ///< -1 means not set
            } m_next;

            struct Time
            {
                unsigned int m_positive;
                unsigned int m_negative;
            } m_time;

            AdaptableSimOperationID::ID m_operation;

            struct Condition
            {
                enum Type
                {
                    T_UNCONDITIONAL,
                    T_IF_ZERO,
                    T_IF_ONE
                } m_type;
                ProcessorFlag m_flag;
            } m_condition;

            struct Operand
            {
                enum Type
                {
                    T_IMMEDIATE,
                    T_REG_DIR,
                    T_REG_INDR,
                    T_DATA_DIR,
                    T_DATA_INDR,
                    T_PROGRAM,
                    T_PORT
                } m_type;

                int m_size; ///< -1 means operand is not present
                int m_fixedValue; /// -1 means do not use fixed value and use permutation instead
                std::vector<int> m_OPCodePermutation; ///< size is either 0, or consistent with m_size
            };

            enum FlagEffect
            {
                FE_DO_NOT_CHANGE,
                FE_ALWAYS_SET,
                FE_ALWAYS_CLEAR,
                FE_DEPENS_ON_RESULT
            } m_flags[FLAG__MAX__];

            struct OperParameters
            {
                enum Result
                {
                    RES_STORE_IN_1ST,
                    RES_STORE_IN_3RD,
                    RES_DISCART
                } m_result;

                enum Parity
                {
                    P_EVEN,
                    P_ODD
                } m_parity;

                bool m_ignoreCarryFlag;
            } m_operParameters;

            std::vector<Operand> m_operands;
        };

    ////    Public Attributes    ////
    public:
        std::string m_name;
        std::vector<Instruction> m_instructionSet;
};

/// @name Tracing operators
//@{
    /**
     * @brief
     * @param[in,out] out
     * @param[in] definition
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const AdjSimProcDef & definition );
//@}

#endif // ADJSIMPROCDEF_H
