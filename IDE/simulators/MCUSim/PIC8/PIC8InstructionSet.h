/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @ingroup PIC8
 * @file PIC8InstructionSet.h
 */

#ifndef PIC8INSTRUCTIONSET_H
#define PIC8INSTRUCTIONSET_H

// Forward declarations
class PIC8ProgramMemory;
class PIC8DataMemory;
class PIC8FusesAndLocks;
class PIC8InterruptController;
class PIC8SystemControl;
class PIC8BootLoader;

#include "../MCUSim.h"
#include "PIC8Sim.h"
#include "PIC8InsNames.h"

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8InstructionSet
 */
class PIC8InstructionSet : public MCUSim::CPU {
public:
    PIC8InstructionSet() {};

    enum PCWidth {
        // .. lower values ...
        PCWIDTH_16 = 1,
        // .. higher values ...
        PCWIDTH_22
    };

    struct Config {
        Config() {
            m_ignoreUndefinedOpCodes = true;
        }

        MCUSim::Family m_family;
        PCWidth m_pcWidth;
        bool m_ignoreUndefinedOpCodes;

        int m_pcMax;
    };

    Config m_config;

    PIC8InstructionSet * link(
        MCUSim::EventLogger * eventLogger,
        PIC8ProgramMemory   * programMemory,
        PIC8DataMemory      * dataMemory,
        MCUSim::Mode        * processorMode,
        PIC8Sim::SleepMode  * sleepMode,
        PIC8FusesAndLocks   * fusesAndLocks,
        PIC8InterruptController * interruptController,
        PIC8SystemControl   * systemControl,
        PIC8Sim::HaltMode   * haltMode,
        PIC8BootLoader      * bootLoader);

    int execInstruction();
    void reset(MCUSim::ResetMode mode);

    unsigned int getProgramCounter() const {
        return (unsigned int)m_pc;
    }
    void setProgramCounter(unsigned int newPc) {
        m_pc = int(newPc);
    }

protected:
    int m_pc;
    int m_actSubprogCounter;
    unsigned int m_instructionCounter[PIC8InsNames::INS__MAX__];
    AVR8InsNames::Instructions m_lastInstruction;

    PIC8ProgramMemory * m_programMemory;
    PIC8DataMemory * m_dataMemory;
    MCUSim::Mode * m_processorMode;
    PIC8Sim::SleepMode * m_sleepMode;
    PIC8FusesAndLocks * m_fusesAndLocks;
    PIC8InterruptController * m_interruptController;
    PIC8SystemControl * m_systemControl;
    PIC8Sim::HaltMode * m_haltMode;
    PIC8BootLoader * m_bootLoader;

    inline void incrPc(const int val = 1);
    inline bool isInstruction32b(const unsigned int opCode) const;

    inline void mcuReset();
    inline void resetToInitialValues();

    inline void instructionEnter(AVR8InsNames::Instructions instName);

public:
    static int (PIC8InstructionSet:: * const m_opCodeDispatchTable[64])(const unsigned int opCode);

    /**
     * Possible instuctions:
     * NOP          00 0000 0xx0 0000 - No Operation
     * RETURN       00 0000 0000 1000 - Return from Subroutine
     * RETFIE       00 0000 0000 1001 - Return from interrupt
     * SLEEP        00 0000 0110 0011 - Go into standby mode
     * CLRWDT       00 0000 0110 0100 - Clear Watchdog Timer
     */
    int instOPCode_000000(const unsigned int opCode);

    /**
     * Possible instuctions:
     * CLRW         00 0001 0xxx xxxx - Clear W
     * CLRF         00 0001 1xxx xxxx - Clear f
     */
    int instOPCode_000001(const unsigned int opCode);

    /**
     * Possible instuctions:
     * BCF          10 00xx xxxx xxxx - Bit Clear f
     * BSF          10 01xx xxxx xxxx - Bit Set f
     * BTFSC        10 10xx xxxx xxxx - Bit Test f, Skip if Clear
     * BTFSS        10 11xx xxxx xxxx - Bit Test f, Skip if Set
     */
    int instOPCode_10(const unsigned int opCode);

    /**
     * INVALID INSTRUCTION -- ERROR CONDITION
     */
    int instInvalid(const unsigned int opCode);
    int inst_ADDWF(const unsigned int opCode);
    int inst_ANDWF(const unsigned int opCode);
    int inst_CLRF(const unsigned int opCode);
    int inst_CLRW(const unsigned int opCode);
    int inst_COMF(const unsigned int opCode);
    int inst_DECF(const unsigned int opCode);
    int inst_DECFSZ(const unsigned int opCode);
    int inst_INCF(const unsigned int opCode);
    int inst_INCFSZ(const unsigned int opCode);
    int inst_IORWF(const unsigned int opCode);
    int inst_MOVF(const unsigned int opCode);
    int inst_MOVWF(const unsigned int opCode);
    int inst_NOP(const unsigned int opCode);
    int inst_RLF(const unsigned int opCode);
    int inst_RRF(const unsigned int opCode);
    int inst_SUBWF(const unsigned int opCode);
    int inst_SWAPF(const unsigned int opCode);
    int inst_XORWF(const unsigned int opCode);
    int inst_BCF(const unsigned int opCode);
    int inst_BSF(const unsigned int opCode);
    int inst_BTFSC(const unsigned int opCode);
    int inst_BTFSS(const unsigned int opCode);
    int inst_ADDLW(const unsigned int opCode);
    int inst_ANDLW(const unsigned int opCode);
    int inst_CALL(const unsigned int opCode);
    int inst_CLRWDT(const unsigned int opCode);
    int inst_GOTO(const unsigned int opCode);
    int inst_IORLW(const unsigned int opCode);
    int inst_MOVLW(const unsigned int opCode);
    int inst_RETFIE(const unsigned int opCode);
    int inst_RETLW(const unsigned int opCode);
    int inst_RETURN(const unsigned int opCode);
    int inst_SLEEP(const unsigned int opCode);
    int inst_SUBLW(const unsigned int opCode);
    int inst_XORLW(const unsigned int opCode);
};


#endif // PIC16F84AINSTRUCTIONSET_H
