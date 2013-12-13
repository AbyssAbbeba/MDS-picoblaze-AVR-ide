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
 * @file PicoBlazeInstructionSet6.cxx
 */
// =============================================================================

#include "PicoBlazeInstructionSet6.h"

#include "PicoBlazeIO.h"
#include "PicoBlazeStack.h"
#include "PicoBlazeRegisters.h"
#include "PicoBlazeDataMemory.h"
#include "PicoBlazeProgramMemory.h"
#include "PicoBlazeInterruptController.h"

void ( PicoBlazeInstructionSet6 :: * const PicoBlazeInstructionSet6::m_opCodeDispatchTable [ 64 ] )
     ( const unsigned int opCode ) =
{
    /* opCode = 0x0---- */
    & PicoBlazeInstructionSet6::inst_LOAD_sX_sY,        // opCode = 0x00---
    & PicoBlazeInstructionSet6::inst_LOAD_sX_kk,        // opCode = 0x01---
    & PicoBlazeInstructionSet6::inst_AND_sX_sY,         // opCode = 0x02---
    & PicoBlazeInstructionSet6::inst_AND_sX_kk,         // opCode = 0x03---
    & PicoBlazeInstructionSet6::inst_OR_sX_sY,          // opCode = 0x04---
    & PicoBlazeInstructionSet6::inst_OR_sX_kk,          // opCode = 0x05---
    & PicoBlazeInstructionSet6::inst_XOR_sX_sY,         // opCode = 0x06---
    & PicoBlazeInstructionSet6::inst_XOR_sX_kk,         // opCode = 0x07---
    & PicoBlazeInstructionSet6::inst_INPUT_sX_sY,       // opCode = 0x08---
    & PicoBlazeInstructionSet6::inst_INPUT_sX_pp,       // opCode = 0x09---
    & PicoBlazeInstructionSet6::inst_FETCH_sX_sY,       // opCode = 0x0A---
    & PicoBlazeInstructionSet6::inst_FETCH_sX_ss,       // opCode = 0x0B---
    & PicoBlazeInstructionSet6::inst_TEST_sX_sY,        // opCode = 0x0C---
    & PicoBlazeInstructionSet6::inst_TEST_sX_kk,        // opCode = 0x0D---
    & PicoBlazeInstructionSet6::inst_TESTCY_sX_sY,      // opCode = 0x0E---
    & PicoBlazeInstructionSet6::inst_TESTCY_sX_kk,      // opCode = 0x0F---

    /* opCode = 0x1---- */
    & PicoBlazeInstructionSet6::inst_ADD_sX_sY,         // opCode = 0x10---
    & PicoBlazeInstructionSet6::inst_ADD_sX_kk,         // opCode = 0x11---
    & PicoBlazeInstructionSet6::inst_ADDCY_sX_sY,       // opCode = 0x12---
    & PicoBlazeInstructionSet6::inst_ADDCY_sX_kk,       // opCode = 0x13---
    & PicoBlazeInstructionSet6::inst_0x14,              // opCode = 0x14---
    & PicoBlazeInstructionSet6::inst__,                 // opCode = 0x15---
    & PicoBlazeInstructionSet6::inst_STAR_sX_sY,        // opCode = 0x16---
    & PicoBlazeInstructionSet6::inst__,                 // opCode = 0x17---
    & PicoBlazeInstructionSet6::inst_SUB_sX_sY,         // opCode = 0x18---
    & PicoBlazeInstructionSet6::inst_SUB_sX_kk,         // opCode = 0x19---
    & PicoBlazeInstructionSet6::inst_SUBCY_sX_sY,       // opCode = 0x1A---
    & PicoBlazeInstructionSet6::inst_SUBCY_sX_kk,       // opCode = 0x1B---
    & PicoBlazeInstructionSet6::inst_COMPARE_sX_sY,     // opCode = 0x1C---
    & PicoBlazeInstructionSet6::inst_COMPARE_sX_kk,     // opCode = 0x1D---
    & PicoBlazeInstructionSet6::inst_COMPARECY_sX_sY,   // opCode = 0x1E---
    & PicoBlazeInstructionSet6::inst_COMPARECY_sX_kk,   // opCode = 0x1F---

    /* opCode = 0x2---- */
    & PicoBlazeInstructionSet6::inst_CALL_aaa,          // opCode = 0x20---
    & PicoBlazeInstructionSet6::inst_LD_RET_sX_sY,      // opCode = 0x21---
    & PicoBlazeInstructionSet6::inst_JUMP_aaa,          // opCode = 0x22---
    & PicoBlazeInstructionSet6::inst__,                 // opCode = 0x23---
    & PicoBlazeInstructionSet6::inst_CALL_sX_sY,        // opCode = 0x24---
    & PicoBlazeInstructionSet6::inst_RETURN,            // opCode = 0x25---
    & PicoBlazeInstructionSet6::inst_JUMP_sX_sY,        // opCode = 0x26---
    & PicoBlazeInstructionSet6::inst__,                 // opCode = 0x27---
    & PicoBlazeInstructionSet6::inst_0x28,              // opCode = 0x28---
    & PicoBlazeInstructionSet6::inst_0x29,              // opCode = 0x29---
    & PicoBlazeInstructionSet6::inst__,                 // opCode = 0x2A---
    & PicoBlazeInstructionSet6::inst_OUTPUTK_kk_p,      // opCode = 0x2B---
    & PicoBlazeInstructionSet6::inst_OUTPUT_sX_sY,      // opCode = 0x2C---
    & PicoBlazeInstructionSet6::inst_OUTPUT_sX_pp,      // opCode = 0x2D---
    & PicoBlazeInstructionSet6::inst_STORE_sX_sY,       // opCode = 0x2E---
    & PicoBlazeInstructionSet6::inst_STORE_sX_ss,       // opCode = 0x2F---

    /* opCode = 0x3---- */
    & PicoBlazeInstructionSet6::inst_CALL_Z_aaa,        // opCode = 0x30---
    & PicoBlazeInstructionSet6::inst_RETURN_Z,          // opCode = 0x31---
    & PicoBlazeInstructionSet6::inst_JUMP_Z_aaa,        // opCode = 0x32---
    & PicoBlazeInstructionSet6::inst__,                 // opCode = 0x33---
    & PicoBlazeInstructionSet6::inst_CALL_NZ_aaa,       // opCode = 0x34---
    & PicoBlazeInstructionSet6::inst_RETURN_NZ,         // opCode = 0x35---
    & PicoBlazeInstructionSet6::inst_JUMP_NZ_aaa,       // opCode = 0x36---
    & PicoBlazeInstructionSet6::inst_0x37,              // opCode = 0x37---
    & PicoBlazeInstructionSet6::inst_CALL_C_aaa,        // opCode = 0x38---
    & PicoBlazeInstructionSet6::inst_RETURN_C,          // opCode = 0x39---
    & PicoBlazeInstructionSet6::inst_JUMP_C_aaa,        // opCode = 0x3A---
    & PicoBlazeInstructionSet6::inst__,                 // opCode = 0x3B---
    & PicoBlazeInstructionSet6::inst_CALL_NC_aaa,       // opCode = 0x3C---
    & PicoBlazeInstructionSet6::inst_RETURN_NC,         // opCode = 0x3D---
    & PicoBlazeInstructionSet6::inst_JUMP_NC_aaa,       // opCode = 0x3E---
    & PicoBlazeInstructionSet6::inst__,                 // opCode = 0x3F---
};

bool PicoBlazeInstructionSet6::isValid() const
{
    if ( MCUSim::FAMILY_KCPSM6 == m_config.m_dev )
    {
        return true;
    }
    else
    {
        return false;
    }
}

int PicoBlazeInstructionSet6::execInstruction()
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
    ( this ->* ( m_opCodeDispatchTable [ opCode >> 12 ] ) ) ( opCode );

    if ( pcOrig != m_pc )
    {
        logEvent ( EVENT_CPU_PC_CHANGED, m_pc );
    }

    return 1;
}

inline void PicoBlazeInstructionSet6::instructionEnter ( PicoBlazeInsNames::Instructions instName )
{
    m_instructionCounter [ instName ] ++;
    m_lastInstruction = instName;
}

void PicoBlazeInstructionSet6::inst_0x14 ( const unsigned int opCode )
{
    switch ( 0xff & opCode )
    {
        case 0x00: inst_SLA_sX     ( opCode ); break;
        case 0x02: inst_RL_sX      ( opCode ); break;
        case 0x0A: inst_SRX_sX     ( opCode ); break;
        case 0x06: inst_SL0_sX     ( opCode ); break;
        case 0x07: inst_SL1_sX     ( opCode ); break;
        case 0x08: inst_SRA_sX     ( opCode ); break;
        case 0x04: inst_SLX_sX     ( opCode ); break;
        case 0x0C: inst_RR_sX      ( opCode ); break;
        case 0x0E: inst_SR0_sX     ( opCode ); break;
        case 0x0F: inst_SR1_sX     ( opCode ); break;
        case 0x80: inst_HWBUILD_sX ( opCode ); break;
        default:   inst__          ( opCode ); break; 
    }
}

void PicoBlazeInstructionSet6::inst_0x28 ( const unsigned int opCode )
{
    if ( 0x28000 == opCode )
    {
        inst_DISABLE_INTERRUPT ( opCode );
    }
    else if ( 0x28001 == opCode )
    {
        inst_ENABLE_INTERRUPT ( opCode );
    }
    else
    {
        inst__ ( opCode );
    }
}

void PicoBlazeInstructionSet6::inst_0x29 ( const unsigned int opCode )
{
    if ( 0x29000 == opCode )
    {
        inst_RETURNI_DISABLE ( opCode );
    }
    else if ( 0x29001 == opCode )
    {
        inst_RETURNI_ENABLE ( opCode );
    }
    else
    {
        inst__ ( opCode );
    }
}

void PicoBlazeInstructionSet6::inst_0x37 ( const unsigned int opCode )
{
    if ( 0x37000 == opCode )
    {
        inst_REGBANK_A ( opCode );
    }
    else if ( 0x37001 == opCode )
    {
        inst_REGBANK_B ( opCode );
    }
    else
    {
        inst__ ( opCode );
    }
}

// -----------------------------------------------------------------------------
// INSTRUCTIONS
// -----------------------------------------------------------------------------

void PicoBlazeInstructionSet6::inst__ ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_NONE );

    logEvent ( EVENT_CPU_ERR_INVALID_OPCODE, m_pc, opCode );
}

void PicoBlazeInstructionSet6::inst_LOAD_sX_sY ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_LOAD );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int sY = ( opCode & 0x0f0 ) >> 4;

    // LOAD sX, sY
    m_registers -> write ( sX, m_registers -> read ( sY ) );
}

void PicoBlazeInstructionSet6::inst_LOAD_sX_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_LOAD );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int kk = ( opCode & 0x0ff );

    // LOAD sX, kk
    m_registers -> write ( sX, kk );
}

void PicoBlazeInstructionSet6::inst_STAR_sX_sY ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_LOAD );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int sY = ( opCode & 0x0f0 ) >> 4;

    // STAR sX, sY
    m_registers -> write ( sX, m_registers -> read ( sY ), ( ( 0 == m_registers -> getBank() ) ? 1 : 0 ) );
}

void PicoBlazeInstructionSet6::inst_AND_sX_sY ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_AND );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int sY = ( opCode & 0x0f0 ) >> 4;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // AND sX, sY
    sXval &= m_registers -> read ( sY );

    // Modify status flags.
    m_statusFlags -> setCarry ( false );
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet6::inst_AND_sX_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_AND );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int kk = ( opCode & 0x0ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // AND sX, kk
    sXval &= kk;

    // Modify status flags.
    m_statusFlags -> setCarry ( false );
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet6::inst_OR_sX_sY ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_OR );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int sY = ( opCode & 0x0f0 ) >> 4;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // OR sX, sY
    sXval |= m_registers -> read ( sY );

    // Modify status flags.
    m_statusFlags -> setCarry ( false );
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet6::inst_OR_sX_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_OR );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int kk = ( opCode & 0x0ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // OR sX, kk
    sXval |= kk;

    // Modify status flags.
    m_statusFlags -> setCarry ( false );
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet6::inst_XOR_sX_sY ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_XOR );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int sY = ( opCode & 0x0f0 ) >> 4;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // XOR sX, sY
    sXval ^= m_registers -> read ( sY );

    // Modify status flags.
    m_statusFlags -> setCarry ( false );
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet6::inst_XOR_sX_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_XOR );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int kk = ( opCode & 0x0ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // XOR sX, kk
    sXval ^= kk;

    // Modify status flags.
    m_statusFlags -> setCarry ( false );
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

void PicoBlazeInstructionSet6::inst_ADD_sX_sY ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_ADD );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int sY = ( opCode & 0x0f0 ) >> 4;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // ADD sX, sY
    sXval += m_registers -> read ( sY );

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    m_statusFlags -> setZero ( ( 0     == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( sXval & 0xff ) );
}

void PicoBlazeInstructionSet6::inst_ADD_sX_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_ADD );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int kk = ( opCode & 0x0ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // ADD sX, kk
    sXval += kk;

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    m_statusFlags -> setZero ( ( 0     == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( sXval & 0xff ) );
}

void PicoBlazeInstructionSet6::inst_ADDCY_sX_sY ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_ADDCY );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int sY = ( opCode & 0x0f0 ) >> 4;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // ADDCY sX, sY
    sXval += m_registers -> read ( sY );

    // Add carry flag to the result.
    if ( true == m_statusFlags -> getCarry() )
    {
        sXval++;
    }

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    m_statusFlags -> setZero ( ( 0     == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( sXval & 0xff ) );
}

void PicoBlazeInstructionSet6::inst_ADDCY_sX_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_ADDCY );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int kk = ( opCode & 0x0ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // ADDCY sX, kk
    sXval += kk;

    // Add carry flag to the result.
    if ( true == m_statusFlags -> getCarry() )
    {
        sXval++;
    }

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    m_statusFlags -> setZero ( ( 0     == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( sXval & 0xff ) );
}

void PicoBlazeInstructionSet6::inst_SUB_sX_sY ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SUB );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int sY = ( opCode & 0x0f0 ) >> 4;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // SUB sX, sY
    sXval -= m_registers -> read ( sY );

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    m_statusFlags -> setZero ( ( 0     == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( sXval & 0xff ) );
}

void PicoBlazeInstructionSet6::inst_SUB_sX_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SUB );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int kk = ( opCode & 0x0ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // SUB sX, kk
    sXval -= kk;

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    m_statusFlags -> setZero ( ( 0     == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( sXval & 0xff ) );
}

void PicoBlazeInstructionSet6::inst_SUBCY_sX_sY ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SUBCY );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int sY = ( opCode & 0x0ff ) >> 4;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // SUBCY sX, sY
    sXval -= m_registers -> read ( sY );

    // Add carry flag to the result.
    if ( true == m_statusFlags -> getCarry() )
    {
        sXval--;
    }

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    m_statusFlags -> setZero ( ( 0     == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( sXval & 0xff ) );
}

void PicoBlazeInstructionSet6::inst_SUBCY_sX_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SUBCY );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int kk = ( opCode & 0x0ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // SUBCY sX, kk
    sXval -= kk;

    // Add carry flag to the result.
    if ( true == m_statusFlags -> getCarry() )
    {
        sXval--;
    }

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    m_statusFlags -> setZero ( ( 0     == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( sXval & 0xff ) );
}

void PicoBlazeInstructionSet6::inst_TEST_sX_sY ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_TEST );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int sY = ( opCode & 0x0f0 ) >> 4;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // TEST sX, sY
    sXval &= m_registers -> read ( sY );

    /*
     * Modify status flags.
     */
    // Set Z, if the result is zero.
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );
    // Set C, if odd parity has been detected in the result.
    m_statusFlags -> setCarry ( false );
    for ( int i = 1; i < 0x100; i <<= 1 )
    {
        if ( 0 == ( sXval & i ) )
        {
            m_statusFlags -> setCarry ( !m_statusFlags -> getCarry() );
        }
    }
}

void PicoBlazeInstructionSet6::inst_TEST_sX_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_TEST );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int kk = ( opCode & 0x0ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // TEST sX, kk
    sXval &= kk;

    /*
     * Modify status flags.
     */
    // Set Z, if the result is zero.
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );
    // Set C, if odd parity has been detected in the result.
    m_statusFlags -> setCarry ( false );
    for ( int i = 1; i < 0x100; i <<= 1 )
    {
        if ( 0 == ( sXval & i ) )
        {
            m_statusFlags -> setCarry ( !m_statusFlags -> getCarry() );
        }
    }
}

void PicoBlazeInstructionSet6::inst_TESTCY_sX_sY ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_TESTCY );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int sY = ( opCode & 0x0f0 ) >> 4;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // TEST sX, sY
    sXval &= m_registers -> read ( sY );

    /*
     * Modify status flags.
     */
    // Set Z, if the result is zero; including previous value of the zero flag.
    m_statusFlags -> setZero ( m_statusFlags -> getZero() && ( ( 0 == sXval ) ? true : false ) );
    // Set C, if odd parity has been detected in the result; including previous value of the carry flag.
    for ( int i = 1; i < 0x100; i <<= 1 )
    {
        if ( 0 == ( sXval & i ) )
        {
            m_statusFlags -> setCarry ( !m_statusFlags -> getCarry() );
        }
    }
}

void PicoBlazeInstructionSet6::inst_TESTCY_sX_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_TESTCY );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int kk = ( opCode & 0x0ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // TEST sX, kk
    sXval &= kk;

    /*
     * Modify status flags.
     */
    // Set Z, if the result is zero; including previous value of the zero flag.
    m_statusFlags -> setZero ( m_statusFlags -> getZero() && ( ( 0 == sXval ) ? true : false ) );
    // Set C, if odd parity has been detected in the result; including previous value of the carry flag.
    for ( int i = 1; i < 0x100; i <<= 1 )
    {
        if ( 0 == ( sXval & i ) )
        {
            m_statusFlags -> setCarry ( !m_statusFlags -> getCarry() );
        }
    }
}

void PicoBlazeInstructionSet6::inst_COMPARE_sX_sY ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_COMPARE );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int sY = ( opCode & 0x0f0 ) >> 4;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // COMPARE sX, sY
    sXval -= m_registers -> read ( sY );

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    m_statusFlags -> setZero  ( ( 0     == sXval ) ? true : false );
}

void PicoBlazeInstructionSet6::inst_COMPARE_sX_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_COMPARE );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int kk = ( opCode & 0x0ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // COMPARE sX, kk
    sXval -= kk;

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    m_statusFlags -> setZero  ( ( 0     == sXval ) ? true : false );
}

void PicoBlazeInstructionSet6::inst_COMPARECY_sX_sY ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_COMPARECY );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int sY = ( opCode & 0x0f0 ) >> 4;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // COMPARE sX, sY
    sXval -= m_registers -> read ( sY );

    // Add carry flag to the result.
    if ( true == m_statusFlags -> getCarry() )
    {
        sXval--;
    }

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    m_statusFlags -> setZero  ( ( ( 0     == sXval ) ? true : false ) && m_statusFlags -> getZero() );
}

void PicoBlazeInstructionSet6::inst_COMPARECY_sX_kk ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_COMPARE );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int kk = ( opCode & 0x0ff );

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // COMPARE sX, kk
    sXval -= kk;

    // Add carry flag to the result.
    if ( true == m_statusFlags -> getCarry() )
    {
        sXval--;
    }

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0x100 &  sXval ) ? true : false );
    m_statusFlags -> setZero  ( ( ( 0     == sXval ) ? true : false ) && m_statusFlags -> getZero() );
}

inline void PicoBlazeInstructionSet6::inst_SR0_sX ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SR0 );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    m_statusFlags -> setCarry ( ( 0 == ( 0x01 & sXval ) ) ? false : true );
    sXval >>= 1;
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

inline void PicoBlazeInstructionSet6::inst_SR1_sX ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SR1 );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;

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

inline void PicoBlazeInstructionSet6::inst_SRX_sX ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SRX );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    m_statusFlags -> setCarry ( ( 0 == ( 0x01 & sXval ) ) ? false : true );
    sXval = ( 0x80 & sXval ) | ( sXval >> 1 );
    m_statusFlags -> setZero ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, sXval );
}

inline void PicoBlazeInstructionSet6::inst_SRA_sX ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SRA );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;

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

inline void PicoBlazeInstructionSet6::inst_RR_sX ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_RR );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;

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

inline void PicoBlazeInstructionSet6::inst_SL0_sX ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SL0 );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;

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

inline void PicoBlazeInstructionSet6::inst_SL1_sX ( const unsigned int opCode )
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
    m_statusFlags -> setCarry ( ( 0 == ( 0x100 & sXval ) ) ? false : true );
    m_statusFlags -> setZero  ( false );

    // Save the result.
    m_registers -> write ( sX, ( 0xff & sXval ) );
}

inline void PicoBlazeInstructionSet6::inst_SLX_sX ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SLX );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;

    // Registers.
    unsigned int sXval = m_registers -> read ( sX );

    // Perform the operation.
    sXval = ( 0x01 & sXval ) | ( sXval << 1 );

    // Modify status flags.
    m_statusFlags -> setCarry ( ( 0 == ( 0x100 & sXval ) ) ? false : true );
    m_statusFlags -> setZero  ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( 0xff & sXval ) );
}

inline void PicoBlazeInstructionSet6::inst_SLA_sX ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_SLA );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;

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
    m_statusFlags -> setZero  ( ( 0 == sXval ) ? true : false );

    // Save the result.
    m_registers -> write ( sX, ( 0xff & sXval ) );
}

inline void PicoBlazeInstructionSet6::inst_RL_sX ( const unsigned int opCode )
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

inline void PicoBlazeInstructionSet6::inst_REGBANK_A ( const unsigned int )
{
    instructionEnter ( PicoBlazeInsNames::INS_REGBANK );

    // Perform the operation.
    m_registers -> setBank ( 0 );
}

inline void PicoBlazeInstructionSet6::inst_REGBANK_B ( const unsigned int )
{
    instructionEnter ( PicoBlazeInsNames::INS_REGBANK );

    // Perform the operation.
    m_registers -> setBank ( 1 );
}

void PicoBlazeInstructionSet6::inst_INPUT_sX_sY ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_INPUT );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int sY = ( opCode & 0x0f0 ) >> 4;

    // INPUT sX, (sY)
    m_registers -> write ( sX, m_io -> input ( m_registers -> read ( sY ) ) );
}

void PicoBlazeInstructionSet6::inst_INPUT_sX_pp ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_INPUT );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int pp = ( opCode & 0x0ff );

    // INPUT sX, pp
    m_registers -> write ( sX, m_io -> input ( pp ) );
}

void PicoBlazeInstructionSet6::inst_OUTPUT_sX_sY ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_OUTPUT );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int sY = ( opCode & 0x0f0 ) >> 4;

    // OUTPUT sX, (sY)
    m_io -> output ( m_registers -> read ( sY ), m_registers -> read ( sX ) );
}

void PicoBlazeInstructionSet6::inst_OUTPUT_sX_pp ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_OUTPUT );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int pp = ( opCode & 0x0ff );

    // OUTPUT sX, pp
    m_io -> output ( pp, m_registers -> read ( sX ) );
}

void PicoBlazeInstructionSet6::inst_OUTPUTK_kk_p ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_OUTPUTK );

    // Extract operands from OP code.
    const unsigned int kk = ( opCode & 0xff0 ) >> 4;
    const unsigned int p  = ( opCode & 0x00f );

    // OUTPUTK kk, p
    m_io -> output ( p, kk );
}

void PicoBlazeInstructionSet6::inst_STORE_sX_sY ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_STORE );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int sY = ( opCode & 0x0f0 ) >> 4;

    // STORE sX, (sY)
    m_dataMemory -> write ( m_registers -> read ( sY ), m_registers -> read ( sX ) );
}

void PicoBlazeInstructionSet6::inst_STORE_sX_ss ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_STORE );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int ss = ( opCode & 0x0ff );

    // STORE sX, ss
    m_dataMemory -> write ( ss, m_registers -> read ( sX ) );
}

void PicoBlazeInstructionSet6::inst_FETCH_sX_sY ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_FETCH );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int sY = ( opCode & 0x0f0 ) >> 4;

    // FETCH sX, (sY)
    m_registers -> write ( sX, m_dataMemory -> read ( m_registers -> read ( sY ) ) );
}

void PicoBlazeInstructionSet6::inst_FETCH_sX_ss ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_FETCH );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int ss = ( opCode & 0x0ff );

    // FETCH sX, SS
    m_registers -> write ( sX, m_dataMemory -> read ( ss ) );
}

inline void PicoBlazeInstructionSet6::inst_DISABLE_INTERRUPT ( const unsigned int )
{
    instructionEnter ( PicoBlazeInsNames::INS_DISABLE_INT );

    // Perform the operation.
    m_statusFlags -> setInte ( false );
}

inline void PicoBlazeInstructionSet6::inst_ENABLE_INTERRUPT ( const unsigned int )
{
    instructionEnter ( PicoBlazeInsNames::INS_ENABLE_INT );

    // Perform the operation.
    m_statusFlags -> setInte ( true );
}

inline void PicoBlazeInstructionSet6::inst_RETURNI_DISABLE ( const unsigned int )
{
    instructionEnter ( PicoBlazeInsNames::INS_RETURNI );

    // RETURNI DISABLE
    m_statusFlags -> setInte ( false );

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

inline void PicoBlazeInstructionSet6::inst_RETURNI_ENABLE ( const unsigned int )
{
    instructionEnter ( PicoBlazeInsNames::INS_RETURNI );

    // RETURNI ENABLE
    m_statusFlags -> setInte ( true );

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

void PicoBlazeInstructionSet6::inst_JUMP_aaa ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_JUMP );

    // Extract operands from OP code.
    const unsigned int addr = ( opCode & 0xfff );

    // Execute jump.
    setProgramCounter ( addr );
}

void PicoBlazeInstructionSet6::inst_JUMP_Z_aaa ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_JUMP );

    // Extract operands from OP code.
    const unsigned int addr = ( opCode & 0xfff );

    // Jump, if ZERO.
    if ( true == m_statusFlags -> getZero() )
    {
        setProgramCounter ( addr );
    }
}

void PicoBlazeInstructionSet6::inst_JUMP_NZ_aaa ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_JUMP );

    // Extract operands from OP code.
    const unsigned int addr = ( opCode & 0xfff );

    // Jump, if NOT ZERO.
    if ( false == m_statusFlags -> getZero() )
    {
        setProgramCounter ( addr );
    }
}

void PicoBlazeInstructionSet6::inst_JUMP_C_aaa ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_JUMP );

    // Extract operands from OP code.
    const unsigned int addr = ( opCode & 0xfff );

    // Jump, if CARRY.
    if ( true == m_statusFlags -> getCarry() )
    {
        setProgramCounter ( addr );
    }
}

void PicoBlazeInstructionSet6::inst_JUMP_NC_aaa ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_JUMP );

    // Extract operands from OP code.
    const unsigned int addr = ( opCode & 0xfff );

    // Jump, if NOT CARRY.
    if ( false == m_statusFlags -> getCarry() )
    {
        setProgramCounter ( addr );
    }
}

void PicoBlazeInstructionSet6::inst_JUMP_sX_sY ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_JUMP );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int sY = ( opCode & 0x0f0 ) >> 4;

    unsigned int addr = ( ( m_registers->read(sX) & 0x0f ) << 8 ) | m_registers->read(sY);

    // Execute jump.
    setProgramCounter ( addr );
}

void PicoBlazeInstructionSet6::inst_CALL_aaa ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_CALL );

    // Extract operands from OP code.
    const unsigned int addr = ( opCode & 0xfff );

    // Execute call.
    logEvent ( EVENT_CPU_CALL, m_pc, PicoBlazeInsNames::INS_CALL );
    m_stack->pushOnStack ( m_pc );
    setProgramCounter ( addr );
    m_actSubprogCounter++;
}

void PicoBlazeInstructionSet6::inst_CALL_Z_aaa ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_CALL );

    // Extract operands from OP code.
    const unsigned int addr = ( opCode & 0xfff );

    // Call, if ZERO.
    if ( false == m_statusFlags -> getZero() )
    {
        logEvent ( EVENT_CPU_CALL, m_pc, PicoBlazeInsNames::INS_CALL );
        m_stack->pushOnStack ( m_pc );
        setProgramCounter ( addr );
        m_actSubprogCounter++;
    }
}

void PicoBlazeInstructionSet6::inst_CALL_NZ_aaa ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_CALL );

    // Extract operands from OP code.
    const unsigned int addr = ( opCode & 0xfff );

    // Call, if NOT ZERO.
    if ( true == m_statusFlags -> getZero() )
    {
        logEvent ( EVENT_CPU_CALL, m_pc, PicoBlazeInsNames::INS_CALL );
        m_stack->pushOnStack ( m_pc );
        setProgramCounter ( addr );
        m_actSubprogCounter++;
    }
}

void PicoBlazeInstructionSet6::inst_CALL_C_aaa ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_CALL );

    // Extract operands from OP code.
    const unsigned int addr = ( opCode & 0xfff );

    // Call, if CARRY.
    if ( false == m_statusFlags -> getCarry() )
    {
        logEvent ( EVENT_CPU_CALL, m_pc, PicoBlazeInsNames::INS_CALL );
        m_stack->pushOnStack ( m_pc );
        setProgramCounter ( addr );
        m_actSubprogCounter++;
    }
}

void PicoBlazeInstructionSet6::inst_CALL_NC_aaa ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_CALL );

    // Extract operands from OP code.
    const unsigned int addr = ( opCode & 0xfff );

    // Call, if NOT CARRY.
    if ( true == m_statusFlags -> getCarry() )
    {
        logEvent ( EVENT_CPU_CALL, m_pc, PicoBlazeInsNames::INS_CALL );
        m_stack->pushOnStack ( m_pc );
        setProgramCounter ( addr );
        m_actSubprogCounter++;
    }
}

void PicoBlazeInstructionSet6::inst_CALL_sX_sY ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_CALL );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int sY = ( opCode & 0x0f0 ) >> 4;

    unsigned int addr = ( ( m_registers->read(sX) & 0x0f ) << 8 ) | m_registers->read(sY);

    // Execute call.
    logEvent ( EVENT_CPU_CALL, m_pc, PicoBlazeInsNames::INS_CALL );
    m_stack->pushOnStack ( m_pc );
    setProgramCounter ( addr );
    m_actSubprogCounter++;
}

void PicoBlazeInstructionSet6::inst_RETURN ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_RETURN );

    // Verify OP code validity.
    if ( 0 != ( opCode & 0xfff ) )
    {
        inst__ ( opCode );
        return;
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

void PicoBlazeInstructionSet6::inst_RETURN_Z ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_RETURN );

    // Verify OP code validity.
    if ( 0 != ( opCode & 0xfff ) )
    {
        inst__ ( opCode );
        return;
    }

    // Return, if ZERO.
    if ( false == m_statusFlags -> getZero() )
    {
        return;
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

void PicoBlazeInstructionSet6::inst_RETURN_NZ ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_RETURN );

    // Verify OP code validity.
    if ( 0 != ( opCode & 0xfff ) )
    {
        inst__ ( opCode );
        return;
    }

    // Return, if NOT ZERO.
    if ( true == m_statusFlags -> getZero() )
    {
        return;
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

void PicoBlazeInstructionSet6::inst_RETURN_C ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_RETURN );

    // Verify OP code validity.
    if ( 0 != ( opCode & 0xfff ) )
    {
        inst__ ( opCode );
        return;
    }

    // Return, if CARRY.
    if ( false == m_statusFlags -> getCarry() )
    {
        return;
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

void PicoBlazeInstructionSet6::inst_RETURN_NC ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_RETURN );

    // Verify OP code validity.
    if ( 0 != ( opCode & 0xfff ) )
    {
        inst__ ( opCode );
        return;
    }

    // Return, if NOT CARRY.
    if ( true == m_statusFlags -> getCarry() )
    {
        return;
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

void PicoBlazeInstructionSet6::inst_LD_RET_sX_sY ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_LD_RET );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;
    const unsigned int kk = ( opCode & 0x0ff );

    // LOAD sX, kk
    m_registers -> write ( sX, kk );

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

inline void PicoBlazeInstructionSet6::inst_HWBUILD_sX ( const unsigned int opCode )
{
    instructionEnter ( PicoBlazeInsNames::INS_HWBUILD );

    // Extract operands from OP code.
    const unsigned int sX = ( opCode & 0xf00 ) >> 8;

    // Perform the operation.
    m_registers -> write ( sX, (unsigned int) m_config.m_hwbuild );

    // Modify status flags.
    m_statusFlags -> setCarry ( true );
    m_statusFlags -> setZero ( ( 0 == m_config.m_hwbuild ) ? true : false );
}
