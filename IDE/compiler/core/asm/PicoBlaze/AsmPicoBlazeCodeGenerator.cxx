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
 * @file AsmPicoBlazeCodeGenerator.cxx
 */
// =============================================================================

#include "AsmPicoBlazeCodeGenerator.h"

// Common compiler header files.
#include "../../CompilerStatement.h"
#include "../../CompilerValue.h"
#include "../../CompilerExpr.h"

// Standard headers.
#include <sstream>
#include <cstdio>

void AsmPicoBlazeCodeGenerator::toSourceLine ( std::string & result,
                                               const CompilerStatement * node )
{
    using namespace CompilerStatementTypes;

    switch ( node->type() )
    {
        case EMPTY_STATEMENT:
            result.clear();
            break;

        case ASMPICOBLAZE_INS_JUMP_AAA:
            result = "    JUMP    ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_JUMP_Z_AAA:
            result = "    JUMP    Z, ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_JUMP_NZ_AAA:
            result = "    JUMP    NZ, ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_JUMP_C_AAA:
            result = "    JUMP    C, ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_JUMP_NC_AAA:
            result = "    JUMP    NC, ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_CALL_AAA:
            result = "    CALL    ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_CALL_Z_AAA:
            result = "    CALL    Z, ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_CALL_NZ_AAA:
            result = "    CALL    NZ, ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_CALL_C_AAA:
            result = "    CALL    C, ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_CALL_NC_AAA:
            result = "    CALL    NC, ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_RETURN:
            result = "    RETURN  ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_RETURN_Z:
            result = "    RETURN  Z, ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_RETURN_NZ:
            result = "    RETURN  NZ, ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_RETURN_C:
            result = "    RETURN  C, ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_RETURN_NC:
            result = "    RETURN  NC, ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_ADD_SX_KK:
            result = "    ADD     ";
            operands(result, node->args(), true);
            break;
        case ASMPICOBLAZE_INS_ADD_SX_SY:
            result = "    ADD     ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_ADDCY_SX_KK:
            result = "    ADDCY   ";
            operands(result, node->args(), true);
            break;
        case ASMPICOBLAZE_INS_ADDCY_SX_SY:
            result = "    ADDCY   ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_SUB_SX_KK:
            result = "    SUB     ";
            operands(result, node->args(), true);
            break;
        case ASMPICOBLAZE_INS_SUB_SX_SY:
            result = "    SUB     ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_SUBCY_SX_KK:
            result = "    SUBCY   ";
            operands(result, node->args(), true);
            break;
        case ASMPICOBLAZE_INS_SUBCY_SX_SY:
            result = "    SUBCY   ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_COMPARE_SX_KK:
            result = "    COMPARE ";
            operands(result, node->args(), true);
            break;
        case ASMPICOBLAZE_INS_COMPARE_SX_SY:
            result = "    COMPARE ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_RETURNI_ENA:
            result = "    RETURNI ENABLE";
            break;
        case ASMPICOBLAZE_INS_RETURNI_DIS:
            result = "    RETURNI DISABLE";
            break;
        case ASMPICOBLAZE_INS_ENABLE_INT:
            result = "    ENABLE INTERRUPT";
            break;
        case ASMPICOBLAZE_INS_DISABLE_INT:
            result = "    DISABLE INTERRUPT";
            break;
        case ASMPICOBLAZE_INS_LOAD_SX_KK:
            result = "    LOAD    ";
            operands(result, node->args(), true);
            break;
        case ASMPICOBLAZE_INS_LOAD_SX_SY:
            result = "    LOAD    ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_AND_SX_KK:
            result = "    AND     ";
            operands(result, node->args(), true);
            break;
        case ASMPICOBLAZE_INS_AND_SX_SY:
            result = "    AND     ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_OR_SX_KK:
            result = "    OR      ";
            operands(result, node->args(), true);
            break;
        case ASMPICOBLAZE_INS_OR_SX_SY:
            result = "    OR      ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_XOR_SX_KK:
            result = "    XOR     ";
            operands(result, node->args(), true);
            break;
        case ASMPICOBLAZE_INS_XOR_SX_SY:
            result = "    XOR     ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_TEST_SX_KK:
            result = "    TEST    ";
            operands(result, node->args(), true);
            break;
        case ASMPICOBLAZE_INS_TEST_SX_SY:
            result = "    TEST    ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_STORE_SX_SS:
            result = "    STORE   ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_STORE_SX_SY:
            result = "    STORE   ";
            operands(result, node->args(), false, true);
            break;
        case ASMPICOBLAZE_INS_FETCH_SX_SS:
            result = "    FETCH   ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_FETCH_SX_SY:
            result = "    FETCH   ";
            operands(result, node->args(), false, true);
            break;
        case ASMPICOBLAZE_INS_SR0_SX:
            result = "    SR0     ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_SR1_SX:
            result = "    SR1     ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_SRX_SX:
            result = "    SRX     ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_SRA_SX:
            result = "    SRA     ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_RR_SX:
            result = "    RR      ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_SL0_SX:
            result = "    SL0     ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_SL1_SX:
            result = "    SL1     ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_SLX_SX:
            result = "    SLX     ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_SLA_SX:
            result = "    SLA     ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_RL_SX:
            result = "    RL      ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_INPUT_SX_PP:
            result = "    INPUT   ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_INPUT_SX_SY:
            result = "    INPUT   ";
            operands(result, node->args(), false, true);
            break;
        case ASMPICOBLAZE_INS_OUTPUT_SX_PP:
            result = "    OUTPUT   ";
            operands(result, node->args());
            break;
        case ASMPICOBLAZE_INS_OUTPUT_SX_SY:
            result = "    OUTPUT   ";
            operands(result, node->args(), false, true);
            break;

        case ASMPICOBLAZE_LABEL:
            result = node->args()->lVal().m_data.m_symbol;
            result += ":";
            break;

        default:
            result = "; ERROR: statement not understood.";
            break;
    }
}

void AsmPicoBlazeCodeGenerator::operands ( std::string & result,
                                           const CompilerExpr * args,
                                           bool immediate,
                                           bool indirect )
{
    bool first = true;
    for ( const CompilerExpr * arg = args;
          NULL != arg;
          arg = arg->next() )
    {
        if ( false == first )
        {
            result += ", ";

            if ( true == immediate )
            {
                result += "#";
            }
            else if ( true == indirect )
            {
                result += "@";
            }
        }

        expr2str(result, arg);

        first = false;
    }
}

void AsmPicoBlazeCodeGenerator::expr2str ( std::string & result,
                                           const CompilerExpr * expr )
{
    switch ( expr->oper() )
    {
        case CompilerExpr::OPER_NONE:
            value2str(result, expr->lVal());
            break;
        case CompilerExpr::OPER_LOW:
            result += "LOW ";
            value2str(result, expr->lVal());
            break;
        case CompilerExpr::OPER_HIGH:
            result += "HIGH ";
            value2str(result, expr->lVal());
            break;
        case CompilerExpr::OPER_INT_PROM:
            result += "+";
            value2str(result, expr->lVal());
            break;
        case CompilerExpr::OPER_ADD_INV:
            result += "-";
            value2str(result, expr->lVal());
            break;
        case CompilerExpr::OPER_CMPL:
            result += "~";
            value2str(result, expr->lVal());
            break;
        case CompilerExpr::OPER_NOT:
            result += "!";
            value2str(result, expr->lVal());
            break;

        default:
            result += "( ";
            value2str(result, expr->lVal());
            result += " ";
            oper2str(result, expr->oper());
            result += " ";
            value2str(result, expr->rVal());
            result += " )";
            break;
    }
}

void AsmPicoBlazeCodeGenerator::value2str ( std::string & result,
                                            const CompilerValue & val )
{
    char buffer[64];
    switch ( val.m_type )
    {
        case CompilerValue::TYPE_EMPTY:
            result += "<EMPTY>";
            break;
        case CompilerValue::TYPE_INT:
            result += "0x";
            sprintf(buffer, "%x", (unsigned int) val.m_data.m_integer);
            result += buffer;
            break;
        case CompilerValue::TYPE_REAL:
            sprintf(buffer, "%f", val.m_data.m_real);
            result += buffer;
            break;
        case CompilerValue::TYPE_EXPR:
            expr2str(result, val.m_data.m_expr);
            break;
        case CompilerValue::TYPE_SYMBOL:
            result += val.m_data.m_symbol;
            break;
        case CompilerValue::TYPE_ARRAY:
            result += "\"";
            for ( int i = 0; i < val.m_data.m_array.m_size; i++ )
            {
                if ( 0 == isprint(int(val.m_data.m_array.m_data[i])) )
                {
                    if ( val.m_data.m_array.m_data[i] < 16 )
                    {
                        result += "\\x0";
                    }
                    else
                    {
                        result += "\\x";
                    }
                    sprintf(buffer, "%x", int(val.m_data.m_array.m_data[i]));
                    result += buffer;
                }
                else
                {
                    result += val.m_data.m_array.m_data[i];
                }
            }
            result += "\"";
            break;
    }
}

void AsmPicoBlazeCodeGenerator::oper2str ( std::string & result,
                                           int oper )
{
    std::ostringstream strstream;
    strstream << CompilerExpr::Operator(oper);
    result += strstream.str();
}
