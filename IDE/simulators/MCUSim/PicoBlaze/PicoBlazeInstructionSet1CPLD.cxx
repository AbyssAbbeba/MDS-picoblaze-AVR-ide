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
 * @file PicoBlazeInstructionSet1CPLD.cxx
 */
// =============================================================================

#include "PicoBlazeInstructionSet1CPLD.h"

#include "PicoBlazeIO.h"
#include "PicoBlazeStack.h"
#include "PicoBlazeRegisters.h"
#include "PicoBlazeDataMemory.h"
#include "PicoBlazeProgramMemory.h"
#include "PicoBlazeInterruptController.h"

void ( PicoBlazeInstructionSet1CPLD :: * const PicoBlazeInstructionSet1CPLD::m_opCodeDispatchTable [ 32 ] )
     ( const unsigned int opCode ) =
{
    & PicoBlazeInstructionSet1CPLD::inst_LOAD_sx_kk,   // opCode = 0000 0xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_AND_sx_kk,    // opCode = 0000 1xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_OR_sx_kk,     // opCode = 0001 0xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_XOR_sx_kk,    // opCode = 0001 1xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_ADD_sx_kk,    // opCode = 0010 0xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_ADDCY_sx_kk,  // opCode = 0010 1xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_SUB_sx_kk,    // opCode = 0011 0xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_SUBCY_sx_kk,  // opCode = 0011 1xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_LOAD_sx_sy,   // opCode = 0100 0xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_AND_sx_sy,    // opCode = 0100 1xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_OR_sx_sy,     // opCode = 0101 0xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_XOR_sx_sy,    // opCode = 0101 1xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_ADD_sx_sy,    // opCode = 0110 0xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_ADDCY_sx_sy,  // opCode = 0110 1xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_SUB_sx_sy,    // opCode = 0111 0xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_SUBCY_sx_sy,  // opCode = 0111 1xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_INPUT_sx_pp,  // opCode = 1000 0xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_OUTPUT_sx_pp, // opCode = 1000 1xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_RETURN,       // opCode = 1001 0xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst__,            // opCode = 1001 1xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_10100,        // opCode = 1010 0xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst__,            // opCode = 1010 1xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_RETURNI,      // opCode = 1011 0xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst__,            // opCode = 1011 1xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_INPUT_sx_sy,  // opCode = 1100 0xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_OUTPUT_sx_sy, // opCode = 1100 1xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_JUMP,         // opCode = 1101 0xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_CALL,         // opCode = 1101 1xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst__,            // opCode = 1110 0xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst__,            // opCode = 1110 1xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst_INTERRUPT,    // opCode = 1111 0xxx | xxxx xxxx
    & PicoBlazeInstructionSet1CPLD::inst__             // opCode = 1111 1xxx | xxxx xxxx
};

bool PicoBlazeInstructionSet1CPLD::isValid() const
{
    if ( MCUSim::FAMILY_KCPSM1CPLD == m_config.m_dev )
    {
        return true;
    }
    else
    {
        return false;
    }
}

int PicoBlazeInstructionSet1CPLD::execInstruction()
{
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
            opCode &= 0x00ffff;
        }
    }

    // Execute instruction from dispatch table
    ( this ->* ( m_opCodeDispatchTable [ opCode >> 11 ] ) ) ( opCode );

    if ( pcOrig != m_pc )
    {
        logEvent ( EVENT_CPU_PC_CHANGED, m_pc );
    }

    return 1;
}

inline void PicoBlazeInstructionSet1CPLD::instructionEnter ( PicoBlazeInsNames::Instructions instName )
{
    m_instructionCounter [ instName ] ++;
    m_lastInstruction = instName;
}

void PicoBlazeInstructionSet1CPLD::inst_10100 ( const unsigned int opCode )
{
    switch ( 0xff & opCode )
    {
        case 0b0000: inst_SLA ( opCode ); break; // SLA Sx : 1010 0xxx 0000 0000
        case 0b0010: inst_SLX ( opCode ); break; // SLX Sx : 1010 0xxx 0000 0010
        case 0b0100: inst_RL  ( opCode ); break; // RL Sx  : 1010 0xxx 0000 0100
        case 0b0110: inst_SL0 ( opCode ); break; // SL0 Sx : 1010 0xxx 0000 0110
        case 0b0111: inst_SL1 ( opCode ); break; // SL1 Sx : 1010 0xxx 0000 0111
        case 0b1000: inst_SRA ( opCode ); break; // SRA Sx : 1010 0xxx 0000 1000
        case 0b1010: inst_SRX ( opCode ); break; // SRX Sx : 1010 0xxx 0000 1010
        case 0b1100: inst_RR  ( opCode ); break; // RR Sx  : 1010 0xxx 0000 1100
        case 0b1110: inst_SR0 ( opCode ); break; // SR0 Sx : 1010 0xxx 0000 1110
        case 0b1111: inst_SR1 ( opCode ); break; // SR1 Sx : 1010 0xxx 0000 1111

        default:
            // All other OP codes are considered to be invalid.
            inst__ ( opCode );
            break;
    }
}

// -----------------------------------------------------------------------------
// INSTRUCTIONS
// -----------------------------------------------------------------------------

void PicoBlazeInstructionSet1CPLD::inst__ ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_NONE );

    logEvent ( EVENT_CPU_ERR_INVALID_OPCODE, m_pc, opCode );
}

void PicoBlazeInstructionSet1CPLD::inst_JUMP ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_JUMP );

    // Extract operands from OP code.
    const unsigned int addr = ( opCode & 0x00ff );
    const unsigned int cond = ( opCode & 0x0700 ) >> 8;

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

void PicoBlazeInstructionSet1CPLD::inst_CALL ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_CALL );

    // Extract operands from OP code.
    const unsigned int addr = ( opCode & 0x00ff );
    const unsigned int cond = ( opCode & 0x0700 ) >> 8;

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
    logEvent ( EVENT_CPU_CALL, m_pc, PicoBlazeInsNames::INS_CALL );
    m_stack->pushOnStack ( m_pc );
    setProgramCounter ( addr );
    m_actSubprogCounter++;
}

void PicoBlazeInstructionSet1CPLD::inst_RETURN ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_RETURN );

    // Extract operands from OP code.
    const unsigned int cond = ( opCode & 0x0700 ) >> 8;

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
    if ( 0 == m_actSubprogCounter )
    {
        logEvent ( EVENT_CPU_ERR_INVALID_RET, m_pc );
    }
    else
    {
        logEvent ( EVENT_CPU_RETURN, m_pc );
        m_actSubprogCounter--;
    }
    setProgramCounter ( m_stack->popFromStack() );
}

void PicoBlazeInstructionSet1CPLD::inst_ADD_sx_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_ADD );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;
    const unsigned int kk = ( opCode & 0x00ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation (ADD sX, kk).
    sXval += kk;

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    m_statusFlags -> setZero  ( ( 0     == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( sXval & 0xff ) );
}

void PicoBlazeInstructionSet1CPLD::inst_ADD_sx_sy ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_ADD );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;
    const unsigned int sY = ( opCode & 0x00e0 ) >> 5;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation (ADD sX, sY).
    sXval += m_registers -> read ( sY );

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    m_statusFlags -> setZero  ( ( 0     == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( sXval & 0xff ) );
}

void PicoBlazeInstructionSet1CPLD::inst_ADDCY_sx_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_ADDCY );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;
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
    m_statusFlags -> setZero  ( ( 0     == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( sXval & 0xff ) );
}

void PicoBlazeInstructionSet1CPLD::inst_ADDCY_sx_sy ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_ADDCY );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;
    const unsigned int sY = ( opCode & 0x00e0 ) >> 5;

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
    m_statusFlags -> setZero  ( ( 0     == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( sXval & 0xff ) );
}

void PicoBlazeInstructionSet1CPLD::inst_SUB_sx_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SUB );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;
    const unsigned int kk = ( opCode & 0x00ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation (SUB sX, kk).
    sXval -= kk;

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    m_statusFlags -> setZero  ( ( 0     == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( sXval & 0xff ) );
}

void PicoBlazeInstructionSet1CPLD::inst_SUB_sx_sy ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SUB );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;
    const unsigned int sY = ( opCode & 0x00e0 ) >> 5;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation (SUB sX, sY).
    sXval -= m_registers -> read ( sY );

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    m_statusFlags -> setZero  ( ( 0     == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( sXval & 0xff ) );
}

void PicoBlazeInstructionSet1CPLD::inst_SUBCY_sx_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SUBCY );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;
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
    m_statusFlags -> setZero  ( ( 0     == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( sXval & 0xff ) );
}

void PicoBlazeInstructionSet1CPLD::inst_SUBCY_sx_sy ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SUBCY );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;
    const unsigned int sY = ( opCode & 0x00e0 ) >> 5;

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
    m_statusFlags -> setZero  ( ( 0     == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( sXval & 0xff ) );
}

void PicoBlazeInstructionSet1CPLD::inst_RETURNI ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_RETURNI );

    if ( 0xb000 == opCode )
    {
        // RETURNI ENABLE
        m_statusFlags -> setInte ( true );
    }
    else if ( 0xb001 == opCode )
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

    // Chech whether there is an ISR in progress to return from.
    if ( m_statusFlags->getInterrupted() <= 0 )
    {
        m_statusFlags -> setInterrupted ( 1 );
        logEvent ( EVENT_CPU_ERR_INVALID_RETI );
    }
    else
    {
        logEvent ( EVENT_CPU_RETURN_FROM_ISR, m_pc );
        m_interruptController->returni();
    }

    // Return from ISR (Interrupt Service Routine).
    setProgramCounter ( m_stack->popFromStack() );
    m_statusFlags -> returni();
}

void PicoBlazeInstructionSet1CPLD::inst_INTERRUPT ( const unsigned int opCode )
{
    if ( 0xf000 == opCode )
    {
        // INTERRUPT ENABLE
        instructionEnter ( PicoBlazeInsNames::INS_ENABLE_INT );
        m_statusFlags -> setInte ( true );
    }
    else if ( 0xf001 == opCode )
    {
        // INTERRUPT DISABLE
        instructionEnter ( PicoBlazeInsNames::INS_DISABLE_INT );
        m_statusFlags -> setInte ( false );
    }
    else
    {
        // Invalid instruction.
        inst__ ( opCode );
    }
}

void PicoBlazeInstructionSet1CPLD::inst_LOAD_sx_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_LOAD );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;
    const unsigned int kk = ( opCode & 0x00ff );

    // LOAD sX, kk
    m_registers -> write ( sX, kk );
}

void PicoBlazeInstructionSet1CPLD::inst_LOAD_sx_sy ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_LOAD );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;
    const unsigned int sY = ( opCode & 0x00e0 ) >> 5;

    // LOAD sX, sY
    m_registers -> write ( sX, m_registers -> read ( sY ) );
}

void PicoBlazeInstructionSet1CPLD::inst_AND_sx_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_AND );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;
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

void PicoBlazeInstructionSet1CPLD::inst_AND_sx_sy ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_AND );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;
    const unsigned int sY = ( opCode & 0x00e0 ) >> 5;

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

void PicoBlazeInstructionSet1CPLD::inst_OR_sx_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_OR );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;
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

void PicoBlazeInstructionSet1CPLD::inst_OR_sx_sy ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_OR );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;
    const unsigned int sY = ( opCode & 0x00e0 ) >> 5;

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

void PicoBlazeInstructionSet1CPLD::inst_XOR_sx_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_XOR );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;
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

void PicoBlazeInstructionSet1CPLD::inst_XOR_sx_sy ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_XOR );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;
    const unsigned int sY = ( opCode & 0x00e0 ) >> 5;

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

inline void PicoBlazeInstructionSet1CPLD::inst_SR0 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SR0 );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    m_statusFlags -> setCarry ( ( 0 == ( 0x01 & sXval ) ) ? false : true );
    sXval >>= 1;
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

inline void PicoBlazeInstructionSet1CPLD::inst_SR1 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SR1 );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;

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

inline void PicoBlazeInstructionSet1CPLD::inst_SRX ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SRX );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    m_statusFlags -> setCarry ( ( 0 == ( 0x01 & sXval ) ) ? false : true );
    sXval = ( 0x80 & sXval ) | ( sXval >> 1 );
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

inline void PicoBlazeInstructionSet1CPLD::inst_SRA ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SRA );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;

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

inline void PicoBlazeInstructionSet1CPLD::inst_RR ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_RR );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;

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

inline void PicoBlazeInstructionSet1CPLD::inst_SL0 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SL0 );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    sXval <<= 1;

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0 == ( 0x100 & sXval ) ) ? false : true );
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( 0xff & sXval ) );
}

inline void PicoBlazeInstructionSet1CPLD::inst_SL1 ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SL1 );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    sXval <<= 1;
    sXval  |= 1;

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0 == ( 0x100 & sXval ) ) ? false : true );
    m_statusFlags -> setZero ( false );

    // Save the result.
    m_registers -> write ( sX, ( 0xff & sXval ) );
}

inline void PicoBlazeInstructionSet1CPLD::inst_SLX ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SLX );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    sXval = ( 0x01 & sXval ) | ( sXval << 1 );

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0 == ( 0x100 & sXval ) ) ? false : true );
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( 0xff & sXval ) );
}

inline void PicoBlazeInstructionSet1CPLD::inst_SLA ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SLA );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;

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
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( 0xff & sXval ) );
}

inline void PicoBlazeInstructionSet1CPLD::inst_RL ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_RL );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;

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

void PicoBlazeInstructionSet1CPLD::inst_INPUT_sx_pp ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_INPUT );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;
    const unsigned int PP = ( opCode & 0x00ff );

    // Perform the operation (INPUT sX, PP).
    m_registers -> write ( sX, m_io -> input ( PP ) );
}

void PicoBlazeInstructionSet1CPLD::inst_INPUT_sx_sy ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_INPUT );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;
    const unsigned int sY = ( opCode & 0x00e0 ) >> 5;

    // Perform the operation (INPUT sX, (sY)).
    m_registers -> write ( sX, m_io -> input ( m_registers -> read ( sY ) ) );
}

void PicoBlazeInstructionSet1CPLD::inst_OUTPUT_sx_pp ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_OUTPUT );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;
    const unsigned int PP = ( opCode & 0x00ff );

    // Perform the operation (OUTPUT sX, PP).
    m_io -> output ( PP, m_registers -> read ( sX ) );
}

void PicoBlazeInstructionSet1CPLD::inst_OUTPUT_sx_sy ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_OUTPUT );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0700 ) >> 8;
    const unsigned int sY = ( opCode & 0x00e0 ) >> 5;

    // Perform the operation (OUTPUT sX, (sY)).
    m_io -> output ( m_registers -> read ( sY ), m_registers -> read ( sX ) );
}
