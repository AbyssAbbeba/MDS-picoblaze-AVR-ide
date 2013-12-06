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
 * @ingroup PicoBlazeAsm
 * @file AsmPicoBlazeInstructionSet1CPLD.cxx
 */
// =============================================================================

#include "AsmPicoBlazeInstructionSet1CPLD.h"

// PicoBlaze assembler semantic analyzer header files.
#include "AsmPicoBlazeSymbolTable.h"
#include "AsmPicoBlazeInstructionSet.h"

unsigned int AsmPicoBlazeInstructionSet1CPLD::getAAA ( const CompilerStatement * stmt,
                                                       int index ) const
{
    return m_instructionSet->checkLimit ( AsmPicoBlazeInstructionSet::LIM_C,
                                          stmt->args()->at(index)->location(),
                                          m_symbolTable->resolveExpr(stmt->args()->at(index), OBS_AAA) );
}

unsigned int AsmPicoBlazeInstructionSet1CPLD::getSXY ( const CompilerStatement * stmt,
                                                       int index ) const
{
    return m_instructionSet->checkLimit ( AsmPicoBlazeInstructionSet::LIM_R,
                                          stmt->args()->at(index)->location(),
                                          m_symbolTable->resolveExpr(stmt->args()->at(index), OBS_SXY) );
}

unsigned int AsmPicoBlazeInstructionSet1CPLD::getKK ( const CompilerStatement * stmt,
                                                      int index ) const
{
    return m_symbolTable->resolveExpr(stmt->args()->at(index), OBS_KK);
}

unsigned int AsmPicoBlazeInstructionSet1CPLD::getPP ( const CompilerStatement * stmt,
                                                      int index ) const
{
    return m_symbolTable->resolveExpr(stmt->args()->at(index), OBS_PP);
}

int AsmPicoBlazeInstructionSet1CPLD::resolveOPcode ( const CompilerStatement * stmt ) const
{
    using namespace CompilerStatementTypes;

    switch ( (int) stmt->type() )
    {
        /* Instruction: JUMP */
        case ASMPICOBLAZE_INS_JUMP_AAA:         return ( 0xd000 | getAAA(stmt, 0) );
        case ASMPICOBLAZE_INS_JUMP_Z_AAA:       return ( 0xd400 | getAAA(stmt, 0) );
        case ASMPICOBLAZE_INS_JUMP_NZ_AAA:      return ( 0xd500 | getAAA(stmt, 0) );
        case ASMPICOBLAZE_INS_JUMP_C_AAA:       return ( 0xd600 | getAAA(stmt, 0) );
        case ASMPICOBLAZE_INS_JUMP_NC_AAA:      return ( 0xd700 | getAAA(stmt, 0) );

        /* Instruction: CALL */
        case ASMPICOBLAZE_INS_CALL_AAA:         return ( 0xd800 | getAAA(stmt, 0) );
        case ASMPICOBLAZE_INS_CALL_Z_AAA:       return ( 0xdc00 | getAAA(stmt, 0) );
        case ASMPICOBLAZE_INS_CALL_NZ_AAA:      return ( 0xdd00 | getAAA(stmt, 0) );
        case ASMPICOBLAZE_INS_CALL_C_AAA:       return ( 0xde00 | getAAA(stmt, 0) );
        case ASMPICOBLAZE_INS_CALL_NC_AAA:      return ( 0xdf00 | getAAA(stmt, 0) );

        /* Instruction: RETURN */
        case ASMPICOBLAZE_INS_RETURN:           return 0x9000;
        case ASMPICOBLAZE_INS_RETURN_Z:         return 0x9400;
        case ASMPICOBLAZE_INS_RETURN_NZ:        return 0x9500;
        case ASMPICOBLAZE_INS_RETURN_C:         return 0x9600;
        case ASMPICOBLAZE_INS_RETURN_NC:        return 0x9700;

        /* Instruction: ENABLE/DISABLE INTERRUPT */
        case ASMPICOBLAZE_INS_ENABLE_INT:       return 0xf000;
        case ASMPICOBLAZE_INS_DISABLE_INT:      return 0xf001;

        /* Instruction: RETURNI ENABLE/DISABLE */
        case ASMPICOBLAZE_INS_RETURNI_ENA:      return 0xb000;
        case ASMPICOBLAZE_INS_RETURNI_DIS:      return 0xb001;

        /* Instruction: LOAD */
        case ASMPICOBLAZE_INS_LOAD_SX_KK:       return ( 0x0000 | ( getSXY(stmt, 0) << 8 ) |   getKK (stmt, 1)        );
        case ASMPICOBLAZE_INS_LOAD_SX_SY:       return ( 0x4000 | ( getSXY(stmt, 0) << 8 ) | ( getSXY(stmt, 1) << 5 ) );

        /* Instruction: AND */
        case ASMPICOBLAZE_INS_AND_SX_KK:        return ( 0x0800 | ( getSXY(stmt, 0) << 8 ) |   getKK (stmt, 1)        );
        case ASMPICOBLAZE_INS_AND_SX_SY:        return ( 0x4800 | ( getSXY(stmt, 0) << 8 ) | ( getSXY(stmt, 1) << 5 ) );

        /* Instruction: OR */
        case ASMPICOBLAZE_INS_OR_SX_KK:         return ( 0x1000 | ( getSXY(stmt, 0) << 8 ) |   getKK (stmt, 1)        );
        case ASMPICOBLAZE_INS_OR_SX_SY:         return ( 0x5000 | ( getSXY(stmt, 0) << 8 ) | ( getSXY(stmt, 1) << 5 ) );

        /* Instruction: XOR */
        case ASMPICOBLAZE_INS_XOR_SX_KK:        return ( 0x1800 | ( getSXY(stmt, 0) << 8 ) |   getKK (stmt, 1)        );
        case ASMPICOBLAZE_INS_XOR_SX_SY:        return ( 0x5800 | ( getSXY(stmt, 0) << 8 ) | ( getSXY(stmt, 1) << 5 ) );

        /* Instruction: ADD */
        case ASMPICOBLAZE_INS_ADD_SX_KK:        return ( 0x2000 | ( getSXY(stmt, 0) << 8 ) |   getKK (stmt, 1)        );
        case ASMPICOBLAZE_INS_ADD_SX_SY:        return ( 0x6000 | ( getSXY(stmt, 0) << 8 ) | ( getSXY(stmt, 1) << 5 ) );

        /* Instruction: ADDCY */
        case ASMPICOBLAZE_INS_ADDCY_SX_KK:      return ( 0x2800 | ( getSXY(stmt, 0) << 8 ) |   getKK (stmt, 1)        );
        case ASMPICOBLAZE_INS_ADDCY_SX_SY:      return ( 0x6800 | ( getSXY(stmt, 0) << 8 ) | ( getSXY(stmt, 1) << 5 ) );

        /* Instruction: SUB */
        case ASMPICOBLAZE_INS_SUB_SX_KK:        return ( 0x3000 | ( getSXY(stmt, 0) << 8 ) |   getKK (stmt, 1)        );
        case ASMPICOBLAZE_INS_SUB_SX_SY:        return ( 0x7000 | ( getSXY(stmt, 0) << 8 ) | ( getSXY(stmt, 1) << 5 ) );

        /* Instruction: SUBCY */
        case ASMPICOBLAZE_INS_SUBCY_SX_KK:      return ( 0x3800 | ( getSXY(stmt, 0) << 8 ) |   getKK (stmt, 1)        );
        case ASMPICOBLAZE_INS_SUBCY_SX_SY:      return ( 0x7800 | ( getSXY(stmt, 0) << 8 ) | ( getSXY(stmt, 1) << 5 ) );

        /* Instruction: SR0, SR1, SRX, SRA, RR */
        case ASMPICOBLAZE_INS_SR0_SX:           return ( 0xa00e | ( getSXY(stmt, 0) << 8 ) );
        case ASMPICOBLAZE_INS_SR1_SX:           return ( 0xa00f | ( getSXY(stmt, 0) << 8 ) );
        case ASMPICOBLAZE_INS_SRX_SX:           return ( 0xa00a | ( getSXY(stmt, 0) << 8 ) );
        case ASMPICOBLAZE_INS_SRA_SX:           return ( 0xa008 | ( getSXY(stmt, 0) << 8 ) );
        case ASMPICOBLAZE_INS_RR_SX:            return ( 0xa00c | ( getSXY(stmt, 0) << 8 ) );

        /* Instruction: SL0, SL1, SLX, SLA, RL */
        case ASMPICOBLAZE_INS_SL0_SX:           return ( 0xa006 | ( getSXY(stmt, 0) << 8 ) );
        case ASMPICOBLAZE_INS_SL1_SX:           return ( 0xa007 | ( getSXY(stmt, 0) << 8 ) );
        case ASMPICOBLAZE_INS_SLX_SX:           return ( 0xa002 | ( getSXY(stmt, 0) << 8 ) );
        case ASMPICOBLAZE_INS_SLA_SX:           return ( 0xa000 | ( getSXY(stmt, 0) << 8 ) );
        case ASMPICOBLAZE_INS_RL_SX:            return ( 0xa004 | ( getSXY(stmt, 0) << 8 ) );

        /* Instruction: INPUT */
        case ASMPICOBLAZE_INS_INPUT_SX_PP:      return ( 0x8000 | ( getSXY(stmt, 0) << 8 ) |   getPP (stmt, 1)        );
        case ASMPICOBLAZE_INS_INPUT_SX_SY:      return ( 0xc000 | ( getSXY(stmt, 0) << 8 ) | ( getSXY(stmt, 1) << 5 ) );

        /* Instruction: OUTPUT */
        case ASMPICOBLAZE_INS_OUTPUT_SX_PP:     return ( 0x8800 | ( getSXY(stmt, 0) << 8 ) |   getPP (stmt, 1)        );
        case ASMPICOBLAZE_INS_OUTPUT_SX_SY:     return ( 0xc800 | ( getSXY(stmt, 0) << 8 ) | ( getSXY(stmt, 1) << 5 ) );

        default:
            // Unrecognized instruction.
            return -1;
    }
}
