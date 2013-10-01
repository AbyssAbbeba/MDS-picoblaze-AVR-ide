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
 * @ingroup MoraviaScript
 * @file MScriptExprSolver.cxx
 */
// =============================================================================

#include "MScriptExprSolver.h"

// MScript language interpreter header files.
#include "MScriptBase.h"
#include "MScriptExpr.h"
#include "MScriptStatement.h"

// Standard header files.
#include <cstring>

#include <iostream> // DEBUG

MScriptStatement * MScriptExprSolver::decompose ( MScriptStatement * node )
{
    std::cout << "MScriptExprSolver::decompose("<<node->args()<<")\n";

    MScriptStatement * result = NULL;;

    for ( MScriptExpr * expr = node->args();
          NULL != expr;
          expr = expr->next() )
    {
        MScriptStatement * intResult = new MScriptStatement ( expr->location(), MScriptStmtTypes::STMT_EXPR, expr );

        std::cout << "MScriptExprSolver::decompose :: expr = "<<expr<<"\n";
        compressExpr(expr);
        std::cout << "MScriptExprSolver::compressExpr :: expr = "<<expr<<"\n";
        decomposeAsgn(expr);
        std::cout << "MScriptExprSolver::decomposeAsgn :: expr = "<<expr<<"\n";
        evalConsts(expr);
        std::cout << "MScriptExprSolver::evalConsts :: expr = "<<expr<<"\n";

        unsigned int counter = 1;
        breakDown(intResult, expr, &counter);
        if ( NULL == result )
        {
            result = intResult->first();
        }
        else
        {
            result->appendLink(intResult);
        }
    }

    std::cout << "MScriptExprSolver::decompose() ==>\n" << result << "\n";

    return result;
}

void MScriptExprSolver::breakDown ( MScriptStatement * result,
                                    MScriptExpr * expr,
                                    unsigned int * counter )
{
    using namespace MScriptStmtTypes;

    const MScriptSrcLocation location = expr->location();

    switch ( expr->oper() )
    {
        case MScriptExpr::OPER_CALL:
        {
            MScriptExpr * funcNameAndArgs;

            if ( MScriptValue::TYPE_EXPR == expr->m_lValue.m_type )
            {
                funcNameAndArgs = expr->m_lValue.m_data.m_expr;
            }
            else
            {
                funcNameAndArgs = new MScriptExpr ( expr->lVal(), location );
            }

            result->prependLink ( new MScriptStatement ( location, STMT_CALL, funcNameAndArgs ) );

            if ( MScriptValue::TYPE_EXPR == expr->m_rValue.m_type )
            {
                funcNameAndArgs->appendLink(expr->m_rValue.m_data.m_expr);

                for ( MScriptExpr * e = expr->m_rValue.m_data.m_expr;
                      NULL != e;
                      e = e->next() )
                {
                    breakDown ( result, e, counter );
                }
            }
            else if ( MScriptValue::TYPE_EMPTY != expr->m_rValue.m_type )
            {
                funcNameAndArgs->appendLink ( new MScriptExpr ( expr->rVal(), location ) );
            }

            expr->m_lValue.m_type = MScriptValue::TYPE_SYMBOL;
            expr->m_lValue.m_data.m_symbol = new char[2];
            strcpy(expr->m_lValue.m_data.m_symbol, "0");
            expr->m_operator = MScriptExpr::OPER_NONE;
            expr->m_rValue.m_type = MScriptValue::TYPE_EMPTY;

            break;
        }

        case MScriptExpr::OPER_LOR:
        {
            MScriptExpr * cnd;
            if ( MScriptValue::TYPE_EXPR == expr->lVal().m_type )
            {
                cnd = expr->lVal().m_data.m_expr;
            }
            else
            {
                cnd = new MScriptExpr ( expr->lVal(), location );
            }

            MScriptStatement * ifBlock = new MScriptStatement ( location, STMT_IF, cnd );
            MScriptStatement * elseBlock = new MScriptStatement ( location, STMT_ELSE );
            MScriptStatement * conditionBlock =  new MScriptStatement ( location, STMT_CONDITION );

            char * tmpVar = new char [8];
            sprintf(tmpVar, "%d", *counter);
            (*counter)++;

            ifBlock -> createBranch ( new MScriptStatement ( location,
                                                             STMT_EXPR,
                                                             new MScriptExpr ( tmpVar,
                                                                               MScriptExpr::OPER_ASSIGN,
                                                                               expr->lVal(),
                                                                               location ) ) );

            elseBlock -> createBranch ( new MScriptStatement ( location,
                                                               STMT_EXPR,
                                                               new MScriptExpr ( tmpVar,
                                                                                 MScriptExpr::OPER_ASSIGN,
                                                                                 expr->rVal(),
                                                                                 location ) ) );

            conditionBlock->createBranch(ifBlock->appendLink(elseBlock));

            result->prependLink ( conditionBlock );

            if ( MScriptValue::TYPE_EXPR == expr->lVal().m_type )
            {
                breakDown ( result, expr->m_lValue.m_data.m_expr, counter );
            }
            if ( MScriptValue::TYPE_EXPR == expr->rVal().m_type )
            {
                breakDown ( elseBlock->branch(), expr->m_rValue.m_data.m_expr, counter );
                elseBlock->m_branch = elseBlock->branch()->first();
            }

            expr->m_lValue.m_data.m_symbol = tmpVar;
            expr->m_lValue.m_type = MScriptValue::TYPE_SYMBOL;
            expr->m_operator = MScriptExpr::OPER_NONE;
            expr->m_rValue.m_type = MScriptValue::TYPE_EMPTY;

            break;
        }

        case MScriptExpr::OPER_LAND:
        {
            MScriptExpr * cnd;
            if ( MScriptValue::TYPE_EXPR == expr->lVal().m_type )
            {
                cnd = expr->lVal().m_data.m_expr;
            }
            else
            {
                cnd = new MScriptExpr ( expr->lVal(), location );
            }

            MScriptStatement * ifBlock = new MScriptStatement ( location, STMT_IF, cnd );
            MScriptStatement * elseBlock = new MScriptStatement ( location, STMT_ELSE );
            MScriptStatement * conditionBlock =  new MScriptStatement ( location, STMT_CONDITION );

            char * tmpVar = new char [8];
            sprintf(tmpVar, "%d", *counter);
            (*counter)++;

            ifBlock -> createBranch ( new MScriptStatement ( location,
                                                             STMT_EXPR,
                                                             new MScriptExpr ( tmpVar,
                                                                               MScriptExpr::OPER_ASSIGN,
                                                                               expr->rVal(),
                                                                               location ) ) );

            elseBlock -> createBranch ( new MScriptStatement ( location,
                                                               STMT_EXPR,
                                                               new MScriptExpr ( tmpVar,
                                                                                 MScriptExpr::OPER_ASSIGN,
                                                                                 expr->lVal(),
                                                                                 location ) ) );

            conditionBlock->createBranch(ifBlock->appendLink(elseBlock));

            result->prependLink ( conditionBlock );

            if ( MScriptValue::TYPE_EXPR == expr->rVal().m_type )
            {
                breakDown ( ifBlock->branch(), expr->m_rValue.m_data.m_expr, counter );
                ifBlock->m_branch = ifBlock->branch()->first();
            }
            if ( MScriptValue::TYPE_EXPR == expr->lVal().m_type )
            {
                breakDown ( result, expr->m_lValue.m_data.m_expr, counter );
            }

            expr->m_lValue.m_data.m_symbol = tmpVar;
            expr->m_lValue.m_type = MScriptValue::TYPE_SYMBOL;
            expr->m_operator = MScriptExpr::OPER_NONE;
            expr->m_rValue.m_type = MScriptValue::TYPE_EMPTY;

            break;
        }
        case MScriptExpr::OPER_TERNARY:
        {
            MScriptExpr * cnd;
            if ( MScriptValue::TYPE_EXPR == expr->lVal().m_type )
            {
                cnd = expr->lVal().m_data.m_expr;
            }
            else
            {
                cnd = new MScriptExpr ( expr->lVal(), location );
            }

            if ( MScriptValue::TYPE_EXPR != expr->rVal().m_type )
            {
                // TODO: error
                return;
            }

            MScriptExpr * subExpr = expr->rVal().m_data.m_expr;
            if ( MScriptExpr::OPER_COLON != subExpr->oper() )
            {
                // TODO: error
                return;
            }

            MScriptStatement * ifBlock = new MScriptStatement ( location, STMT_IF, cnd );
            MScriptStatement * elseBlock = new MScriptStatement ( location, STMT_ELSE );
            MScriptStatement * conditionBlock =  new MScriptStatement ( location, STMT_CONDITION );

            char * tmpVar = new char [8];
            sprintf(tmpVar, "%d", *counter);
            (*counter)++;

            ifBlock -> createBranch ( new MScriptStatement ( location,
                                                             STMT_EXPR,
                                                             new MScriptExpr ( tmpVar,
                                                                               MScriptExpr::OPER_ASSIGN,
                                                                               subExpr->lVal(),
                                                                               location ) ) );

            elseBlock -> createBranch ( new MScriptStatement ( location,
                                                               STMT_EXPR,
                                                               new MScriptExpr ( tmpVar,
                                                                                 MScriptExpr::OPER_ASSIGN,
                                                                                 subExpr->rVal(),
                                                                                 location ) ) );

            conditionBlock->createBranch(ifBlock->appendLink(elseBlock));

            result->prependLink ( conditionBlock );

            if ( MScriptValue::TYPE_EXPR == subExpr->lVal().m_type )
            {
                breakDown ( ifBlock->branch(), subExpr->m_lValue.m_data.m_expr, counter );
                ifBlock->m_branch = ifBlock->branch()->first();
            }
            if ( MScriptValue::TYPE_EXPR == subExpr->rVal().m_type )
            {
                breakDown ( elseBlock->branch(), subExpr->m_rValue.m_data.m_expr, counter );
                elseBlock->m_branch = elseBlock->branch()->first();
            }

            expr->m_lValue.m_data.m_symbol = tmpVar;
            expr->m_lValue.m_type = MScriptValue::TYPE_SYMBOL;
            expr->m_operator = MScriptExpr::OPER_NONE;
            expr->m_rValue.m_type = MScriptValue::TYPE_EMPTY;

            break;
        }
        case MScriptExpr::OPER_COMMA:
        {
            MScriptExpr * leftExpr;
            if ( MScriptValue::TYPE_EXPR == expr->lVal().m_type )
            {
                leftExpr = expr->lVal().m_data.m_expr;
            }
            else
            {
                leftExpr = new MScriptExpr ( expr->lVal(), location );
            }

            result->prependLink ( new MScriptStatement ( location, STMT_EXPR, leftExpr ) );

            if ( MScriptValue::TYPE_EXPR == expr->rVal().m_type )
            {
                breakDown ( result, expr->m_rValue.m_data.m_expr, counter );
            }
            breakDown ( result, leftExpr, counter );

            expr->m_lValue = expr->rVal();
            expr->m_operator = MScriptExpr::OPER_NONE;
            expr->m_rValue.m_type = MScriptValue::TYPE_EMPTY;

            break;
        }
        case MScriptExpr::OPER_PRE_INC:
        {
            if ( MScriptValue::TYPE_SYMBOL != expr->lVal().m_type )
            {
                // Only variables can be assigned a value.
                // TODO: error message
                return;
            }

            expr->m_operator = MScriptExpr::OPER_NONE;

            MScriptExpr * opSubExpr = new MScriptExpr ( expr->lVal().makeCopy(), MScriptExpr::OPER_ADD, 1, location );
            MScriptExpr * op = new MScriptExpr ( expr->lVal(), MScriptExpr::OPER_ASSIGN, opSubExpr, location );
            result->prependLink ( new MScriptStatement ( location, STMT_EXPR, op ) );

            break;
        }
        case MScriptExpr::OPER_PRE_DEC:
        {
            if ( MScriptValue::TYPE_SYMBOL != expr->lVal().m_type )
            {
                // Only variables can be assigned a value.
                // TODO: error message
                return;
            }

            expr->m_operator = MScriptExpr::OPER_NONE;

            MScriptExpr * opSubExpr = new MScriptExpr ( expr->lVal().makeCopy(), MScriptExpr::OPER_SUB, 1, location );
            MScriptExpr * op = new MScriptExpr ( expr->lVal(), MScriptExpr::OPER_ASSIGN, opSubExpr, location );
            result->prependLink ( new MScriptStatement ( location, STMT_EXPR, op ) );

            break;
        }
        case MScriptExpr::OPER_POST_INC:
        {
            if ( MScriptValue::TYPE_SYMBOL != expr->lVal().m_type )
            {
                // Only variables can be assigned a value.
                // TODO: error message
                return;
            }

            expr->m_operator = MScriptExpr::OPER_NONE;

            MScriptExpr * opSubExpr = new MScriptExpr ( expr->lVal().makeCopy(), MScriptExpr::OPER_ADD, 1, location );
            MScriptExpr * op = new MScriptExpr ( expr->lVal(), MScriptExpr::OPER_ASSIGN, opSubExpr, location );
            result->first()->insertLink ( new MScriptStatement ( location, STMT_EXPR, op ) );

            break;
        }
        case MScriptExpr::OPER_POST_DEC:
        {
            if ( MScriptValue::TYPE_SYMBOL != expr->lVal().m_type )
            {
                // Only variables can be assigned a value.
                // TODO: error message
                return;
            }

            expr->m_operator = MScriptExpr::OPER_NONE;

            MScriptExpr * opSubExpr = new MScriptExpr ( expr->lVal().makeCopy(), MScriptExpr::OPER_SUB, 1, location );
            MScriptExpr * op = new MScriptExpr ( expr->lVal(), MScriptExpr::OPER_ASSIGN, opSubExpr, location );
            result->first()->insertLink ( new MScriptStatement ( location, STMT_EXPR, op ) );

            break;
        }
        default:
        {
            MScriptValue * value = &( expr->m_lValue );
            for ( int i = 0; i < 2; i++ )
            {
                if ( MScriptValue::TYPE_EXPR == value->m_type )
                {
                    MScriptExpr * subExpr = value->m_data.m_expr;

                    if ( MScriptExpr::OPER_ASSIGN == subExpr->oper() )
                    {
                        value->m_data.m_symbol = subExpr->lVal().m_data.m_symbol;
                        value->m_type = MScriptValue::TYPE_SYMBOL;

                        result->prependLink ( new MScriptStatement ( subExpr->location(), STMT_EXPR, subExpr ) );
                    }
                    else if ( MScriptExpr::OPER_ASSIGN == expr->oper() )
                    {
                        if ( MScriptValue::TYPE_SYMBOL != expr->lVal().m_type )
                        {
                            // Only variables can be assigned a value.
                            // TODO: error message
                            return;
                        }
                    }
                    else if ( MScriptExpr::OPER_INDEX == expr->oper() )
                    {

                    }
                    else
                    {
                        char * tmpVar = new char [8];
                        sprintf(tmpVar, "%d", *counter);
                        (*counter)++;

                        value->m_data.m_symbol = tmpVar;
                        value->m_type = MScriptValue::TYPE_SYMBOL;

                        MScriptExpr * substitute = new MScriptExpr ( tmpVar,
                                                                     MScriptExpr::OPER_ASSIGN,
                                                                     subExpr,
                                                                     subExpr->location() );

                        result->prependLink ( new MScriptStatement ( substitute->location(), STMT_EXPR, substitute ) );
                    }

                    breakDown(result, subExpr, counter);
                }

                value = &( expr->m_rValue );
            }

            break;
        }
    }
}

void MScriptExprSolver::compressExpr ( MScriptExpr * expr )
{
    MScriptValue * value = &( expr->m_lValue );
    for ( int i = 0; i < 2; i++ )
    {
        if ( MScriptValue::TYPE_EXPR == value->m_type )
        {
            MScriptExpr * subExpr = value->m_data.m_expr;
            if ( MScriptExpr::OPER_NONE == subExpr->oper() )
            {
                *value = subExpr->m_lValue;
            }
            else
            {
                compressExpr ( subExpr );
            }
        }
        value = &( expr->m_rValue );
    }
}

void MScriptExprSolver::decomposeAsgn ( MScriptExpr * expr )
{
    MScriptExpr::Operator subExprOper;

    switch ( expr->oper() )
    {
        case MScriptExpr::OPER_ADD_ASSIGN:  subExprOper = MScriptExpr::OPER_ADD;  break;
        case MScriptExpr::OPER_SUB_ASSIGN:  subExprOper = MScriptExpr::OPER_SUB;  break;
        case MScriptExpr::OPER_MUL_ASSIGN:  subExprOper = MScriptExpr::OPER_MULT; break;
        case MScriptExpr::OPER_DIV_ASSIGN:  subExprOper = MScriptExpr::OPER_DIV;  break;
        case MScriptExpr::OPER_MOD_ASSIGN:  subExprOper = MScriptExpr::OPER_MOD;  break;
        case MScriptExpr::OPER_SHL_ASSIGN:  subExprOper = MScriptExpr::OPER_SHL;  break;
        case MScriptExpr::OPER_SHR_ASSIGN:  subExprOper = MScriptExpr::OPER_SHR;  break;
        case MScriptExpr::OPER_BAND_ASSIGN: subExprOper = MScriptExpr::OPER_BAND; break;
        case MScriptExpr::OPER_BOR_ASSIGN:  subExprOper = MScriptExpr::OPER_BOR;  break;
        case MScriptExpr::OPER_LAND_ASSIGN: subExprOper = MScriptExpr::OPER_LAND; break;
        case MScriptExpr::OPER_LOR_ASSIGN:  subExprOper = MScriptExpr::OPER_LOR;  break;
        case MScriptExpr::OPER_XOR_ASSIGN:  subExprOper = MScriptExpr::OPER_BXOR; break;

        default:
            return;
    }

    expr->m_operator = MScriptExpr::OPER_ASSIGN;
    MScriptExpr * subExpr = new MScriptExpr ( expr->lVal().makeCopy(), subExprOper, expr->rVal(), expr->location() );
    expr->m_rValue.m_type = MScriptValue::TYPE_EXPR;
    expr->m_rValue.m_data.m_expr = subExpr;

    MScriptValue * value = &( subExpr->m_lValue );
    for ( int i = 0; i < 2; i++ )
    {
        if ( MScriptValue::TYPE_EXPR == value->m_type )
        {
            decomposeAsgn ( value->m_data.m_expr );
        }

        value = &( subExpr->m_rValue );
    }
}

void MScriptExprSolver::evalConsts ( MScriptExpr * expr,
                                     MScriptValue * value )
{
    for ( MScriptExpr * e = expr; NULL != e; e = e->next() )
    {
        MScriptValue * value = &( e->m_lValue );
        for ( int i = 0; i < 2; i++ )
        {
            if ( MScriptValue::TYPE_EXPR == value->m_type )
            {
                evalConsts ( value->m_data.m_expr, value );
            }

            value = &( e->m_rValue );
        }

        if (
               MScriptExpr::OPER_NONE    == e->oper()
                   ||
               MScriptValue::TYPE_EMPTY  == e->lVal().m_type
                   ||
               MScriptValue::TYPE_EMPTY  == e->rVal().m_type
                   ||
               MScriptValue::TYPE_EXPR   == e->lVal().m_type
                   ||
               MScriptValue::TYPE_EXPR   == e->rVal().m_type
                   ||
               MScriptValue::TYPE_SYMBOL == e->lVal().m_type
                   ||
               MScriptValue::TYPE_SYMBOL == e->rVal().m_type
           )
        {
            return;
        }

        switch ( e->lVal().m_type )
        {
            case MScriptValue::TYPE_INT:
            case MScriptValue::TYPE_REAL:
            case MScriptValue::TYPE_ARRAY:
            default:
                break;
        }
    }
}

MScriptStatement * MScriptExprSolver::removeNoOps ( MScriptStatement * expr )
{

}

bool MScriptExprSolver::eval ( const MScriptExpr * expr )
{
    return true;
}
