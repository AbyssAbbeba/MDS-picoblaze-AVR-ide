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
 * @ingroup PicoBlazeAsm
 * @file AsmPicoBlazeInstructionSet6.cxx
 */
// =============================================================================

#include "AsmPicoBlazeInstructionSet6.h"

// PicoBlaze assembler semantic analyzer header files.
#include "AsmPicoBlazeSymbolTable.h"
#include "AsmPicoBlazeInstructionSet.h"

unsigned int AsmPicoBlazeInstructionSet6::getAAA ( const CompilerStatement * stmt,
                                                   int index ) const
{
    unsigned int result = m_instructionSet->checkLimit ( AsmPicoBlazeInstructionSet::LIM_C,
                                                         stmt->args()->at(index)->location(),
                                                         m_symbolTable->resolveExpr(stmt->args()->at(index), OBS_AAA) );

    // Make it (AAA - 1);
    return ( result - 1 ) & ~( ~0U << OBS_AAA );
}

unsigned int AsmPicoBlazeInstructionSet6::getSXY ( const CompilerStatement * stmt,
                                                   int index ) const
{
    return m_instructionSet->checkLimit ( AsmPicoBlazeInstructionSet::LIM_R,
                                          stmt->args()->at(index)->location(),
                                          m_symbolTable->resolveExpr(stmt->args()->at(index), OBS_SXY) );
}

unsigned int AsmPicoBlazeInstructionSet6::getKK ( const CompilerStatement * stmt,
                                                  int index ) const
{
    return m_symbolTable->resolveExpr(stmt->args()->at(index), OBS_KK);
}

unsigned int AsmPicoBlazeInstructionSet6::getPP ( const CompilerStatement * stmt,
                                                  int index ) const
{
    return m_symbolTable->resolveExpr(stmt->args()->at(index), OBS_PP);
}

unsigned int AsmPicoBlazeInstructionSet6::getP ( const CompilerStatement * stmt,
                                                 int index ) const
{
    return m_symbolTable->resolveExpr(stmt->args()->at(index), OBS_P);
}

unsigned int AsmPicoBlazeInstructionSet6::getSS ( const CompilerStatement * stmt,
                                                  int index ) const
{
    return m_instructionSet->checkLimit ( AsmPicoBlazeInstructionSet::LIM_D,
                                          stmt->args()->at(index)->location(),
                                          m_symbolTable->resolveExpr(stmt->args()->at(index), OBS_SS) );
}

int AsmPicoBlazeInstructionSet6::resolveOPcode ( const CompilerStatement * stmt ) const
{
    using namespace CompilerStatementTypes;

    switch ( (int) stmt->type() )
    {
        /* Instruction: LOAD */
        case ASMPICOBLAZE_INS_LOAD_SX_SY:       return ( 0x00000 | ( getSXY(stmt,0) << 8 ) | ( getSXY(stmt,1) << 4 ) );
        case ASMPICOBLAZE_INS_LOAD_SX_KK:       return ( 0x01000 | ( getSXY(stmt,0) << 8 ) |   getKK(stmt,1) );

        /* Instruction: STAR */
        case ASMPICOBLAZE_INS_STAR_SX_SY:       return ( 0x16000 | ( getSXY(stmt,0) << 8 ) | ( getSXY(stmt,1) << 4 ) );

        /* Instruction: AND */
        case ASMPICOBLAZE_INS_AND_SX_SY:        return ( 0x02000 | ( getSXY(stmt,0) << 8 ) | ( getSXY(stmt,1) << 4 ) );
        case ASMPICOBLAZE_INS_AND_SX_KK:        return ( 0x03000 | ( getSXY(stmt,0) << 8 ) |   getKK(stmt,1) );

        /* Instruction: OR */
        case ASMPICOBLAZE_INS_OR_SX_SY:         return ( 0x04000 | ( getSXY(stmt,0) << 8 ) | ( getSXY(stmt,1) << 4 ) );
        case ASMPICOBLAZE_INS_OR_SX_KK:         return ( 0x05000 | ( getSXY(stmt,0) << 8 ) |   getKK(stmt,1) );

        /* Instruction: XOR */
        case ASMPICOBLAZE_INS_XOR_SX_SY:        return ( 0x06000 | ( getSXY(stmt,0) << 8 ) | ( getSXY(stmt,1) << 4 ) );
        case ASMPICOBLAZE_INS_XOR_SX_KK:        return ( 0x07000 | ( getSXY(stmt,0) << 8 ) |   getKK(stmt,1) );

        /* Instruction: ADD */
        case ASMPICOBLAZE_INS_ADD_SX_SY:        return ( 0x10000 | ( getSXY(stmt,0) << 8 ) | ( getSXY(stmt,1) << 4 ) );
        case ASMPICOBLAZE_INS_ADD_SX_KK:        return ( 0x11000 | ( getSXY(stmt,0) << 8 ) |   getKK(stmt,1) );

        /* Instruction: ADDCY */
        case ASMPICOBLAZE_INS_ADDCY_SX_SY:      return ( 0x12000 | ( getSXY(stmt,0) << 8 ) | ( getSXY(stmt,1) << 4 ) );
        case ASMPICOBLAZE_INS_ADDCY_SX_KK:      return ( 0x13000 | ( getSXY(stmt,0) << 8 ) |   getKK(stmt,1) );

        /* Instruction: SUB */
        case ASMPICOBLAZE_INS_SUB_SX_SY:        return ( 0x18000 | ( getSXY(stmt,0) << 8 ) | ( getSXY(stmt,1) << 4 ) );
        case ASMPICOBLAZE_INS_SUB_SX_KK:        return ( 0x19000 | ( getSXY(stmt,0) << 8 ) |   getKK(stmt,1) );

        /* Instruction: SUBCY */
        case ASMPICOBLAZE_INS_SUBCY_SX_SY:      return ( 0x1A000 | ( getSXY(stmt,0) << 8 ) | ( getSXY(stmt,1) << 4 ) );
        case ASMPICOBLAZE_INS_SUBCY_SX_KK:      return ( 0x1B000 | ( getSXY(stmt,0) << 8 ) |   getKK(stmt,1) );

        /* Instruction: TEST */
        case ASMPICOBLAZE_INS_TEST_SX_SY:       return ( 0x0C000 | ( getSXY(stmt,0) << 8 ) | ( getSXY(stmt,1) << 4 ) );
        case ASMPICOBLAZE_INS_TEST_SX_KK:       return ( 0x0D000 | ( getSXY(stmt,0) << 8 ) |   getKK(stmt,1) );

        /* Instruction: TESTCY */
        case ASMPICOBLAZE_INS_TESTCY_SX_SY:     return ( 0x0E000 | ( getSXY(stmt,0) << 8 ) | ( getSXY(stmt,1) << 4 ) );
        case ASMPICOBLAZE_INS_TESTCY_SX_KK:     return ( 0x0F000 | ( getSXY(stmt,0) << 8 ) |   getKK(stmt,1) );

        /* Instruction: COMPARE */
        case ASMPICOBLAZE_INS_COMPARE_SX_SY:    return ( 0x1C000 | ( getSXY(stmt,0) << 8 ) | ( getSXY(stmt,1) << 4 ) );
        case ASMPICOBLAZE_INS_COMPARE_SX_KK:    return ( 0x1D000 | ( getSXY(stmt,0) << 8 ) |   getKK(stmt,1) );

        /* Instruction: COMPARECY */
        case ASMPICOBLAZE_INS_COMPARECY_SX_SY:  return ( 0x1E000 | ( getSXY(stmt,0) << 8 ) | ( getSXY(stmt,1) << 4 ) );
        case ASMPICOBLAZE_INS_COMPARECY_SX_KK:  return ( 0x1F000 | ( getSXY(stmt,0) << 8 ) |   getKK(stmt,1) );

        /* Instruction: SL0, SL1, SLX, SLA, RL */
        case ASMPICOBLAZE_INS_SL0_SX:           return ( 0x14006 | ( getSXY(stmt,0) << 8 ) );
        case ASMPICOBLAZE_INS_SL1_SX:           return ( 0x14007 | ( getSXY(stmt,0) << 8 ) );
        case ASMPICOBLAZE_INS_SLX_SX:           return ( 0x14004 | ( getSXY(stmt,0) << 8 ) );
        case ASMPICOBLAZE_INS_SLA_SX:           return ( 0x14000 | ( getSXY(stmt,0) << 8 ) );
        case ASMPICOBLAZE_INS_RL_SX:            return ( 0x14002 | ( getSXY(stmt,0) << 8 ) );

        /* Instruction: SR0, SR1, SRX, SRA, RR */
        case ASMPICOBLAZE_INS_SR0_SX:           return ( 0x1400E | ( getSXY(stmt,0) << 8 ) );
        case ASMPICOBLAZE_INS_SR1_SX:           return ( 0x1400F | ( getSXY(stmt,0) << 8 ) );
        case ASMPICOBLAZE_INS_SRX_SX:           return ( 0x1400A | ( getSXY(stmt,0) << 8 ) );
        case ASMPICOBLAZE_INS_SRA_SX:           return ( 0x14008 | ( getSXY(stmt,0) << 8 ) );
        case ASMPICOBLAZE_INS_RR_SX:            return ( 0x1400C | ( getSXY(stmt,0) << 8 ) );

        /* Instruction: REGBANK A/B */
        case ASMPICOBLAZE_INS_REGBANK_A:        return 0x37000;
        case ASMPICOBLAZE_INS_REGBANK_B:        return 0x37001;

        /* Instruction: INPUT */
        case ASMPICOBLAZE_INS_INPUT_SX_SY:      return ( 0x08000 | ( getSXY(stmt,0) << 8 ) | ( getSXY(stmt,1) << 4 ) );
        case ASMPICOBLAZE_INS_INPUT_SX_PP:      return ( 0x09000 | ( getSXY(stmt,0) << 8 ) |   getPP(stmt,1) );

        /* Instruction: OUTPUT */
        case ASMPICOBLAZE_INS_OUTPUT_SX_SY:     return ( 0x2C000 | ( getSXY(stmt,0) << 8 ) | ( getSXY(stmt,1) << 4 ) );
        case ASMPICOBLAZE_INS_OUTPUT_SX_PP:     return ( 0x2D000 | ( getSXY(stmt,0) << 8 ) |   getPP(stmt,1) );

        /* Instruction: OUTPUTK */
        case ASMPICOBLAZE_INS_OUTPUTK_KK_P:     return ( 0x2B000 | ( getKK(stmt,0) << 4 ) | getP(stmt,1) );

        /* Instruction: STORE */
        case ASMPICOBLAZE_INS_STORE_SX_SY:      return ( 0x2E000 | ( getSXY(stmt,0) << 8 ) | ( getSXY(stmt,1) << 4 ) );
        case ASMPICOBLAZE_INS_STORE_SX_SS:      return ( 0x2F000 | ( getSXY(stmt,0) << 8 ) |   getSS(stmt,1) );

        /* Instruction: FETCH */
        case ASMPICOBLAZE_INS_FETCH_SX_SY:      return ( 0x0A000 | ( getSXY(stmt,0) << 8 ) | ( getSXY(stmt,1) << 4 ) );
        case ASMPICOBLAZE_INS_FETCH_SX_SS:      return ( 0x0B000 | ( getSXY(stmt,0) << 8 ) |   getSS(stmt,1) );

        /* Instruction: ENABLE/DISABLE INTERRUPT */
        case ASMPICOBLAZE_INS_ENABLE_INT:       return 0x28001;
        case ASMPICOBLAZE_INS_DISABLE_INT:      return 0x28000;

        /* Instruction: RETURNI ENABLE/DISABLE */
        case ASMPICOBLAZE_INS_RETURNI_ENA:      return 0x29001;
        case ASMPICOBLAZE_INS_RETURNI_DIS:      return 0x29000;

        /* Instruction: JUMP */
        case ASMPICOBLAZE_INS_JUMP_AAA:         return ( 0x22000 | getAAA(stmt,0) );
        case ASMPICOBLAZE_INS_JUMP_Z_AAA:       return ( 0x32000 | getAAA(stmt,0) );
        case ASMPICOBLAZE_INS_JUMP_NZ_AAA:      return ( 0x36000 | getAAA(stmt,0) );
        case ASMPICOBLAZE_INS_JUMP_C_AAA:       return ( 0x3A000 | getAAA(stmt,0) );
        case ASMPICOBLAZE_INS_JUMP_NC_AAA:      return ( 0x3E000 | getAAA(stmt,0) );
        case ASMPICOBLAZE_INS_JUMP_SX_SY:       return ( 0x26000 | ( getSXY(stmt,0) << 8 ) | ( getSXY(stmt,1) << 4 ) );

        /* Instruction: CALL */
        case ASMPICOBLAZE_INS_CALL_AAA:         return ( 0x20000 | getAAA(stmt,0) );
        case ASMPICOBLAZE_INS_CALL_Z_AAA:       return ( 0x30000 | getAAA(stmt,0) );
        case ASMPICOBLAZE_INS_CALL_NZ_AAA:      return ( 0x34000 | getAAA(stmt,0) );
        case ASMPICOBLAZE_INS_CALL_C_AAA:       return ( 0x38000 | getAAA(stmt,0) );
        case ASMPICOBLAZE_INS_CALL_NC_AAA:      return ( 0x3C000 | getAAA(stmt,0) );
        case ASMPICOBLAZE_INS_CALL_SX_SY:       return ( 0x24000 | ( getSXY(stmt,0) << 8 ) | ( getSXY(stmt,1) << 4 ) );

        /* Instruction: RETURN */
        case ASMPICOBLAZE_INS_RETURN:           return ( 0x25000 );
        case ASMPICOBLAZE_INS_RETURN_Z:         return ( 0x31000 );
        case ASMPICOBLAZE_INS_RETURN_NZ:        return ( 0x35000 );
        case ASMPICOBLAZE_INS_RETURN_C:         return ( 0x39000 );
        case ASMPICOBLAZE_INS_RETURN_NC:        return ( 0x3D000 );
        case ASMPICOBLAZE_INS_LD_RET_SX_KK:     return ( 0x21000 | ( getSXY(stmt,0) << 8 ) | getKK(stmt,1) );

        /* Instruction: HWBUILD */
        case ASMPICOBLAZE_INS_HWBUILD_SX:       return ( 0x14080 | ( getSXY(stmt,0) << 8 ));

        default:
            return -1;
    }
}
