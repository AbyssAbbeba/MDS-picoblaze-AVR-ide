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
 * @file MScriptExprProcessor.cxx
 */
// =============================================================================

#include "MScriptExprProcessor.h"

// MScript language interpreter header files.
#include "MScriptBase.h"
#include "MScriptExpr.h"
#include "MScriptStatement.h"
#include "MScriptInterpretInterface.h"

// Standard header files.
#include <cmath>
#include <cstring>
#include <cstdlib>

// Used for i18n only.
#include <QObject>

#include <iostream> // DEBUG

MScriptStatement * MScriptExprProcessor::decompose ( MScriptStatement * node )
{
    return decompose(node->args());
}

MScriptStatement * MScriptExprProcessor::decompose ( MScriptExpr & expr )
{
    return decompose(&expr);
}

MScriptStatement * MScriptExprProcessor::decompose ( MScriptExpr * expr )
{
    MScriptStatement * result = new MScriptStatement();

    compressExpr(expr);
    decomposeAsgn(expr);
    evalConsts(expr);

    unsigned int counter = 1;
    breakDown(result, expr, &counter);
    removeNoOps(result->first());
    removeEmpty(&result);

    return result;
}

bool MScriptExprProcessor::isConstantExpr ( const MScriptExpr * expr )
{
    for ( const MScriptExpr * e = expr;
          NULL != e;
          e = e->next() )
    {
        const MScriptValue * value = &( e->lVal() );
        for ( int i = 0; i < 2; i++ )
        {
            if ( MScriptValue::TYPE_EXPR == value->m_type )
            {
                if ( false == isConstantExpr(value->m_data.m_expr) )
                {
                    return false;
                }
            }
            else if ( MScriptValue::TYPE_SYMBOL == value->m_type )
            {
                return false;
            }

            value = &( e->rVal() );
        }
    }

    return true;
}

void MScriptExprProcessor::breakDown ( MScriptStatement * result,
                                       MScriptExpr * expr,
                                       unsigned int * counter )
{
    using namespace MScriptStmtTypes;

    const MScriptSrcLocation & location = expr->location();

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
                m_interpret->interpreterMessage ( location,
                                                  MScriptBase::MT_ERROR,
                                                  QObject::tr ( "invalid expression" ) . toStdString() );
                return;
            }

            MScriptExpr * subExpr = expr->rVal().m_data.m_expr;
            if ( MScriptExpr::OPER_COLON != subExpr->oper() )
            {
                m_interpret->interpreterMessage ( location,
                                                  MScriptBase::MT_ERROR,
                                                  QObject::tr ( "invalid expression" ) . toStdString() );
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
            if ( false == checkAssignment ( location, expr->lVal() ) )
            {
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
            if ( false == checkAssignment ( location, expr->lVal() ) )
            {
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
            if ( false == checkAssignment ( location, expr->lVal() ) )
            {
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
            if ( false == checkAssignment ( location, expr->lVal() ) )
            {
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
                        value->completeDelete();
                        subExpr->lVal().makeCopy(*value);

                        result->prependLink ( new MScriptStatement ( subExpr->location(), STMT_EXPR, subExpr ) );
                    }
                    else if ( MScriptExpr::OPER_ASSIGN == expr->oper() )
                    {
                        if ( false == checkAssignment ( location, expr->lVal() ) )
                        {
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

void MScriptExprProcessor::compressExpr ( MScriptExpr & expr )
{
    compressExpr(&expr);
}

void MScriptExprProcessor::compressExpr ( MScriptExpr * expr )
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

void MScriptExprProcessor::decomposeAsgn ( MScriptExpr * expr )
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
        case MScriptExpr::OPER_POW_ASSIGN:  subExprOper = MScriptExpr::OPER_POW;  break;

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

void MScriptExprProcessor::removeNoOps ( MScriptStatement * root )
{
    for ( MScriptStatement * node = root;
          NULL != node;
          node = node->next() )
    {
        if ( NULL != node->branch() )
        {
            removeNoOps ( node->branch() );
        }

        if ( MScriptStmtTypes::STMT_EXPR == node->type() )
        {
            if ( MScriptExpr::OPER_NONE == node->args()->oper() )
            {
                node->m_type = MScriptStmtTypes::STMT_EMPTY;
                node->args()->completeDelete();
            }
        }
    }
}

inline void MScriptExprProcessor::removeEmpty ( MScriptStatement ** snakeNode )
{
    *snakeNode = (*snakeNode)->first();
    for ( MScriptStatement * node = (*snakeNode)->next();
          NULL != node;
          node = node->next() )
    {
        if ( MScriptStmtTypes::STMT_EMPTY == node->m_type )
        {
            node = node->prev();
            delete node->next();
        }
    }

    if ( MScriptStmtTypes::STMT_EMPTY == (*snakeNode)->m_type )
    {
        delete *snakeNode;
        *snakeNode = NULL;
    }
}

void MScriptExprProcessor::evalConsts ( MScriptExpr * expr,
                                        MScriptValue * value )
{
    for ( MScriptExpr * e = expr;
          NULL != e;
          e = e->next() )
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

        if ( false == isConstantExpr(e) )
        {
            continue;
        }

        if (
               MScriptExpr::OPER_NONE    == e->oper()
                   ||
               MScriptValue::TYPE_EMPTY  == e->lVal().m_type
                   ||
               MScriptValue::TYPE_EXPR   == e->lVal().m_type
                   ||
               MScriptValue::TYPE_SYMBOL == e->lVal().m_type
           )
        {
            return;
        }

        const MScriptSrcLocation & location = expr->location();
        MScriptValue & result = ( ( NULL == value ) ? expr->m_lValue : *value );

        if ( true == isUnary(e->oper()) )
        {
            unaryOperation ( result, e->lVal(), location, e->oper() );
            continue;
        }

        if (
               MScriptValue::TYPE_EMPTY  == e->rVal().m_type
                   ||
               MScriptValue::TYPE_EXPR   == e->rVal().m_type
                   ||
               MScriptValue::TYPE_SYMBOL == e->rVal().m_type
           )
        {
            continue;
        }

        binaryOperation ( result, e->lVal(), e->rVal(), location, e->oper() );

        if ( true == isComparison ( e->oper() ) )
        {
            if ( true == result.toBool(m_interpret, location) )
            {
                m_interpret->interpreterMessage ( location,
                                                  MScriptBase::MT_WARNING,
                                                  QObject::tr ( "comparing two constants, result is always positive" )
                                                              . toStdString() );
            }
            else
            {
                m_interpret->interpreterMessage ( location,
                                                  MScriptBase::MT_WARNING,
                                                  QObject::tr ( "comparing two constants, result is always negative" )
                                                              . toStdString() );
            }
        }

        if ( NULL == value )
        {
            e->m_operator = MScriptExpr::OPER_NONE;
            e->m_rValue.completeDelete();
            e->m_rValue.m_type = MScriptValue::TYPE_EMPTY;
        }
    }
}

inline bool MScriptExprProcessor::isComparison ( const MScriptExpr::Operator oper )
{
    switch ( oper )
    {
        case MScriptExpr::OPER_EQ:
        case MScriptExpr::OPER_NE:
        case MScriptExpr::OPER_LT:
        case MScriptExpr::OPER_LE:
        case MScriptExpr::OPER_GE:
        case MScriptExpr::OPER_GT:
            return true;

        default:
            return false;
    }
}

bool MScriptExprProcessor::detectFunctionCall ( const MScriptExpr * expr ) const
{
    for ( const MScriptExpr * e = expr;
          NULL != e;
          e = e->next() )
    {
        if ( MScriptExpr::OPER_CALL == e->oper() )
        {
            return true;
        }

        const MScriptValue * value = &( e->m_lValue );
        for ( int i = 0; i < 2; i++ )
        {
            if ( MScriptValue::TYPE_EXPR == value->m_type )
            {
                if ( true == detectFunctionCall ( value->m_data.m_expr ) )
                {
                    return true;
                }
            }

            value = &( e->m_rValue );
        }
    }

    return false;
}
