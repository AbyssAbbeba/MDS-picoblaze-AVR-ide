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
    ( this ->* ( m_opCodeDispatchTable [ opCode >> 13 ] ) ) ( opCode );

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
    switch ( opCode )
    {
        case 0x00: inst_SLA_sX     ( opCode ); break;
        case 0x02: inst_RL_sX      ( opCode ); break;
        case 0x04: inst_SRX_sX     ( opCode ); break;
        case 0x06: inst_SL0_sX     ( opCode ); break;
        case 0x07: inst_SL1_sX     ( opCode ); break;
        case 0x08: inst_SRA_sX     ( opCode ); break;
        case 0x0A: inst_SLX_sX     ( opCode ); break;
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

void PicoBlazeInstructionSet6::inst_LOAD_sX_sY ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_LOAD_sX_kk ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_STAR_sX_sY ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_AND_sX_sY ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_AND_sX_kk ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_OR_sX_sY ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_OR_sX_kk ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_XOR_sX_sY ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_XOR_sX_kk ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_ADD_sX_sY ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_ADD_sX_kk ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_ADDCY_sX_sY ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_ADDCY_sX_kk ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_SUB_sX_sY ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_SUB_sX_kk ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_SUBCY_sX_sY ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_SUBCY_sX_kk ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_TEST_sX_sY ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_TEST_sX_kk ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_TESTCY_sX_sY ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_TESTCY_sX_kk ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_COMPARE_sX_sY ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_COMPARE_sX_kk ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_COMPARECY_sX_sY ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_COMPARECY_sX_kk ( const unsigned int /*opCode*/ )
{
}

inline void PicoBlazeInstructionSet6::inst_SR0_sX ( const unsigned int /*opCode*/ )
{
}

inline void PicoBlazeInstructionSet6::inst_SR1_sX ( const unsigned int /*opCode*/ )
{
}

inline void PicoBlazeInstructionSet6::inst_SRX_sX ( const unsigned int /*opCode*/ )
{
}

inline void PicoBlazeInstructionSet6::inst_SRA_sX ( const unsigned int /*opCode*/ )
{
}

inline void PicoBlazeInstructionSet6::inst_RR_sX ( const unsigned int /*opCode*/ )
{
}

inline void PicoBlazeInstructionSet6::inst_SL0_sX ( const unsigned int /*opCode*/ )
{
}

inline void PicoBlazeInstructionSet6::inst_SL1_sX ( const unsigned int /*opCode*/ )
{
}

inline void PicoBlazeInstructionSet6::inst_SLX_sX ( const unsigned int /*opCode*/ )
{
}

inline void PicoBlazeInstructionSet6::inst_SLA_sX ( const unsigned int /*opCode*/ )
{
}

inline void PicoBlazeInstructionSet6::inst_RL_sX ( const unsigned int /*opCode*/ )
{
}

inline void PicoBlazeInstructionSet6::inst_REGBANK_A ( const unsigned int /*opCode*/ )
{
}

inline void PicoBlazeInstructionSet6::inst_REGBANK_B ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_INPUT_sX_sY ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_INPUT_sX_pp ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_OUTPUT_sX_sY ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_OUTPUT_sX_pp ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_OUTPUTK_kk_p ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_STORE_sX_sY ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_STORE_sX_ss ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_FETCH_sX_sY ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_FETCH_sX_ss ( const unsigned int /*opCode*/ )
{
}

inline void PicoBlazeInstructionSet6::inst_DISABLE_INTERRUPT ( const unsigned int /*opCode*/ )
{
}

inline void PicoBlazeInstructionSet6::inst_ENABLE_INTERRUPT ( const unsigned int /*opCode*/ )
{
}

inline void PicoBlazeInstructionSet6::inst_RETURNI_DISABLE ( const unsigned int /*opCode*/ )
{
}

inline void PicoBlazeInstructionSet6::inst_RETURNI_ENABLE ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_JUMP_aaa ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_JUMP_Z_aaa ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_JUMP_NZ_aaa ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_JUMP_C_aaa ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_JUMP_NC_aaa ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_JUMP_sX_sY ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_CALL_aaa ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_CALL_Z_aaa ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_CALL_NZ_aaa ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_CALL_C_aaa ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_CALL_NC_aaa ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_CALL_sX_sY ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_RETURN ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_RETURN_Z ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_RETURN_NZ ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_RETURN_C ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_RETURN_NC ( const unsigned int /*opCode*/ )
{
}

void PicoBlazeInstructionSet6::inst_LD_RET_sX_sY ( const unsigned int /*opCode*/ )
{
}

inline void PicoBlazeInstructionSet6::inst_HWBUILD_sX ( const unsigned int /*opCode*/ )
{
}
