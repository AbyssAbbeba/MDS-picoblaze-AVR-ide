// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup Kcpsm3Asm
 * @file AsmKcpsm3InstructionSet.cxx
 */
// =============================================================================

#include "AsmKcpsm3InstructionSet.h"
#include "../../CompilerStatement.h"

int AsmKcpsm3InstructionSet::resolveOPcode ( CompilerStatement    * stmt,
                                             AsmKcpsm3SymbolTable * symbolTable )
{
    using namespace StatementTypes;

    switch ( (int) stmt->type() )
    {
        /* Instruction: JUMP */
        case ASMKCPSM3_INS_JUMP_AAA:
            return ( 0b110100000000000000 | symbolTable->resolveExpr ( stmt->args()->at(0), OBS_AAA ) );
        case ASMKCPSM3_INS_JUMP_Z_AAA:
            return ( 0b110101000000000000 | symbolTable->resolveExpr ( stmt->args()->at(0), OBS_AAA ) );
        case ASMKCPSM3_INS_JUMP_NZ_AAA:
            return ( 0b110101010000000000 | symbolTable->resolveExpr ( stmt->args()->at(0), OBS_AAA ) );
        case ASMKCPSM3_INS_JUMP_C_AAA:
            return ( 0b110101100000000000 | symbolTable->resolveExpr ( stmt->args()->at(0), OBS_AAA ) );
        case ASMKCPSM3_INS_JUMP_NC_AAA:
            return ( 0b110101110000000000 | symbolTable->resolveExpr ( stmt->args()->at(0), OBS_AAA ) );

        /* Instruction: CALL */
        case ASMKCPSM3_INS_CALL_AAA:
            return ( 0b110000000000000000 | symbolTable->resolveExpr ( stmt->args()->at(0), OBS_AAA ) );
        case ASMKCPSM3_INS_CALL_Z_AAA:
            return ( 0b110001000000000000 | symbolTable->resolveExpr ( stmt->args()->at(0), OBS_AAA ) );
        case ASMKCPSM3_INS_CALL_NZ_AAA:
            return ( 0b110001010000000000 | symbolTable->resolveExpr ( stmt->args()->at(0), OBS_AAA ) );
        case ASMKCPSM3_INS_CALL_C_AAA:
            return ( 0b110001100000000000 | symbolTable->resolveExpr ( stmt->args()->at(0), OBS_AAA ) );
        case ASMKCPSM3_INS_CALL_NC_AAA:
            return ( 0b110001110000000000 | symbolTable->resolveExpr ( stmt->args()->at(0), OBS_AAA ) );

        /* Instruction: RETURN */
        case ASMKCPSM3_INS_RETURN:
            return ( 0b101010000000000000 | symbolTable->resolveExpr ( stmt->args()->at(0), OBS_AAA ) );
        case ASMKCPSM3_INS_RETURN_Z:
            return ( 0b101011000000000000 | symbolTable->resolveExpr ( stmt->args()->at(0), OBS_AAA ) );
        case ASMKCPSM3_INS_RETURN_NZ:
            return ( 0b101011010000000000 | symbolTable->resolveExpr ( stmt->args()->at(0), OBS_AAA ) );
        case ASMKCPSM3_INS_RETURN_C:
            return ( 0b101011100000000000 | symbolTable->resolveExpr ( stmt->args()->at(0), OBS_AAA ) );
        case ASMKCPSM3_INS_RETURN_NC:
            return ( 0b101011110000000000 | symbolTable->resolveExpr ( stmt->args()->at(0), OBS_AAA ) );

        /* Instruction: ADD */
        case ASMKCPSM3_INS_ADD_SX_KK:
            return ( 0b011000000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_KK  ) ) );
        case ASMKCPSM3_INS_ADD_SX_SY:
            return ( 0b011001000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_SXY ) << 4 ) );

        /* Instruction: ADDCY */
        case ASMKCPSM3_INS_ADDCY_SX_KK:
            return ( 0b011010000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_KK  ) ) );
        case ASMKCPSM3_INS_ADDCY_SX_SY:
            return ( 0b011011000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_SXY ) << 4 ) );

        /* Instruction: SUB */
        case ASMKCPSM3_INS_SUB_SX_KK:
            return ( 0b011100000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_KK  ) ) );
        case ASMKCPSM3_INS_SUB_SX_SY:
            return ( 0b011101000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_SXY ) << 4 ) );

        /* Instruction: SUBCY */
        case ASMKCPSM3_INS_SUBCY_SX_KK:
            return ( 0b011110000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_KK  ) ) );
        case ASMKCPSM3_INS_SUBCY_SX_SY:
            return ( 0b011111000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_SXY ) << 4 ) );

        /* Instruction: COMPARE */
        case ASMKCPSM3_INS_COMPARE_SX_KK:
            return ( 0b010100000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_KK  ) ) );
        case ASMKCPSM3_INS_COMPARE_SX_SY:
            return ( 0b010101000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_SXY ) << 4 ) );

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
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_KK  ) ) );
        case ASMKCPSM3_INS_LOAD_SX_SY:
            return ( 0b000001000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_SXY ) << 4 ) );

        /* Instruction: AND */
        case ASMKCPSM3_INS_AND_SX_KK:
            return ( 0b001010000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_KK  ) ) );
        case ASMKCPSM3_INS_AND_SX_SY:
            return ( 0b001011000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_SXY ) << 4 ) );

        /* Instruction: OR */
        case ASMKCPSM3_INS_OR_SX_KK:
            return ( 0b001100000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_KK  ) ) );
        case ASMKCPSM3_INS_OR_SX_SY:
            return ( 0b001101000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_SXY ) << 4 ) );

        /* Instruction: XOR */
        case ASMKCPSM3_INS_XOR_SX_KK:
            return ( 0b001110000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_KK  ) ) );
        case ASMKCPSM3_INS_XOR_SX_SY:
            return ( 0b001111000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_SXY ) << 4 ) );

        /* Instruction: TEST */
        case ASMKCPSM3_INS_TEST_SX_KK:
            return ( 0b010010000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_KK  ) ) );
        case ASMKCPSM3_INS_TEST_SX_SY:
            return ( 0b010011000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_SXY ) << 4 ) );

        /* Instruction: STORE */
        case ASMKCPSM3_INS_STORE_SX_SS:
            return ( 0b101110000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_SS  ) ) );
        case ASMKCPSM3_INS_STORE_SX_SY:
            return ( 0b101111000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_SXY ) << 4 ) );

        /* Instruction: FETCH */
        case ASMKCPSM3_INS_FETCH_SX_SS:
            return ( 0b000110000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_SS  ) ) );
        case ASMKCPSM3_INS_FETCH_SX_SY:
            return ( 0b000111000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_SXY ) << 4 ) );

        /* Instruction: SR0, SR1, SRX, SRA, RR */
        case ASMKCPSM3_INS_SR0_SX:
            return ( 0b100000000000001110 | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 ) );
        case ASMKCPSM3_INS_SR1_SX:
            return ( 0b100000000000001111 | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 ) );
        case ASMKCPSM3_INS_SRX_SX:
            return ( 0b100000000000001010 | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 ) );
        case ASMKCPSM3_INS_SRA_SX:
            return ( 0b100000000000001000 | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 ) );
        case ASMKCPSM3_INS_RR_SX:
            return ( 0b100000000000001100 | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 ) );

        /* Instruction: SL0, SL1, SLX, SLA, RL */
        case ASMKCPSM3_INS_SL0_SX:
            return ( 0b100000000000000110 | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 ) );
        case ASMKCPSM3_INS_SL1_SX:
            return ( 0b100000000000000111 | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 ) );
        case ASMKCPSM3_INS_SLX_SX:
            return ( 0b100000000000000100 | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 ) );
        case ASMKCPSM3_INS_SLA_SX:
            return ( 0b100000000000000000 | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 ) );
        case ASMKCPSM3_INS_RL_SX:
            return ( 0b100000000000000010 | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 ) );

        /* Instruction: INPUT */
        case ASMKCPSM3_INS_INPUT_SX_PP:
            return ( 0b000100000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_PP  ) ) );
        case ASMKCPSM3_INS_INPUT_SX_SY:
            return ( 0b000101000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_SXY ) << 4 ) );

        /* Instruction: OUTPUT */
        case ASMKCPSM3_INS_OUTPUT_SX_PP:
            return ( 0b101100000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_PP  ) ) );
        case ASMKCPSM3_INS_OUTPUT_SX_SY:
            return ( 0b101101000000000000
                     | ( symbolTable->resolveExpr ( stmt->args()->at(0), OBS_SXY ) << 8 )
                     | ( symbolTable->resolveExpr ( stmt->args()->at(1), OBS_SXY ) << 4 ) );
        default:
            return -1;
    }
}

void AsmKcpsm3InstructionSet::encapsulate ( CompilerStatement * stmt,
                                            AsmKcpsm3SymbolTable * symbolTable,
                                            int codePointer )
{
    for ( CompilerExpr * arg = stmt->args();
          NULL != arg;
          arg = arg->next() )
    {
        symbolTable->resolveSymbols(arg, codePointer);
    }
}

bool AsmKcpsm3InstructionSet::isInstruction ( const CompilerStatement * const stmt ) const
{
    using namespace StatementTypes;

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
