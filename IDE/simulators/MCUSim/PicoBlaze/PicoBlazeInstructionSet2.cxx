// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup PicoBlaze
 * @file PicoBlazeInstructionSet2.cxx
 */
// =============================================================================

#include "PicoBlazeInstructionSet2.h"

#include "PicoBlazeIO.h"
#include "PicoBlazeStack.h"
#include "PicoBlazeRegisters.h"
#include "PicoBlazeDataMemory.h"
#include "PicoBlazeProgramMemory.h"
#include "PicoBlazeInterruptController.h"

void ( PicoBlazeInstructionSet2 :: * const PicoBlazeInstructionSet2::m_opCodeDispatchTable [ 32 ] )
     ( const unsigned int opCode ) =
{
    & PicoBlazeInstructionSet2::inst_LOAD0,   // opCode = 00 | 000x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_AND0,    // opCode = 00 | 001x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_OR0,     // opCode = 00 | 010x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_XOR0,    // opCode = 00 | 011x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_ADD0,    // opCode = 00 | 100x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_ADDCY0,  // opCode = 00 | 101x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_SUB0,    // opCode = 00 | 110x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_SUBCY0,  // opCode = 00 | 111x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_LOAD1,   // opCode = 01 | 000x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_AND1,    // opCode = 01 | 001x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_OR1,     // opCode = 01 | 010x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_XOR1,    // opCode = 01 | 011x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_ADD1,    // opCode = 01 | 100x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_ADDCY1,  // opCode = 01 | 101x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_SUB1,    // opCode = 01 | 110x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_SUBCY1,  // opCode = 01 | 111x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_INPUT0,  // opCode = 10 | 000x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_OUTPUT0, // opCode = 10 | 001x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_RETURN,  // opCode = 10 | 010x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst__,       // opCode = 10 | 011x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_10100,   // opCode = 10 | 100x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst__,       // opCode = 10 | 101x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_RETURNI, // opCode = 10 | 110x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst__,       // opCode = 10 | 111x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_INPUT1,  // opCode = 11 | 000x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_OUTPUT1, // opCode = 11 | 001x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_JUMP,    // opCode = 11 | 010x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_CALL,    // opCode = 11 | 011x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst__,       // opCode = 11 | 100x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst__,       // opCode = 11 | 101x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst_11110,   // opCode = 11 | 110x xxxx | xxxx xxxx
    & PicoBlazeInstructionSet2::inst__        // opCode = 11 | 111x xxxx | xxxx xxxx
};

bool PicoBlazeInstructionSet2::isValid() const
{
    return ( MCUSim::FAMILY_KCPSM2 == m_config.m_dev );
}

void PicoBlazeInstructionSet2::forceReturn()
{
    if ( m_statusFlags->getInterrupted() > 0 )
    {
        returnFromInterrupt();
    }
    else
    {
        returnFromSubroutine();
    }
}

int PicoBlazeInstructionSet2::execInstruction()
{
    const int pcOrig = m_pc;
    unsigned int opCode = m_programMemory->readRaw(m_pc);

    if ( opCode & 0xfffc0000 )
    {
        logEvent ( MCUSimEventLogger::FLAG_HI_PRIO, EVENT_CPU_ERR_UNDEFINED_OPCODE, m_pc );

        if ( true == m_config.m_ignoreUndefinedOpCodes )
        {
            incrPc();
            logEvent ( EVENT_CPU_PC_CHANGED, m_pc );
            return -1;
        }
        else
        {
            opCode &= 0x03ffff;
        }
    }

    incrPc();

    // Execute instruction from dispatch table
    ( this ->* ( m_opCodeDispatchTable [ opCode >> 13 ] ) ) ( opCode );

    if ( pcOrig != m_pc )
    {
        logEvent ( EVENT_CPU_PC_CHANGED, m_pc );
    }

    return 1;
}

inline void PicoBlazeInstructionSet2::instructionEnter ( PicoBlazeInsNames::Instructions instName )
{
    m_instructionCounter [ instName ] ++;
    m_lastInstruction = instName;
}

void PicoBlazeInstructionSet2::inst_10100 ( const unsigned int opCode )
{
    switch ( 0xff & opCode )
    {
        case 0b1000: inst_SRA ( opCode ); break; // SRA Sx : 10 100 x xxxx 0000 1000
        case 0b1010: inst_SRX ( opCode ); break; // SRX Sx : 10 100 x xxxx 0000 1010
        case 0b1100: inst_RR  ( opCode ); break; // RR Sx  : 10 100 x xxxx 0000 1100
        case 0b1110: inst_SR0 ( opCode ); break; // SR0 Sx : 10 100 x xxxx 0000 1110
        case 0b1111: inst_SR1 ( opCode ); break; // SR1 Sx : 10 100 x xxxx 0000 1111
        case 0b0000: inst_SLA ( opCode ); break; // SLA Sx : 10 100 x xxxx 0000 0000
        case 0b0010: inst_RL  ( opCode ); break; // RL Sx  : 10 100 x xxxx 0000 0010
        case 0b0100: inst_SLX ( opCode ); break; // SLX Sx : 10 100 x xxxx 0000 0100
        case 0b0110: inst_SL0 ( opCode ); break; // SL0 Sx : 10 100 x xxxx 0000 0110
        case 0b0111: inst_SL1 ( opCode ); break; // SL1 Sx : 10 100 x xxxx 0000 0111

        default:
            // All other OP codes are considered to be invalid.
            inst__ ( opCode );
            break;
    }
}

void PicoBlazeInstructionSet2::inst_11110 ( const unsigned int opCode )
{
    if ( 0x3C001 == opCode )
    {
        // ENABLE INTERRUPT  : 11 1100 0000 0000 0001
        inst_ENABLE_INT ( opCode );
    }
    else if ( 0x3C000 == opCode )
    {
        // DISABLE INTERRUPT : 11 1100 0000 0000 0000
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

void PicoBlazeInstructionSet2::inst__ ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_NONE );

    logEvent ( MCUSimEventLogger::FLAG_HI_PRIO, EVENT_CPU_ERR_INVALID_OPCODE, m_pc, opCode );
}

void PicoBlazeInstructionSet2::inst_JUMP ( const unsigned int opCode )
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
            if ( false == m_statusFlags -> getZero() )
            {
                return;
            }
            break;
        case 0b101:
            // Jump, if NOT ZERO.
            if ( true == m_statusFlags -> getZero() )
            {
                return;
            }
            break;
        case 0b110:
            // Jump, if CARRY.
            if ( false == m_statusFlags -> getCarry() )
            {
                return;
            }
            break;
        case 0b111:
            // Jump, if NOT CARRY.
            if ( true == m_statusFlags -> getCarry() )
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

void PicoBlazeInstructionSet2::inst_CALL ( const unsigned int opCode )
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
            if ( false == m_statusFlags -> getZero() )
            {
                return;
            }
            break;
        case 0b101:
            // Call, if NOT ZERO.
            if ( true == m_statusFlags -> getZero() )
            {
                return;
            }
            break;
        case 0b110:
            // Call, if CARRY.
            if ( false == m_statusFlags -> getCarry() )
            {
                return;
            }
            break;
        case 0b111:
            // Call, if NOT CARRY.
            if ( true == m_statusFlags -> getCarry() )
            {
                return;
            }
            break;
        default:
            // Uncoditional call.
            break;
    }

    // Execute call.
    logEvent ( MCUSimEventLogger::FLAG_HI_PRIO, EVENT_CPU_CALL, m_pc, addr );
    m_stack->pushOnStack ( m_pc );
    setProgramCounter ( addr );
    m_actSubprogCounter++;
}

void PicoBlazeInstructionSet2::inst_RETURN ( const unsigned int opCode )
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
            if ( false == m_statusFlags -> getZero() )
            {
                return;
            }
            break;
        case 0b101:
            // Return, if NOT ZERO.
            if ( true == m_statusFlags -> getZero() )
            {
                return;
            }
            break;
        case 0b110:
            // Return, if CARRY.
            if ( false == m_statusFlags -> getCarry() )
            {
                return;
            }
            break;
        case 0b111:
            // Return, if NOT CARRY.
            if ( true == m_statusFlags -> getCarry() )
            {
                return;
            }
            break;
        default:
            // Uncoditional call.
            break;
    }

    // Execute return from subprogram.
    returnFromSubroutine();
}

void PicoBlazeInstructionSet2::inst_ADD0 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_ADD );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;
    const unsigned int kk = ( opCode & 0x00ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation (ADD sX, kk).
    sXval += kk;

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    sXval &= 0xff;
    m_statusFlags -> setZero  ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet2::inst_ADD1 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_ADD );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;
    const unsigned int sY = ( opCode & 0x00f8 ) >> 3;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation (ADD sX, sY).
    sXval += m_registers -> read ( sY );

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    sXval &= 0xff;
    m_statusFlags -> setZero  ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet2::inst_ADDCY0 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_ADDCY );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;
    const unsigned int kk = ( opCode & 0x00ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation (ADDCY sX, kk).
    sXval += kk;

    // Add carry flag to the result.
    if ( true == m_statusFlags -> getCarry() )
    {
        sXval++;
    }

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    sXval &= 0xff;
    m_statusFlags -> setZero  ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet2::inst_ADDCY1 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_ADDCY );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;
    const unsigned int sY = ( opCode & 0x00f8 ) >> 3;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation (ADDCY sX, sY).
    sXval += m_registers -> read ( sY );

    // Add carry flag to the result.
    if ( true == m_statusFlags -> getCarry() )
    {
        sXval++;
    }

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    sXval &= 0xff;
    m_statusFlags -> setZero  ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet2::inst_SUB0 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SUB );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;
    const unsigned int kk = ( opCode & 0x00ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation (SUB sX, kk).
    sXval -= kk;

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    sXval &= 0xff;
    m_statusFlags -> setZero  ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet2::inst_SUB1 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SUB );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;
    const unsigned int sY = ( opCode & 0x00f8 ) >> 3;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation (SUB sX, sY).
    sXval -= m_registers -> read ( sY );

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    sXval &= 0xff;
    m_statusFlags -> setZero  ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet2::inst_SUBCY0 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SUBCY );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;
    const unsigned int kk = ( opCode & 0x00ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation (SUBCY sX, kk).
    sXval -= kk;

    // Add carry flag to the result.
    if ( true == m_statusFlags -> getCarry() )
    {
        sXval--;
    }

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    sXval &= 0xff;
    m_statusFlags -> setZero  ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet2::inst_SUBCY1 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SUBCY );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;
    const unsigned int sY = ( opCode & 0x00f8 ) >> 3;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation (SUBCY sX, sY).
    sXval -= m_registers -> read ( sY );

    // Add carry flag to the result.
    if ( true == m_statusFlags -> getCarry() )
    {
        sXval--;
    }

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    sXval &= 0xff;
    m_statusFlags -> setZero  ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet2::inst_RETURNI ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_RETURNI );

    if ( 0x2C001 == opCode )
    {
        // RETURNI ENABLE
        m_statusFlags -> setInte ( true );
    }
    else if ( 0x2C000 == opCode )
    {
        // RETURNI DISABLE
        m_statusFlags -> setInte ( false );
    }
    else
    {
        // Invalid instruction.
        inst__ ( opCode );
        return;
    }

    // Return from ISR (Interrupt Service Routine).
    returnFromInterrupt();
}

inline void PicoBlazeInstructionSet2::inst_ENABLE_INT ( const unsigned int )
{
    instructionEnter ( PicoBlazeInsNames::INS_ENABLE_INT );

    // Perform the operation.
    m_statusFlags -> setInte ( true );
}

inline void PicoBlazeInstructionSet2::inst_DISABLE_INT ( const unsigned int )
{
    instructionEnter ( PicoBlazeInsNames::INS_DISABLE_INT );

    // Perform the operation.
    m_statusFlags -> setInte ( false );
}

void PicoBlazeInstructionSet2::inst_LOAD0 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_LOAD );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;
    const unsigned int kk = ( opCode & 0x00ff );

    // LOAD sX, kk
    m_registers -> write ( sX, kk );
}

void PicoBlazeInstructionSet2::inst_LOAD1 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_LOAD );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;
    const unsigned int sY = ( opCode & 0x00f8 ) >> 3;

    // LOAD sX, sY
    m_registers -> write ( sX, m_registers -> read ( sY ) );
}

void PicoBlazeInstructionSet2::inst_AND0 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_AND );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;
    const unsigned int kk = ( opCode & 0x00ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation (AND sX, kk).
    sXval &= kk;

    // Modify status flags.
    m_statusFlags -> setCarry ( false );
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet2::inst_AND1 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_AND );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;
    const unsigned int sY = ( opCode & 0x00f8 ) >> 3;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation (AND sX, sY).
    sXval &= m_registers -> read ( sY );

    // Modify status flags.
    m_statusFlags -> setCarry ( false );
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet2::inst_OR0 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_OR );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;
    const unsigned int kk = ( opCode & 0x00ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation (OR sX, kk).
    sXval |= kk;

    // Modify status flags.
    m_statusFlags -> setCarry ( false );
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet2::inst_OR1 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_OR );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;
    const unsigned int sY = ( opCode & 0x00f8 ) >> 3;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation (OR sX, sY).
    sXval |= m_registers -> read ( sY );

    // Modify status flags.
    m_statusFlags -> setCarry ( false );
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet2::inst_XOR0 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_XOR );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;
    const unsigned int kk = ( opCode & 0x00ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation (XOR sX, kk).
    sXval ^= kk;

    // Modify status flags.
    m_statusFlags -> setCarry ( false );
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet2::inst_XOR1 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_XOR );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;
    const unsigned int sY = ( opCode & 0x00f8 ) >> 3;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation (XOR sX, sY).
    sXval ^= m_registers -> read ( sY );

    // Modify status flags.
    m_statusFlags -> setCarry ( false );
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

inline void PicoBlazeInstructionSet2::inst_SR0 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SR0 );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    m_statusFlags -> setCarry ( ( 0 == ( 0x01 & sXval ) ) ? false : true );
    sXval >>= 1;
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

inline void PicoBlazeInstructionSet2::inst_SR1 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SR1 );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    m_statusFlags -> setZero ( false );
    m_statusFlags -> setCarry ( ( 0 == ( 0x01 & sXval ) ) ? false : true );
    sXval >>= 1;
    sXval |= 0x80;

    // Save the result.
    m_registers -> write ( sX, sXval );
}

inline void PicoBlazeInstructionSet2::inst_SRX ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SRX );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    m_statusFlags -> setCarry ( ( 0 == ( 0x01 & sXval ) ) ? false : true );
    sXval = ( 0x80 & sXval ) | ( sXval >> 1 );
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

inline void PicoBlazeInstructionSet2::inst_SRA ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SRA );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    bool carryOrig = m_statusFlags -> getCarry();
    m_statusFlags -> setCarry ( ( 0 == ( 0x01 & sXval ) ) ? false : true );
    sXval >>= 1;
    if ( true == carryOrig )
    {
        sXval |= 0x80;
    }
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

inline void PicoBlazeInstructionSet2::inst_RR ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_RR );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    m_statusFlags -> setCarry ( ( 0 == ( 0x01 & sXval ) ) ? false : true );
    sXval >>= 1;
    if ( true == m_statusFlags -> getCarry() )
    {
        sXval |= 0x80;
    }
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

inline void PicoBlazeInstructionSet2::inst_SL0 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SL0 );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    sXval <<= 1;

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0 == ( 0x100 & sXval ) ) ? false : true );
    sXval &= 0xff;
    m_statusFlags -> setZero  ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

inline void PicoBlazeInstructionSet2::inst_SL1 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SL1 );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    sXval <<= 1;
    sXval  |= 1;

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0 == ( 0x100 & sXval ) ) ? false : true );
    sXval &= 0xff;
    m_statusFlags -> setZero  ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

inline void PicoBlazeInstructionSet2::inst_SLX ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SLX );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    sXval = ( 0x01 & sXval ) | ( sXval << 1 );

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0 == ( 0x100 & sXval ) ) ? false : true );
    sXval &= 0xff;
    m_statusFlags -> setZero  ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

inline void PicoBlazeInstructionSet2::inst_SLA ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SLA );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    sXval <<= 1;
    if ( true == m_statusFlags -> getCarry() )
    {
        sXval |= 0x01;
    }

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0 == ( 0x100 & sXval ) ) ? false : true );
    sXval &= 0xff;
    m_statusFlags -> setZero  ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

inline void PicoBlazeInstructionSet2::inst_RL ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_RL );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    sXval <<= 1;
    if ( 0x100 & sXval )
    {
        sXval &= 0xff;
        sXval |= 0x01;
        m_statusFlags -> setCarry ( true );
    }
    else
    {
        m_statusFlags -> setCarry ( false );
    }
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet2::inst_INPUT0 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_INPUT );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;
    const unsigned int PP = ( opCode & 0x00ff );

    // Perform the operation (INPUT sX, PP).
    m_registers -> write ( sX, m_io -> input ( PP ) );
}

void PicoBlazeInstructionSet2::inst_INPUT1 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_INPUT );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;
    const unsigned int sY = ( opCode & 0x00f8 ) >> 3;

    // Perform the operation (INPUT sX, (sY)).
    m_registers -> write ( sX, m_io -> input ( m_registers -> read ( sY ) ) );
}

void PicoBlazeInstructionSet2::inst_OUTPUT0 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_OUTPUT );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;
    const unsigned int PP = ( opCode & 0x00ff );

    // Perform the operation (OUTPUT sX, PP).
    m_io -> output ( PP, m_registers -> read ( sX ) );
}

void PicoBlazeInstructionSet2::inst_OUTPUT1 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_OUTPUT );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x1f00 ) >> 8;
    const unsigned int sY = ( opCode & 0x00f8 ) >> 3;

    // Perform the operation (OUTPUT sX, (sY)).
    m_io -> output ( m_registers -> read ( sY ), m_registers -> read ( sX ) );
}

inline void PicoBlazeInstructionSet2::returnFromInterrupt()
{
    // Chech whether there is an ISR in progress to return from.
    if ( m_statusFlags->getInterrupted() <= 0 )
    {
        m_statusFlags -> setInterrupted ( 1 );
        logEvent ( MCUSimEventLogger::FLAG_HI_PRIO, EVENT_CPU_ERR_INVALID_RETI, m_pc );
    }
    else
    {
        logEvent ( MCUSimEventLogger::FLAG_HI_PRIO, EVENT_CPU_RETURN_FROM_ISR, m_pc );
        m_interruptController->returni();
    }

    // Return from ISR (Interrupt Service Routine).
    setProgramCounter ( m_stack->popFromStack() );
}

inline void PicoBlazeInstructionSet2::returnFromSubroutine()
{
    if ( 0 == m_actSubprogCounter )
    {
        logEvent ( MCUSimEventLogger::FLAG_HI_PRIO, EVENT_CPU_ERR_INVALID_RET, m_pc );
    }
    else
    {
        logEvent ( MCUSimEventLogger::FLAG_HI_PRIO, EVENT_CPU_RETURN, m_pc );
        m_actSubprogCounter--;
    }
    setProgramCounter ( m_stack->popFromStack() );
}
