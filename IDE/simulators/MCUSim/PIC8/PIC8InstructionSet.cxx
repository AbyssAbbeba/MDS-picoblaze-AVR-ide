// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2012 Moravia Microsystems, s.r.o.
 *
 * @authors Erik Chalupa <xchalu10@stud.fit.vutbr.cz>
 * @authors Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup PIC8
 * @file PIC8InstructionSet.cxx
 */
// =============================================================================

#include "PIC8InstructionSet.h"
#include "PIC8ProgramMemory.h"
#include "PIC8DataMemory.h"
#include "PIC8ConfigWord.h"
#include "PIC8InterruptController.h"
#include "PIC8RegNames.h"
#include "PIC8Stack.h"
#include "PIC8WatchDogTimer.h"

int ( PIC8InstructionSet :: * const PIC8InstructionSet::m_opCodeDispatchTable [ 64 ] ) ( const unsigned int opCode ) =
{
    &PIC8InstructionSet::instOPCode_000000, // opCode = 00 0000 xxxx xxxx
    &PIC8InstructionSet::instOPCode_000001, // opCode = 00 0001 xxxx xxxx
    &PIC8InstructionSet::inst_SUBWF,        // opCode = 00 0010 xxxx xxxx
    &PIC8InstructionSet::inst_DECF,         // opCode = 00 0011 xxxx xxxx
    &PIC8InstructionSet::inst_IORWF,        // opCode = 00 0100 xxxx xxxx
    &PIC8InstructionSet::inst_ANDWF,        // opCode = 00 0101 xxxx xxxx
    &PIC8InstructionSet::inst_XORWF,        // opCode = 00 0110 xxxx xxxx
    &PIC8InstructionSet::inst_ADDWF,        // opCode = 00 0111 xxxx xxxx
    &PIC8InstructionSet::inst_MOVF,         // opCode = 00 1000 xxxx xxxx
    &PIC8InstructionSet::inst_COMF,         // opCode = 00 1001 xxxx xxxx
    &PIC8InstructionSet::inst_INCF,         // opCode = 00 1010 xxxx xxxx
    &PIC8InstructionSet::inst_DECFSZ,       // opCode = 00 1011 xxxx xxxx
    &PIC8InstructionSet::inst_RRF,          // opCode = 00 1100 xxxx xxxx
    &PIC8InstructionSet::inst_RLF,          // opCode = 00 1101 xxxx xxxx
    &PIC8InstructionSet::inst_SWAPF,        // opCode = 00 1110 xxxx xxxx
    &PIC8InstructionSet::inst_INCFSZ,       // opCode = 00 1111 xxxx xxxx
    &PIC8InstructionSet::instOPCode_01,     // opCode = 01 0000 xxxx xxxx
    &PIC8InstructionSet::instOPCode_01,     // opCode = 01 0001 xxxx xxxx
    &PIC8InstructionSet::instOPCode_01,     // opCode = 01 0010 xxxx xxxx
    &PIC8InstructionSet::instOPCode_01,     // opCode = 01 0011 xxxx xxxx
    &PIC8InstructionSet::instOPCode_01,     // opCode = 01 0100 xxxx xxxx
    &PIC8InstructionSet::instOPCode_01,     // opCode = 01 0101 xxxx xxxx
    &PIC8InstructionSet::instOPCode_01,     // opCode = 01 0110 xxxx xxxx
    &PIC8InstructionSet::instOPCode_01,     // opCode = 01 0111 xxxx xxxx
    &PIC8InstructionSet::instOPCode_01,     // opCode = 01 1000 xxxx xxxx
    &PIC8InstructionSet::instOPCode_01,     // opCode = 01 1001 xxxx xxxx
    &PIC8InstructionSet::instOPCode_01,     // opCode = 01 1010 xxxx xxxx
    &PIC8InstructionSet::instOPCode_01,     // opCode = 01 1011 xxxx xxxx
    &PIC8InstructionSet::instOPCode_01,     // opCode = 01 1100 xxxx xxxx
    &PIC8InstructionSet::instOPCode_01,     // opCode = 01 1101 xxxx xxxx
    &PIC8InstructionSet::instOPCode_01,     // opCode = 01 1110 xxxx xxxx
    &PIC8InstructionSet::instOPCode_01,     // opCode = 01 1111 xxxx xxxx
    &PIC8InstructionSet::inst_CALL,         // opCode = 10 0000 xxxx xxxx
    &PIC8InstructionSet::inst_CALL,         // opCode = 10 0001 xxxx xxxx
    &PIC8InstructionSet::inst_CALL,         // opCode = 10 0010 xxxx xxxx
    &PIC8InstructionSet::inst_CALL,         // opCode = 10 0011 xxxx xxxx
    &PIC8InstructionSet::inst_CALL,         // opCode = 10 0100 xxxx xxxx
    &PIC8InstructionSet::inst_CALL,         // opCode = 10 0101 xxxx xxxx
    &PIC8InstructionSet::inst_CALL,         // opCode = 10 0110 xxxx xxxx
    &PIC8InstructionSet::inst_CALL,         // opCode = 10 0111 xxxx xxxx
    &PIC8InstructionSet::inst_GOTO,         // opCode = 10 1000 xxxx xxxx
    &PIC8InstructionSet::inst_GOTO,         // opCode = 10 1001 xxxx xxxx
    &PIC8InstructionSet::inst_GOTO,         // opCode = 10 1010 xxxx xxxx
    &PIC8InstructionSet::inst_GOTO,         // opCode = 10 1011 xxxx xxxx
    &PIC8InstructionSet::inst_GOTO,         // opCode = 10 1100 xxxx xxxx
    &PIC8InstructionSet::inst_GOTO,         // opCode = 10 1101 xxxx xxxx
    &PIC8InstructionSet::inst_GOTO,         // opCode = 10 1110 xxxx xxxx
    &PIC8InstructionSet::inst_GOTO,         // opCode = 10 1111 xxxx xxxx
    &PIC8InstructionSet::inst_MOVLW,        // opCode = 11 0000 xxxx xxxx
    &PIC8InstructionSet::inst_MOVLW,        // opCode = 11 0001 xxxx xxxx
    &PIC8InstructionSet::inst_MOVLW,        // opCode = 11 0010 xxxx xxxx
    &PIC8InstructionSet::inst_MOVLW,        // opCode = 11 0011 xxxx xxxx
    &PIC8InstructionSet::inst_RETLW,        // opCode = 11 0100 xxxx xxxx
    &PIC8InstructionSet::inst_RETLW,        // opCode = 11 0101 xxxx xxxx
    &PIC8InstructionSet::inst_RETLW,        // opCode = 11 0110 xxxx xxxx
    &PIC8InstructionSet::inst_RETLW,        // opCode = 11 0111 xxxx xxxx
    &PIC8InstructionSet::inst_IORLW,        // opCode = 11 1000 xxxx xxxx
    &PIC8InstructionSet::inst_ANDLW,        // opCode = 11 1001 xxxx xxxx
    &PIC8InstructionSet::inst_XORLW,        // opCode = 11 1010 xxxx xxxx
    &PIC8InstructionSet::instInvalid,       // opCode = 11 1011 xxxx xxxx
    &PIC8InstructionSet::inst_SUBLW,        // opCode = 11 1100 xxxx xxxx
    &PIC8InstructionSet::inst_SUBLW,        // opCode = 11 1101 xxxx xxxx
    &PIC8InstructionSet::inst_ADDLW,        // opCode = 11 1110 xxxx xxxx
    &PIC8InstructionSet::inst_ADDLW,        // opCode = 11 1111 xxxx xxxx
};

PIC8InstructionSet * PIC8InstructionSet::link ( MCUSim::EventLogger     * eventLogger,
                                                MCUSim::Mode            * processorMode,
                                                PIC8ProgramMemory       * programMemory,
                                                PIC8DataMemory          * dataMemory,
                                                PIC8ConfigWord          * configWord,
                                                PIC8Stack               * stack,
                                                PIC8InterruptController * interruptCtrl,
                                                PIC8WatchDogTimer       * watchDogTimer )
{
    MCUSim::CPU::link(eventLogger);

    m_programMemory = programMemory;
    m_dataMemory = dataMemory;
    m_processorMode = processorMode;
    m_configWord = configWord;
    m_stack = stack;
    m_interruptController = interruptCtrl;
    m_watchDogTimer = watchDogTimer;

    return this;
}

void PIC8InstructionSet::reset ( MCUSim::ResetMode mode )
{
    switch ( mode )
    {
        case MCUSim::RSTMD_INITIAL_VALUES:
            resetToInitialValues();
            break;
        case MCUSim::RSTMD_MCU_RESET:
            mcuReset();
            break;
        default:
            // Irrelevant requests are silently ignored
            break;
    }
}

inline void PIC8InstructionSet::mcuReset()
{
    m_pc = 0;
    m_actSubprogCounter = 0;
    m_lastInstruction = PIC8InsNames::INS_NONE;
}

inline void PIC8InstructionSet::resetToInitialValues()
{
    mcuReset();

    for ( int i = 0; i < PIC8InsNames::INS__MAX__; i++ )
    {
        m_instructionCounter[i] = 0;
    }
}

void PIC8InstructionSet::wakeFromSleep()
{
    *m_processorMode = MCUSim::MD_NORMAL;
    logEvent(EVENT_CPU_MODE_CHANGED, m_pc, MCUSim::MD_NORMAL);
}

inline int PIC8InstructionSet::incrPc ( const int val )
{
    m_pc += val;
    while ( m_pc > m_config.m_pcMax )
    {
        m_pc -= m_config.m_pcMax;
        logEvent(EVENT_CPU_PC_OVERFLOW);
    }
    while ( m_pc < 0 )
    {
        m_pc += m_config.m_pcMax;
        logEvent(EVENT_CPU_PC_UNDERFLOW);
    }
    return m_pc;
}

int PIC8InstructionSet::execInstruction()
{
    int result = -1;
    const int pcOrig = m_pc;
    unsigned int opCode = m_programMemory->readRaw(m_pc);
    incrPc();

    if ( opCode & 0xffff0000 )
    {
        if ( true == m_config.m_ignoreUndefinedOpCodes )
        {
            return -1;
        }
        else
        {
            opCode &= 0x0ffff;
        }
    }

    result = (this->*(m_opCodeDispatchTable[opCode >> 8]))(opCode);

    if ( pcOrig != m_pc )
    {
        logEvent(EVENT_CPU_PC_CHANGED, m_pc);
    }

    return result;
}

int PIC8InstructionSet::instOPCode_000000 ( const unsigned int opCode )
{
    // opCode = 00 0000 xxxx xxxx
    if ( !(opCode & 0x080) )
    {
        // opCode = 00 0000 0xxx xxxx
        if ( opCode & 0x01 )
        {
            // opCode = 00 0000 0xxx xxx1
            if ( opCode & 0x02 )
            {
                // opCode = 00 0000 0xxx xx11
                if ( 99 == opCode )
                {
                    // opCode = 00 0000 0110 0011
                    return inst_SLEEP(opCode);
                }
                else
                {
                    return instInvalid(opCode);
                }
            }
            else
            {
                // opCode = 00 0000 0xxx xx01
                if ( 9 == opCode )
                {
                    // opCode = 00 0000 0000 1001
                    return inst_RETFIE(opCode);
                }
                else
                {
                    return instInvalid(opCode);
                }
            }
        }
        else
        {
            // opCode = 00 0000 0xxx xxx0
            if ( opCode & 0x04  )
            {
                // opCode = 00 0000 xxxx x1x0
                if ( 100 == opCode )
                {
                    // opCode = 00 0000 0110 0100
                    return inst_CLRWDT(opCode);
                }
                else
                {
                    return instInvalid(opCode);
                }
            }
            else
            {
                if ( opCode & 0x08 )
                {
                    // opCode = 00 0000 0xxx 10x0
                    if ( 8 == opCode)
                    {
                        // opCode == 00 0000 0000 1000
                        return inst_RETURN(opCode);
                    }
                    else
                    {
                        return instInvalid(opCode);
                    }
                }
                else
                {
                    //opCode = 00 0000 0xxx 00x0
                    if ( ( opCode & 0x02 ) || ( opCode & 0x016 ) )
                    {
                        return instInvalid(opCode);
                    }
                    else
                    {
                        // opCode = 00 0000 0xx0 0000
                        return inst_NOP(opCode);
                    }
                }

            }
        }
    }

    // opCode = 00 0000 1xxx xxxx
    return inst_MOVWF(opCode);
}

int PIC8InstructionSet::instOPCode_000001 ( const unsigned int opCode )
{
    if ( opCode & 0x080 )
    {
        //opCode = 00 0001 1xxx xxxx
        return inst_CLRF(opCode);
    }
    else
    {
        // opCode = 00 0001 0xxx xxxx
        return inst_CLRW(opCode);
    }
}

int PIC8InstructionSet::instOPCode_01 ( const unsigned int opCode )
{
    if ( opCode & 0x02 )
    {
        if ( opCode & 0x01 )
        {
            // opCode = 01 11xx xxxx xxxx
            return inst_BTFSS(opCode);
        }
        else
        {
            // opCode = 01 10xx xxxx xxxx
            return inst_BTFSC(opCode);
        }
    }
    else
    {
        if ( opCode & 0x01 )
        {
            // opCode = 01 01xx xxxx xxxx
            return inst_BSF(opCode);
        }
        else
        {
            // opCode = 01 00xx xxxx xxxx
            return inst_BCF(opCode);
        }
    }
}

inline void PIC8InstructionSet::instructionEnter ( PIC8InsNames::Instructions instName )
{
    m_instructionCounter[instName]++;
    m_lastInstruction = instName;
}

// -----------------------------------------------------------------------------
// INSTRUCTIONS
// -----------------------------------------------------------------------------

int PIC8InstructionSet::instInvalid ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_NONE);

    logEvent(EVENT_CPU_ERR_INVALID_OPCODE, m_pc, opCode);
    return -1;
}

/*
 * Instruction: ADDWF
 * OP Code: 00 0111 dfff ffff - Add W and f
 * Operation: (W) + (f) → destination
 * Status affected: C, DC, Z
 */
int PIC8InstructionSet::inst_ADDWF ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_ADDWF);

    // Operands
    const unsigned int f = ( opCode & 0x03F );
    const unsigned int d = ( opCode & 0x080 );

    // Registers
    unsigned int valF = m_dataMemory->read(f);
    unsigned int valSReg = m_dataMemory->readFast(PIC8RegNames::STATUS);

    // Perform the operation (addition)
    unsigned int result = valF + m_W;

    // Set/Clear Carry bit in Status Register
    if ( result & 0x100 )
    {
        result &= 0xff;
        valSReg |= PIC8RegNames::STATUS_C;
    }
    else
    {
        valSReg &= ~(PIC8RegNames::STATUS_C);
    }

    // Set/Clear Zero bit in Status Register
    if ( 0 == result)
    {
        valSReg |= PIC8RegNames::STATUS_Z;
    }
    else
    {
        valSReg &= ~( PIC8RegNames::STATUS_Z );
    }

    // Write changes to Status Register
    m_dataMemory->writeFast(PIC8RegNames::STATUS, valSReg);

    // Store the result
    if ( 0 != d )
    {
        // d is 1, stored in f
        m_dataMemory->write(f, result);
    }
    else
    {
        // d is 0, stored in W
        m_W = result;
        logEvent(EVENT_ACCUMULATOR_WRITE, m_W);
    }

    return 1;
}

/*
 * Instruction: ANDWF
 * OP Code: 00 0101 dfff ffff - AND W and f
 * Operation: (W) • (f) → destination
 * Status affected: Z
 */
int PIC8InstructionSet::inst_ANDWF ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_ANDWF);

    // Operands
    const unsigned int f = ( opCode & 0x03F );
    const unsigned int d = ( opCode & 0x080 );

    // Registers
    unsigned int valF = m_dataMemory->read(f);
    unsigned int valSReg = m_dataMemory->readFast(PIC8RegNames::STATUS);

    // Perform the operation
    unsigned int result = ( m_W & valF );

    // Set/Clear Zero bit in Status Register
    if ( 0 == result )
    {
        valSReg |= PIC8RegNames::STATUS_Z;
    }
    else
    {
        valSReg &= ~(PIC8RegNames::STATUS_Z);
    }

    // Write changes to Status Register
    m_dataMemory->writeFast(PIC8RegNames::STATUS, valSReg);

    // Store the result
    if ( 0 != d )
    {
        // d is 1, stored in f
        m_dataMemory->write(f, result);
    }
    else
    {
        // d is 0, stored in W
        m_W = result;
        logEvent(EVENT_ACCUMULATOR_WRITE, m_W);
    }

    return 1;
}

/*
 * Instruction: CLRF
 * OP Code: 00 0001 1fff ffff - Clear f
 * Operation: 00h → f; 1 → Z
 * Status affected: Z
 */
int PIC8InstructionSet::inst_CLRF ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_CLRF);

    // Operands
    const unsigned int f = ( opCode & 0x03F );

    // Perform the operation
    m_dataMemory->write(f, 0x00);

    // Set Zero bit in Status Register
    unsigned int valSReg = m_dataMemory->readFast(PIC8RegNames::STATUS);
    valSReg |= PIC8RegNames::STATUS_Z;
    m_dataMemory->writeFast(PIC8RegNames::STATUS, valSReg);

    return 1;
}

/*
 * Instruction: CLRW
 * OP Code: 00 0111 0xxx xxxx - Clear W
 * Operation: 00h → W; 1 → Z
 * Status affected: Z
 */
int PIC8InstructionSet::inst_CLRW ( const unsigned int )
{
    instructionEnter(PIC8InsNames::INS_CLRW);

    // Perform the operation
    m_W = 0;
    logEvent(EVENT_ACCUMULATOR_WRITE, m_W);

    // Set Zero bit in Status Register
    unsigned int valSReg = m_dataMemory->readFast(PIC8RegNames::STATUS);
    valSReg |= PIC8RegNames::STATUS_Z;
    m_dataMemory->writeFast(PIC8RegNames::STATUS, valSReg);

    return 1;
}

/*
 * Instruction: COMF
 * OP Code: 00 1001 dfff ffff - Complement f
 * Operation: (~f) → destination
 * Status affected: Z
 */
int PIC8InstructionSet::inst_COMF ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_COMF);

    // Operands
    const unsigned int f = ( opCode & 0x03F );
    const unsigned int d = ( opCode & 0x080 );

    // Registers
    unsigned int valF = m_dataMemory->read(f);
    unsigned int valSReg = m_dataMemory->readFast(PIC8RegNames::STATUS);

    // Perform the operation
    unsigned int result = ( valF ^ 0xff );

    // Set/Clear Zero bit in Status Register
    if ( 0 == result )
    {
        valSReg |= PIC8RegNames::STATUS_Z;
    }
    else
    {
        valSReg &= ~(PIC8RegNames::STATUS_Z);
    }

    // Write changes to Status Register
    m_dataMemory->writeFast(PIC8RegNames::STATUS, valSReg);

    // Store the result
    if ( 0 != d )
    {
        // d is 1, stored in f
        m_dataMemory->write(f, result);
    }
    else
    {
        // d is 0, stored in W
        m_W = result;
        logEvent(EVENT_ACCUMULATOR_WRITE, m_W);
    }

    return 1;
}

/*
 * Instruction: DECF
 * OP Code: 00 0011 dfff ffff - Decrement f
 * Operation: (f) - 1 → destination
 * Status affected: Z
 */
int PIC8InstructionSet::inst_DECF ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_DECF);

    // Operands
    const unsigned int f = ( opCode & 0x03F );
    const unsigned int d = ( opCode & 0x080 );

    // Registers
    unsigned int valF = m_dataMemory->read(f);

    // Perform the operation
    unsigned int result = 0xff & ( valF - 1 );

    // Set/Clear Zero bit in Status Register
    unsigned int valSReg = m_dataMemory->readFast(PIC8RegNames::STATUS);
    if ( 0 == result )
    {
        valSReg |= PIC8RegNames::STATUS_Z;
    }
    else
    {
        valSReg &= ~(PIC8RegNames::STATUS_Z);
    }

    // Write changes to Status Register
    m_dataMemory->writeFast(PIC8RegNames::STATUS, valSReg);

    // Store the result
    if ( d != 0 )
    {
        // d is 1, stored in f
        m_dataMemory->write(f, result);
    }
    else
    {
        // d is 0, stored in W
        m_W = result;
        logEvent(EVENT_ACCUMULATOR_WRITE, m_W);
    }

    return 1;
}

/*
 * Instruction: DECFSZ
 * OP Code: 00 1011 dfff ffff - Decrement f, skip if 0
 * Operation: (f) - 1 → destination; skip if result = 0
 * Status affected: None
 */
int PIC8InstructionSet::inst_DECFSZ ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_DECFSZ);

    // Operands
    const unsigned int f = ( opCode & 0x03F );
    const unsigned int d = ( opCode & 0x080 );

    // Registers
    unsigned int valF = m_dataMemory->read(f);

    // Perform the operation
    unsigned int result = 0xff & ( valF - 1 );

    // Store the result
    if ( 0 != d )
    {
        // d is 1, stored in f
        m_dataMemory->write(f, result);
    }
    else
    {
        // d is 0, stored in W
        m_W = result;
        logEvent(EVENT_ACCUMULATOR_WRITE, m_W);
    }

    // Evaluate how many cycles it takes to execute, and conditionally skip the next instruction
    if ( result == 0)
    {
        incrPc(); // Skip the next instruction
        return 2;
    }
    else
    {
        return 1;
    }
}

/*
 * Instruction: INCF
 * OP Code: 00 1010 dfff ffff - Increment f
 * Operation: (f) + 1 → destination
 * Status affected: Z
 */
int PIC8InstructionSet::inst_INCF ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_INCF);

    // Operands
    const unsigned int f = ( opCode & 0x03F );
    const unsigned int d = ( opCode & 0x080 );

    // Registers
    unsigned int valF = m_dataMemory->read(f);
    unsigned int valSReg = m_dataMemory->readFast(PIC8RegNames::STATUS);

    // Perform the operation
    unsigned int result = ( 0xff & ( valF + 1 ) );

    // Set/Clear Zero bit in Status Register
    if ( 0 == result )
    {
        valSReg |= PIC8RegNames::STATUS_Z;
    }
    else
    {
        valSReg &= ~(PIC8RegNames::STATUS_Z);
    }

    // Write changes to Status Register
    m_dataMemory->writeFast(PIC8RegNames::STATUS, valSReg);

    // Store the result
    if ( 0 != d )
    {
        // d is 1, stored in f
        m_dataMemory->write(f, result);
    }
    else
    {
        // d is 0, stored in W
        m_W = result;
        logEvent(EVENT_ACCUMULATOR_WRITE, m_W);
    }

    return 1;
}

/*
 * Instruction: INCFSZ
 * OP Code: 00 1111 dfff ffff - Increment f, skip if 0
 * Operation: (f) + 1 → destination; skip if result = 0
 * Status affected: None
 */
int PIC8InstructionSet::inst_INCFSZ ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_INCFSZ);

    // Operands
    const unsigned int f = ( opCode & 0x03F );
    const unsigned int d = ( opCode & 0x080 );

    // Registers
    unsigned int valF = m_dataMemory->read(f);

    // Perform the operation
    unsigned int result = ( 0xff & ( valF + 1 ) );

    if ( 0 != d )
    {
        // d is 1, stored in f
        m_dataMemory->write(f, valF);
    }
    else
    {
        // d is 0, stored in W
        m_W = result;
        logEvent(EVENT_ACCUMULATOR_WRITE, m_W);
    }

    // Evaluate how many cycles it takes to execute, and conditionally skip the next instruction
    if ( 0 == result )
    {
        incrPc(); // Skip the next instruction
        return 2;
    }
    else
    {
        return 1;
    }
}

/*
 * Instruction: IORWF
 * OP Code: 00 0100 dfff ffff - Inclusive OR W with f
 * Operation: (W) v (f) → destination
 * Status affected: Z
 */
int PIC8InstructionSet::inst_IORWF ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_IORWF);

    // Operands
    const unsigned int f = ( opCode & 0x03F );
    const unsigned int d = ( opCode & 0x080 );

    // Registers
    unsigned int valF = m_dataMemory->read(f);
    unsigned int valSReg = m_dataMemory->readFast(PIC8RegNames::STATUS);

    // Perform the operation
    unsigned int result = ( m_W | valF );

    // Set/Clear Zero bit in Status Register
    if ( 0 == result )
    {
        valSReg |= PIC8RegNames::STATUS_Z;
    }
    else
    {
        valSReg &= ~(PIC8RegNames::STATUS_Z);
    }

    // Write changes to Status Register
    m_dataMemory->writeFast(PIC8RegNames::STATUS, valSReg);

    // Strore the result
    if ( 0 != d )
    {
        // d is 1, stored in f
        m_dataMemory->write(f, m_W);
    }
    else
    {
        // d is 0, stored in W
        m_W = result;
        logEvent(EVENT_ACCUMULATOR_WRITE, m_W);
    }

    return 1;
}

/*
 * Instruction: MOVF
 * OP Code: 00 1000 dfff ffff - Move f
 * Operation: (f) → destination
 * Status affected: Z
 */
int PIC8InstructionSet::inst_MOVF ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_MOVF);

    // Operands
    const unsigned int f = ( opCode & 0x03F );
    const unsigned int d = ( opCode & 0x080 );

    // Registers
    unsigned int valF = m_dataMemory->read(f);
    unsigned int valSReg = m_dataMemory->readFast(PIC8RegNames::STATUS);

    // Set/Clear Zero bit in Status Register
    if ( 0 == valF )
    {
        valSReg |= PIC8RegNames::STATUS_Z;
    }
    else
    {
        valSReg &= ~(PIC8RegNames::STATUS_Z);
    }

    // Write changes to Status Register
    m_dataMemory->writeFast(PIC8RegNames::STATUS, valSReg);

    // Strore the result
    if ( 0 == d )
    {
        // d is 0, stored in W
        m_W = valF;
        logEvent(EVENT_ACCUMULATOR_WRITE, m_W);
    }

    return 1;
}

/*
 * Instruction: MOVWF
 * OP Code: 00 0000 1fff ffff - Move W to f
 * Operation: (W) → f
 * Status affected: None
 */
int PIC8InstructionSet::inst_MOVWF ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_MOVWF);

    // Operands
    const unsigned int f = ( opCode & 0x03F );

    // Perform the operation
    m_dataMemory->write(f, m_W);

    return 1;
}

/*
 * Instruction: NOP
 * OP Code: 00 0000 0xx0 0000 - No operation
 * Operation: No operation
 * Status affected: None
 */
int PIC8InstructionSet::inst_NOP ( const unsigned int )
{
    instructionEnter(PIC8InsNames::INS_NOP);
    return 1;
}

/*
 * Instruction: RLF
 * OP Code: 00 1101 dfff ffff - Rotate Left f through Carry
 * Operation: Content of register 'f' are rotated 1 bit to the left through Carry Flag.
 *            If 'd' is 0 the result is placed in the W register.
 *            If 'd' is 1 the result is stored back in the register 'f'.
 * Status affected: C
 */
int PIC8InstructionSet::inst_RLF ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_RLF);

    // Operands
    const unsigned int f = ( opCode & 0x03F );
    const unsigned int d = ( opCode & 0x080 );

    // Registers
    unsigned int valF = m_dataMemory->read(f);
    unsigned int valSReg = m_dataMemory->readFast(PIC8RegNames::STATUS);

    // Perform the operation
    unsigned int result = ( valF << 1 );

    // Add the carry flag to the result
    if ( valSReg & PIC8RegNames::STATUS_C )
    {
        result |= 0x01;
    }

    // Change the carry flag in the processor status register
    if ( 0x100 & result )
    {
        // Set Carry
        valSReg |= PIC8RegNames::STATUS_C;

        // The result has to be 8-bit only
        result &= 0xff;
    }
    else
    {
        // Clear Carry
        valSReg &= ~(PIC8RegNames::STATUS_C);
    }

    m_dataMemory->writeFast(PIC8RegNames::STATUS, valSReg);

    // Strore the result
    if ( 0 != d )
    {
        // d is 1, stored in f
        m_dataMemory->write(f, m_W);
    }
    else
    {
        // d is 0, stored in W
        m_W = result;
        logEvent(EVENT_ACCUMULATOR_WRITE, m_W);
    }

    return 1;
}

/*
 * Instruction: RRF
 * OP Code: 00 1100 dfff ffff - Rotate Right f through Carry
 * Operation: Content of register 'f' are rotated 1 bit to the right through Carry Flag.
 *            If 'd' is 0 the result is placed in the W register.
 *            If 'd' is 1 the result is stored back in the register 'f'.
 * Status affected: C
 */
int PIC8InstructionSet::inst_RRF ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_RRF);

    // Operands
    const unsigned int f = ( opCode & 0x03F );
    const unsigned int d = ( opCode & 0x080 );

    // Registers
    unsigned int valF = m_dataMemory->read(f);
    unsigned int valSReg = m_dataMemory->readFast(PIC8RegNames::STATUS);

    // Perform the operation
    bool newCarry = ( bool ) ( 0x01 & valF );
    unsigned int result = ( valF >> 1 );

    // Add the carry flag to the result
    result |= ( ( valSReg & PIC8RegNames::STATUS_C ) ? 0x80 : 0x00 );

    // Change the carry flag in the processor status register
    if ( true == newCarry )
    {
        // Set Carry
        valSReg |= PIC8RegNames::STATUS_C;
    }
    else
    {
        // Clear Carry
        valSReg &= ~(PIC8RegNames::STATUS_C);
    }
    m_dataMemory->writeFast(PIC8RegNames::STATUS, valSReg);

    // Strore the result
    if ( 0 != d )
    {
        // d is 1, stored in f
        m_dataMemory->write(f, m_W);
    }
    else
    {
        // d is 0, stored in W
        m_W = result;
        logEvent(EVENT_ACCUMULATOR_WRITE, m_W);
    }

    return 1;
}

/*
 * Instruction: SUBWF
 * OP Code: 00 0010 dfff ffff - Substract W from f
 * Operation: (f) - (W) → destination
 * Status affected: C,DC,Z
 */
int PIC8InstructionSet::inst_SUBWF ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_SUBWF);

    // Operands
    const unsigned int f = ( opCode & 0x03F );
    const unsigned int d = ( opCode & 0x080 );

    // Registers
    unsigned int valF = m_dataMemory->read(f);
    unsigned int valSReg = m_dataMemory->readFast(PIC8RegNames::STATUS);

    // Perform the operation
    unsigned int result = ( valF - m_W );

    // Set/Clear Carry bit in Status Register
    if ( result & 0x100 )
    {
        valSReg |= PIC8RegNames::STATUS_C;

        // The result has to be an 8-bit value.
        result &= 0xff;
    }
    else
    {
        valSReg &= ~(PIC8RegNames::STATUS_C);
    }

    // Set/Clear Zero bit in Status Register
    if ( 0 == result )
    {
        valSReg |= PIC8RegNames::STATUS_Z;
    }
    else
    {
        valSReg &= ~(PIC8RegNames::STATUS_Z);
    }

    // Write changes to Status Register
    m_dataMemory->writeFast(PIC8RegNames::STATUS, valSReg);

    // Strore the result
    if ( 0 != d )
    {
        // d is 1, stored in f
        m_dataMemory->write(f, m_W);
    }
    else
    {
        // d is 0, stored in W
        m_W = result;
        logEvent(EVENT_ACCUMULATOR_WRITE, m_W);
    }

    return 1;
}

/*
 * Instruction: SWAPF
 * OP Code: 00 1110 dfff ffff - Swap nibbles in f
 * Operation: (f<3:0>) → destination<7:4>
 *            (f<7:4>) → destination<3:0>
 * Status affected: None
 */
int PIC8InstructionSet::inst_SWAPF ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_SWAPF);

    // Operands
    const unsigned int f = ( opCode & 0x03F );
    const unsigned int d = ( opCode & 0x080 );

    // Registers
    unsigned int valF = m_dataMemory->read(f);

    // Perform the operation
    unsigned int result = ( valF >> 4 ) | ( ( 0x0f & valF ) << 4 );

    // Strore the result
    if ( 0 != d )
    {
        // d is 1, stored in f
        m_dataMemory->write(f, result);
    }
    else
    {
        // d is 0, stored in W
        m_W = result;
        logEvent(EVENT_ACCUMULATOR_WRITE, result);
    }

    return 1;
}

/*
 * Instruction: XORWF
 * OP Code: 00 0110 dfff ffff - Exclusive OR W with f
 * Operation: (W) ⊕ (f) → destination
 * Status affected: Z
 */
int PIC8InstructionSet::inst_XORWF ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_XORWF);

    // Operands
    const unsigned int f = ( opCode & 0x03F );
    const unsigned int d = ( opCode & 0x080 );

    // Registers
    unsigned int valF = m_dataMemory->read(f);

    // Perform the operation
    unsigned int result = m_W ^ valF;

    // Set/Clear Zero bit in Status Register
    unsigned int valSReg = m_dataMemory->readFast(PIC8RegNames::STATUS);
    if ( 0 == result )
    {
        valSReg |= PIC8RegNames::STATUS_Z;
    }
    else
    {
        valSReg &= ~(PIC8RegNames::STATUS_Z);
    }
    m_dataMemory->writeFast(PIC8RegNames::STATUS, valSReg);

    // Strore the result
    if ( 0 != d )
    {
        // d is 1, stored in f
        m_dataMemory->write(f, result);
    }
    else
    {
        // d is 0, stored in W
        m_W = result;
        logEvent(EVENT_ACCUMULATOR_WRITE, result);
    }

    return 1;
}

/*
 * Instruction: BCF
 * OP Code: 01 00bb bfff ffff - Bit Clear f
 * Operation: 0 → f<b>
 * Status affected: None
 */
int PIC8InstructionSet::inst_BCF ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_BCF);

    // Operands
    const unsigned int f = ( opCode & 0x03F );
    const unsigned int b = ( opCode >> 7 ) & 0x07;

    // Registers
    unsigned int valF = m_dataMemory->read(f);

    // Perform the operation
    valF &= ~( 1 << b );
    m_dataMemory->write(f, valF);

    return 1;
}

/*
 * Instruction: BSF
 * OP Code: 01 01bb bfff ffff - Bit Set f
 * Operation: 1 → f<b>
 * Status affected: None
 */
int PIC8InstructionSet::inst_BSF ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_BSF);

    // Operands
    const unsigned int f = ( opCode & 0x03F );
    const unsigned int b = ( opCode >> 7 ) & 0x07;

    // Registers
    unsigned int valF = m_dataMemory->read(f);

    // Perform the operation
    valF |= ( 1 << b );
    m_dataMemory->write(f, valF);

    return 1;
}

/*
 * Instruction: BTFSC
 * OP Code: 01 10bb bfff ffff - Bit Test f, Skip if Clear
 * Operation: skip if (f<b>) = 0
 * Status affected: None
 */
int PIC8InstructionSet::inst_BTFSC ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_BTFSC);

    // Operands
    const unsigned int f = ( opCode & 0x03F );
    const unsigned int b = ( opCode >> 7 ) & 0x07;

    // Registers
    unsigned int valF = m_dataMemory->read(f);

    // Evaluate how many cycles it takes to execute, and conditionally skip the next instruction
    if ( valF & ( 1 << b ) )
    {
        return 1;
    }
    else
    {
        incrPc(); // Skip the next instruction
        return 2;
    }
}

/*
 * Instruction: BTFSS
 * OP Code: 01 11bb bfff ffff - Bit Test f, Skip if Set
 * Operation: skip if (f<b>) = 1
 * Status affected: None
 */
int PIC8InstructionSet::inst_BTFSS ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_BTFSS);

    // Operands
    const unsigned int f = ( opCode & 0x03F );
    const unsigned int b = ( opCode >> 7 ) & 0x07;

    // Registers
    unsigned int valF = m_dataMemory->read(f);

    // Evaluate how many cycles it takes to execute, and conditionally skip the next instruction
    if ( valF & ( 1 << b ) )
    {
        incrPc(); // Skip the next instruction
        return 2;
    }
    else
    {
        return 1;
    }
}

/*
 * Instruction: ADDLW
 * OP Code: 11 111x kkkk kkkk - Add literal and W
 * Operation: (W) + k → W
 * Status affected: C, DC, Z
 */
int PIC8InstructionSet::inst_ADDLW ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_ADDLW);

    // Operands
    const unsigned int k = ( opCode & 0x0FF );

    // Registers
    unsigned int valSReg = m_dataMemory->readFast(PIC8RegNames::STATUS);

    // Perform the operation
    m_W += k;
    logEvent(EVENT_ACCUMULATOR_WRITE, m_W);

    // Set/Clear Carry bit in Status Register
    if ( m_W & 0x100 )
    {
        valSReg |= PIC8RegNames::STATUS_C;

        // The result has to be an 8-bit value.
        m_W &= 0xff;
    }
    else
    {
        valSReg &= ~(PIC8RegNames::STATUS_C);
    }

    // Set/Clear Zero bit in Status Register
    if ( 0 == m_W )
    {
        valSReg |= PIC8RegNames::STATUS_Z;
    }
    else
    {
        valSReg &= ~(PIC8RegNames::STATUS_Z);
    }

    // Write changes to Status Register
    m_dataMemory->writeFast(PIC8RegNames::STATUS, valSReg);

    return 1;
}

/*
 * Instruction: ANDLW
 * OP Code: 11 1001 kkkk kkkk - AND literal with W
 * Operation: (W) • (k) → W
 * Status affected: Z
 */
int PIC8InstructionSet::inst_ANDLW ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_ANDLW);

    // Operands
    const unsigned int k = ( opCode & 0x0FF );

    // Registers
    unsigned int valSReg = m_dataMemory->readFast(PIC8RegNames::STATUS);

    // Perform the operation
    m_W &= k;
    logEvent(EVENT_ACCUMULATOR_WRITE, m_W);

    // Set/Clear Zero bit in Status Register
    if ( 0 == m_W )
    {
        valSReg |= PIC8RegNames::STATUS_Z;
    }
    else
    {
        valSReg &= ~(PIC8RegNames::STATUS_Z);
    }
    m_dataMemory->writeFast(PIC8RegNames::STATUS, valSReg);

    return 1;
}

/*
 * Instruction: CALL
 * OP Code: 10 0kkk kkkk kkkk - Call subroutine
 * Operation: (PC) + 1 → TOS; k → PC<10:0>
 * Status affected: None
 */
int PIC8InstructionSet::inst_CALL ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_CALL);

    // Operands
    const unsigned int k = ( opCode & 0x07FF );

    // Perform the operation
    // Write PC+1 in TOS
    m_stack->pushOnStack( incrPc() );

    logEvent(EVENT_CPU_CALL, m_pc, k);

    // Write k in PC<10:0>
    m_pc = k;

    return 2;
}

/*
 * Instruction: CLRWDT
 * OP Code: 00 0000 0110 0100 - Clear Watchdog Timer
 * Operation: 00h → WDT; 0 → WDT (prescaler count); 1 → TO; 1 → PD
 * Status affected: TO, PD
 */
int PIC8InstructionSet::inst_CLRWDT ( const unsigned int )
{
    instructionEnter(PIC8InsNames::INS_CLRWDT);

    // Set TO and PD bit in Status Register
    unsigned int valSReg = m_dataMemory->readFast(PIC8RegNames::STATUS);
    valSReg |= (PIC8RegNames::STATUS_TO | PIC8RegNames::STATUS_PD);
    m_dataMemory->writeFast(PIC8RegNames::STATUS, valSReg);

    // Clear WDT
    m_watchDogTimer->watchDogReset();
    m_watchDogTimer->watchDogPrescalerReset();

    return 1;
}

/*
 * Instruction: GOTO
 * OP Code: 10 1kkk kkkk kkkk - Go to address
 * Operation: k → PC<10:0>; PCLATH<4:3> → PC<12:11>
 * Status affected: None
 */
int PIC8InstructionSet::inst_GOTO ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_GOTO);

    // Operands
    const unsigned int k = ( opCode & 0x07FF );

    /*
     * Perform the operation
     */
    // Set upper bits of PC
    unsigned int pcUp = m_dataMemory->readFast(PIC8RegNames::PCLATH);
    pcUp = ((pcUp >> 3) << 12);
    m_pc = pcUp;
    // Set lower bits of PC
    m_pc |= k;

    return 2;
}

/*
 * Instruction: IORLW
 * OP Code: 11 1000 kkkk kkkk - Inclusive OR literal with W
 * Operation: (W) v k → W
 * Status affected: Z
 */
int PIC8InstructionSet::inst_IORLW ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_IORLW);

    // Operands
    const unsigned int k = ( opCode & 0x0FF );

    // Registers
    unsigned int valSReg = m_dataMemory->readFast(PIC8RegNames::STATUS);

    // Perform the operation
    m_W |= k;
    logEvent(EVENT_ACCUMULATOR_WRITE, m_W);

    // Set/Clear Zero bit in Status Register
    if ( 0 == m_W )
    {
        valSReg |= PIC8RegNames::STATUS_Z;
    }
    else
    {
        valSReg &= ~(PIC8RegNames::STATUS_Z);
    }
    m_dataMemory->writeFast(PIC8RegNames::STATUS, valSReg);

    return 1;
}

/*
 * Instruction: MOVLW
 * OP Code: 11 00xx kkkk kkkk - Mov literal to W
 * Operation: k → W
 * Status affected: None
 */
int PIC8InstructionSet::inst_MOVLW ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_MOVLW);

    // Operands
    const unsigned int k = ( opCode & 0x0FF );

    // Perform the operation
    m_W = k;
    logEvent(EVENT_ACCUMULATOR_WRITE, m_W);

    return 1;
}

/*
 * Instruction: RETFIE
 * OP Code: 00 0000 0000 1001 - Return from interrupt
 * Operation: TOS -> PC; 1 -> GIE
 * Status affected: None
 */
int PIC8InstructionSet::inst_RETFIE ( const unsigned int )
{
    instructionEnter(PIC8InsNames::INS_RETFIE);

    // Set GIE bit in INTCON
    unsigned int valIntCon = m_dataMemory->readFast(PIC8RegNames::INTCON);
    valIntCon |= PIC8RegNames::INTCON_GIE;
    m_dataMemory->writeFast(PIC8RegNames::INTCON, valIntCon);

    // Write Top of Stack in Program Counter
    m_pc = m_stack->popFromStack();

    // Inform the interrupt subsystem about the return
    if ( false == m_interruptController->retfie() )
    {
        logEvent(EVENT_CPU_ERR_INVALID_RETI);
    }

    return 2;
}

/*
 * Instruction: RETLW
 * OP Code: 11 01xx kkkk kkkk - Return with literal in W
 * Operation: k → W; TOS -> PC
 * Status affected: None
 */
int PIC8InstructionSet::inst_RETLW ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_RETLW);

    // Operands
    const unsigned int k = ( opCode & 0x0FF );

    // Perform the operation
    m_W = k;
    logEvent(EVENT_ACCUMULATOR_WRITE, m_W);

    // Write TOS in PC
    m_pc = m_stack->popFromStack();

    return 2;
}

/*
 * Instruction: RETURN
 * OP Code: 00 0000 0000 1000 - Return from subroutine
 * Operation: TOS -> PC
 * Status affected: None
 */
int PIC8InstructionSet::inst_RETURN ( const unsigned int )
{
    instructionEnter(PIC8InsNames::INS_RETURN);

    // Perform the operation
    m_pc = m_stack->popFromStack();

    return 2;
}

/*
 * Instruction: SLEEP
 * OP Code: 00 0000 0110 0011 - Go into standby mode
 * Operation: 00h → WDT; 0 → WDT (prescaler count); 1 -> TO; 0 -> PD
 * Status affected: TO, PD
 */
int PIC8InstructionSet::inst_SLEEP ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_SLEEP);

    /*
     * If an interrupt occurs before the execution of a SLEEP instruction,
     * the SLEEP instruction will complete as a NOP.
     */
    if ( true == m_interruptController->isInterruptPending() )
    {
        logEvent(EVENT_CPU_MODE_CHANGE_CANCELED, m_pc);
        return inst_NOP(opCode);
    }

    // Perform the operation
    *m_processorMode = MCUSim::MD_SLEEP;
    logEvent(EVENT_CPU_MODE_CHANGED, m_pc, MCUSim::MD_SLEEP);

    // Set TO and clear PD bit in Status Register
    unsigned int valSReg = m_dataMemory->readFast(PIC8RegNames::STATUS);
    valSReg |= PIC8RegNames::STATUS_TO;
    valSReg &= ~PIC8RegNames::STATUS_PD;
    m_dataMemory->writeFast(PIC8RegNames::STATUS, valSReg);

    // Clear WDT
    m_watchDogTimer->watchDogReset();
    m_watchDogTimer->watchDogPrescalerReset();

    return 1;
}

/*
 * Instruction: SUBLW
 * OP Code: 11 110x kkkk kkkk - Substract W from literal
 * Operation: k - (W) → W
 * Status affected: C, DC, Z
 */
int PIC8InstructionSet::inst_SUBLW ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_SUBLW);

    // Operands
    const unsigned int k = ( opCode & 0x0FF );

    // Registers
    unsigned int valSReg = m_dataMemory->readFast(PIC8RegNames::STATUS);

    // Perform the operation
    m_W = ( k - m_W );
    logEvent(EVENT_ACCUMULATOR_WRITE, m_W);

    // Set/Clear Carry bit in Status Register
    if ( 0x100 & m_W )
    {
        valSReg &= ~(PIC8RegNames::STATUS_C);

        // The result has to be 8-bit only
        m_W &= 0xff;
    }
    else
    {
        valSReg |= PIC8RegNames::STATUS_C;
    }

    // Set/Clear Zero bit in Status Register
    if ( 0 == m_W )
    {
        valSReg |= PIC8RegNames::STATUS_Z;
    }
    else
    {
        valSReg &= ~(PIC8RegNames::STATUS_Z);
    }

    // Write changes to Status Register
    m_dataMemory->writeFast(PIC8RegNames::STATUS, valSReg);

    return 1;
}

/*
 * Instruction: XORLW
 * OP Code: 11 1010 kkkk kkkk - Exclusive OR literal with W
 * Operation: (W) ⊕ k → W
 * Status affected: Z
 */
int PIC8InstructionSet::inst_XORLW ( const unsigned int opCode )
{
    instructionEnter(PIC8InsNames::INS_XORLW);

    // Operands
    const unsigned int k = ( opCode & 0x0FF );

    // Registers
    unsigned int valSReg = m_dataMemory->readFast(PIC8RegNames::STATUS);

    // Perform the operation
    logEvent(EVENT_ACCUMULATOR_WRITE, m_W);
    m_W ^= k;

    // Set/Clear Zero bit in Status Register
    if ( 0 == m_W )
    {
        valSReg |= PIC8RegNames::STATUS_Z;
    }
    else
    {
        valSReg &= ~(PIC8RegNames::STATUS_Z);
    }

    // Write changes to Status Register
    m_dataMemory->writeFast(PIC8RegNames::STATUS, valSReg);

    return 1;
}
