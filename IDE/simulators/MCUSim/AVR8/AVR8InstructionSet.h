// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup AVR8
 * @file AVR8InstructionSet.h
 */
// =============================================================================

#ifndef AVR8INSTRUCTIONSET_H
#define AVR8INSTRUCTIONSET_H

// Forward declarations
class AVR8ProgramMemory;
class AVR8DataMemory;
class AVR8DES;
class AVR8FusesAndLocks;
class AVR8InterruptController;
class AVR8SystemControl;
class AVR8BootLoader;

#include "../MCUSim.h"
#include "AVR8Sim.h"
#include "AVR8InsNames.h"

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8InstructionSet
 */
class AVR8InstructionSet : public MCUSim::CPU
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum PCWidth
        {
            // .. lower values ...
            PCWIDTH_16 = 1,     ///<

            // .. higher values ...
            PCWIDTH_22          ///<
        };

        /**
         * @brief
         */
        struct Config
        {
            /// @brief
            Config()
            {
                m_ignoreUndefinedOpCodes = true;
            }

            ///
            bool m_availableInstructions [ AVR8InsNames::SPECI__MAX__ ];

            ///
            MCUSim::Family m_family;

            ///
            PCWidth m_pcWidth;

            ///
            bool m_ignoreUndefinedOpCodes;

            ///
            int m_pcMax;
        };

    ////    Static Public Constants    ////
    public:
        /**
         * @brief
         */
        static int ( AVR8InstructionSet :: * const m_opCodeDispatchTable [ 64 ] ) ( const unsigned int opCode );

        /**
         * @brief
         */
        static int ( AVR8InstructionSet :: * const m_opCodeDispatchTable_100100 [ 32 ] ) ( const unsigned int opCode );

        /**
         * @brief
         */
        static int ( AVR8InstructionSet :: * const m_opCodeDispatchTable_1001_010x_xxxx_100x [ 64 ] ) ( const unsigned int opCode );

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        AVR8InstructionSet() {};

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        unsigned int getProgramCounter() const
        {
            return (unsigned int)m_pc;
        }

        /**
         * @brief
         * @param
         * @return
         */
        void setProgramCounter ( unsigned int newPc )
        {
            m_pc = int(newPc);
        }

        /**
         * @brief
         * Helper function for instruction incorporating hardware multiplier.
         *
         * It works with signed and unsigned integers and fractional numbers.
         * In this context, an example of unsigned 8b fractional number might be 10110010₂ which is:
         * = 1×2⁻⁰ + 0×2⁻¹ + 1×2⁻²  + 1×2⁻³   + 0×2⁻⁴    + 0×2⁻⁵     + 1×2⁻⁶      + 0×2⁻⁷      =
         * = 1×1   + 0×0.5 + 1×0.25 + 1×0.125 + 0×0.0625 + 0×0.03125 + 1×0.015625 + 0×0.078125 =
         * = 1.390625₁₀.
         * If it was signed, it would be 1.390625₁₀ - 2₁₀ = -0.609375₁₀.
         *
         * @tparam rdSigned
         * @tparam rrSigned
         * @tparam fractional
         *
         * @param[in] addrRd
         * @param[in] addrRr
         * @return
         */
        template < bool rdSigned, bool rrSigned, bool fractional >
            inline void instMul ( const unsigned int addrRd,
                                  const unsigned int addrRr );

        /**
         * @brief
         *
         * @tparam instruction
         * @tparam pointerRegH
         * @tparam pointerRegL
         * @tparam preDecrement
         * @tparam postIncrement
         *
         * @param[in] addrRd
         * @param[in] displacement
         * @return
         */
        template < int instruction, unsigned int pointerRegH, unsigned int pointerRegL, bool preDecrement, bool postIncrement >
            inline int instLD ( unsigned int addrRd,
                                int displacement = 0 );

        /**
         * @brief
         *
         * @tparam instruction
         * @tparam pointerRegH
         * @tparam pointerRegL
         * @tparam preDecrement
         * @tparam postIncrement
         *
         * @param[in] addrRr
         * @param[in] displacement
         * @return
         */
        template < int instruction, unsigned int pointerRegH, unsigned int pointerRegL, bool preDecrement, bool postIncrement >
            inline int instST ( unsigned int addrRr,
                                int displacement = 0 );

        /**
         * @brief
         * @tparam bitMask
         * @return
         */
        template < int bitMask >
            inline int instSregSetBit();

        /**
         * @brief
         * @tparam bitMask
         * @return
         */
        template < int bitMask >
            inline int instSregClearBit();

        /**
         * @brief
         *
         * @tparam instruction
         * @tparam destR0
         * @tparam postIncrement
         * @tparam extendedLoad
         *
         * @param[in] opCode
         * @return
         */
        template < int instruction, bool destR0 = false, bool postIncrement = false, bool extendedLoad = false >
            inline int instLoadProgMemory ( const unsigned int opCode );

        /**
         * @brief
         *
         * @tparam withCarry:
         *
         * @param[in] leftVal
         * @param[in] rightVal
         * @return
         */
        template < bool withCarry >
            inline unsigned int instSub8b ( unsigned int leftVal,
                                            unsigned int rightVal );

        /**
         * @brief
         *
         * @tparam instruction
         * @tparam store
         *
         * @param[in] opCode
         * @return
         */
        template < int instruction, bool store >
            inline int instLoadOrStore16b ( const unsigned int opCode );

        /**
         * @brief
         *
         * @tparam instruction
         * @tparam store
         *
         * @param[in] opCode
         * @return
         */
        template < int instruction, bool store >
            inline int instLoadOrStore32b ( const unsigned int opCode );

    ////    Public Operations and Inline Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] programMemory
         * @param[in,out] dataMemory
         * @param[in,out] processorMode
         * @param[in,out] sleepMode
         * @param[in,out] fusesAndLocks
         * @param[in,out] interruptController
         * @param[in,out] systemControl
         * @param[in,out] haltMode
         * @param[in,out] bootLoader
         * @return
         */
        AVR8InstructionSet * link ( MCUSim::EventLogger     * eventLogger,
                                    AVR8ProgramMemory       * programMemory,
                                    AVR8DataMemory          * dataMemory,
                                    MCUSim::Mode            * processorMode,
                                    AVR8Sim::SleepMode      * sleepMode,
                                    AVR8FusesAndLocks       * fusesAndLocks,
                                    AVR8InterruptController * interruptController,
                                    AVR8SystemControl       * systemControl,
                                    AVR8Sim::HaltMode       * haltMode,
                                    AVR8BootLoader          * bootLoader );

        /**
         * @brief
         * @return
         */
        int execInstruction();

        /**
         * @brief
         * @param[in] mode
         * @return
         */
        void reset ( MCUSim::ResetMode mode );

        /// @name Processor Instructions
        //@{
            /**
             * @brief
             *
             * Possible instuctions:
             * NOP                  0000 0000 0000 0000 - No Operation
             * MOVW Rd+1:Rd,Rr+1:Rr 0000 0001 dddd rrrr - Copy Register Word
             * MULS Rd, Rr          0000 0010 dddd rrrr - Multiply Signed
             * MULSU Rd, Rr         0000 0011 0ddd 0rrr - Multiply Signed with Unsigned
             * FMUL Rd, Rr          0000 0011 0ddd 1rrr - Fractional Multiply Unsigned
             * FMULS Rd, Rr         0000 0011 1ddd 0rrr - Fractional Multiply Signed
             * FMULSU Rd, Rr        0000 0011 1ddd 1rrr - Fractional Multiply Signed with Unsigned
             *
             * @param[in] opCode
             * @return
             */
            int instOPCode_000000 ( const unsigned int opCode );

            /**
             * @brief
             *
             * Possible instuctions:
             * LDS Rd, k            1010 0kkk dddd kkkk - Load Direct from Data Space
             * STS k, Rr            1010 1kkk dddd kkkk - Store Direct to Data Space
             * STD Z+q, Rr          10q0 qq1r rrrr 0qqq - Store Indirect From Register to Data Space using Index Z
             * ST Z, Rr             1000 001r rrrr 0000 - Store Indirect From Register to Data Space using Index Z
             * STD Y+q, Rr          10q0 qq1r rrrr 1qqq - Store Indirect From Register to Data Space using Index Y
             * ST Y, Rr             1000 001r rrrr 1000 - Store Indirect From Register to Data Space using Index Y
             * LDD Rd, Z+q          10q0 qq0d dddd 0qqq - Load Indirect From Data Space to Register using Index Z
             * LD Rd, Z             1000 000d dddd 0000 - Load Indirect From Data Space to Register using Index Z
             * LDD Rd, Y+q          10q0 qq0d dddd 1qqq - Load Indirect from Data Space to Register using Index Y
             * LD Rd, Y             1000 000d dddd 1000 - Load Indirect from Data Space to Register using Index Y
             *
             * @param[in] opCode
             * @return
             */
            int instOPCode_10x0xx ( const unsigned int opCode );

            /**
             * @brief
             *
             * Possible instuctions:
             * LDS Rd, k            1001 000d dddd 0000 \
             *                      kkkk kkkk kkkk kkkk - Load Direct from Data Space
             * LD Rd, Z+            1001 000d dddd 0001 - Load Indirect From Data Space to Register using Index Z
             * LD Rd, -Z            1001 000d dddd 0010 - Load Indirect From Data Space to Register using Index Z
             * LPM Rd, Z            1001 000d dddd 0100 - Load Program Memory
             * LPM Rd, Z+           1001 000d dddd 0101 - Load Program Memory
             * ELPM Rd, Z           1001 000d dddd 0110 - Extended Load Program Memory
             * ELPM Rd, Z+          1001 000d dddd 0111 - Extended Load Program Memory
             * LD Rd, Y+            1001 000d dddd 1001 - Load Indirect from Data Space to Register using Index Y
             * LD Rd, -Y            1001 000d dddd 1010 - Load Indirect from Data Space to Register using Index Y
             * LD Rd, X             1001 000d dddd 1100 - Load Indirect from Data Space to Register using Index X
             * LD Rd, X+            1001 000d dddd 1101 - Load Indirect from Data Space to Register using Index X
             * LD Rd, -X            1001 000d dddd 1110 - Load Indirect from Data Space to Register using Index X
             * POP Rd               1001 000d dddd 1111 - Pop Register from Stack
             * STS k, Rr            1001 001d dddd 0000 \
             *                      kkkk kkkk kkkk kkkk - Store Direct to Data Space
             * ST Z+, Rr            1001 001r rrrr 0001 - Store Indirect From Register to Data Space using Index Z
             * ST -Z, Rr            1001 001r rrrr 0010 - Store Indirect From Register to Data Space using Index Z
             * XCH Z, Rd            1001 001r rrrr 0100 - Exchange
             * LAS Z, Rd            1001 001r rrrr 0101 - Load And Set
             * LAC Z, Rd            1001 001r rrrr 0110 - Load And Clear
             * LAT Z, Rd            1001 001r rrrr 0111 - Load And Toggle
             * ST Y+, Rr            1001 001r rrrr 1001 - Store Indirect From Register to Data Space using Index Y
             * ST -Y, Rr            1001 001r rrrr 1010 - Store Indirect From Register to Data Space using Index Y
             * ST X, Rr             1001 001r rrrr 1100 - Store Indirect From Register to Data Space using Index X
             * ST X+, Rr            1001 001r rrrr 1101 - Store Indirect From Register to Data Space using Index X
             * ST -X, Rr            1001 001r rrrr 1110 - Store Indirect From Register to Data Space using Index X
             * PUSH Rr              1001 001d dddd 1111 - Push Register on Stack
             *
             * @param[in] opCode
             * @return
             */
            int instOPCode_100100 ( const unsigned int opCode );

            /**
             * @brief
             *
             * Possible instuctions:
             * ADIW Rd+1:Rd, K      1001 0110 KKdd KKKK - Add Immediate to Word
             * SBIW Rd+1:Rd, K      1001 0111 KKdd KKKK - Subtract Immediate from Word
             * JMP k                1001 010k kkkk 110k \
             *                      kkkk kkkk kkkk kkkk - Jump
             * CALL k               1001 010k kkkk 111k \
             *                      kkkk kkkk kkkk kkkk - Long Call to a Subroutine
             * DEC Rd               1001 010d dddd 1010 - Decrement
             * DES K                1001 0100 KKKK 1011 - Data Encryption Standard
             * ASR Rd               1001 010d dddd 0101 - Arithmetic Shift Right
             * LSR Rd               1001 010d dddd 0110 - Logical Shift Right
             * ROR Rd               1001 010d dddd 0111 - Rotate Right through Carry
             * COM Rd               1001 010d dddd 0000 - One’s Complement
             * NEG Rd               1001 010d dddd 0001 - Two’s Complement
             * SWAP Rd              1001 010d dddd 0010 - Swap Nibbles
             * INC Rd               1001 010d dddd 0011 - Increment
             *
             * And all instructions mentioned in m_opCodeDispatchTable_1001_010x_xxxx_100x dispatch table.
             *
             * @param[in] opCode
             * @return
             */
            int instOPCode_100101 ( const unsigned int opCode );

            /**
             * @brief
             *
             * Possible instuctions:
             * CBI A, b             1001 1000 AAAA Abbb - Clear Bit in I/O Register
             * SBIC A, b            1001 1001 AAAA Abbb - Skip if Bit in I/O Register is Cleared
             * SBI A, b             1001 1010 AAAA Abbb - Set Bit in I/O Register
             * SBIS A, b            1001 1011 AAAA Abbb - Skip if Bit in I/O Register is Set
             *
             * @param[in] opCode
             * @return
             */
            int instOPCode_100110 ( const unsigned int opCode );

            /**
             * @brief
             *
             * Possible instuctions:
             * BLD Rd, b            1111 100d dddd 0bbb - Bit Load from the T Flag in SREG to a Bit in Register
             * BST Rd, b            1111 101d dddd 0bbb - Bit Store from Bit in Register to T Flag in SREG
             *
             * @param[in] opCode
             * @return
             */
            int instOPCode_111110 ( const unsigned int opCode );

            /**
             * @brief
             *
             * Possible instuctions:
             * SBRC Rr, b           1111 110r rrrr 0bbb - Skip if Bit in Register is Cleared
             * SBRS Rr, b           1111 111r rrrr 0bbb - Skip if Bit in Register is Set
             *
             * @param[in] opCode
             * @return
             */
            int instOPCode_111111 ( const unsigned int opCode );

            /**
             * @brief Invalid instruction.
             * @param[in] opCode
             * @return
             *
             * @warning Invocation of this method is an error condition.
             */
            int instInvalid ( const unsigned int opCode );

            /**
             * @brief
             *
             * ADD Rd, Rr           0000 11rd dddd rrrr - Add without Carry
             * LSL Rd               0000 11dd dddd dddd - Logical Shift Left
             *
             * @param[in] opCode
             * @return
             */
            int inst_ADD_Rd_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * SBC Rd, Rr           0000 10rd dddd rrrr - Subtract with Carry
             *
             * @param[in] opCode
             * @return
             */
            int inst_SBC_Rd_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * CPC Rd, Rr           0000 01rd dddd rrrr - Compare with Carry
             *
             * @param[in] opCode
             * @return
             */
            int inst_CPC_Rd_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * CPSE Rd, Rr          0001 00rd dddd rrrr - Compare Skip if Equal
             *
             * @param[in] opCode
             * @return
             */
            int inst_CPSE_Rd_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * CP Rd, Rr            0001 01rd dddd rrrr - Compare
             *
             * @param[in] opCode
             * @return
             */
            int inst_CP_Rd_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * SUB Rd, Rr           0001 10rd dddd rrrr - Subtract without Carry
             *
             * @param[in] opCode
             * @return
             */
            int inst_SUB_Rd_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * ADC Rd, Rr           0001 11rd dddd rrrr - Add with Carry
             * ROL Rd               0001 11dd dddd dddd - Rotate Left trough Carry
             *
             * @param[in] opCode
             * @return
             */
            int inst_ADC_Rd_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * AND Rd, Rr           0010 00rd dddd rrrr - Logical AND
             * TST Rd               0010 00dd dddd dddd - Test for Zero or Minus
             *
             * @param[in] opCode
             * @return
             */
            int inst_AND_Rd_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * EOR Rd, Rr           0010 01rd dddd rrrr - Exclusive OR
             * CLR Rd               0010 01dd dddd dddd - Clear Register
             *
             * @param[in] opCode
             * @return
             */
            int inst_EOR_Rd_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * OR Rd, Rr            0010 10rd dddd rrrr - Logical OR
             *
             * @param[in] opCode
             * @return
             */
            int inst_OR_Rd_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * MOV Rd, Rr           0010 11rd dddd rrrr - Copy Register
             *
             * @param[in] opCode
             * @return
             */
            int inst_MOV_Rd_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * CPI Rd, K            0011 KKKK dddd KKKK - Compare with Immediate
             *
             * @param[in] opCode
             * @return
             */
            int inst_CPI_Rd_K ( const unsigned int opCode );

            /**
             * @brief
             *
             * SBCI Rd, K           0100 KKKK dddd KKKK - Subtract Immediate with Carry
             *
             * @param[in] opCode
             * @return
             */
            int inst_SBCI_Rd_K ( const unsigned int opCode );

            /**
             * @brief
             *
             * SUBI Rd, K           0101 KKKK dddd KKKK - Subtract Immediate
             *
             * @param[in] opCode
             * @return
             */
            int inst_SUBI_Rd_K ( const unsigned int opCode );

            /**
             * @brief
             *
             * ORI Rd, K            0110 KKKK dddd KKKK - Logical OR with Immediatee
             * SBR Rd, K            0110 KKKK dddd KKKK - Set Bits in Register
             *
             * @param[in] opCode
             * @return
             */
            int inst_ORI_Rd_K ( const unsigned int opCode );

            /**
             * @brief
             *
             * ANDI Rd, K           0111 KKKK dddd KKKK - Logical AND with Immediate
             * CBR Rd, K            0111 kkkk dddd kkkk - Clear Bits in Register (`k' is complemented `K')
             *
             * @param[in] opCode
             * @return
             */
            int inst_ANDI_Rd_K ( const unsigned int opCode );

            /**
             * @brief
             *
             * MUL Rd, Rr           1001 11rd dddd rrrr - Multiply Unsigned
             *
             * @param[in] opCode
             * @return
             */
            int inst_MUL_Rd_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * IN                   1011 0AAd dddd AAAA - Load an I/O Location to Register
             *
             * @param[in] opCode
             * @return
             */
            int inst_IN ( const unsigned int opCode );

            /**
             * @brief
             *
             * OUT A, Rr            1011 1AAr rrrr AAAA - Store Register to I/O Location
             *
             * @param[in] opCode
             * @return
             */
            int inst_OUT_A_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * RJMP k               1100 kkkk kkkk kkkk - Relative Jump
             *
             * @param[in] opCode
             * @return
             */
            int inst_RJMP_k ( const unsigned int opCode );

            /**
             * @brief
             *
             * RCALL k              1101 kkkk kkkk kkkk - Relative Call to Subroutine
             *
             * @param[in] opCode
             * @return
             */
            int inst_RCALL_k ( const unsigned int opCode );

            /**
             * @brief
             *
             * LDI Rd, K            1110 KKKK dddd KKKK - Load Immediate
             *
             * @param[in] opCode
             * @return
             */
            int inst_LDI_Rd_K ( const unsigned int opCode );

            /**
             * @brief
             *
             * BRBS s, k            1111 00kk kkkk ksss - Branch if Bit in SREG is Set
             * BRCS k               1111 00kk kkkk k000 - Branch if Carry Set
             * BRLO k               1111 00kk kkkk k000 - Branch if Lower (Unsigned)
             * BREQ k               1111 00kk kkkk k001 - Branch if Equal
             * BRMI k               1111 00kk kkkk k010 - Branch if Minus
             * BRVS k               1111 00kk kkkk k011 - Branch if Overflow Set
             * BRLT k               1111 00kk kkkk k100 - Branch if Less Than (Signed)
             * BRHS k               1111 00kk kkkk k101 - Branch if Half Carry Flag is Set
             * BRTS k               1111 00kk kkkk k110 - Branch if the T Flag is Set
             * BRIE k               1111 00kk kkkk k111 - Branch if Global Interrupt is Enabled
             *
             * @param[in] opCode
             * @return
             */
            int inst_BRBS_s_k ( const unsigned int opCode );

            /**
             * @brief
             *
             * BRBC s, k            1111 01kk kkkk ksss - Branch if Bit in SREG is Cleared
             * BRCC k               1111 01kk kkkk k000 - Branch if Carry Cleared
             * BRSH k               1111 01kk kkkk k000 - Branch if Same or Higher (Unsigned)
             * BRNE k               1111 01kk kkkk k001 - Branch if Not Equal
             * BRPL k               1111 01kk kkkk k010 - Branch if Plus
             * BRVC k               1111 01kk kkkk k011 - Branch if Overflow Cleared
             * BRGE k               1111 01kk kkkk k100 - Branch if Greater or Equal (Signed)
             * BRHC k               1111 01kk kkkk k101 - Branch if Half Carry Flag is Cleared
             * BRTC k               1111 01kk kkkk k110 - Branch if the T Flag is Cleared
             * BRID k               1111 01kk kkkk k111 - Branch if Global Interrupt is Disabled
             *
             * @param[in] opCode
             * @return
             */
            int inst_BRBC_s_k ( const unsigned int opCode );

            /**
             * @brief
             *
             * ICALL                1001 0101 0000 1001 - Indirect Call to Subroutine
             *
             * @param[in] opCode
             * @return
             */
            int inst_ICALL ( const unsigned int );

            /**
             * @brief
             *
             * EICALL                1001 0101 0001 1001 - Extended Indirect Call to Subroutine
             *
             * @param[in] opCode
             * @return
             */
            int inst_EICALL ( const unsigned int );

            /**
             * @brief
             *
             * IJMP                 1001 0100 0000 1001 - Indirect Jump
             *
             * @param[in] opCode
             * @return
             */
            int inst_IJMP ( const unsigned int );

            /**
             * @brief
             *
             * EIJMP                1001 0100 0001 1001 - Extended Indirect Jump
             *
             * @param[in] opCode
             * @return
             */
            int inst_EIJMP ( const unsigned int );

            /**
             * @brief
             *
             * LPM                  1001 0101 1100 1000 - Load Program Memory
             *
             * @param[in] opCode
             * @return
             */
            int inst_LPM ( const unsigned int );

            /**
             * @brief
             *
             * ELPM                 1001 0101 1101 1000 - Extended Load Program Memory
             *
             * @param[in] opCode
             * @return
             */
            int inst_ELPM ( const unsigned int );

            /**
             * @brief
             *
             * SPM                  1001 0101 1110 1000 - Store Program Memory
             *
             * @param[in] opCode
             * @return
             */
            int inst_SPM ( const unsigned int );

            /**
             * @brief
             *
             * SPM Z+               1001 0101 1111 1000 - Store Program Memory
             *
             * @param[in] opCode
             * @return
             */
            int inst_SPM_Zplus ( const unsigned int );

            /**
             * @brief
             *
             * SLEEP                1001 0101 1000 1000 - SLEEP
             *
             * @param[in] opCode
             * @return
             */
            int inst_SLEEP ( const unsigned int );

            /**
             * @brief
             *
             * BREAK                1001 0101 1001 1000 - Break
             *
             * @param[in] opCode
             * @return
             */
            int inst_BREAK ( const unsigned int );

            /**
             * @brief
             *
             * WDR                  1001 0101 1010 1000 - Watchdog Reset
             *
             * @param[in] opCode
             * @return
             */
            int inst_WDR ( const unsigned int );

            /**
             * @brief
             *
             * RET                  1001 0101 0000 1000 - Return from Subroutine
             *
             * @param[in] opCode
             * @return
             */
            int inst_RET ( const unsigned int );

            /**
             * @brief
             *
             * RETI                 1001 0101 0001 1000 - Return from Interrupt
             *
             * @param[in] opCode
             * @return
             */
            int inst_RETI ( const unsigned int );

            /**
             * @brief
             *
             * LDS Rd, k            1001 000d dddd 0000 \
             *                      kkkk kkkk kkkk kkkk - Load Direct from Data Space
             *
             * @param[in] opCode
             * @return
             */
            int inst_LDS_Rd_k ( const unsigned int opCode );

            /**
             * @brief
             *
             * POP Rd               1001 000d dddd 1111 - Pop Register from Stack
             *
             * @param[in] opCode
             * @return
             */
            int inst_POP_Rd ( const unsigned int opCode );

            /**
             * @brief
             *
             * STS k, Rr            1001 001d dddd 0000 \
             *                      kkkk kkkk kkkk kkkk - Store Direct to Data Space
             *
             * @param[in] opCode
             * @return
             */
            int inst_STS_k_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * XCH Z, Rd            1001 001r rrrr 0100 - Exchange
             *
             * @param[in] opCode
             * @return
             */
            int inst_XCH_Z_Rd ( const unsigned int opCode );

            /**
             * @brief
             *
             * LAS Z, Rd            1001 001r rrrr 0101 - Load And Set
             *
             * @param[in] opCode
             * @return
             */
            int inst_LAS_Z_Rd ( const unsigned int opCode );

            /**
             * @brief
             *
             * LAC Z, Rd            1001 001r rrrr 0110 - Load And Clear
             *
             * @param[in] opCode
             * @return
             */
            int inst_LAC_Z_Rd ( const unsigned int opCode );

            /**
             * @brief
             *
             * LAT Z, Rd            1001 001r rrrr 0111 - Load And Toggle
             *
             * @param[in] opCode
             * @return
             */
            int inst_LAT_Z_Rd ( const unsigned int opCode );

            /**
             * @brief
             *
             * ST Z+, Rr            1001 001r rrrr 0001 - Store Indirect From Register to Data Space using Index Z
             *
             * @param[in] opCode
             * @return
             */
            int inst_ST_Zplus_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * ST -Z, Rr            1001 001r rrrr 0010 - Store Indirect From Register to Data Space using Index Z
             *
             * @param[in] opCode
             * @return
             */
            int inst_ST_minusZ_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * ST Y+, Rr            1001 001r rrrr 1001 - Store Indirect From Register to Data Space using Index Y
             *
             * @param[in] opCode
             * @return
             */
            int inst_ST_Yplus_Rr ( const unsigned int opCode );

            /**
             * ST -Y, Rr            1001 001r rrrr 1010 - Store Indirect From Register to Data Space using Index Y
             *
             * @param[in] opCode
             * @return
             */
            int inst_ST_minusY_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * ST X, Rr             1001 001r rrrr 1100 - Store Indirect From Register to Data Space using Index X
             *
             * @param[in] opCode
             * @return
             */
            int inst_ST_X_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * ST X+, Rr            1001 001r rrrr 1101 - Store Indirect From Register to Data Space using Index X
             *
             * @param[in] opCode
             * @return
             */
            int inst_ST_Xplus_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * ST -X, Rr            1001 001r rrrr 1110 - Store Indirect From Register to Data Space using Index X
             *
             * @param[in] opCode
             * @return
             */
            int inst_ST_minusX_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * PUSH Rr              1001 001d dddd 1111 - Push Register on Stack
             *
             * @param[in] opCode
             * @return
             */
            int inst_PUSH_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * BSET s               1001 0100 0sss 1000 - Bit Set in SREG
             *
             * @param[in] opCode
             * @return
             */
            int inst_BSET_s ( const unsigned int opCode );

            /**
             * @brief
             *
             * SEC                  1001 0100 0000 1000 - Set Carry Flag
             *
             * @param[in] opCode
             * @return
             */
            int inst_SEC ( const unsigned int );

            /**
             * @brief
             *
             * SEZ                  1001 0100 0001 1000 - Set Zero Flag
             *
             * @param[in] opCode
             * @return
             */
            int inst_SEZ ( const unsigned int );

            /**
             * @brief
             *
             * SEN                  1001 0100 0010 1000 - Set Negative Flag
             *
             * @param[in] opCode
             * @return
             */
            int inst_SEN ( const unsigned int );

            /**
             * @brief
             *
             * SEV                  1001 0100 0011 1000 - Set Overflow Flag
             *
             * @param[in] opCode
             * @return
             */
            int inst_SEV ( const unsigned int );

            /**
             * @brief
             *
             * SES                  1001 0100 0100 1000 - Set Signed Flag
             *
             * @param[in] opCode
             * @return
             */
            int inst_SES ( const unsigned int );

            /**
             * @brief
             *
             * SEH                  1001 0100 0101 1000 - Set Half Carry Flag
             *
             * @param[in] opCode
             * @return
             */
            int inst_SEH ( const unsigned int );

            /**
             * @brief
             *
             * SET                  1001 0100 0110 1000 - Set T Flag
             *
             * @param[in] opCode
             * @return
             */
            int inst_SET ( const unsigned int );

            /**
             * @brief
             *
             * SEI                  1001 0100 0111 1000 - Set Global Interrupt Flag
             *
             * @param[in] opCode
             * @return
             */
            int inst_SEI ( const unsigned int );

            /**
             * @brief
             *
             * BCLR s               1001 0100 1sss 1000 - Bit Clear in SREG
             *
             * @param[in] opCode
             * @return
             */
            int inst_BCLR_s ( const unsigned int opCode );

            /**
             * @brief
             *
             * CLC                  1001 0100 1000 1000 - Clear Carry Flag
             *
             * @param[in] opCode
             * @return
             */
            int inst_CLC ( const unsigned int );

            /**
             * @brief
             *
             * CLZ                  1001 0100 1001 1000 - Clear Zero Flag
             *
             * @param[in] opCode
             * @return
             */
            int inst_CLZ ( const unsigned int );

            /**
             * @brief
             *
             * CLN                  1001 0100 1010 1000 - Clear Negative Flag
             *
             * @param[in] opCode
             * @return
             */
            int inst_CLN ( const unsigned int );

            /**
             * @brief
             *
             * CLV                  1001 0100 1011 1000 - Clear Overflow Flag
             *
             * @param[in] opCode
             * @return
             */
            int inst_CLV ( const unsigned int );

            /**
             * @brief
             *
             * CLS                  1001 0100 1100 1000 - Clear Signed Flag
             *
             * @param[in] opCode
             * @return
             */
            int inst_CLS ( const unsigned int );

            /**
             * @brief
             *
             * CLH                  1001 0100 1101 1000 - Clear Half Carry Flag
             *
             * @param[in] opCode
             * @return
             */
            int inst_CLH ( const unsigned int );

            /**
             * @brief
             *
             * CLT                  1001 0100 1110 1000 - Clear T Flag
             *
             * @param[in] opCode
             * @return
             */
            int inst_CLT ( const unsigned int );

            /**
             * @brief
             *
             * CLI                  1001 0100 1111 1000 - Clear Global Interrupt Flag
             *
             * @param[in] opCode
             * @return
             */
            int inst_CLI ( const unsigned int );

            /**
             * @brief
             *
             * FMULSU Rd, Rr        0000 0011 1ddd 1rrr - Fractional Multiply Signed with Unsigned
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_FMULSU_Rd_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * FMULS Rd, Rr         0000 0011 1ddd 0rrr - Fractional Multiply Signed
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_FMULS_Rd_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * FMUL Rd, Rr          0000 0011 0ddd 1rrr - Fractional Multiply Unsigned
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_FMUL_Rd_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * MULSU Rd, Rr         0000 0011 0ddd 0rrr - Multiply Signed with Unsigned
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_MULSU_Rd_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * MULS Rd, Rr          0000 0010 dddd rrrr - Multiply Signed
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_MULS_Rd_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * MOVW Rd+1:Rd, Rr+1   0000 0001 dddd rrrr - Copy Register Word
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_MOVW_Rd_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * NOP                  0000 0000 0000 0000 - No Operation
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_NOP ( const unsigned int );

            /**
             * @brief
             *
             * SBIW Rd+1:Rd, K      1001 0111 KKdd KKKK - Subtract Immediate from Word
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_SBIW_Rd_1_Rd_K ( const unsigned int opCode );

            /**
             * @brief
             *
             * ADIW Rd+1:Rd, K      1001 0110 KKdd KKKK - Add Immediate to Word
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_ADIW_Rd_1_Rd_K ( const unsigned int opCode );

            /**
             * @brief
             *
             * CALL k               1001 010k kkkk 111k \
             *                      kkkk kkkk kkkk kkkk - Long Call to a Subroutine
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_CALL_k ( const unsigned int opCode );

            /**
             * @brief
             *
             * JMP k                1001 010k kkkk 110k \
             *                      kkkk kkkk kkkk kkkk - Jump
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_JMP_k ( const unsigned int opCode );

            /**
             * @brief
             *
             * DES K                1001 0100 KKKK 1011 - Data Encryption Standard
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_DES_K ( const unsigned int opCode );

            /**
             * @brief
             *
             * DEC Rd               1001 010d dddd 1010 - Decrement
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_DEC_Rd ( const unsigned int opCode );

            /**
             * @brief
             *
             * ROR Rd               1001 010d dddd 0111 - Rotate Right through Carry
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_ROR_Rd ( const unsigned int opCode );

            /**
             * @brief
             *
             * LSR Rd               1001 010d dddd 0110 - Logical Shift Right
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_LSR_Rd ( const unsigned int opCode );

            /**
             * @brief
             *
             * ASR Rd               1001 010d dddd 0101 - Arithmetic Shift Right
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_ASR_Rd ( const unsigned int opCode );

            /**
             * @brief
             *
             * INC Rd               1001 010d dddd 0011 - Increment
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_INC_Rd ( const unsigned int opCode );

            /**
             * @brief
             *
             * SWAP Rd              1001 010d dddd 0010 - Swap Nibbles
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_SWAP_Rd ( const unsigned int opCode );

            /**
             * @brief
             *
             * NEG Rd               1001 010d dddd 0001 - Two’s Complement
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_NEG_Rd ( const unsigned int opCode );

            /**
             * @brief
             *
             * COM Rd               1001 010d dddd 0000 - One’s Complement
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_COM_Rd ( const unsigned int opCode );

            /**
             * @brief
             *
             * SBIS A, b            1001 1011 AAAA Abbb - Skip if Bit in I/O Register is Set
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_SBIS_A_b ( const unsigned int opCode );

            /**
             * @brief
             *
             * SBI A, b             1001 1010 AAAA Abbb - Set Bit in I/O Register
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_SBI_A_b ( const unsigned int opCode );

            /**
             * @brief
             *
             * SBIC A, b            1001 1001 AAAA Abbb - Skip if Bit in I/O Register is Cleared
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_SBIC_A_b ( const unsigned int opCode );

            /**
             * @brief
             *
             * CBI A, b             1001 1000 AAAA Abbb - Clear Bit in I/O Register
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_CBI_A_b ( const unsigned int opCode );

            /**
             * @brief
             *
             * BST Rd, b            1111 101d dddd 0bbb - Bit Store from Bit in Register to T Flag in SREG
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_BST_Rd_b ( const unsigned int opCode );

            /**
             * @brief
             *
             * BLD Rd, b            1111 100d dddd 0bbb - Bit Load from the T Flag in SREG to a Bit in Register
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_BLD_Rd_b ( const unsigned int opCode );

            /**
             * @brief
             *
             * SBRS Rr, b           1111 111r rrrr 0bbb - Skip if Bit in Register is Set
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_SBRS_Rr_b ( const unsigned int opCode );

            /**
             * SBRC Rr, b           1111 110r rrrr 0bbb - Skip if Bit in Register is Cleared
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_SBRC_Rr_b ( const unsigned int opCode );

            /**
             * @brief
             *
             * STS k, Rr            1010 1kkk dddd kkkk - Store Direct to Data Space
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_STS_k_Rr_16b ( const unsigned int opCode );

            /**
             * @brief
             *
             * LDS Rd, k            1010 0kkk dddd kkkk - Load Direct from Data Space
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_LDS_Rd_k_16b ( const unsigned int opCode );

            /**
             * @brief
             *
             * STD Z+q, Rr          10q0 qq1r rrrr 0qqq - Store Indirect From Register to Data Space using Index Z
             * ST Z, Rr             1000 001r rrrr 0000 - Store Indirect From Register to Data Space using Index Z
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_STD_Zq_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * STD Y+q, Rr          10q0 qq1r rrrr 1qqq - Store Indirect From Register to Data Space using Index Y
             * ST Y, Rr             1000 001r rrrr 1000 - Store Indirect From Register to Data Space using Index Y
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_STD_Yq_Rr ( const unsigned int opCode );

            /**
             * @brief
             *
             * LDD Rd, Z+q          10q0 qq0d dddd 0qqq - Load Indirect From Data Space to Register using Index Z
             * LD Rd, Z             1000 000d dddd 0000 - Load Indirect From Data Space to Register using Index Z
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_LDD_Rd_Zq ( const unsigned int opCode );

            /**
             * @brief
             *
             * LDD Rd, Y+q          10q0 qq0d dddd 1qqq - Load Indirect from Data Space to Register using Index Y
             * LD Rd, Y             1000 000d dddd 1000 - Load Indirect from Data Space to Register using Index Y
             *
             * @param[in] opCode
             * @return
             */
            inline int inst_LDD_Rd_Yq ( const unsigned int opCode );

            /**
             * @brief
             *
             * LD Rd, Z+            1001 000d dddd 0001 - Load Indirect From Data Space to Register using Index Z
             *
             * @param[in] opCode
             * @return
             */
            int inst_LD_Rd_Zplus ( const unsigned int opCode );

            /**
             * @brief
             *
             * LD Rd, -Z            1001 000d dddd 0010 - Load Indirect From Data Space to Register using Index Z
             *
             * @param[in] opCode
             * @return
             */
            int inst_LD_Rd_minusZ ( const unsigned int opCode );

            /**
             * @brief
             *
             * LPM Rd, Z            1001 000d dddd 0100 - Load Program Memory
             *
             * @param[in] opCode
             * @return
             */
            int inst_LPM_Rd_Z ( const unsigned int opCode );

            /**
             * @brief
             *
             * LPM Rd, Z+           1001 000d dddd 0101 - Load Program Memory
             *
             * @param[in] opCode
             * @return
             */
            int inst_LPM_Rd_Zplus ( const unsigned int opCode );

            /**
             * @brief
             *
             * ELPM Rd, Z           1001 000d dddd 0110 - Extended Load Program Memory
             *
             * @param[in] opCode
             * @return
             */
            int inst_ELPM_Rd_Z ( const unsigned int opCode );

            /**
             * @brief
             *
             * ELPM Rd, Z+          1001 000d dddd 0111 - Extended Load Program Memory
             *
             * @param[in] opCode
             * @return
             */
            int inst_ELPM_Rd_Zplus ( const unsigned int opCode );

            /**
             * @brief
             *
             * LD Rd, Y+            1001 000d dddd 1001 - Load Indirect from Data Space to Register using Index Y
             *
             * @param[in] opCode
             * @return
             */
            int inst_LD_Rd_Yplus ( const unsigned int opCode );

            /**
             * @brief
             *
             * LD Rd, -Y            1001 000d dddd 1010 - Load Indirect from Data Space to Register using Index Y
             *
             * @param[in] opCode
             * @return
             */
            int inst_LD_Rd_minusY ( const unsigned int opCode );

            /**
             * @brief
             *
             * LD Rd, X             1001 000d dddd 1100 - Load Indirect from Data Space to Register using Index X
             *
             * @param[in] opCode
             * @return
             */
            int inst_LD_Rd_X ( const unsigned int opCode );

            /**
             * @brief
             *
             * LD Rd, X+            1001 000d dddd 1101 - Load Indirect from Data Space to Register using Index X
             *
             * @param[in] opCode
             * @return
             */
            int inst_LD_Rd_Xplus ( const unsigned int opCode );

            /**
             * @brief
             *
             * LD Rd, -X            1001 000d dddd 1110 - Load Indirect from Data Space to Register using Index X
             *
             * @param[in] opCode
             * @return
             */
            int inst_LD_Rd_minusX ( const unsigned int opCode );
        //@}

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] val
         */
        inline void incrPc ( const int val = 1 );

        /**
         * @brief
         * @param[in] opCode
         * @return
         */
        inline bool isInstruction32b ( const unsigned int opCode ) const;

        /**
         * @brief
         * @return
         */
        inline void mcuReset();

        /**
         * @brief
         * @return
         */
        inline void resetToInitialValues();

        /**
         * @brief
         * @param[in] instName
         * @return
         */
        inline void instructionEnter ( AVR8InsNames::Instructions instName );

    ////    Protected Attributes    ////
    protected:
        /// @brief
        int m_pc;

        /// @brief
        int m_actSubprogCounter;

        /// @brief
        unsigned int m_instructionCounter [ AVR8InsNames::INS__MAX__ ];

        /// @brief
        AVR8InsNames::Instructions m_lastInstruction;

        /// @brief
        AVR8DES * m_des;

        /// @name AVR8 simulator subsystems
        //@{
            /// @brief
            AVR8ProgramMemory * m_programMemory;

            /// @brief
            AVR8DataMemory * m_dataMemory;

            /// @brief
            MCUSim::Mode * m_processorMode;

            /// @brief
            AVR8Sim::SleepMode * m_sleepMode;

            /// @brief
            AVR8FusesAndLocks * m_fusesAndLocks;

            /// @brief
            AVR8InterruptController * m_interruptController;

            /// @brief
            AVR8SystemControl * m_systemControl;

            /// @brief
            AVR8Sim::HaltMode * m_haltMode;

            /// @brief
            AVR8BootLoader * m_bootLoader;
        //@}

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        Config m_config;
};

#endif // AVR8INSTRUCTIONSET_H
