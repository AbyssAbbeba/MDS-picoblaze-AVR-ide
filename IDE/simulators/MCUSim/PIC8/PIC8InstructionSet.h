// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2012 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @ingroup PIC8
 * @file PIC8InstructionSet.h
 */
// =============================================================================

#ifndef PIC8INSTRUCTIONSET_H
#define PIC8INSTRUCTIONSET_H

// Forward declarations
class PIC8ProgramMemory;
class PIC8DataMemory;
class PIC8ConfigWord;
class PIC8InterruptController;
class PIC8WatchDogTimer;

#include "../MCUSim.h"
#include "PIC8Sim.h"
#include "PIC8InsNames.h"

#include <cstdint>

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8InstructionSet
 */
class PIC8InstructionSet : public MCUSim::CPU
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct Config
        {
            ///
            Config()
            {
                m_ignoreUndefinedOpCodes = true;
            }

            ///
            MCUSim::Family m_family;

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
        static int ( PIC8InstructionSet :: * const m_opCodeDispatchTable [ 64 ] ) ( const unsigned int opCode );

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        PIC8InstructionSet() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] processorMode
         * @param[in,out] programMemory
         * @param[in,out] dataMemory
         * @param[in,out] configWord
         * @param[in,out] stack
         * @param[in,out] interruptCtrl
         * @return
         */
        PIC8InstructionSet * link ( MCUSim::EventLogger     * eventLogger,
                                    MCUSim::Mode            * processorMode,
                                    PIC8ProgramMemory       * programMemory,
                                    PIC8DataMemory          * dataMemory,
                                    PIC8ConfigWord          * configWord,
                                    PIC8Stack               * stack,
                                    PIC8InterruptController * interruptCtrl,
                                    PIC8WatchDogTimer       * watchDogTimer );

        /**
         * @brief
         * @return
         */
        int execInstruction();

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSim::ResetMode mode );

        /**
         * @brief
         */
        void wakeFromSleep();

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        unsigned int getProgramCounter() const
        {
            return (unsigned int) m_pc;
        }

        /**
         * @brief
         * @param[in] newPc
         */
        void setProgramCounter ( unsigned int newPc )
        {
            m_pc = int ( newPc );
        }

        /**
         * @brief
         * @return
         */
        uint8_t getW() const
        {
            return (uint8_t) m_W;
        }

        /**
         * @brief
         * @param[in] value
         */
        void setW ( uint8_t value )
        {
            m_W = (int) value;
        }

    ////    Instruction Operations - Public Operations and Inline Public Operations    ////
    public:
        /**
         * @brief
         *
         * Possible instuctions:
         * - NOP    : 00 0000 0xx0 0000 - No Operation
         * - RETURN : 00 0000 0000 1000 - Return from Subroutine
         * - RETFIE : 00 0000 0000 1001 - Return from interrupt
         * - SLEEP  : 00 0000 0110 0011 - Go into standby mode
         * - CLRWDT : 00 0000 0110 0100 - Clear Watchdog Timer
         */
        int instOPCode_000000 ( const unsigned int opCode );

        /**
         * @brief
         *
         * Possible instuctions:
         * - CLRW : 00 0001 0xxx xxxx - Clear W
         * - CLRF : 00 0001 1xxx xxxx - Clear f
         */
        int instOPCode_000001 ( const unsigned int opCode );

        /**
         * @brief
         *
         * Possible instuctions:
         * - BCF   : 10 00xx xxxx xxxx - Bit Clear f
         * - BSF   : 10 01xx xxxx xxxx - Bit Set f
         * - BTFSC : 10 10xx xxxx xxxx - Bit Test f, Skip if Clear
         * - BTFSS : 10 11xx xxxx xxxx - Bit Test f, Skip if Set
         */
        int instOPCode_01 ( const unsigned int opCode );

        /// @name Processor Instructions
        //@{
            /**
             * @brief Invalid instruction.
             * @param[in] opCode
             * @return
             *
             * @warning Invocation of this method is an error condition.
             */
            int instInvalid ( const unsigned int opCode );

            /// @name Byte-oriented file register operations
            //@{
                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_ADDWF ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_ANDWF ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                inline int inst_CLRF ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                inline int inst_CLRW ( const unsigned int );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_COMF ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_DECF ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_DECFSZ ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_INCF ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_INCFSZ ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_IORWF ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_MOVF ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_MOVWF ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                inline int inst_NOP ( const unsigned int );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_RLF ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_RRF ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_SUBWF ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_SWAPF ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_XORWF ( const unsigned int opCode );
            //@}

            /// @name Bit-oriented file register operations
            //@{
                /**
                 * @brief
                 * @param[in] opCode
                 */
                inline int inst_BCF ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                inline int inst_BSF ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                inline int inst_BTFSC ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                inline int inst_BTFSS ( const unsigned int opCode );
            //@}

            /// @name Literal and control operations
            //@{
                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_ADDLW ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_ANDLW ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_CALL ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                inline int inst_CLRWDT ( const unsigned int );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_GOTO ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_IORLW ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_MOVLW ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                inline int inst_RETFIE ( const unsigned int );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_RETLW ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                inline int inst_RETURN ( const unsigned int );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                inline int inst_SLEEP ( const unsigned int );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_SUBLW ( const unsigned int opCode );

                /**
                 * @brief
                 * @param[in] opCode
                 */
                int inst_XORLW ( const unsigned int opCode );
            //@}
        //@}

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] val
         * @return New value the program counter (PC)
         */
        inline int incrPc ( const int val = 1 );

        /**
         * @brief
         * @param[in] opCode
         * @return
         */
        inline bool isInstruction32b ( const unsigned int opCode ) const;

        /**
         * @brief
         */
        inline void mcuReset();

        /**
         * @brief
         */
        inline void resetToInitialValues();

        /**
         * @brief
         * @param[in] instName
         */
        inline void instructionEnter ( PIC8InsNames::Instructions instName );

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        Config m_config;

    ////    Protected Attributes    ////
    protected:
        ///
        unsigned int m_W;

        ///
        int m_pc;

        ///
        int m_actSubprogCounter;

        ///
        unsigned int m_instructionCounter [ PIC8InsNames::INS__MAX__ ];

        ///
        PIC8InsNames::Instructions m_lastInstruction;

        /// @name PIC8 simulator subsystems
        //@{
            ///
            PIC8ProgramMemory * m_programMemory;

            ///
            PIC8DataMemory * m_dataMemory;

            ///
            PIC8ConfigWord * m_configWord;

            ///
            PIC8InterruptController * m_interruptController;

            ///
            PIC8Stack * m_stack;

            ///
            PIC8WatchDogTimer * m_watchDogTimer;

            ///
            MCUSim::Mode * m_processorMode;
        //@}
};

#endif // PIC16F84AINSTRUCTIONSET_H
