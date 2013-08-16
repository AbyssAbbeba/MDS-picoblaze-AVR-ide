// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup PicoBlaze
 * @file PicoBlazeInstructionSet3.cxx
 */
// =============================================================================

#include "PicoBlazeInstructionSet3.h"

#include "PicoBlazeIO.h"
#include "PicoBlazeStack.h"
#include "PicoBlazeRegisters.h"
#include "PicoBlazeDataMemory.h"
#include "PicoBlazeProgramMemory.h"
#include "PicoBlazeInterruptController.h"

void ( PicoBlazeInstructionSet3 :: * const PicoBlazeInstructionSet3::m_opCodeDispatchTable [ 32 ] )
     ( const unsigned int opCode ) =
{
    & PicoBlazeInstructionSet3::inst_LOAD,       // opCode = 00 | 000x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst__,          // opCode = 00 | 001x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst_INPUT,      // opCode = 00 | 010x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst_FETCH,      // opCode = 00 | 011x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst__,          // opCode = 00 | 100x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst_AND,        // opCode = 00 | 101x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst_OR,         // opCode = 00 | 110x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst_XOR,        // opCode = 00 | 111x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst__,          // opCode = 01 | 000x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst_TEST,       // opCode = 01 | 001x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst_COMPARE,    // opCode = 01 | 010x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst__,          // opCode = 01 | 011x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst_ADD,        // opCode = 01 | 100x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst_ADDCY,      // opCode = 01 | 101x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst_SUB,        // opCode = 01 | 110x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst_SUBCY,      // opCode = 01 | 111x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst_10000,      // opCode = 10 | 000x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst__,          // opCode = 10 | 001x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst__,          // opCode = 10 | 010x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst__,          // opCode = 10 | 011x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst__,          // opCode = 10 | 100x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst_RETURN,     // opCode = 10 | 101x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst_OUTPUT,     // opCode = 10 | 110x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst_STORE,      // opCode = 10 | 111x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst_CALL,       // opCode = 11 | 000x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst__,          // opCode = 11 | 001x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst_JUMP,       // opCode = 11 | 010x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst__,          // opCode = 11 | 011x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst_RETURNI,    // opCode = 11 | 100x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst__,          // opCode = 11 | 101x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst_11110,      // opCode = 11 | 110x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet3::inst__           // opCode = 11 | 111x xxxx | xxxx xxxx
};

bool PicoBlazeInstructionSet3::isValid() const
{
    if ( MCUSim::FAMILY_KCPSM3 == m_config.m_dev )
    {
        return true;
    }
    else
    {
        return false;
    }
}

int PicoBlazeInstructionSet3::execInstruction()
{
    const int pcOrig = m_pc;
    unsigned int opCode = m_programMemory->readRaw(m_pc);
    incrPc();

    if ( opCode & 0xfffc0000 )
    {
        if ( true == m_config.m_ignoreUndefinedOpCodes )
        {
            return -1;
        }
        else
        {
            opCode &= 0x03ffff;
        }
    }

    // Execute instruction from dispatch table
    ( this ->* ( m_opCodeDispatchTable [ opCode >> 13 ] ) ) ( opCode );

    if ( pcOrig != m_pc )
    {
        logEvent ( EVENT_CPU_PC_CHANGED, m_pc );
    }

    return 1;
}

inline void PicoBlazeInstructionSet3::instructionEnter ( PicoBlazeInsNames::Instructions instName )
{
    m_instructionCounter [ instName ] ++;
    m_lastInstruction = instName;
}

void PicoBlazeInstructionSet3::inst_10000 ( const unsigned int opCode )
{
    switch ( 0x10ff & opCode )
    {
        case 0b1000: inst_SRA ( opCode ); break; // SRA Sx : 10 000 0 xxxx 0000 1000
        case 0b1010: inst_SRX ( opCode ); break; // SRX Sx : 10 000 0 xxxx 0000 1010
        case 0b1100: inst_RR  ( opCode ); break; // RR Sx  : 10 000 0 xxxx 0000 1100
        case 0b1110: inst_SR0 ( opCode ); break; // SR0 Sx : 10 000 0 xxxx 0000 1110
        case 0b1111: inst_SR1 ( opCode ); break; // SR1 Sx : 10 000 0 xxxx 0000 1111
        case 0b0000: inst_SLA ( opCode ); break; // SLA Sx : 10 000 0 xxxx 0000 0000
        case 0b0010: inst_RL  ( opCode ); break; // RL Sx  : 10 000 0 xxxx 0000 0010
        case 0b0100: inst_SLX ( opCode ); break; // SLX Sx : 10 000 0 xxxx 0000 0100
        case 0b0110: inst_SL0 ( opCode ); break; // SL0 Sx : 10 000 0 xxxx 0000 0110
        case 0b0111: inst_SL1 ( opCode ); break; // SL1 Sx : 10 000 0 xxxx 0000 0111

        default:
            // All other OP codes are considered to be invalid.
            inst__ ( opCode );
            break;
    }
}

void PicoBlazeInstructionSet3::inst_11110 ( const unsigned int opCode )
{
    if ( 0x3C001 == opCode )
    {
        // ENABLE INTERRUPT  : 11 1110 0000 0000 0001
        inst_ENABLE_INT ( opCode );
    }
    else if ( 0x3C000 == opCode )
    {
        // DISABLE INTERRUPT : 11 1110 0000 0000 0000
        inst_DISABLE_INT ( opCode );
    }
    else
    {
        // All other OP codes are considered to be invalid.
        inst__ ( opCode );
    }
}

// -----------------------------------------------------------------------------
// INSTRUCTIONS
// -----------------------------------------------------------------------------

void PicoBlazeInstructionSet3::inst__ ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_NONE );

    logEvent ( EVENT_CPU_ERR_INVALID_OPCODE, m_pc, opCode );
}

void PicoBlazeInstructionSet3::inst_JUMP ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_JUMP );

    // Extract operands from OP code.
    const unsigned int addr = ( opCode & 0x03ff );
    const unsigned int cond = ( opCode & 0x1c00 ) >> 10;

    // Evaluate jump conditions.
    switch ( cond )
    {
        case 0b100:
            // Jump, if ZERO.
            if ( false == m_statusFlags.m_zero )
            {
                return;
            }
            break;
        case 0b101:
            // Jump, if NOT ZERO.
            if ( true == m_statusFlags.m_zero )
            {
                return;
            }
            break;
        case 0b110:
            // Jump, if CARRY.
            if ( false == m_statusFlags.m_carry )
            {
                return;
            }
            break;
        case 0b111:
            // Jump, if NOT CARRY.
            if ( true == m_statusFlags.m_carry )
            {
                return;
            }
            break;
        default:
            // Uncoditional jump.
            break;
    }

    // Execute jump.
    setProgramCounter ( addr );
}

void PicoBlazeInstructionSet3::inst_CALL ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_CALL );

    // Extract operands from OP code.
    const unsigned int addr = ( opCode & 0x03ff );
    const unsigned int cond = ( opCode & 0x1c00 ) >> 10;

    // Evaluate call conditions.
    switch ( cond )
    {
        case 0b100:
            // Call, if ZERO.
            if ( false == m_statusFlags.m_zero )
            {
                return;
            }
            break;
        case 0b101:
            // Call, if NOT ZERO.
            if ( true == m_statusFlags.m_zero )
            {
                return;
            }
            break;
        case 0b110:
            // Call, if CARRY.
            if ( false == m_statusFlags.m_carry )
            {
                return;
            }
            break;
        case 0b111:
            // Call, if NOT CARRY.
            if ( true == m_statusFlags.m_carry )
            {
                return;
            }
            break;
        default:
            // Uncoditional call.
            break;
    }

    // Execute call.
    logEvent ( EVENT_CPU_CALL, m_pc, PicoBlazeInsNames::INS_CALL );
    m_stack->pushOnStack ( m_pc );
    setProgramCounter ( addr );
    m_actSubprogCounter++;
}

void PicoBlazeInstructionSet3::inst_RETURN ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_RETURN );

    // Verify OP code validity.
    if ( 0 != ( opCode & 0x3ff ) )
    {
        inst__ ( opCode );
        return;
    }

    // Extract operands from OP code.
    const unsigned int cond = ( opCode & 0x1c00 ) >> 10;

    // Evaluate return conditions.
    switch ( cond )
    {
        case 0b100:
            // Return, if ZERO.
            if ( false == m_statusFlags.m_zero )
            {
                return;
            }
            break;
        case 0b101:
            // Return, if NOT ZERO.
            if ( true == m_statusFlags.m_zero )
            {
                return;
            }
            break;
        case 0b110:
            // Return, if CARRY.
            if ( false == m_statusFlags.m_carry )
            {
                return;
            }
            break;
        case 0b111:
            // Return, if NOT CARRY.
            if ( true == m_statusFlags.m_carry )
            {
                return;
            }
            break;
        default:
            // Uncoditional call.
            break;
    }

    // Execute return from subprogram.
    if ( 0 == m_actSubprogCounter )
    {
        logEvent ( EVENT_CPU_ERR_INVALID_RET, m_pc );
    }
    else
    {
        logEvent ( EVENT_CPU_RETURN, m_pc );
        m_actSubprogCounter--;
    }
    setProgramCounter ( 1 + m_stack->popFromStack() );
}

void PicoBlazeInstructionSet3::inst_ADD ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_ADD );

    // Extract operands from OP code.
    const unsigned int sX   = ( opCode & 0xf00 ) >> 8;
    const unsigned int sYkk = ( opCode & 0x0ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    if ( 0x1000 & opCode )
    {
        // ADD sX, sY
        sXval += m_registers -> read ( sYkk >> 4 );
    }
    else
    {
        // ADD sX, kk
        sXval += sYkk;
    }

    // Modify status flags.
    m_statusFlags.m_carry = ( ( 0x100 &  sXval ) ? true : false );
    m_statusFlags.m_zero  = ( ( 0     == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( sXval & 0xff ) );
}

void PicoBlazeInstructionSet3::inst_ADDCY ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_ADDCY );

    // Extract operands from OP code.
    const unsigned int sX   = ( opCode & 0xf00 ) >> 8;
    const unsigned int sYkk = ( opCode & 0x0ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    if ( 0x1000 & opCode )
    {
        // ADDCY sX, sY
        sXval += m_registers -> read ( sYkk >> 4 );
    }
    else
    {
        // ADDCY sX, kk
        sXval += sYkk;
    }

    // Add carry flag to the result.
    if ( true == m_statusFlags.m_carry )
    {
        sXval++;
    }

    // Modify status flags.
    m_statusFlags.m_carry = ( ( 0x100 &  sXval ) ? true : false );
    m_statusFlags.m_zero  = ( ( 0     == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( sXval & 0xff ) );
}

void PicoBlazeInstructionSet3::inst_SUB ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SUB );

    // Extract operands from OP code.
    const unsigned int sX   = ( opCode & 0xf00 ) >> 8;
    const unsigned int sYkk = ( opCode & 0x0ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    if ( 0x1000 & opCode )
    {
        // SUB sX, sY
        sXval -= m_registers -> read ( sYkk >> 4 );
    }
    else
    {
        // SUB sX, kk
        sXval -= sYkk;
    }

    // Modify status flags.
    m_statusFlags.m_carry = ( ( 0x100 &  sXval ) ? true : false );
    m_statusFlags.m_zero  = ( ( 0     == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( sXval & 0xff ) );
}

void PicoBlazeInstructionSet3::inst_SUBCY ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SUBCY );

    // Extract operands from OP code.
    const unsigned int sX   = ( opCode & 0xf00 ) >> 8;
    const unsigned int sYkk = ( opCode & 0x0ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    if ( 0x1000 & opCode )
    {
        // SUBCY sX, sY
        sXval -= m_registers -> read ( sYkk >> 4 );
    }
    else
    {
        // SUBCY sX, kk
        sXval -= sYkk;
    }

    // Add carry flag to the result.
    if ( true == m_statusFlags.m_carry )
    {
        sXval--;
    }

    // Modify status flags.
    m_statusFlags.m_carry = ( ( 0x100 &  sXval ) ? true : false );
    m_statusFlags.m_zero  = ( ( 0     == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( sXval & 0xff ) );
}

void PicoBlazeInstructionSet3::inst_COMPARE ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_COMPARE );

    // Extract operands from OP code.
    const unsigned int sX   = ( opCode & 0xf00 ) >> 8;
    const unsigned int sYkk = ( opCode & 0x0ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    if ( 0x1000 & opCode )
    {
        // COMPARE sX, sY
        sXval -= m_registers -> read ( sYkk >> 4 );
    }
    else
    {
        // COMPARE sX, kk
        sXval -= sYkk;
    }

    // Modify status flags.
    m_statusFlags.m_carry = ( ( 0x100 &  sXval ) ? true : false );
    m_statusFlags.m_zero  = ( ( 0     == sXval ) ? true : false );
}

void PicoBlazeInstructionSet3::inst_RETURNI ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_RETURNI );

    if ( 0x38001 == opCode )
    {
        // RETURNI ENABLE
        m_statusFlags.m_inte = true;
    }
    else if ( 0x38000 == opCode )
    {
        // RETURNI DISABLE
        m_statusFlags.m_inte = false;
    }
    else
    {
        // Invalid instruction.
        inst__ ( opCode );
        return;
    }

    // Chech whether there is an ISR in progress to return from.
    if ( m_statusFlags.m_interrupted <= 0 )
    {
        m_statusFlags.m_interrupted = 1;
        logEvent ( EVENT_CPU_ERR_INVALID_RETI );
    }
    else
    {
        logEvent ( EVENT_CPU_RETURN_FROM_ISR, m_pc );
        m_interruptController->returni();
    }

    // Return from ISR (Interrupt Service Routine).
    setProgramCounter ( m_stack->popFromStack() );
    m_statusFlags.returni();
}

inline void PicoBlazeInstructionSet3::inst_ENABLE_INT ( const unsigned int )
{
    instructionEnter ( PicoBlazeInsNames::INS_ENABLE_INT );

    // Perform the operation.
    m_statusFlags.m_inte = true;
}

inline void PicoBlazeInstructionSet3::inst_DISABLE_INT ( const unsigned int )
{
    instructionEnter ( PicoBlazeInsNames::INS_DISABLE_INT );

    // Perform the operation.
    m_statusFlags.m_inte = false;
}

void PicoBlazeInstructionSet3::inst_LOAD ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_LOAD );

    // Extract operands from OP code.
    const unsigned int sX   = ( opCode & 0xf00 ) >> 8;
    const unsigned int sYkk = ( opCode & 0x0ff );

    if ( 0x1000 & opCode )
    {
        // LOAD sX, sY
        m_registers -> write ( sX, m_registers -> read ( sYkk >> 4 ) );
    }
    else
    {
        // LOAD sX, kk
        m_registers -> write ( sX, sYkk );
    }
}

void PicoBlazeInstructionSet3::inst_AND ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_AND );

    // Extract operands from OP code.
    const unsigned int sX   = ( opCode & 0xf00 ) >> 8;
    const unsigned int sYkk = ( opCode & 0x0ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    if ( 0x1000 & opCode )
    {
        // AND sX, sY
        sXval &= m_registers -> read ( sYkk >> 4 );
    }
    else
    {
        // AND sX, kk
        sXval &= sYkk;
    }

    // Modify status flags.
    m_statusFlags.m_carry = false;
    m_statusFlags.m_zero = ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet3::inst_OR ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_OR );

    // Extract operands from OP code.
    const unsigned int sX   = ( opCode & 0xf00 ) >> 8;
    const unsigned int sYkk = ( opCode & 0x0ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    if ( 0x1000 & opCode )
    {
        // OR sX, sY
        sXval |= m_registers -> read ( sYkk >> 4 );
    }
    else
    {
        // OR sX, kk
        sXval |= sYkk;
    }

    // Modify status flags.
    m_statusFlags.m_carry = false;
    m_statusFlags.m_zero = ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet3::inst_XOR ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_XOR );

    // Extract operands from OP code.
    const unsigned int sX   = ( opCode & 0xf00 ) >> 8;
    const unsigned int sYkk = ( opCode & 0x0ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    if ( 0x1000 & opCode )
    {
        // XOR sX, sY
        sXval ^= m_registers -> read ( sYkk >> 4 );
    }
    else
    {
        // XOR sX, kk
        sXval ^= sYkk;
    }

    // Modify status flags.
    m_statusFlags.m_carry = false;
    m_statusFlags.m_zero = ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet3::inst_TEST ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_TEST );

    // Extract operands from OP code.
    const unsigned int sX   = ( opCode & 0xf00 ) >> 8;
    const unsigned int sYkk = ( opCode & 0x0ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    if ( 0x1000 & opCode )
    {
        // TEST sX, sY
        sXval &= m_registers -> read ( sYkk >> 4 );
    }
    else
    {
        // TEST sX, kk
        sXval &= sYkk;
    }

    /*
     * Modify status flags.
     */
    // Set Z, if the result is zero.
    m_statusFlags.m_zero = ( ( 0 == sXval ) ? true : false );
    // Set C, if odd parity has been detected in the result.
    m_statusFlags.m_carry = false;
    for ( int i = 1; i < 0x100; i <<= 1 )
    {
        if ( 0 == ( sXval & i ) )
        {
            m_statusFlags.m_carry = !m_statusFlags.m_carry;
        }
    }
}

void PicoBlazeInstructionSet3::inst_STORE ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_STORE );

    // Extract operands from OP code.
    const unsigned int sX   = ( opCode & 0xf00 ) >> 8;
    const unsigned int sYSS = ( opCode & 0x0ff );

    // Perform the operation.
    if ( 0x1000 & opCode )
    {
        // STORE sX, (sY)
        m_dataMemory -> write ( ( 0x3f & m_registers -> read ( sYSS >> 4 ) ), m_registers -> read ( sX ) );
    }
    else
    {
        // STORE sX, SS
        m_dataMemory -> write ( ( 0x3f & sYSS ), m_registers -> read ( sX ) );
    }
}

void PicoBlazeInstructionSet3::inst_FETCH ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_FETCH );

    // Extract operands from OP code.
    const unsigned int sX   = ( opCode & 0xf00 ) >> 8;
    const unsigned int sYSS = ( opCode & 0x0ff );

    // Perform the operation.
    if ( 0x1000 & opCode )
    {
        // FETCH sX, (sY)
        m_registers -> write ( sX, m_dataMemory -> read ( 0x3f & m_registers -> read ( sYSS >> 4 ) ) );
    }
    else
    {
        // FETCH sX, SS
        m_registers -> write ( sX, m_dataMemory -> read ( ( 0x3f & sYSS ) ) );
    }
}

inline void PicoBlazeInstructionSet3::inst_SR0 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SR0 );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    m_statusFlags.m_carry = ( ( 0 == ( 0x01 & sXval ) ) ? false : true );
    sXval >>= 1;
    m_statusFlags.m_zero = ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

inline void PicoBlazeInstructionSet3::inst_SR1 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SR1 );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    m_statusFlags.m_zero = false;
    m_statusFlags.m_carry = ( ( 0 == ( 0x01 & sXval ) ) ? false : true );
    sXval >>= 1;
    sXval |= 0x80;

    // Save the result.
    m_registers -> write ( sX, sXval );
}

inline void PicoBlazeInstructionSet3::inst_SRX ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SRX );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    m_statusFlags.m_carry = ( ( 0 == ( 0x01 & sXval ) ) ? false : true );
    sXval = ( 0x80 & sXval ) | ( ( 0x7f & sXval ) >> 1 );
    m_statusFlags.m_zero = ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

inline void PicoBlazeInstructionSet3::inst_SRA ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SRA );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    bool carryOrig = m_statusFlags.m_carry;
    m_statusFlags.m_carry = ( ( 0 == ( 0x01 & sXval ) ) ? false : true );
    sXval >>= 1;
    if ( true == carryOrig )
    {
        sXval |= 0x80;
    }
    m_statusFlags.m_zero = ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

inline void PicoBlazeInstructionSet3::inst_RR ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_RR );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    m_statusFlags.m_carry = ( ( 0 == ( 0x01 & sXval ) ) ? false : true );
    sXval >>= 1;
    if ( true == m_statusFlags.m_carry )
    {
        sXval |= 0x80;
    }
    m_statusFlags.m_zero = ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

inline void PicoBlazeInstructionSet3::inst_SL0 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SL0 );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    sXval <<= 1;

    // Modify status flags.
    m_statusFlags.m_carry = ( ( 0 == ( 0x100 & sXval ) ) ? false : true );
    m_statusFlags.m_zero = ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( 0xff & sXval ) );
}

inline void PicoBlazeInstructionSet3::inst_SL1 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SL1 );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    sXval <<= 1;
    sXval  |= 1;

    // Modify status flags.
    m_statusFlags.m_carry = ( ( 0 == ( 0x100 & sXval ) ) ? false : true );
    m_statusFlags.m_zero = false;

    // Save the result.
    m_registers -> write ( sX, ( 0xff & sXval ) );
}

inline void PicoBlazeInstructionSet3::inst_SLX ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SLX );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    sXval = ( 0x01 & sXval ) | ( ( 0xfe & sXval ) << 1 );

    // Modify status flags.
    m_statusFlags.m_carry = ( ( 0 == ( 0x100 & sXval ) ) ? false : true );
    m_statusFlags.m_zero = ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( 0xff & sXval ) );
}

inline void PicoBlazeInstructionSet3::inst_SLA ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SLA );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    sXval <<= 1;
    if ( true == m_statusFlags.m_carry )
    {
        sXval |= 0x01;
    }

    // Modify status flags.
    m_statusFlags.m_carry = ( ( 0 == ( 0x100 & sXval ) ) ? false : true );
    m_statusFlags.m_zero = ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( 0xff & sXval ) );
}

inline void PicoBlazeInstructionSet3::inst_RL ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_RL );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    sXval <<= 1;
    if ( 0x100 & sXval )
    {
        sXval &= 0xff;
        sXval |= 0x01;
        m_statusFlags.m_carry = true;
    }
    else
    {
        m_statusFlags.m_carry = false;
    }
    m_statusFlags.m_zero = ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet3::inst_INPUT ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_INPUT );

    // Extract operands from OP code.
    const unsigned int sX   = ( opCode & 0xf00 ) >> 8;
    const unsigned int sYPP = ( opCode & 0x0ff );

    // Perform the operation.
    if ( 0x1000 & opCode )
    {
        // INPUT sX, (sY)
        m_registers -> write ( sX, m_io -> input ( m_registers -> read ( sYPP >> 4 ) ) );
    }
    else
    {
        // INPUT sX, PP
        m_registers -> write ( sX, m_io -> input ( sYPP ) );
    }
}

void PicoBlazeInstructionSet3::inst_OUTPUT ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_OUTPUT );

    // Extract operands from OP code.
    const unsigned int sX   = ( opCode & 0xf00 ) >> 8;
    const unsigned int sYPP = ( opCode & 0x0ff );

    // Perform the operation.
    if ( 0x1000 & opCode )
    {
        // OUTPUT sX, (sY)
        m_io -> output ( m_registers -> read ( sYPP >> 4 ), m_registers -> read ( sX ) );
    }
    else
    {
        // OUTPUT sX, PP
        m_io -> output ( sYPP, m_registers -> read ( sX ) );
    }
}
