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
 * @file AsmPicoBlazeInstructionSet3.cxx
 */
// =============================================================================

#include "AsmPicoBlazeInstructionSet3.h"

// PicoBlaze assembler semantic analyzer header files.
#include "AsmPicoBlazeSymbolTable.h"
#include "AsmPicoBlazeInstructionSet.h"

unsigned int AsmPicoBlazeInstructionSet3::getAAA ( const CompilerStatement * stmt,
                                                   int index ) const
{
    return m_instructionSet->checkLimit ( AsmPicoBlazeInstructionSet::LIM_C,
                                          stmt->args()->at(index)->location(),
                                          m_symbolTable->resolveExpr(stmt->args()->at(index), OBS_AAA) );
}

unsigned int AsmPicoBlazeInstructionSet3::getSXY ( const CompilerStatement * stmt,
                                                   int index ) const
{
    return m_instructionSet->checkLimit ( AsmPicoBlazeInstructionSet::LIM_R,
                                          stmt->args()->at(index)->location(),
                                          m_symbolTable->resolveExpr(stmt->args()->at(index), OBS_SXY) );
}

unsigned int AsmPicoBlazeInstructionSet3::getKK ( const CompilerStatement * stmt,
                                                  int index ) const
{
    return m_symbolTable->resolveExpr(stmt->args()->at(index), OBS_KK);
}

unsigned int AsmPicoBlazeInstructionSet3::getPP ( const CompilerStatement * stmt,
                                                  int index ) const
{
    return m_symbolTable->resolveExpr(stmt->args()->at(index), OBS_PP);
}

unsigned int AsmPicoBlazeInstructionSet3::getSS ( const CompilerStatement * stmt,
                                                  int index ) const
{
    return m_instructionSet->checkLimit ( AsmPicoBlazeInstructionSet::LIM_D,
                                          stmt->args()->at(index)->location(),
                                          m_symbolTable->resolveExpr(stmt->args()->at(index), OBS_SS) );
}

int AsmPicoBlazeInstructionSet3::resolveOPcode ( const CompilerStatement * stmt ) const
{
    using namespace CompilerStatementTypes;

    switch ( (int) stmt->type() )
    {
        /* Instruction: JUMP */
        case ASMPICOBLAZE_INS_JUMP_AAA:
            return ( 0b110100000000000000 | getAAA(stmt, 0) );
        case ASMPICOBLAZE_INS_JUMP_Z_AAA:
            return ( 0b110101000000000000 | getAAA(stmt, 0) );
        case ASMPICOBLAZE_INS_JUMP_NZ_AAA:
            return ( 0b110101010000000000 | getAAA(stmt, 0) );
        case ASMPICOBLAZE_INS_JUMP_C_AAA:
            return ( 0b110101100000000000 | getAAA(stmt, 0) );
        case ASMPICOBLAZE_INS_JUMP_NC_AAA:
            return ( 0b110101110000000000 | getAAA(stmt, 0) );

        /* Instruction: CALL */
        case ASMPICOBLAZE_INS_CALL_AAA:
            return ( 0b110000000000000000 | getAAA(stmt, 0) );
        case ASMPICOBLAZE_INS_CALL_Z_AAA:
            return ( 0b110001000000000000 | getAAA(stmt, 0) );
        case ASMPICOBLAZE_INS_CALL_NZ_AAA:
            return ( 0b110001010000000000 | getAAA(stmt, 0) );
        case ASMPICOBLAZE_INS_CALL_C_AAA:
            return ( 0b110001100000000000 | getAAA(stmt, 0) );
        case ASMPICOBLAZE_INS_CALL_NC_AAA:
            return ( 0b110001110000000000 | getAAA(stmt, 0) );

        /* Instruction: RETURN */
        case ASMPICOBLAZE_INS_RETURN:
            return 0b101010000000000000;
        case ASMPICOBLAZE_INS_RETURN_Z:
            return 0b101011000000000000;
        case ASMPICOBLAZE_INS_RETURN_NZ:
            return 0b101011010000000000;
        case ASMPICOBLAZE_INS_RETURN_C:
            return 0b101011100000000000;
        case ASMPICOBLAZE_INS_RETURN_NC:
            return 0b101011110000000000;

        /* Instruction: ADD */
        case ASMPICOBLAZE_INS_ADD_SX_KK:
            return ( 0b011000000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getKK(stmt, 1) );
        case ASMPICOBLAZE_INS_ADD_SX_SY:
            return ( 0b011001000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: ADDCY */
        case ASMPICOBLAZE_INS_ADDCY_SX_KK:
            return ( 0b011010000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getKK(stmt, 1) );
        case ASMPICOBLAZE_INS_ADDCY_SX_SY:
            return ( 0b011011000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: SUB */
        case ASMPICOBLAZE_INS_SUB_SX_KK:
            return ( 0b011100000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getKK(stmt, 1) );
        case ASMPICOBLAZE_INS_SUB_SX_SY:
            return ( 0b011101000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: SUBCY */
        case ASMPICOBLAZE_INS_SUBCY_SX_KK:
            return ( 0b011110000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getKK(stmt, 1) );
        case ASMPICOBLAZE_INS_SUBCY_SX_SY:
            return ( 0b011111000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: COMPARE */
        case ASMPICOBLAZE_INS_COMPARE_SX_KK:
            return ( 0b010100000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getKK(stmt, 1) );
        case ASMPICOBLAZE_INS_COMPARE_SX_SY:
            return ( 0b010101000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: RETURNI ENABLE/DISABLE */
        case ASMPICOBLAZE_INS_RETURNI_ENA:
            return 0b111000000000000001;
        case ASMPICOBLAZE_INS_RETURNI_DIS:
            return 0b111000000000000000;

        /* Instruction: ENABLE/DISABLE INTERRUPT */
        case ASMPICOBLAZE_INS_ENABLE_INT:
            return 0b111100000000000001;
        case ASMPICOBLAZE_INS_DISABLE_INT:
            return 0b111100000000000000;

        /* Instruction: LOAD */
        case ASMPICOBLAZE_INS_LOAD_SX_KK:
            return ( 0b000000000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getKK(stmt, 1) );
        case ASMPICOBLAZE_INS_LOAD_SX_SY:
            return ( 0b000001000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: AND */
        case ASMPICOBLAZE_INS_AND_SX_KK:
            return ( 0b001010000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getKK(stmt, 1) );
        case ASMPICOBLAZE_INS_AND_SX_SY:
            return ( 0b001011000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: OR */
        case ASMPICOBLAZE_INS_OR_SX_KK:
            return ( 0b001100000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getKK(stmt, 1) );
        case ASMPICOBLAZE_INS_OR_SX_SY:
            return ( 0b001101000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: XOR */
        case ASMPICOBLAZE_INS_XOR_SX_KK:
            return ( 0b001110000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getKK(stmt, 1) );
        case ASMPICOBLAZE_INS_XOR_SX_SY:
            return ( 0b001111000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: TEST */
        case ASMPICOBLAZE_INS_TEST_SX_KK:
            return ( 0b010010000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getKK(stmt, 1) );
        case ASMPICOBLAZE_INS_TEST_SX_SY:
            return ( 0b010011000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: STORE */
        case ASMPICOBLAZE_INS_STORE_SX_SS:
            return ( 0b101110000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getSS(stmt, 1) );
        case ASMPICOBLAZE_INS_STORE_SX_SY:
            return ( 0b101111000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: FETCH */
        case ASMPICOBLAZE_INS_FETCH_SX_SS:
            return ( 0b000110000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getSS(stmt, 1) );
        case ASMPICOBLAZE_INS_FETCH_SX_SY:
            return ( 0b000111000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: SR0, SR1, SRX, SRA, RR */
        case ASMPICOBLAZE_INS_SR0_SX:
            return ( 0b100000000000001110 | ( getSXY(stmt, 0) << 8 ) );
        case ASMPICOBLAZE_INS_SR1_SX:
            return ( 0b100000000000001111 | ( getSXY(stmt, 0) << 8 ) );
        case ASMPICOBLAZE_INS_SRX_SX:
            return ( 0b100000000000001010 | ( getSXY(stmt, 0) << 8 ) );
        case ASMPICOBLAZE_INS_SRA_SX:
            return ( 0b100000000000001000 | ( getSXY(stmt, 0) << 8 ) );
        case ASMPICOBLAZE_INS_RR_SX:
            return ( 0b100000000000001100 | ( getSXY(stmt, 0) << 8 ) );

        /* Instruction: SL0, SL1, SLX, SLA, RL */
        case ASMPICOBLAZE_INS_SL0_SX:
            return ( 0b100000000000000110 | ( getSXY(stmt, 0) << 8 ) );
        case ASMPICOBLAZE_INS_SL1_SX:
            return ( 0b100000000000000111 | ( getSXY(stmt, 0) << 8 ) );
        case ASMPICOBLAZE_INS_SLX_SX:
            return ( 0b100000000000000100 | ( getSXY(stmt, 0) << 8 ) );
        case ASMPICOBLAZE_INS_SLA_SX:
            return ( 0b100000000000000000 | ( getSXY(stmt, 0) << 8 ) );
        case ASMPICOBLAZE_INS_RL_SX:
            return ( 0b100000000000000010 | ( getSXY(stmt, 0) << 8 ) );

        /* Instruction: INPUT */
        case ASMPICOBLAZE_INS_INPUT_SX_PP:
            return ( 0b000100000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getPP(stmt, 1) );
        case ASMPICOBLAZE_INS_INPUT_SX_SY:
            return ( 0b000101000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: OUTPUT */
        case ASMPICOBLAZE_INS_OUTPUT_SX_PP:
            return ( 0b101100000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getPP(stmt, 1) );
        case ASMPICOBLAZE_INS_OUTPUT_SX_SY:
            return ( 0b101101000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );
        default:
            return -1;
    }
}
