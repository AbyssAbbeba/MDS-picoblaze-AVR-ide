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
 * @ingroup Kcpsm3Asm
 * @file AsmKcpsm3InstructionSet.cxx
 */
// =============================================================================

#include "AsmKcpsm3InstructionSet.h"
#include "AsmKcpsm3SymbolTable.h"

#include "../../CompilerStatement.h"
#include "../../CompilerOptions.h"

unsigned int AsmKcpsm3InstructionSet::checkLimit ( LimitType type,
                                                   const CompilerSourceLocation & location,
                                                   unsigned int value ) const
{
    int limit = -1;
    switch ( type )
    {
        case LIM_C:
            limit = m_opts->m_processorlimits.m_iCodeMemSize;
            break;
        case LIM_R:
            limit = m_opts->m_processorlimits.m_regFileSize;
            break;
        case LIM_D:
            limit = m_opts->m_processorlimits.m_iDataMemSize;
            break;
    }

    if ( -1 == limit )
    {
        // There is no limit set.
        return value;
    }

    if ( int(value) < limit )
    {
        // Limit is not exceeded.
        return value;
    }

    // Limit exceeded, print error message.
    std::string msg;
    switch ( type )
    {
        case LIM_C:
            msg = QObject::tr("user defined memory limit for program memory exceeded").toStdString();
            break;
        case LIM_R:
            msg = QObject::tr("user defined memory limit for register file exceeded").toStdString();
            break;
        case LIM_D:
            msg = QObject::tr("user defined memory limit for Scratch Pad RAM exceeded").toStdString();
            break;
    }
    m_compilerCore -> compilerMessage ( location, CompilerBase::MT_ERROR, msg );

    return 0;
}

unsigned int AsmKcpsm3InstructionSet::getAAA ( const CompilerStatement * stmt,
                                               int index ) const
{
    unsigned int result = checkLimit ( LIM_C,
                                       stmt->args()->at(index)->location(),
                                       m_symbolTable->resolveExpr(stmt->args()->at(index), OBS_AAA) );

    // Make it (AAA - 1);
    return ( result - 1 ) & ~( ~0U << OBS_AAA );
}

unsigned int AsmKcpsm3InstructionSet::getSXY ( const CompilerStatement * stmt,
                                               int index ) const
{
    return checkLimit ( LIM_R,
                        stmt->args()->at(index)->location(),
                        m_symbolTable->resolveExpr(stmt->args()->at(index), OBS_SXY) );
}

unsigned int AsmKcpsm3InstructionSet::getKK ( const CompilerStatement * stmt,
                                              int index ) const
{
    return m_symbolTable->resolveExpr(stmt->args()->at(index), OBS_KK);
}

unsigned int AsmKcpsm3InstructionSet::getPP ( const CompilerStatement * stmt,
                                              int index ) const
{
    return m_symbolTable->resolveExpr(stmt->args()->at(index), OBS_KK);
}

unsigned int AsmKcpsm3InstructionSet::getSS ( const CompilerStatement * stmt,
                                              int index ) const
{
    return checkLimit ( LIM_D,
                        stmt->args()->at(index)->location(),
                        m_symbolTable->resolveExpr(stmt->args()->at(index), OBS_SS) );
}

int AsmKcpsm3InstructionSet::resolveOPcode ( CompilerStatement * stmt ) const
{
    using namespace CompilerStatementTypes;

    switch ( (int) stmt->type() )
    {
        /* Instruction: JUMP */
        case ASMKCPSM3_INS_JUMP_AAA:
            return ( 0b110100000000000000 | getAAA(stmt, 0) );
        case ASMKCPSM3_INS_JUMP_Z_AAA:
            return ( 0b110101000000000000 | getAAA(stmt, 0) );
        case ASMKCPSM3_INS_JUMP_NZ_AAA:
            return ( 0b110101010000000000 | getAAA(stmt, 0) );
        case ASMKCPSM3_INS_JUMP_C_AAA:
            return ( 0b110101100000000000 | getAAA(stmt, 0) );
        case ASMKCPSM3_INS_JUMP_NC_AAA:
            return ( 0b110101110000000000 | getAAA(stmt, 0) );

        /* Instruction: CALL */
        case ASMKCPSM3_INS_CALL_AAA:
            return ( 0b110000000000000000 | getAAA(stmt, 0) );
        case ASMKCPSM3_INS_CALL_Z_AAA:
            return ( 0b110001000000000000 | getAAA(stmt, 0) );
        case ASMKCPSM3_INS_CALL_NZ_AAA:
            return ( 0b110001010000000000 | getAAA(stmt, 0) );
        case ASMKCPSM3_INS_CALL_C_AAA:
            return ( 0b110001100000000000 | getAAA(stmt, 0) );
        case ASMKCPSM3_INS_CALL_NC_AAA:
            return ( 0b110001110000000000 | getAAA(stmt, 0) );

        /* Instruction: RETURN */
        case ASMKCPSM3_INS_RETURN:
            return 0b101010000000000000;
        case ASMKCPSM3_INS_RETURN_Z:
            return 0b101011000000000000;
        case ASMKCPSM3_INS_RETURN_NZ:
            return 0b101011010000000000;
        case ASMKCPSM3_INS_RETURN_C:
            return 0b101011100000000000;
        case ASMKCPSM3_INS_RETURN_NC:
            return 0b101011110000000000;

        /* Instruction: ADD */
        case ASMKCPSM3_INS_ADD_SX_KK:
            return ( 0b011000000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getKK(stmt, 1) );
        case ASMKCPSM3_INS_ADD_SX_SY:
            return ( 0b011001000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: ADDCY */
        case ASMKCPSM3_INS_ADDCY_SX_KK:
            return ( 0b011010000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getKK(stmt, 1) );
        case ASMKCPSM3_INS_ADDCY_SX_SY:
            return ( 0b011011000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: SUB */
        case ASMKCPSM3_INS_SUB_SX_KK:
            return ( 0b011100000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getKK(stmt, 1) );
        case ASMKCPSM3_INS_SUB_SX_SY:
            return ( 0b011101000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: SUBCY */
        case ASMKCPSM3_INS_SUBCY_SX_KK:
            return ( 0b011110000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getKK(stmt, 1) );
        case ASMKCPSM3_INS_SUBCY_SX_SY:
            return ( 0b011111000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: COMPARE */
        case ASMKCPSM3_INS_COMPARE_SX_KK:
            return ( 0b010100000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getKK(stmt, 1) );
        case ASMKCPSM3_INS_COMPARE_SX_SY:
            return ( 0b010101000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: RETURNI ENABLE/DISABLE */
        case ASMKCPSM3_INS_RETURNI_ENA:
            return 0b111000000000000001;
        case ASMKCPSM3_INS_RETURNI_DIS:
            return 0b111000000000000000;

        /* Instruction: ENABLE/DISABLE INTERRUPT */
        case ASMKCPSM3_INS_ENABLE_INT:
            return 0b111100000000000001;
        case ASMKCPSM3_INS_DISABLE_INT:
            return 0b111100000000000000;

        /* Instruction: LOAD */
        case ASMKCPSM3_INS_LOAD_SX_KK:
            return ( 0b000000000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getKK(stmt, 1) );
        case ASMKCPSM3_INS_LOAD_SX_SY:
            return ( 0b000001000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: AND */
        case ASMKCPSM3_INS_AND_SX_KK:
            return ( 0b001010000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getKK(stmt, 1) );
        case ASMKCPSM3_INS_AND_SX_SY:
            return ( 0b001011000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: OR */
        case ASMKCPSM3_INS_OR_SX_KK:
            return ( 0b001100000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getKK(stmt, 1) );
        case ASMKCPSM3_INS_OR_SX_SY:
            return ( 0b001101000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: XOR */
        case ASMKCPSM3_INS_XOR_SX_KK:
            return ( 0b001110000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getKK(stmt, 1) );
        case ASMKCPSM3_INS_XOR_SX_SY:
            return ( 0b001111000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: TEST */
        case ASMKCPSM3_INS_TEST_SX_KK:
            return ( 0b010010000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getKK(stmt, 1) );
        case ASMKCPSM3_INS_TEST_SX_SY:
            return ( 0b010011000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: STORE */
        case ASMKCPSM3_INS_STORE_SX_SS:
            return ( 0b101110000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getSS(stmt, 1) );
        case ASMKCPSM3_INS_STORE_SX_SY:
            return ( 0b101111000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: FETCH */
        case ASMKCPSM3_INS_FETCH_SX_SS:
            return ( 0b000110000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getSS(stmt, 1) );
        case ASMKCPSM3_INS_FETCH_SX_SY:
            return ( 0b000111000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: SR0, SR1, SRX, SRA, RR */
        case ASMKCPSM3_INS_SR0_SX:
            return ( 0b100000000000001110 | ( getSXY(stmt, 0) << 8 ) );
        case ASMKCPSM3_INS_SR1_SX:
            return ( 0b100000000000001111 | ( getSXY(stmt, 0) << 8 ) );
        case ASMKCPSM3_INS_SRX_SX:
            return ( 0b100000000000001010 | ( getSXY(stmt, 0) << 8 ) );
        case ASMKCPSM3_INS_SRA_SX:
            return ( 0b100000000000001000 | ( getSXY(stmt, 0) << 8 ) );
        case ASMKCPSM3_INS_RR_SX:
            return ( 0b100000000000001100 | ( getSXY(stmt, 0) << 8 ) );

        /* Instruction: SL0, SL1, SLX, SLA, RL */
        case ASMKCPSM3_INS_SL0_SX:
            return ( 0b100000000000000110 | ( getSXY(stmt, 0) << 8 ) );
        case ASMKCPSM3_INS_SL1_SX:
            return ( 0b100000000000000111 | ( getSXY(stmt, 0) << 8 ) );
        case ASMKCPSM3_INS_SLX_SX:
            return ( 0b100000000000000100 | ( getSXY(stmt, 0) << 8 ) );
        case ASMKCPSM3_INS_SLA_SX:
            return ( 0b100000000000000000 | ( getSXY(stmt, 0) << 8 ) );
        case ASMKCPSM3_INS_RL_SX:
            return ( 0b100000000000000010 | ( getSXY(stmt, 0) << 8 ) );

        /* Instruction: INPUT */
        case ASMKCPSM3_INS_INPUT_SX_PP:
            return ( 0b000100000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getPP(stmt, 1) );
        case ASMKCPSM3_INS_INPUT_SX_SY:
            return ( 0b000101000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );

        /* Instruction: OUTPUT */
        case ASMKCPSM3_INS_OUTPUT_SX_PP:
            return ( 0b101100000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | getPP(stmt, 1) );
        case ASMKCPSM3_INS_OUTPUT_SX_SY:
            return ( 0b101101000000000000
                     | ( getSXY(stmt, 0) << 8 )
                     | ( getSXY(stmt, 1) << 4 ) );
        default:
            return -1;
    }
}
inline void AsmKcpsm3InstructionSet::detAccSymTypes ( const CompilerStatement * stmt,
                                                      int * acceptableTypes ) const
{
    using namespace CompilerStatementTypes;

    acceptableTypes[0] = (int) ( AsmKcpsm3SymbolTable::STYPE_NUMBER | AsmKcpsm3SymbolTable::STYPE_EXPRESSION );
    acceptableTypes[1] = (int) ( AsmKcpsm3SymbolTable::STYPE_NUMBER | AsmKcpsm3SymbolTable::STYPE_EXPRESSION );

    switch ( (int) stmt->type() )
    {
        case ASMKCPSM3_INS_JUMP_AAA:
        case ASMKCPSM3_INS_JUMP_Z_AAA:
        case ASMKCPSM3_INS_JUMP_NZ_AAA:
        case ASMKCPSM3_INS_JUMP_C_AAA:
        case ASMKCPSM3_INS_JUMP_NC_AAA:
        case ASMKCPSM3_INS_CALL_AAA:
        case ASMKCPSM3_INS_CALL_Z_AAA:
        case ASMKCPSM3_INS_CALL_NZ_AAA:
        case ASMKCPSM3_INS_CALL_C_AAA:
        case ASMKCPSM3_INS_CALL_NC_AAA:
            acceptableTypes[0] |= AsmKcpsm3SymbolTable::STYPE_LABEL;
            break;

        case ASMKCPSM3_INS_RETURN:
        case ASMKCPSM3_INS_RETURN_Z:
        case ASMKCPSM3_INS_RETURN_NZ:
        case ASMKCPSM3_INS_RETURN_C:
        case ASMKCPSM3_INS_RETURN_NC:
        case ASMKCPSM3_INS_RETURNI_ENA:
        case ASMKCPSM3_INS_RETURNI_DIS:
        case ASMKCPSM3_INS_ENABLE_INT:
        case ASMKCPSM3_INS_DISABLE_INT:
            break;

        case ASMKCPSM3_INS_TEST_SX_KK:
        case ASMKCPSM3_INS_ADD_SX_KK:
        case ASMKCPSM3_INS_ADDCY_SX_KK:
        case ASMKCPSM3_INS_SUB_SX_KK:
        case ASMKCPSM3_INS_SUBCY_SX_KK:
        case ASMKCPSM3_INS_COMPARE_SX_KK:
        case ASMKCPSM3_INS_LOAD_SX_KK:
        case ASMKCPSM3_INS_AND_SX_KK:
        case ASMKCPSM3_INS_OR_SX_KK:
        case ASMKCPSM3_INS_XOR_SX_KK:
            acceptableTypes[0] |= AsmKcpsm3SymbolTable::STYPE_REGISTER;
            break;

        case ASMKCPSM3_INS_ADD_SX_SY:
        case ASMKCPSM3_INS_ADDCY_SX_SY:
        case ASMKCPSM3_INS_SUB_SX_SY:
        case ASMKCPSM3_INS_SUBCY_SX_SY:
        case ASMKCPSM3_INS_COMPARE_SX_SY:
        case ASMKCPSM3_INS_LOAD_SX_SY:
        case ASMKCPSM3_INS_AND_SX_SY:
        case ASMKCPSM3_INS_OR_SX_SY:
        case ASMKCPSM3_INS_XOR_SX_SY:
        case ASMKCPSM3_INS_TEST_SX_SY:
        case ASMKCPSM3_INS_STORE_SX_SY:
        case ASMKCPSM3_INS_FETCH_SX_SY:
        case ASMKCPSM3_INS_OUTPUT_SX_SY:
        case ASMKCPSM3_INS_INPUT_SX_SY:
            acceptableTypes[0] |= AsmKcpsm3SymbolTable::STYPE_REGISTER;
            acceptableTypes[1] |= AsmKcpsm3SymbolTable::STYPE_REGISTER;
            break;

        case ASMKCPSM3_INS_SR0_SX:
        case ASMKCPSM3_INS_SR1_SX:
        case ASMKCPSM3_INS_SRX_SX:
        case ASMKCPSM3_INS_SRA_SX:
        case ASMKCPSM3_INS_RR_SX:
        case ASMKCPSM3_INS_SL0_SX:
        case ASMKCPSM3_INS_SL1_SX:
        case ASMKCPSM3_INS_SLA_SX:
        case ASMKCPSM3_INS_SLX_SX:
        case ASMKCPSM3_INS_RL_SX:
            acceptableTypes[0] |= AsmKcpsm3SymbolTable::STYPE_REGISTER;
            break;

        case ASMKCPSM3_INS_STORE_SX_SS:
        case ASMKCPSM3_INS_FETCH_SX_SS:
            acceptableTypes[0] |= AsmKcpsm3SymbolTable::STYPE_REGISTER;
            acceptableTypes[1] |= AsmKcpsm3SymbolTable::STYPE_DATA;
            break;

        case ASMKCPSM3_INS_OUTPUT_SX_PP:
        case ASMKCPSM3_INS_INPUT_SX_PP:
            acceptableTypes[0] |= AsmKcpsm3SymbolTable::STYPE_REGISTER;
            acceptableTypes[1] |= AsmKcpsm3SymbolTable::STYPE_PORT;
            break;

        default:
            break;
    }
}

inline std::string AsmKcpsm3InstructionSet::getInstructionName ( const CompilerStatement * stmt ) const
{
    using namespace CompilerStatementTypes;

    switch ( (int) stmt->type() )
    {
        case ASMKCPSM3_INS_JUMP_AAA:      return "JUMP aaa";
        case ASMKCPSM3_INS_JUMP_Z_AAA:    return "JUMP Z, aaa";
        case ASMKCPSM3_INS_JUMP_NZ_AAA:   return "JUMP NZ, aaa";
        case ASMKCPSM3_INS_JUMP_C_AAA:    return "JUMP C, aaa";
        case ASMKCPSM3_INS_JUMP_NC_AAA:   return "JUMP NC, aaa";
        case ASMKCPSM3_INS_CALL_AAA:      return "CALL aaa";
        case ASMKCPSM3_INS_CALL_Z_AAA:    return "CALL Z, aaa";
        case ASMKCPSM3_INS_CALL_NZ_AAA:   return "CALL NZ, aaa";
        case ASMKCPSM3_INS_CALL_C_AAA:    return "CALL C, aaa";
        case ASMKCPSM3_INS_CALL_NC_AAA:   return "CALL NC, aaa";
        case ASMKCPSM3_INS_RETURN:        return "RETURN";
        case ASMKCPSM3_INS_RETURN_Z:      return "RETURN Z";
        case ASMKCPSM3_INS_RETURN_NZ:     return "RETURN NZ";
        case ASMKCPSM3_INS_RETURN_C:      return "RETURN C";
        case ASMKCPSM3_INS_RETURN_NC:     return "RETURN NC";
        case ASMKCPSM3_INS_ADD_SX_KK:     return "ADD sX, kk";
        case ASMKCPSM3_INS_ADD_SX_SY:     return "ADD sX, sY";
        case ASMKCPSM3_INS_ADDCY_SX_KK:   return "ADDCY sX, kk";
        case ASMKCPSM3_INS_ADDCY_SX_SY:   return "ADDCY sX, sY";
        case ASMKCPSM3_INS_SUB_SX_KK:     return "ADD sX, kk";
        case ASMKCPSM3_INS_SUB_SX_SY:     return "SUB sX, sY";
        case ASMKCPSM3_INS_SUBCY_SX_KK:   return "SUBCY sX, kk";
        case ASMKCPSM3_INS_SUBCY_SX_SY:   return "SUBCY sX, sY";
        case ASMKCPSM3_INS_COMPARE_SX_KK: return "COMPARE sX, kk";
        case ASMKCPSM3_INS_RETURNI_ENA:   return "RETURN ENABLE";
        case ASMKCPSM3_INS_RETURNI_DIS:   return "RETURN DISABLE";
        case ASMKCPSM3_INS_COMPARE_SX_SY: return "COMPARE sX, sY";
        case ASMKCPSM3_INS_ENABLE_INT:    return "ENABLE INTERRUPT";
        case ASMKCPSM3_INS_DISABLE_INT:   return "DISABLE INTERRUPT";
        case ASMKCPSM3_INS_LOAD_SX_KK:    return "LOAD sX, kk";
        case ASMKCPSM3_INS_LOAD_SX_SY:    return "LOAD sX, sY";
        case ASMKCPSM3_INS_AND_SX_KK:     return "AND sX, kk";
        case ASMKCPSM3_INS_AND_SX_SY:     return "AND sX, sY";
        case ASMKCPSM3_INS_OR_SX_KK:      return "OR sX, kk";
        case ASMKCPSM3_INS_OR_SX_SY:      return "OR sX, sY";
        case ASMKCPSM3_INS_XOR_SX_KK:     return "XOR sX, kk";
        case ASMKCPSM3_INS_XOR_SX_SY:     return "YOR sX, sY";
        case ASMKCPSM3_INS_TEST_SX_KK:    return "TEST sX, kk";
        case ASMKCPSM3_INS_TEST_SX_SY:    return "TEST sX, sY";
        case ASMKCPSM3_INS_STORE_SX_SS:   return "STORE sX, ss";
        case ASMKCPSM3_INS_STORE_SX_SY:   return "STORE sX, sY";
        case ASMKCPSM3_INS_FETCH_SX_SS:   return "FETCH sX, ss";
        case ASMKCPSM3_INS_FETCH_SX_SY:   return "FETCH sX, sY";
        case ASMKCPSM3_INS_SR0_SX:        return "SR0 sX";
        case ASMKCPSM3_INS_SR1_SX:        return "SR1 sX";
        case ASMKCPSM3_INS_SRX_SX:        return "SRX sX";
        case ASMKCPSM3_INS_SRA_SX:        return "SRA sX";
        case ASMKCPSM3_INS_RR_SX:         return "RR sX";
        case ASMKCPSM3_INS_SL0_SX:        return "SL0 sX";
        case ASMKCPSM3_INS_SL1_SX:        return "SL1 sX";
        case ASMKCPSM3_INS_SLA_SX:        return "SLA sX";
        case ASMKCPSM3_INS_SLX_SX:        return "SLX sX";
        case ASMKCPSM3_INS_RL_SX:         return "RL sX";
        case ASMKCPSM3_INS_OUTPUT_SX_PP:  return "OUTPUT sX, pp";
        case ASMKCPSM3_INS_OUTPUT_SX_SY:  return "OUTPUT sX, sY";
        case ASMKCPSM3_INS_INPUT_SX_PP:   return "INPUT sX, pp";
        case ASMKCPSM3_INS_INPUT_SX_SY:   return "INPUT sX, sY";
        default:                          return "<unknown>";
    }
}

inline std::string AsmKcpsm3InstructionSet::getSymbolTypes ( int types ) const
{
    std::string result;

    if ( types & AsmKcpsm3SymbolTable::STYPE_NUMBER )
    {
        result += "number";
    }

    if ( types & AsmKcpsm3SymbolTable::STYPE_REGISTER )
    {
        if ( false == result.empty() )
        {
            result += ", or ";
        }
        result += "register";
    }

    if ( types & AsmKcpsm3SymbolTable::STYPE_LABEL )
    {
        if ( false == result.empty() )
        {
            result += ", or ";
        }
        result += "label";
    }

    if ( types & AsmKcpsm3SymbolTable::STYPE_PORT )
    {
        if ( false == result.empty() )
        {
            result += ", or ";
        }
        result += "port";
    }

    if ( types & AsmKcpsm3SymbolTable::STYPE_DATA )
    {
        if ( false == result.empty() )
        {
            result += ", or ";
        }
        result += "data";
    }

    if ( types & AsmKcpsm3SymbolTable::STYPE_EXPRESSION )
    {
        if ( false == result.empty() )
        {
            result += ", or ";
        }
        result += "expression";
    }

    if ( true == result.empty() )
    {
        result = "<no type allowed>";
    }

    return result;
}

void AsmKcpsm3InstructionSet::encapsulate ( CompilerStatement * stmt,
                                            int codePointer ) const
{
    int acceptableTypes[2];
    detAccSymTypes(stmt, acceptableTypes);

    int i = 0;
    for ( CompilerExpr * arg = stmt->args();
          NULL != arg;
          arg = arg->next() )
    {
        int symbolType = (int) m_symbolTable->getType(arg);
        if ( 0 == ( acceptableTypes[i] & symbolType ) )
        {
            m_compilerCore->compilerMessage(arg->location(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "instruction `%1' requires operand #%2 to be of type(s): %3"
                                                          "; while the given operand is of type: %4" )
                                                        . arg ( getInstructionName(stmt).c_str() )
                                                        . arg ( i + 1 )
                                                        . arg ( getSymbolTypes(acceptableTypes[i]).c_str() )
                                                        . arg ( getSymbolTypes(symbolType).c_str() )
                                                        . toStdString() );
        }

        m_symbolTable->resolveSymbols(arg, codePointer);
        i++;
    }
}

bool AsmKcpsm3InstructionSet::isInstruction ( const CompilerStatement * const stmt ) const
{
    using namespace CompilerStatementTypes;

    switch ( (int) stmt->type() )
    {
        case ASMKCPSM3_INS_JUMP_AAA:    case ASMKCPSM3_INS_JUMP_Z_AAA:  case ASMKCPSM3_INS_JUMP_NZ_AAA:
        case ASMKCPSM3_INS_JUMP_C_AAA:  case ASMKCPSM3_INS_JUMP_NC_AAA: case ASMKCPSM3_INS_CALL_AAA:
        case ASMKCPSM3_INS_CALL_Z_AAA:  case ASMKCPSM3_INS_CALL_NZ_AAA: case ASMKCPSM3_INS_CALL_C_AAA:
        case ASMKCPSM3_INS_CALL_NC_AAA: case ASMKCPSM3_INS_RETURN:      case ASMKCPSM3_INS_RETURN_Z:
        case ASMKCPSM3_INS_RETURN_NZ:   case ASMKCPSM3_INS_RETURN_C:    case ASMKCPSM3_INS_RETURN_NC:
        case ASMKCPSM3_INS_ADD_SX_KK:   case ASMKCPSM3_INS_ADD_SX_SY:   case ASMKCPSM3_INS_ADDCY_SX_KK:
        case ASMKCPSM3_INS_ADDCY_SX_SY: case ASMKCPSM3_INS_SUB_SX_KK:   case ASMKCPSM3_INS_SUB_SX_SY:
        case ASMKCPSM3_INS_SUBCY_SX_KK: case ASMKCPSM3_INS_SUBCY_SX_SY: case ASMKCPSM3_INS_COMPARE_SX_KK:
        case ASMKCPSM3_INS_RETURNI_ENA: case ASMKCPSM3_INS_RETURNI_DIS: case ASMKCPSM3_INS_COMPARE_SX_SY:
        case ASMKCPSM3_INS_ENABLE_INT:  case ASMKCPSM3_INS_DISABLE_INT: case ASMKCPSM3_INS_LOAD_SX_KK:
        case ASMKCPSM3_INS_LOAD_SX_SY:  case ASMKCPSM3_INS_AND_SX_KK:   case ASMKCPSM3_INS_AND_SX_SY:
        case ASMKCPSM3_INS_OR_SX_KK:    case ASMKCPSM3_INS_OR_SX_SY:    case ASMKCPSM3_INS_XOR_SX_KK:
        case ASMKCPSM3_INS_XOR_SX_SY:   case ASMKCPSM3_INS_TEST_SX_KK:  case ASMKCPSM3_INS_TEST_SX_SY:
        case ASMKCPSM3_INS_STORE_SX_SS: case ASMKCPSM3_INS_STORE_SX_SY: case ASMKCPSM3_INS_FETCH_SX_SS:
        case ASMKCPSM3_INS_FETCH_SX_SY: case ASMKCPSM3_INS_SR0_SX:      case ASMKCPSM3_INS_SR1_SX:
        case ASMKCPSM3_INS_SRX_SX:      case ASMKCPSM3_INS_SRA_SX:      case ASMKCPSM3_INS_RR_SX:
        case ASMKCPSM3_INS_SL0_SX:      case ASMKCPSM3_INS_SL1_SX:      case ASMKCPSM3_INS_OUTPUT_SX_SY:
        case ASMKCPSM3_INS_SLA_SX:      case ASMKCPSM3_INS_RL_SX:       case ASMKCPSM3_INS_INPUT_SX_PP:
        case ASMKCPSM3_INS_INPUT_SX_SY: case ASMKCPSM3_INS_SLX_SX:      case ASMKCPSM3_INS_OUTPUT_SX_PP:
            return true;
        default:
            return false;
    }
}
