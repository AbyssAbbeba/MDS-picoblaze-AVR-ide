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

#include "../../simulators/MCUSim/AdaptableSim/AdaptableSimOperationID.h"

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
                FLAG_HALF_CARRY,
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

            struct Next
            {
                int m_positive; ///< -1 means not set
                int m_negative; ///< -1 means not set
            };

            struct Time
            {
                unsigned int m_positive;
                unsigned int m_negative;
            };

            struct Condition
            {
                enum Type
                {
                    T_UNCONDITIONAL,
                    T_IF_ONE,
                    T_IF_ZERO
                } m_type;
                ProcessorFlag m_flag;
            };

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
                unsigned int m_number; /// Operand number as appears in source code line, starting with 0.
                std::vector<int> m_OPCodePermutation; ///< size is either 0, or consistent with m_size
            };

            enum FlagEffect
            {
                FE_DO_NOT_CHANGE,
                FE_ALWAYS_SET,
                FE_ALWAYS_CLEAR,
                FE_DEPENS_ON_RESULT
            };

            struct OperParameters
            {
                enum Result
                {
                    RES_STORE_IN_1ST,
                    RES_STORE_IN_3RD,
                    RES_DISCARD
                } m_result;

                enum Parity
                {
                    P_EVEN,
                    P_ODD
                } m_parity;

                bool m_ignoreCarryFlag;
            };

            Next m_next;
            Time m_time;
            FlagEffect m_flags[FLAG__MAX__];
            Condition m_condition;
            AdaptableSimOperationID::ID m_operation;
            OperParameters m_operParameters;
            std::string m_mnemonic;
            std::vector<OpCodeBit> m_opCode;
            std::vector<Operand> m_operands;
        };

        struct Memory
        {
            struct Register
            {
                unsigned int m_size;
                unsigned int m_banks; //< should divide m_size
            } m_register;

            struct Data
            {
                unsigned int m_size;
            } m_data;

            struct Program
            {
                unsigned int m_size;

                enum WordSize
                {
                    WORD_1B,
                    WORD_2B,
                    WORD_3B
                } m_word;

                enum Endian
                {
                    END_BIG,
                    END_LITTLE
                } m_endian;
            } m_program;
        };

        struct Stack
        {
            /// Stack capacity, set to 0 when simple stack is used or no stack is used at all.
            unsigned int m_size;

            /// Use stack as designated single purpose memory accessible only by specific instruction subset.
            bool m_useDesignatedStack;

            /**
             * @brief Settings related to simple stack, i.e. stack not implemented as designated single purpose memory.
             */
            struct SimpleStack
            {
                /// Target memory space.
                enum Space
                {
                    SP_REG, ///< Register file.
                    SP_DATA ///< Data memory.
                };

                /// Mode of stack operation.
                enum Operation
                {
                    OP_PREINC,  ///< Increment SP before PUSH, decrement after  POP.
                    OP_POSTINC, ///< Increment SP after  PUSH, decrement before POP.
                    OP_PREDEC,  ///< Decrement SP before PUSH, increment after  POP.
                    OP_POSTDEC  ///< Decrement SP after  PUSH, increment before POP.
                } m_operation;

                /// Location where the stacked values are stored.
                struct Content
                {
                    Space m_space; ///< Memory space.
                    int m_offset;  ///< This value is added to the stack pointer value before accessing stack memory.
                } m_content;

                /// Location designated for the stack pointer.
                struct Pointer
                {
                    Space m_space;   ///< Memory space.
                    int m_address;   ///< Address in the designated memory space.
                    int m_maxSize; ///< Maximum allowed stack pointer value, -1 means no limit.
                    bool m_indirect; ///< If true, "m_address" is threated as indirect address.
                } m_pointer;
            } m_simpleStack;
        };

        struct Ports
        {
            unsigned int m_addrWidth;
            unsigned int m_dataWidth;
        };

        struct Flags
        {
            bool m_backupWhenInterrupted;
            bool m_autoDisableInterrupts; // When when entering ISR.
            bool m_autoEnableInterrupts; // When when leaving ISR.

            bool m_hasZero;
            bool m_hasCarry;
            bool m_hasOverflow;
            bool m_hasNegative;
            bool m_hasHalfCarry;
            bool m_hasParity;
        };

    ////    Public Attributes    ////
    public:
        std::string m_name;
        unsigned int m_interruptVector;
        Ports m_ports;
        Memory m_memory;
        Stack m_stack;
        Flags m_flags;
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

    std::ostream & operator << ( std::ostream & out,
                                 const AdjSimProcDef::Ports & definition );

    std::ostream & operator << ( std::ostream & out,
                                 const AdjSimProcDef::Memory & definition );

    std::ostream & operator << ( std::ostream & out,
                                 const AdjSimProcDef::Stack & definition );

    std::ostream & operator << ( std::ostream & out,
                                 const AdjSimProcDef::Flags & definition );

    std::ostream & operator << ( std::ostream & out,
                                 const AdjSimProcDef::Instruction & definition );

    std::ostream & operator << ( std::ostream & out,
                                 const AdjSimProcDef::Stack::SimpleStack::Space & definition );

    std::ostream & operator << ( std::ostream & out,
                                 const AdjSimProcDef::Instruction::ProcessorFlag & flag );
//@}

#endif // ADJSIMPROCDEF_H
