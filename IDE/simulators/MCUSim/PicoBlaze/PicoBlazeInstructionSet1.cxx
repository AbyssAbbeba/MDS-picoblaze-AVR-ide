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
 * @file PicoBlazeInstructionSet1.cxx
 */
// =============================================================================

#include "PicoBlazeInstructionSet1.h"

#include "PicoBlazeIO.h"
#include "PicoBlazeStack.h"
#include "PicoBlazeRegisters.h"
#include "PicoBlazeDataMemory.h"
#include "PicoBlazeProgramMemory.h"
#include "PicoBlazeInterruptController.h"

void ( PicoBlazeInstructionSet1 :: * const PicoBlazeInstructionSet1::m_opCodeDispatchTable [ 16 ] )
     ( const unsigned int opCode ) =
{
    & PicoBlazeInstructionSet1::inst_LOAD_sx_kk,   // opCode = 0000 xxxx | xxxx xxxx
    & PicoBlazeInstructionSet1::inst_AND_sx_kk,    // opCode = 0001 xxxx | xxxx xxxx
    & PicoBlazeInstructionSet1::inst_OR_sx_kk,     // opCode = 0010 xxxx | xxxx xxxx
    & PicoBlazeInstructionSet1::inst_XOR_sx_kk,    // opCode = 0011 xxxx | xxxx xxxx
    & PicoBlazeInstructionSet1::inst_ADD_sx_kk,    // opCode = 0100 xxxx | xxxx xxxx
    & PicoBlazeInstructionSet1::inst_ADDCY_sx_kk,  // opCode = 0101 xxxx | xxxx xxxx
    & PicoBlazeInstructionSet1::inst_SUB_sx_kk,    // opCode = 0110 xxxx | xxxx xxxx
    & PicoBlazeInstructionSet1::inst_SUBCY_sx_kk,  // opCode = 0111 xxxx | xxxx xxxx

    & PicoBlazeInstructionSet1::inst_1000,         // opCode = 1000 xxxx | xxxx xxxx
    & PicoBlazeInstructionSet1::inst_1001,         // opCode = 1001 xxxx | xxxx xxxx
    & PicoBlazeInstructionSet1::inst_INPUT_sx_pp,  // opCode = 1010 xxxx | xxxx xxxx
    & PicoBlazeInstructionSet1::inst_INPUT_sx_sy,  // opCode = 1011 xxxx | xxxx xxxx
    & PicoBlazeInstructionSet1::inst_1100,         // opCode = 1100 xxxx | xxxx xxxx
    & PicoBlazeInstructionSet1::inst_1101,         // opCode = 1101 xxxx | xxxx xxxx
    & PicoBlazeInstructionSet1::inst_OUTPUT_sx_pp, // opCode = 1110 xxxx | xxxx xxxx
    & PicoBlazeInstructionSet1::inst_OUTPUT_sx_sy, // opCode = 1111 xxxx | xxxx xxxx
};

void ( PicoBlazeInstructionSet1 :: * const PicoBlazeInstructionSet1::m_inst_1100_DTable [ 8 ] )
     ( const unsigned int opCode ) =
{
    & PicoBlazeInstructionSet1::inst_LOAD_sx_sy,   // opCode = 1100 xxxx | yyyy 0000
    & PicoBlazeInstructionSet1::inst_AND_sx_sy,    // opCode = 1100 xxxx | yyyy 0001
    & PicoBlazeInstructionSet1::inst_OR_sx_sy,     // opCode = 1100 xxxx | yyyy 0010
    & PicoBlazeInstructionSet1::inst_XOR_sx_sy,    // opCode = 1100 xxxx | yyyy 0011
    & PicoBlazeInstructionSet1::inst_ADD_sx_sy,    // opCode = 1100 xxxx | yyyy 0100
    & PicoBlazeInstructionSet1::inst_ADDCY_sx_sy,  // opCode = 1100 xxxx | yyyy 0101
    & PicoBlazeInstructionSet1::inst_SUB_sx_sy,    // opCode = 1100 xxxx | yyyy 0110
    & PicoBlazeInstructionSet1::inst_SUBCY_sx_sy,  // opCode = 1100 xxxx | yyyy 0111
};


void ( PicoBlazeInstructionSet1 :: * const PicoBlazeInstructionSet1::m_inst_1101_DTable [ 16 ] )
     ( const unsigned int opCode ) =
{
    & PicoBlazeInstructionSet1::inst_SLA_sx, // opCode = 1101 xxxx | 0000 0000
    & PicoBlazeInstructionSet1::inst__,      // opCode = 1101 xxxx | 0000 0001
    & PicoBlazeInstructionSet1::inst_RL_sx,  // opCode = 1101 xxxx | 0000 0010
    & PicoBlazeInstructionSet1::inst__,      // opCode = 1101 xxxx | 0000 0011

    & PicoBlazeInstructionSet1::inst_SLX_sx, // opCode = 1101 xxxx | 0000 0100
    & PicoBlazeInstructionSet1::inst__,      // opCode = 1101 xxxx | 0000 0101
    & PicoBlazeInstructionSet1::inst_SL0_sx, // opCode = 1101 xxxx | 0000 0110
    & PicoBlazeInstructionSet1::inst_SL1_sx, // opCode = 1101 xxxx | 0000 0111

    & PicoBlazeInstructionSet1::inst_SRA_sx, // opCode = 1101 xxxx | 0000 1000
    & PicoBlazeInstructionSet1::inst__,      // opCode = 1101 xxxx | 0000 1001
    & PicoBlazeInstructionSet1::inst_SRX_sx, // opCode = 1101 xxxx | 0000 1010
    & PicoBlazeInstructionSet1::inst__,      // opCode = 1101 xxxx | 0000 1011

    & PicoBlazeInstructionSet1::inst_RR_sx,  // opCode = 1101 xxxx | 0000 1100
    & PicoBlazeInstructionSet1::inst__,      // opCode = 1101 xxxx | 0000 1101
    & PicoBlazeInstructionSet1::inst_SR0_sx, // opCode = 1101 xxxx | 0000 1110
    & PicoBlazeInstructionSet1::inst_SR1_sx, // opCode = 1101 xxxx | 0000 1111
};

bool PicoBlazeInstructionSet1::isValid() const
{
    if ( MCUSim::FAMILY_KCPSM1 == m_config.m_dev )
    {
        return true;
    }
    else
    {
        return false;
    }
}

int PicoBlazeInstructionSet1::execInstruction()
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
            opCode &= 0x0ffff;
        }
    }

    // Execute instruction from dispatch table.
    ( this ->* ( m_opCodeDispatchTable [ opCode >> 12 ] ) ) ( opCode );

    if ( pcOrig != m_pc )
    {
        logEvent ( EVENT_CPU_PC_CHANGED, m_pc );
    }

    return 1;
}

inline void PicoBlazeInstructionSet1::instructionEnter ( PicoBlazeInsNames::Instructions instName )
{
    m_instructionCounter [ instName ] ++;
    m_lastInstruction = instName;
}

void PicoBlazeInstructionSet1::inst_1000 ( const unsigned int opCode )
{
    switch ( opCode )
    {
        // ENABLE INTERRUPT  : 1000 0000 0011 0000
        case 0x8030:
            inst_ENABLE_INT ( opCode );
            return;

        // DISABLE INTERRUPT : 1000 0000 0001 0000
        case 0x8010:
            inst_DISABLE_INT ( opCode );
            return;

        // RETURNI DISABLE : 1000 0000 1101 0000
        case 0x80D0:
            inst_RETURNI_DIS ( opCode );
            return;

        // RETURNI ENABLE : 1000 0000 1111 0000
        case 0x80F0:
            inst_RETURNI_ENA ( opCode );
            return;

        default:
            if ( opCode & 0x0200 )
            {
                // opCode = 1000 --1- ---- ----
                if ( opCode & 0x0100 )
                {
                    // CALL aaa : 1000 --11 aaaa aaaa
                    inst_CALL ( opCode );
                    return;
                }
            }
            else
            {
                // opCode = 1000 --0- ---- ----
                if ( opCode & 0x0100 )
                {
                    // JUMP aaa : 1000 --01 aaaa aaaa
                    inst_JUMP ( opCode );
                    return;
                }
                else if ( 0x80 == ( opCode & 0xff ) )
                {
                    // RETURN : 1000 --00 1000 0000
                    inst_RETURN ( opCode );
                    return;
                }
            }
    }
}

void PicoBlazeInstructionSet1::inst_1001 ( const unsigned int opCode )
{
    bool cond;

    // Evaluate execution condition.
    if ( opCode & 0x0800 )
    {
        cond = m_statusFlags -> getCarry();
    }
    else
    {
        cond = m_statusFlags -> getZero();
    }
    if ( opCode & 0x0400 )
    {
        cond = !cond;
    }

    switch ( ( opCode >> 8 ) & 0x3 )
    {
        case 0:
            if ( 0x80 == ( opCode & 0xff ) )
            {
                if ( true == cond )
                {
                    inst_RETURN ( opCode );
                }
            }
            else
            {
                inst__ ( opCode );
            }
            break;

        case 1:
            if ( true == cond )
            {
                inst_JUMP ( opCode );
            }
            break;

        case 2:
            inst__ ( opCode );
            break;

        case 3:
            if ( true == cond )
            {
                inst_CALL ( opCode );
            }
            break;
    }
}

void PicoBlazeInstructionSet1::inst_1100 ( const unsigned int opCode )
{
    // Execute instruction from dispatch table
    ( this ->* ( m_inst_1100_DTable [ opCode & 0x7 ] ) ) ( opCode );
}

void PicoBlazeInstructionSet1::inst_1101 ( const unsigned int opCode )
{
    // Execute instruction from dispatch table
    ( this ->* ( m_inst_1101_DTable [ opCode & 0xf ] ) ) ( opCode );
}

// -----------------------------------------------------------------------------
// INSTRUCTIONS
// -----------------------------------------------------------------------------

void PicoBlazeInstructionSet1::inst__ ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_NONE );

    logEvent ( EVENT_CPU_ERR_INVALID_OPCODE, m_pc, opCode );
}

inline void PicoBlazeInstructionSet1::inst_JUMP ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_JUMP );

    // Execute jump.
    setProgramCounter ( opCode & 0xff );
}

inline void PicoBlazeInstructionSet1::inst_CALL ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_CALL );

    // Execute call.
    logEvent ( EVENT_CPU_CALL, m_pc, PicoBlazeInsNames::INS_CALL );
    m_stack->pushOnStack ( m_pc );
    setProgramCounter ( opCode & 0xff );
    m_actSubprogCounter++;
}

inline void PicoBlazeInstructionSet1::inst_RETURN ( const unsigned int )
{
    instructionEnter ( PicoBlazeInsNames::INS_RETURN );

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

void PicoBlazeInstructionSet1::inst_ADD_sx_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_ADD );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;
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

void PicoBlazeInstructionSet1::inst_ADD_sx_sy ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_ADD );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;
    const unsigned int sY = ( opCode & 0x00f0 ) >> 4;

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

void PicoBlazeInstructionSet1::inst_ADDCY_sx_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_ADDCY );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;
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

void PicoBlazeInstructionSet1::inst_ADDCY_sx_sy ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_ADDCY );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;
    const unsigned int sY = ( opCode & 0x00f0 ) >> 4;

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

void PicoBlazeInstructionSet1::inst_SUB_sx_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SUB );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;
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

void PicoBlazeInstructionSet1::inst_SUB_sx_sy ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SUB );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;
    const unsigned int sY = ( opCode & 0x00f0 ) >> 4;

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

void PicoBlazeInstructionSet1::inst_SUBCY_sx_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SUBCY );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;
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

void PicoBlazeInstructionSet1::inst_SUBCY_sx_sy ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SUBCY );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;
    const unsigned int sY = ( opCode & 0x00f0 ) >> 4;

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

inline void PicoBlazeInstructionSet1::inst_RETURNI_DIS ( const unsigned int )
{
    instructionEnter ( PicoBlazeInsNames::INS_RETURNI );

    // RETURNI DISABLE
    m_statusFlags -> setInte ( false );
    inst_RETURNI_aux();
}

inline void PicoBlazeInstructionSet1::inst_RETURNI_ENA ( const unsigned int )
{
    instructionEnter ( PicoBlazeInsNames::INS_RETURNI );

    // RETURNI ENABLE
    m_statusFlags -> setInte ( true );
    inst_RETURNI_aux();
}

inline void PicoBlazeInstructionSet1::inst_RETURNI_aux()
{
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

inline void PicoBlazeInstructionSet1::inst_ENABLE_INT ( const unsigned int )
{
    instructionEnter ( PicoBlazeInsNames::INS_ENABLE_INT );

    // Perform the operation.
    m_statusFlags -> setInte ( true );
}

inline void PicoBlazeInstructionSet1::inst_DISABLE_INT ( const unsigned int )
{
    instructionEnter ( PicoBlazeInsNames::INS_DISABLE_INT );

    // Perform the operation.
    m_statusFlags -> setInte ( false );
}

void PicoBlazeInstructionSet1::inst_LOAD_sx_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_LOAD );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;
    const unsigned int kk = ( opCode & 0x00ff );

    // LOAD sX, kk
    m_registers -> write ( sX, kk );
}

void PicoBlazeInstructionSet1::inst_LOAD_sx_sy ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_LOAD );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;
    const unsigned int sY = ( opCode & 0x00f0 ) >> 4;

    // LOAD sX, sY
    m_registers -> write ( sX, m_registers -> read ( sY ) );
}

void PicoBlazeInstructionSet1::inst_AND_sx_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_AND );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;
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

void PicoBlazeInstructionSet1::inst_AND_sx_sy ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_AND );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;
    const unsigned int sY = ( opCode & 0x00f0 ) >> 4;

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

void PicoBlazeInstructionSet1::inst_OR_sx_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_OR );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;
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

void PicoBlazeInstructionSet1::inst_OR_sx_sy ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_OR );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;
    const unsigned int sY = ( opCode & 0x00f0 ) >> 4;

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

void PicoBlazeInstructionSet1::inst_XOR_sx_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_XOR );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;
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

void PicoBlazeInstructionSet1::inst_XOR_sx_sy ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_XOR );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;
    const unsigned int sY = ( opCode & 0x00f0 ) >> 4;

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

inline void PicoBlazeInstructionSet1::inst_SR0_sx ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SR0 );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    m_statusFlags -> setCarry ( ( 0 == ( 0x01 & sXval ) ) ? false : true );
    sXval >>= 1;
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

inline void PicoBlazeInstructionSet1::inst_SR1_sx ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SR1 );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;

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

inline void PicoBlazeInstructionSet1::inst_SRX_sx ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SRX );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    m_statusFlags -> setCarry ( ( 0 == ( 0x01 & sXval ) ) ? false : true );
    sXval = ( 0x80 & sXval ) | ( sXval >> 1 );
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

inline void PicoBlazeInstructionSet1::inst_SRA_sx ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SRA );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;

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

inline void PicoBlazeInstructionSet1::inst_RR_sx ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_RR );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;

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

inline void PicoBlazeInstructionSet1::inst_SL0_sx ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SL0 );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;

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

inline void PicoBlazeInstructionSet1::inst_SL1_sx ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SL1 );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;

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

inline void PicoBlazeInstructionSet1::inst_SLX_sx ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SLX );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;

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

inline void PicoBlazeInstructionSet1::inst_SLA_sx ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SLA );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;

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

inline void PicoBlazeInstructionSet1::inst_RL_sx ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_RL );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;

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

void PicoBlazeInstructionSet1::inst_INPUT_sx_pp ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_INPUT );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;
    const unsigned int PP = ( opCode & 0x00ff );

    // Perform the operation (INPUT sX, PP).
    m_registers -> write ( sX, m_io -> input ( PP ) );
}

void PicoBlazeInstructionSet1::inst_INPUT_sx_sy ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_INPUT );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;
    const unsigned int sY = ( opCode & 0x00f0 ) >> 4;

    // Perform the operation (INPUT sX, (sY)).
    m_registers -> write ( sX, m_io -> input ( m_registers -> read ( sY ) ) );
}

void PicoBlazeInstructionSet1::inst_OUTPUT_sx_pp ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_OUTPUT );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;
    const unsigned int PP = ( opCode & 0x00ff );

    // Perform the operation (OUTPUT sX, PP).
    m_io -> output ( PP, m_registers -> read ( sX ) );
}

void PicoBlazeInstructionSet1::inst_OUTPUT_sx_sy ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_OUTPUT );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0x0f00 ) >> 8;
    const unsigned int sY = ( opCode & 0x00f0 ) >> 4;

    // Perform the operation (OUTPUT sX, (sY)).
    m_io -> output ( m_registers -> read ( sY ), m_registers -> read ( sX ) );
}
