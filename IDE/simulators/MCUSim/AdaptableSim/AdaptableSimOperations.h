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

#include "AdaptableSimInstruction.h"
#include "AdaptableSimInstructionSet.h"

class AdaptableSimOperations : public AdaptableSimInstructionSet
{

    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        enum LogOperation
        {
            LOG_AND, ///<
            LOG_OR,  ///<
            LOG_XOR  ///<
        };

        /**
         * @brief
         */
        enum ShiftRotateOper
        {
            SRO_SHL0,   ///<
            SRO_SHL1,   ///<
            SRO_SHR0,   ///<
            SRO_SHR1,   ///<
            SRO_SHLR,   ///<
            SRO_SHRR,   ///<
            SRO_SHLC,   ///<
            SRO_SHRC,   ///<
            SRO_RL,     ///<
            SRO_RR      ///<
        };

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] operation
         * @param[in] operands
         * @param[in] inst
         * @return
         */
        virtual bool operationSwitch ( AdaptableSimOperationID::ID operation,
                                       unsigned int operands [ AdaptableSimInstruction::OPERANDS_MAX ],
                                       const AdaptableSimInstruction & inst ) final override;

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] addrVal
         * @param[in] addrMode
         * @param[in] jump
         * @return
         */
        unsigned int getValue ( unsigned int addrVal,
                                AdaptableSimInstruction::OperParam::AddressingMode addrMode,
                                bool jump = false );

        /**
         * @brief
         * @param[in] destination
         * @param[in] addrMode
         * @param[in] value
         * @return
         */
        void setValue ( unsigned int destination,
                        AdaptableSimInstruction::OperParam::AddressingMode addrMode,
                        unsigned int value );

        /**
         * @brief
         * @param[in] source
         * @param[in] parameters
         * @param[in] permutation
         * @return
         */
        unsigned int setFlagsAndTrim ( unsigned int source,
                                       AdaptableSimInstruction::OperParam parameters,
                                       const std::vector<unsigned char> & permutation );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] permutation
         * @return
         */
        inline unsigned int getSize ( const std::vector<unsigned char> & permutation );

        /**
         * @brief
         * @param[in] value
         * @param[in] size
         * @return
         */
        inline int toSigned ( unsigned int value, unsigned int size );

    ////    Instruction Operations - Inline Private Operations    ////
    private:
        /// @name Control Flow Operations
        //@{
            /**
             * @brief
             * @param[in] parameters
             * @param[in] address
             */
            inline void instAbsoluteJump ( AdaptableSimInstruction::OperParam parameters,
                                           unsigned int address );

            /**
             * @brief
             * @param[in] parameters
             * @param[in] address
             */
            inline void instAbsoluteCall ( AdaptableSimInstruction::OperParam parameters,
                                           unsigned int address );

            /**
             * @brief
             * @param[in] parameters
             * @param[in] address
             * @param[in] permutation
             */
            inline void instRelativeJump ( AdaptableSimInstruction::OperParam parameters,
                                           unsigned int address,
                                           const std::vector<unsigned char> & permutation );

            /**
             * @brief
             * @param[in] parameters
             * @param[in] address
             * @param[in] permutation
             */
            inline void instRelativeCall ( AdaptableSimInstruction::OperParam parameters,
                                           unsigned int address,
                                           const std::vector<unsigned char> & permutation );

            /**
             * @brief
             * @param[in] parameters
             * @param[in] address
             * @param[in] offset
             */
            inline void instOffsetJump ( AdaptableSimInstruction::OperParam parameters,
                                         unsigned int address,
                                         unsigned int offset );

            /**
             * @brief
             * @param[in] parameters
             * @param[in] address
             * @param[in] offset
             */
            inline void instOffsetCall ( AdaptableSimInstruction::OperParam parameters,
                                         unsigned int address,
                                         unsigned int offset );

            /**
             * @brief
             * @param[in] parameters
             * @param[in] address
             * @param[in] index
             * @param[in] idxShift
             */
            inline void instIndexJump ( AdaptableSimInstruction::OperParam parameters,
                                        unsigned int address,
                                        unsigned int index,
                                        unsigned int idxShift );

            /**
             * @brief
             * @param[in] parameters
             * @param[in] address
             * @param[in] index
             * @param[in] idxShift
             */
            inline void instIndexCall ( AdaptableSimInstruction::OperParam parameters,
                                        unsigned int address,
                                        unsigned int index,
                                        unsigned int idxShift );

            /**
             * @brief
             * @param[in] parameters
             * @param[in] value
             * @param[in] bitNumber
             * @return
             */
            inline bool instBitTest ( AdaptableSimInstruction::OperParam parameters,
                                      unsigned int value,
                                      unsigned int bitNumber );

            /**
             * @brief
             */
            inline void instReturn();
        //@}

        /// @name Data Flow Operations
        //@{
            /**
             * @brief
             * @param[in] inst
             * @param[in] destination
             * @param[in] source
             * @return
             */
            inline void instMove ( const AdaptableSimInstruction & inst,
                                   unsigned int destination,
                                   unsigned int source );

            /**
             * @brief
             * @param[in] inst
             * @param[in] destination
             * @param[in] source
             * @return
             */
            inline void instCbMove ( const AdaptableSimInstruction & inst,
                                     unsigned int destination,
                                     unsigned int source );

            /**
             * @brief
             * @param[in] inst
             * @param[in] destination
             * @param[in] source
             * @return
             */
            inline void instSwap ( const AdaptableSimInstruction & inst,
                                   unsigned int destination,
                                   unsigned int source );

            /**
             * @brief
             * @param[in] inst
             * @param[in] destination
             * @param[in] source
             * @return
             */
            inline void instCbSwap ( const AdaptableSimInstruction & inst,
                                     unsigned int destination,
                                     unsigned int source );

            /**
             * @brief
             * @param[in] inst
             * @param[in] destination
             * @param[in] source
             * @param[in] bitNumbers [7:4] - destination bit, [3:0] - source bit
             * @return
             */
            inline void instCbMoveBit ( AdaptableSimInstruction::OperParam parameters,
                                        unsigned int destination,
                                        unsigned int source,
                                        unsigned int bitNumbers );

            /**
             * @brief
             * @param[in] inst
             * @param[in] destination
             * @param[in] source
             * @param[in] bitNumbers [7:4] - destination bit, [3:0] - source bit
             * @return
             */
            inline void instMoveBit ( AdaptableSimInstruction::OperParam parameters,
                                      unsigned int destination,
                                      unsigned int source,
                                      unsigned int bitNumbers );

            /**
             * @brief
             * @param[in] inst
             * @param[in] destination
             * @param[in] source
             * @param[in] bitNumbers [7:4] - destination bit, [3:0] - source bit
             * @return
             */
            inline void instSwapBit ( AdaptableSimInstruction::OperParam parameters,
                                      unsigned int destination,
                                      unsigned int source,
                                      unsigned int bitNumbers );

            /**
             * @brief
             * @param[in] inst
             * @param[in] destination
             * @param[in] source
             * @param[in] bitNumbers [7:4] - destination bit, [3:0] - source bit
             * @return
             */
            inline void instCbSwapBit ( AdaptableSimInstruction::OperParam parameters,
                                        unsigned int destination,
                                        unsigned int source,
                                        unsigned int bitNumbers );
        //@}

        /// @name Interrupt Request and Service Operations
        //@{
            /**
             * @brief
             * @return
             */
            inline void instIsrReturn();
        //@}

        /// @name Arithmetic Operations
        //@{
            /**
             * @brief
             * @param[in] inst
             * @param[in] opr0
             * @param[in] opr1
             * @param[in] opr2
             * @param[in] subtract
             * @return
             */
            inline void instAdd ( const AdaptableSimInstruction & inst,
                                  unsigned int opr0,
                                  unsigned int opr1,
                                  unsigned int opr2,
                                  bool subtract = false );
        //@}

        /// @name Logical Operations
        //@{
            /**
             * @brief
             * @param[in] inst
             * @param[in] opr0
             * @param[in] opr1
             * @param[in] opr2
             * @param[in] operation
             */
            inline void instLogOper ( const AdaptableSimInstruction & inst,
                                      unsigned int opr0,
                                      unsigned int opr1,
                                      unsigned int opr2,
                                      LogOperation operation );

            /**
             * @brief
             * @param[in] inst
             * @param[in] operand
             */
            inline void instCpl ( const AdaptableSimInstruction & inst,
                                  unsigned int operand );
        //@}

        /// @name Shift and Rotate Operations
        //@{
            /**
             * @brief
             * @param[in] inst
             * @param[in] opr0
             * @param[in] opr1
             * @param[in] opr2
             * @param[in] operation
             */
            inline void instShiftRotateOper ( const AdaptableSimInstruction & inst,
                                              unsigned int opr0,
                                              unsigned int opr1,
                                              unsigned int opr2,
                                              ShiftRotateOper operation );
        //@}

        /// @name System Control Operations
        //@{
            /**
             * @brief
             * @param[in] bank
             * @return
             */
            inline void instSetBank ( unsigned int bank );
        //@}
};

#endif // ADAPTABLESIMOPERATIONS_H
