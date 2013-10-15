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
#include "MScriptInterpretInterface.h"

// Standard header files.
#include <climits>
#include <cstdlib>

// Used for i18n only.
#include <QObject>

#include <iostream> // DEBUG

MScriptValue MScriptExprSolver::eval ( const MScriptExpr * expr )
{
    MScriptValue result;
    const MScriptSrcLocation & location = expr->location();

    switch ( expr->oper() )
    {
        // No operator, i.e. expression represents a single value.
        case MScriptExpr::OPER_NONE:
        {
            getFinalValue(result, expr->lVal(), location);
            break;
        }

        // Array/hash index/key.
        case MScriptExpr::OPER_INDEX:
        {
            MScriptVarTable::Index index;
            getIndex(index, expr->rVal(), location);
            result = *( m_varTable->access ( expr->lVal().m_data.m_symbol, &index, &location ) );
            break;
        }

        // Comma operator.
        case MScriptExpr::OPER_COMMA:
        {
            getFinalValue ( result, expr->lVal(), location );
            result.completeDelete();
            getFinalValue ( result, expr->rVal(), location );
            break;
        }

        // Ternary operator.
        case MScriptExpr::OPER_TERNARY:
        {
            const MScriptExpr * subExpr = expr->rVal().m_data.m_expr;
            getFinalValue(result, expr->lVal(), location);
            const MScriptValue & input = ( ( true == result.toBool(m_interpret, location) )
                                           ? subExpr->lVal() : subExpr->rVal() );

            getFinalValue(result, input, location);
            break;
        }

        // Unary operators.
        case MScriptExpr::OPER_CMPL:          case MScriptExpr::OPER_NOT:
        case MScriptExpr::OPER_PRE_INC:       case MScriptExpr::OPER_PRE_DEC:
        case MScriptExpr::OPER_POST_INC:      case MScriptExpr::OPER_POST_DEC:
        case MScriptExpr::OPER_INT_PROM:      case MScriptExpr::OPER_ADD_INV:
        {
            unaryOperation ( result, expr->lVal(), location, expr->oper() );
            break;
        }

        // Assignment operators.
        case MScriptExpr::OPER_ASSIGN:        case MScriptExpr::OPER_ASSIGN_REF:
        case MScriptExpr::OPER_ADD_ASSIGN:    case MScriptExpr::OPER_SUB_ASSIGN:
        case MScriptExpr::OPER_MUL_ASSIGN:    case MScriptExpr::OPER_DIV_ASSIGN:
        case MScriptExpr::OPER_MOD_ASSIGN:    case MScriptExpr::OPER_SHL_ASSIGN:
        case MScriptExpr::OPER_SHR_ASSIGN:    case MScriptExpr::OPER_BAND_ASSIGN:
        case MScriptExpr::OPER_BOR_ASSIGN:    case MScriptExpr::OPER_LAND_ASSIGN:
        case MScriptExpr::OPER_LOR_ASSIGN:    case MScriptExpr::OPER_XOR_ASSIGN:
        {
            assignment ( result, expr->lVal(), expr->rVal(), location, expr->oper() );
            break;
        }

        // Computational operators.
        case MScriptExpr::OPER_ADD:           case MScriptExpr::OPER_SUB:
        case MScriptExpr::OPER_MULT:          case MScriptExpr::OPER_DIV:
        case MScriptExpr::OPER_MOD:           case MScriptExpr::OPER_BOR:
        case MScriptExpr::OPER_BXOR:          case MScriptExpr::OPER_BAND:
        case MScriptExpr::OPER_LOR:           case MScriptExpr::OPER_LAND:
        case MScriptExpr::OPER_EQ:            case MScriptExpr::OPER_NE:
        case MScriptExpr::OPER_LT:            case MScriptExpr::OPER_LE:
        case MScriptExpr::OPER_GE:            case MScriptExpr::OPER_GT:
        case MScriptExpr::OPER_SHR:           case MScriptExpr::OPER_SHL:
        {
            MScriptValue left, right;
            getFinalValue(left, expr->lVal(), location);
            getFinalValue(right, expr->rVal(), location);
            binaryOperation(result, left, right, location, expr->oper());
            break;
        }

        // If the control flow reaches this point, there is something wrong with the interpreter.
        case MScriptExpr::OPER_CALL:
        case MScriptExpr::OPER_REF:
        {
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "illegal operation" )
                                                          . toStdString() );
        }

        default:
        {
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "unknown operation" )
                                                          . toStdString() );
        }
    }

    return result;
}

inline void MScriptExprSolver::getFinalValue ( MScriptValue & result,
                                               const MScriptValue & input,
                                               const MScriptSrcLocation & location )
{
    switch ( input.m_type )
    {
        case MScriptValue::TYPE_EMPTY:
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "cannot use empty value in computation" )
                                                          . toStdString() );
            break;

        case MScriptValue::TYPE_EXPR:
            result = eval ( input.m_data.m_expr );
            return;

        case MScriptValue::TYPE_SYMBOL:
            result = *( m_varTable->access ( input.m_data.m_symbol, NULL, &location ) );
            break;

        case MScriptValue::TYPE_INT:
        case MScriptValue::TYPE_FLOAT:
        case MScriptValue::TYPE_STRING:
        case MScriptValue::TYPE_BOOL:
        case MScriptValue::TYPE_COMPLEX:
            result = input;
            break;
    }
}

inline const char * MScriptExprSolver::getVariableName ( MScriptVarTable::Index * & index,
                                                         const MScriptValue & source,
                                                         const MScriptSrcLocation & location )
{
    if ( MScriptValue::TYPE_EXPR == source.m_type )
    {
        const MScriptExpr * expr = source.m_data.m_expr;
        if ( ( MScriptExpr::OPER_INDEX == expr->oper() ) && ( MScriptValue::TYPE_SYMBOL == expr->lVal().m_type ) )
        {
            index = new MScriptVarTable::Index;
            getIndex ( *index, expr->rVal(), location );
            return expr->lVal().m_data.m_symbol;
        }
        else
        {
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "only variable can be assigned value, or referenced" )
                                                          . toStdString() );
        }
    }
    else if ( MScriptValue::TYPE_SYMBOL == source.m_type )
    {
        return source.m_data.m_symbol;
    }
    else
    {
        m_interpret->interpreterMessage ( location,
                                          MScriptBase::MT_ERROR,
                                          QObject::tr ( "only variable can be assigned value, or referenced" )
                                                      . toStdString() );
    }

    return NULL;
}

inline void MScriptExprSolver::assignment ( MScriptValue & result,
                                            const MScriptValue & left,
                                            const MScriptValue & right,
                                            const MScriptSrcLocation & location,
                                            const MScriptExpr::Operator oper )
{
    MScriptVarTable::Index * index = NULL;
    const char * variable = getVariableName ( index, left, location );
    MScriptExpr::Operator subOper;

    switch ( oper )
    {
        case MScriptExpr::OPER_ASSIGN:
        {
            getFinalValue(result, right, location);
            m_varTable -> assign ( variable, location, result, index );
            return;
        }

        case MScriptExpr::OPER_ASSIGN_REF:
        {
            MScriptVarTable::Index * targetIndex = NULL;
            const char * target = getVariableName ( targetIndex, right, location );
            m_varTable -> refer ( variable, target, location, index, targetIndex );
            getFinalValue(result, right, location);
            return;
        }

        case MScriptExpr::OPER_ADD_ASSIGN:  subOper = MScriptExpr::OPER_ADD;  break;
        case MScriptExpr::OPER_SUB_ASSIGN:  subOper = MScriptExpr::OPER_SUB;  break;
        case MScriptExpr::OPER_MUL_ASSIGN:  subOper = MScriptExpr::OPER_MULT; break;
        case MScriptExpr::OPER_DIV_ASSIGN:  subOper = MScriptExpr::OPER_DIV;  break;
        case MScriptExpr::OPER_MOD_ASSIGN:  subOper = MScriptExpr::OPER_MOD;  break;
        case MScriptExpr::OPER_SHL_ASSIGN:  subOper = MScriptExpr::OPER_SHL;  break;
        case MScriptExpr::OPER_SHR_ASSIGN:  subOper = MScriptExpr::OPER_SHR;  break;
        case MScriptExpr::OPER_BAND_ASSIGN: subOper = MScriptExpr::OPER_BAND; break;
        case MScriptExpr::OPER_BOR_ASSIGN:  subOper = MScriptExpr::OPER_BOR;  break;
        case MScriptExpr::OPER_LAND_ASSIGN: subOper = MScriptExpr::OPER_LAND; break;
        case MScriptExpr::OPER_LOR_ASSIGN:  subOper = MScriptExpr::OPER_LOR;  break;
        case MScriptExpr::OPER_XOR_ASSIGN:  subOper = MScriptExpr::OPER_BXOR; break;

        default:
            return;
    }

    MScriptValue leftValue;
    MScriptValue rightValue;

    getFinalValue(leftValue, left, location);
    getFinalValue(rightValue, right, location);

    binaryOperation(result, leftValue, rightValue, location, subOper);
    m_varTable -> assign ( variable, location, result, index );
}

void MScriptExprSolver::getIndex ( MScriptVarTable::Index & index,
                                   const MScriptValue & input,
                                   const MScriptSrcLocation & location )
{
    switch ( input.m_type )
    {
        case MScriptValue::TYPE_EMPTY:
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "cannot use empty value as array index" )
                                                          . toStdString() );
            break;
        case MScriptValue::TYPE_EXPR:
        {
            if ( MScriptExpr::OPER_INDEX == input.m_data.m_expr->oper() )
            {
                getIndex ( index, input.m_data.m_expr->lVal(), location );
                getIndex ( index, input.m_data.m_expr->rVal(), location );
            }
            else
            {
                getIndex ( index, eval ( input.m_data.m_expr ), location );
            }
            break;
        }
        case MScriptValue::TYPE_SYMBOL:
        {
            MScriptValue varValue;
            getFinalValue ( varValue, input, location);
            getIndex ( index, varValue, location );
            break;
        }
        case MScriptValue::TYPE_INT:
            if ( input.m_data.m_integer < 0 )
            {
                m_interpret->interpreterMessage ( location,
                                                  MScriptBase::MT_ERROR,
                                                  QObject::tr ( "negative integer used as array index: %1" )
                                                              . arg ( input.m_data.m_integer )
                                                              . toStdString() );
            }
            else if ( input.m_data.m_integer > INT_MAX )
            {
                m_interpret->interpreterMessage ( location,
                                                  MScriptBase::MT_ERROR,
                                                  QObject::tr ( "integer used here as array index is too big: %1" )
                                                              . arg ( input.m_data.m_integer )
                                                              . toStdString() );
            }
            else
            {
                index.m_index.push_back ( (unsigned int) input.m_data.m_integer );
            }
            break;
        case MScriptValue::TYPE_FLOAT:
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "cannot use float as array index" )
                                                          . toStdString() );
            break;
        case MScriptValue::TYPE_STRING:
            index.m_key.push_back ( std::string ( input.m_data.m_string.m_data,
                                                  input.m_data.m_string.m_size ) );
            if ( std::string::npos != index.m_key.back().find('\n') )
            {
                m_interpret->interpreterMessage ( location,
                                                  MScriptBase::MT_ERROR,
                                                  QObject::tr ( "array key cannot contain new line character (LF)" )
                                                              . toStdString() );
            }
            break;
        case MScriptValue::TYPE_BOOL:
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "cannot use boolean value as array index" )
                                                          . toStdString() );
            break;
        case MScriptValue::TYPE_COMPLEX:
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "cannot use complex number as array index" )
                                                          . toStdString() );
            break;
    }

    if ( ( 0 != index.m_index.size() ) && ( 0 != index.m_key.size() ) )
    {
        m_interpret->interpreterMessage ( location,
                                          MScriptBase::MT_ERROR,
                                          QObject::tr ( "array index cannot be combined hash key" )
                                                      . toStdString() );
    }
}

void MScriptExprSolver::declaration ( const MScriptExpr * expr,
                                      DeclarationFlags flags )
{
    MScriptVariable::Flags baseFlags = MScriptVariable::FLAG_NO_FLAGS;

    if ( DF_CONST & flags )
    {
        baseFlags = MScriptVariable::Flags ( baseFlags | MScriptVariable::FLAG_CONST );
    }

    for ( const MScriptExpr * e = expr;
          NULL != e;
          e = e->next() )
    {
        MScriptVariable::Flags varFlags = baseFlags;
        unsigned int dimensions = 0;
        const char * variable;

        if ( MScriptValue::TYPE_EXPR == e->lVal().m_type )
        {
            MScriptVarTable::Index index;
            const MScriptExpr * v = e->lVal().m_data.m_expr;
            int dim;

            getIndex(index, v->rVal(), v->location());
            dim = index.dimensions();

            variable = v->lVal().m_data.m_symbol;
            dimensions = abs(dim);
            varFlags = MScriptVariable::Flags ( varFlags | MScriptVariable::FLAG_ARRAY );

            if ( dim < 0 )
            {
                varFlags = MScriptVariable::Flags ( varFlags | MScriptVariable::FLAG_HASH );
            }
        }
        else
        {
            variable = e->lVal().m_data.m_symbol;
        }

        m_varTable->declare ( variable, e->location(), varFlags, dimensions );
        if ( MScriptExpr::OPER_ASSIGN == e->oper() )
        {
            eval(e);
        }
    }
}
