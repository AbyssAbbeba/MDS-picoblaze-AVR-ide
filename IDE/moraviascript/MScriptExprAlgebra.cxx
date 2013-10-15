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
 * @file MScriptExprAlgebra.cxx
 */
// =============================================================================

#include "MScriptExprAlgebra.h"

// MScript language interpreter header files.
#include "MScriptBase.h"
#include "MScriptInterpretInterface.h"

// Standard header files.
#include <cstring>
#include <cstdlib>
#include <climits>

// Used for i18n only.
#include <QObject>

#include <iostream> // DEBUG

void MScriptExprAlgebra::unaryOperation ( MScriptValue & result,
                                          const MScriptValue & input,
                                          const MScriptSrcLocation & location,
                                          const MScriptExpr::Operator oper )
{
    switch ( oper )
    {
        // a = ! a;
        case MScriptExpr::OPER_NOT:
            switch ( input.m_type )
            {
                case MScriptValue::TYPE_INT:
                    result.m_data.m_integer = ( input.m_data.m_integer ? 0 : 1 );
                    result.m_type = MScriptValue::TYPE_BOOL;
                    break;
                case MScriptValue::TYPE_FLOAT:
                    result.m_data.m_float = ( ( 0 == input.m_data.m_float ) ? 1 : 0 );
                    result.m_type = MScriptValue::TYPE_BOOL;
                    break;
                case MScriptValue::TYPE_BOOL:
                    result.m_data.m_bool = ! input.m_data.m_bool;
                    break;
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                case MScriptValue::TYPE_STRING:
                case MScriptValue::TYPE_COMPLEX:
                    incompatibleType(location, input.m_type);
                    break;
            }
            break;

        // a = ~ a;
        case MScriptExpr::OPER_CMPL:
            switch ( input.m_type )
            {
                case MScriptValue::TYPE_INT:
                    result.m_data.m_integer = ~ ( input.m_data.m_integer );
                    break;
                case MScriptValue::TYPE_FLOAT:
                case MScriptValue::TYPE_BOOL:
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                case MScriptValue::TYPE_STRING:
                case MScriptValue::TYPE_COMPLEX:
                    incompatibleType(location, input.m_type);
                    break;
            }
            break;

        // a = - a;
        case MScriptExpr::OPER_ADD_INV:
            switch ( input.m_type )
            {
                case MScriptValue::TYPE_INT:
                    result.m_data.m_integer = - ( input.m_data.m_integer );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    result.m_data.m_float = - ( input.m_data.m_float );
                    break;
                case MScriptValue::TYPE_BOOL:
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                case MScriptValue::TYPE_STRING:
                case MScriptValue::TYPE_COMPLEX:
                    incompatibleType(location, input.m_type);
                    break;
            }
            break;

        // a = + a;
        case MScriptExpr::OPER_INT_PROM:
            switch ( input.m_type )
            {
                case MScriptValue::TYPE_INT:
                case MScriptValue::TYPE_FLOAT:
                    result = input;
                    break;
                case MScriptValue::TYPE_BOOL:
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                case MScriptValue::TYPE_STRING:
                case MScriptValue::TYPE_COMPLEX:
                    incompatibleType(location, input.m_type);
                    break;
            }
            break;

        default:
            undefinedOperation(location);
            break;
    }
}

void MScriptExprAlgebra::binaryOperation ( MScriptValue & result,
                                           const MScriptValue & left,
                                           const MScriptValue & right,
                                           const MScriptSrcLocation & location,
                                           const MScriptExpr::Operator oper )
{
    switch ( left.m_type )
    {
        case MScriptValue::TYPE_INT:
        {
            long long a = left.m_data.m_integer;
            switch ( right.m_type )
            {
                case MScriptValue::TYPE_INT:
                    intInt ( result, a, oper, left.m_data.m_integer, location );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    intFloat ( result, a, oper, left.m_data.m_float, location );
                    break;
                case MScriptValue::TYPE_STRING:
                    intString ( result, a, oper, left.m_data.m_string, location );
                    break;
                case MScriptValue::TYPE_BOOL:
                    intBool ( result, a, oper, left.m_data.m_bool, location );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    intComplex ( result, a, oper, left.m_data.m_complex, location );
                    break;
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    incompatibleTypes(location, left.m_type, right.m_type);
                    break;
            }
            break;
        }
        case MScriptValue::TYPE_FLOAT:
        {
            double a = left.m_data.m_float;
            switch ( right.m_type )
            {
                case MScriptValue::TYPE_INT:
                    floatInt ( result, a, oper, left.m_data.m_integer, location );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    floatFloat ( result, a, oper, left.m_data.m_float, location );
                    break;
                case MScriptValue::TYPE_STRING:
                    floatString ( result, a, oper, left.m_data.m_string, location );
                    break;
                case MScriptValue::TYPE_BOOL:
                    floatBool ( result, a, oper, left.m_data.m_bool, location );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    floatComplex ( result, a, oper, left.m_data.m_complex, location );
                    break;
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    incompatibleTypes(location, left.m_type, right.m_type);
                    break;
            }
            break;
        }
        case MScriptValue::TYPE_STRING:
        {
            const MScriptValue::Data::String & a = left.m_data.m_string;
            switch ( right.m_type )
            {
                case MScriptValue::TYPE_INT:
                    stringInt ( result, a, oper, left.m_data.m_integer, location );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    stringFloat ( result, a, oper, left.m_data.m_float, location );
                    break;
                case MScriptValue::TYPE_STRING:
                    stringString ( result, a, oper, left.m_data.m_string, location );
                    break;
                case MScriptValue::TYPE_BOOL:
                    stringBool ( result, a, oper, left.m_data.m_bool, location );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    stringComplex ( result, a, oper, left.m_data.m_complex, location );
                    break;
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    incompatibleTypes(location, left.m_type, right.m_type);
                    break;
            }
            break;
        }
        case MScriptValue::TYPE_BOOL:
        {
            bool a = left.m_data.m_bool;
            switch ( right.m_type )
            {
                case MScriptValue::TYPE_INT:
                    boolInt ( result, a, oper, left.m_data.m_integer, location );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    boolFloat ( result, a, oper, left.m_data.m_float, location );
                    break;
                case MScriptValue::TYPE_STRING:
                    boolString ( result, a, oper, left.m_data.m_string, location );
                    break;
                case MScriptValue::TYPE_BOOL:
                    boolBool ( result, a, oper, left.m_data.m_bool, location );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    boolComplex ( result, a, oper, left.m_data.m_complex, location );
                    break;
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    incompatibleTypes(location, left.m_type, right.m_type);
                    break;
            }
            break;
        }
        case MScriptValue::TYPE_COMPLEX:
        {
            const MScriptValue::Data::Complex & a = left.m_data.m_complex;
            switch ( right.m_type )
            {
                case MScriptValue::TYPE_INT:
                    complexInt ( result, a, oper, left.m_data.m_integer, location );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    complexFloat ( result, a, oper, left.m_data.m_float, location );
                    break;
                case MScriptValue::TYPE_STRING:
                    complexString ( result, a, oper, left.m_data.m_string, location );
                    break;
                case MScriptValue::TYPE_BOOL:
                    complexBool ( result, a, oper, left.m_data.m_bool, location );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    complexComplex ( result, a, oper, left.m_data.m_complex, location );
                    break;
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    incompatibleTypes(location, left.m_type, right.m_type);
                    break;
            }
            break;
        }
            break;
        case MScriptValue::TYPE_EMPTY:
        case MScriptValue::TYPE_EXPR:
        case MScriptValue::TYPE_SYMBOL:
            incompatibleTypes(location, left.m_type, right.m_type);
            break;
    }
}

inline void MScriptExprAlgebra::intInt ( MScriptValue & result,
                                         long long a,
                                         MScriptExpr::Operator oper,
                                         long long b,
                                         const MScriptSrcLocation & location )
{
    bool boolResult = false;
    bool br;
    long long r = 1;

    switch ( oper )
    {
        case MScriptExpr::OPER_ADD:  r = a + b;  break;
        case MScriptExpr::OPER_SUB:  r = a - b;  break;
        case MScriptExpr::OPER_MULT: r = a * b;  break;
        case MScriptExpr::OPER_BOR:  r = a | b;  break;
        case MScriptExpr::OPER_BXOR: r = a ^ b;  break;
        case MScriptExpr::OPER_BAND: r = a & b;  break;
        case MScriptExpr::OPER_LOR:  br = ( a || b ); boolResult = true; break;
        case MScriptExpr::OPER_LAND: br = ( a && b ); boolResult = true; break;
        case MScriptExpr::OPER_EQ:   br = ( a == b ); boolResult = true; break;
        case MScriptExpr::OPER_NE:   br = ( a != b ); boolResult = true; break;
        case MScriptExpr::OPER_LT:   br = ( a  < b ); boolResult = true; break;
        case MScriptExpr::OPER_LE:   br = ( a <= b ); boolResult = true; break;
        case MScriptExpr::OPER_GE:   br = ( a >= b ); boolResult = true; break;
        case MScriptExpr::OPER_GT:   br = ( a  > b ); boolResult = true; break;
        case MScriptExpr::OPER_SHR:
            if ( true == checkShift(location, b) )
            {
                r = a >> b;
            }
            break;
        case MScriptExpr::OPER_SHL:
            if ( true == checkShift(location, b) )
            {
                r = a << b;
            }
            break;
        case MScriptExpr::OPER_DIV:
            if ( true == checkDivisor(location, b) )
            {
                r = a / b;
            }
            break;
        case MScriptExpr::OPER_MOD:
            if ( true == checkDivisor(location, b) )
            {
                r = a % b;
            }
            break;
        default:
            undefinedOperation(location);
            return;
    }

    if ( true == boolResult )
    {
        result.m_type = MScriptValue::TYPE_BOOL;
        result.m_data.m_bool = br;
    }
    else
    {
        result.m_type = MScriptValue::TYPE_INT;
        result.m_data.m_integer = r;
    }
}

inline void MScriptExprAlgebra::intFloat ( MScriptValue & result,
                                           long long a,
                                           MScriptExpr::Operator oper,
                                           double b,
                                           const MScriptSrcLocation & location )
{
    bool boolResult = false;
    bool br;
    double r = 1.0;

    switch ( oper )
    {
        case MScriptExpr::OPER_ADD:  r = a + b; break;
        case MScriptExpr::OPER_SUB:  r = a - b; break;
        case MScriptExpr::OPER_MULT: r = a * b; break;

        case MScriptExpr::OPER_EQ:   br = ( a == b ); boolResult = true; break;
        case MScriptExpr::OPER_NE:   br = ( a != b ); boolResult = true; break;
        case MScriptExpr::OPER_LT:   br = ( a  < b ); boolResult = true; break;
        case MScriptExpr::OPER_LE:   br = ( a <= b ); boolResult = true; break;
        case MScriptExpr::OPER_GE:   br = ( a >= b ); boolResult = true; break;
        case MScriptExpr::OPER_GT:   br = ( a  > b ); boolResult = true; break;
        case MScriptExpr::OPER_LOR:  br = ( a || b ); boolResult = true; break;
        case MScriptExpr::OPER_LAND: br = ( a && b ); boolResult = true; break;

        case MScriptExpr::OPER_DIV:
            if ( true == checkDivisor(location, b) )
            {
                r = a / b;
            }
            break;

        case MScriptExpr::OPER_MOD:  case MScriptExpr::OPER_SHR:  case MScriptExpr::OPER_SHL:
        case MScriptExpr::OPER_BOR:  case MScriptExpr::OPER_BXOR: case MScriptExpr::OPER_BAND:
            incompatibleTypes(location, MScriptValue::TYPE_INT, MScriptValue::TYPE_FLOAT);
            return;

        default:
            undefinedOperation(location);
            return;
    }

    if ( true == boolResult )
    {
        result.m_type = MScriptValue::TYPE_BOOL;
        result.m_data.m_bool = br;
    }
    else
    {
        result.m_type = MScriptValue::TYPE_FLOAT;
        result.m_data.m_float = r;
    }
}

inline void MScriptExprAlgebra::intString ( MScriptValue & result,
                                            long long a,
                                            MScriptExpr::Operator oper,
                                            const MScriptValue::Data::String & b,
                                            const MScriptSrcLocation & location )
{
    switch ( oper )
    {
        case MScriptExpr::OPER_MULT:
        {
            const unsigned int strSize = b.m_size;
            const long long resultSize = a * strSize;

            if ( resultSize < 0 )
            {
                m_interpret->interpreterMessage ( location,
                                                  MScriptBase::MT_ERROR,
                                                  QObject::tr ( "this operation would result in a string of negative "
                                                                "length" ) . toStdString() );
                return;
            }
            else if ( resultSize > INT_MAX )
            {
                m_interpret->interpreterMessage ( location,
                                                  MScriptBase::MT_ERROR,
                                                  QObject::tr ( "this operation would result in an extremely huge "
                                                                "string (%1 bytes)" ) . arg ( resultSize )
                                                                                      . toStdString() );
                return;
            }

            result.m_type = MScriptValue::TYPE_STRING;
            result.m_data.m_string.m_data = (char*) malloc ( resultSize );
            result.m_data.m_string.m_size = (unsigned int) resultSize;

            for ( long long i = 0; i < resultSize; )
            {
                for ( unsigned int j = 0; j < strSize; )
                {
                    result.m_data.m_string.m_data[i++] = b.m_data[j++];
                }
            }

            break;
        }

        case MScriptExpr::OPER_EQ:   case MScriptExpr::OPER_NE:   case MScriptExpr::OPER_LT:
        case MScriptExpr::OPER_GE:   case MScriptExpr::OPER_GT:   case MScriptExpr::OPER_ADD:
        case MScriptExpr::OPER_BOR:  case MScriptExpr::OPER_BXOR: case MScriptExpr::OPER_BAND:
        case MScriptExpr::OPER_LAND: case MScriptExpr::OPER_SHR:  case MScriptExpr::OPER_SHL:
        case MScriptExpr::OPER_MOD:  case MScriptExpr::OPER_LE:   case MScriptExpr::OPER_SUB:
        case MScriptExpr::OPER_LOR:  case MScriptExpr::OPER_DIV:
            incompatibleTypes(location, MScriptValue::TYPE_INT, MScriptValue::TYPE_STRING);
            return;
        default:
            undefinedOperation(location);
            return;
    }
}

inline void MScriptExprAlgebra::floatFloat ( MScriptValue & result,
                                             double a,
                                             MScriptExpr::Operator oper,
                                             double b,
                                             const MScriptSrcLocation & location )
{
    bool boolResult = false;
    bool br;
    double r = 1.0;

    switch ( oper )
    {
        case MScriptExpr::OPER_ADD:  r = a + b;  break;
        case MScriptExpr::OPER_SUB:  r = a - b;  break;
        case MScriptExpr::OPER_MULT: r = a * b;  break;
        case MScriptExpr::OPER_DIV:
            if ( true == checkDivisor(location, b) )
            {
                r = a / b;
            }
            break;

        case MScriptExpr::OPER_LOR:  br = ( a || b ); boolResult = true; break;
        case MScriptExpr::OPER_LAND: br = ( a && b ); boolResult = true; break;
        case MScriptExpr::OPER_EQ:   br = ( a == b ); boolResult = true; break;
        case MScriptExpr::OPER_NE:   br = ( a != b ); boolResult = true; break;
        case MScriptExpr::OPER_LT:   br = ( a  < b ); boolResult = true; break;
        case MScriptExpr::OPER_LE:   br = ( a <= b ); boolResult = true; break;
        case MScriptExpr::OPER_GE:   br = ( a >= b ); boolResult = true; break;
        case MScriptExpr::OPER_GT:   br = ( a  > b ); boolResult = true; break;

        case MScriptExpr::OPER_BOR:  case MScriptExpr::OPER_BXOR: case MScriptExpr::OPER_BAND:
        case MScriptExpr::OPER_SHR:  case MScriptExpr::OPER_SHL:  case MScriptExpr::OPER_MOD:
            incompatibleTypes(location, MScriptValue::TYPE_FLOAT, MScriptValue::TYPE_FLOAT);
            return;

        default:
            undefinedOperation(location);
            return;
    }

    if ( true == boolResult )
    {
        result.m_type = MScriptValue::TYPE_BOOL;
        result.m_data.m_bool = br;
    }
    else
    {
        result.m_type = MScriptValue::TYPE_FLOAT;
        result.m_data.m_float = r;
    }
}

inline void MScriptExprAlgebra::stringString ( MScriptValue & result,
                                               const MScriptValue::Data::String & a,
                                               MScriptExpr::Operator oper,
                                               const MScriptValue::Data::String & b,
                                               const MScriptSrcLocation & location )
{
    bool r;

    switch ( oper )
    {
        case MScriptExpr::OPER_ADD:
        {
            result.m_type = MScriptValue::TYPE_STRING;
            result.m_data.m_string.m_size = a.m_size + b.m_size;
            result.m_data.m_string.m_data = (char*) malloc ( result.m_data.m_string.m_size );

            for ( unsigned int i = 0; i < a.m_size; i++ )
            {
                result.m_data.m_string.m_data[i] = a.m_data[i];
            }
            for ( unsigned int i = 0; i < b.m_size; i++ )
            {
                result.m_data.m_string.m_data [ i + a.m_size ] = b.m_data[i];
            }

            return;
        }

        case MScriptExpr::OPER_EQ:
        case MScriptExpr::OPER_NE:
        {
            if ( a.m_size != b.m_size )
            {
                r = false;
            }
            else
            {
                for ( unsigned int i = 0; i < a.m_size; i++ )
                {
                    if ( a.m_data[i] != b.m_data[i] )
                    {
                        r = false;
                        break;
                    }
                }
            }

            if ( MScriptExpr::OPER_NE == oper )
            {
                r = !r;
            }

            break;
        }

        case MScriptExpr::OPER_LT:   r = ( a.m_size  < b.m_size ); break;
        case MScriptExpr::OPER_LE:   r = ( a.m_size <= b.m_size ); break;
        case MScriptExpr::OPER_GE:   r = ( a.m_size >= b.m_size ); break;
        case MScriptExpr::OPER_GT:   r = ( a.m_size  > b.m_size ); break;

        case MScriptExpr::OPER_LOR:  case MScriptExpr::OPER_LAND: case MScriptExpr::OPER_DIV:
        case MScriptExpr::OPER_MOD:  case MScriptExpr::OPER_SHR:  case MScriptExpr::OPER_SHL:
        case MScriptExpr::OPER_BOR:  case MScriptExpr::OPER_BXOR: case MScriptExpr::OPER_BAND:
        case MScriptExpr::OPER_SUB:  case MScriptExpr::OPER_MULT:
            incompatibleTypes(location, MScriptValue::TYPE_STRING, MScriptValue::TYPE_STRING);
            return;

        default:
            undefinedOperation(location);
            return;
    }

    result.m_type = MScriptValue::TYPE_BOOL;
    result.m_data.m_bool = r;
}

inline void MScriptExprAlgebra::intBool ( MScriptValue & result,
                                          long long a,
                                          MScriptExpr::Operator oper,
                                          bool b,
                                          const MScriptSrcLocation & location )
{
    bool boolResult = false;
    bool br;
    long long r = 1;

    switch ( oper )
    {
        case MScriptExpr::OPER_ADD:  r = a + ( b ? 1 : 0 );  break;
        case MScriptExpr::OPER_SUB:  r = a - ( b ? 1 : 0 );  break;
        case MScriptExpr::OPER_MULT: r = a * ( b ? 1 : 0 );  break;
        case MScriptExpr::OPER_LOR:  br = ( a || b ); boolResult = true; break;
        case MScriptExpr::OPER_LAND: br = ( a && b ); boolResult = true; break;
        case MScriptExpr::OPER_EQ:   br = ( a == b ); boolResult = true; break;
        case MScriptExpr::OPER_NE:   br = ( a != b ); boolResult = true; break;
        case MScriptExpr::OPER_LT:   br = ( a  < b ); boolResult = true; break;
        case MScriptExpr::OPER_LE:   br = ( a <= b ); boolResult = true; break;
        case MScriptExpr::OPER_GE:   br = ( a >= b ); boolResult = true; break;
        case MScriptExpr::OPER_GT:   br = ( a  > b ); boolResult = true; break;

        case MScriptExpr::OPER_BOR:
        case MScriptExpr::OPER_BXOR:
        case MScriptExpr::OPER_BAND:
        case MScriptExpr::OPER_SHR:
        case MScriptExpr::OPER_SHL:
        case MScriptExpr::OPER_DIV:
        case MScriptExpr::OPER_MOD:
            incompatibleTypes(location, MScriptValue::TYPE_INT, MScriptValue::TYPE_BOOL);
            return;
        default:
            undefinedOperation(location);
            return;
    }

    if ( true == boolResult )
    {
        result.m_type = MScriptValue::TYPE_BOOL;
        result.m_data.m_bool = br;
    }
    else
    {
        result.m_type = MScriptValue::TYPE_INT;
        result.m_data.m_integer = r;
    }
}

inline void MScriptExprAlgebra::intComplex ( MScriptValue & result,
                                             long long a,
                                             MScriptExpr::Operator oper,
                                             const MScriptValue::Data::Complex & b,
                                             const MScriptSrcLocation & location )
{
    
}

inline void MScriptExprAlgebra::floatBool ( MScriptValue & result,
                                            double a,
                                            MScriptExpr::Operator oper,
                                            bool b,
                                            const MScriptSrcLocation & location )
{
    incompatibleTypes(location, MScriptValue::TYPE_FLOAT, MScriptValue::TYPE_BOOL);
}

inline void MScriptExprAlgebra::floatComplex ( MScriptValue & result,
                                               double a,
                                               MScriptExpr::Operator oper,
                                               const MScriptValue::Data::Complex & b,
                                               const MScriptSrcLocation & location )
{
    
}

inline void MScriptExprAlgebra::stringBool ( MScriptValue & result,
                                             const MScriptValue::Data::String & a,
                                             MScriptExpr::Operator oper,
                                             bool b,
                                             const MScriptSrcLocation & location )
{
    incompatibleTypes(location, MScriptValue::TYPE_STRING, MScriptValue::TYPE_BOOL);
}

inline void MScriptExprAlgebra::stringComplex ( MScriptValue & result,
                                                const MScriptValue::Data::String & a,
                                                MScriptExpr::Operator oper,
                                                const MScriptValue::Data::Complex & b,
                                                const MScriptSrcLocation & location )
{
    incompatibleTypes(location, MScriptValue::TYPE_STRING, MScriptValue::TYPE_COMPLEX);
}

inline void MScriptExprAlgebra::boolInt ( MScriptValue & result,
                                          bool a,
                                          MScriptExpr::Operator oper,
                                          long long b,
                                          const MScriptSrcLocation & location )
{
    intBool(result, b, oper, a, location);
}

inline void MScriptExprAlgebra::boolFloat ( MScriptValue & result,
                                            bool a,
                                            MScriptExpr::Operator oper,
                                            double b,
                                            const MScriptSrcLocation & location )
{
    incompatibleTypes(location, MScriptValue::TYPE_BOOL, MScriptValue::TYPE_FLOAT);
}

inline void MScriptExprAlgebra::boolString ( MScriptValue & result,
                                             bool a,
                                             MScriptExpr::Operator oper,
                                             const MScriptValue::Data::String & b,
                                             const MScriptSrcLocation & location )
{
    incompatibleTypes(location, MScriptValue::TYPE_BOOL, MScriptValue::TYPE_STRING);
}

inline void MScriptExprAlgebra::boolBool ( MScriptValue & result,
                                           bool a,
                                           MScriptExpr::Operator oper,
                                           bool b,
                                           const MScriptSrcLocation & location )
{
    
}

inline void MScriptExprAlgebra::boolComplex ( MScriptValue & /*result*/,
                                              bool /*a*/,
                                              MScriptExpr::Operator /*oper*/,
                                              const MScriptValue::Data::Complex & /*b*/,
                                              const MScriptSrcLocation & location )
{
    incompatibleTypes(location, MScriptValue::TYPE_BOOL, MScriptValue::TYPE_COMPLEX);
}

inline void MScriptExprAlgebra::complexInt ( MScriptValue & result,
                                             const MScriptValue::Data::Complex & a,
                                             MScriptExpr::Operator oper,
                                             long long b,
                                             const MScriptSrcLocation & location )
{
    
}

inline void MScriptExprAlgebra::complexFloat ( MScriptValue & result,
                                               const MScriptValue::Data::Complex & a,
                                               MScriptExpr::Operator oper,
                                               double b,
                                               const MScriptSrcLocation & location )
{
    
}

inline void MScriptExprAlgebra::complexString ( MScriptValue & /*result*/,
                                                const MScriptValue::Data::Complex & /*a*/,
                                                MScriptExpr::Operator /*oper*/,
                                                const MScriptValue::Data::String & /*b*/,
                                                const MScriptSrcLocation & location )
{
    incompatibleTypes(location, MScriptValue::TYPE_COMPLEX, MScriptValue::TYPE_STRING);
}

inline void MScriptExprAlgebra::complexBool ( MScriptValue & /*result*/,
                                              const MScriptValue::Data::Complex & /*a*/,
                                              MScriptExpr::Operator /*oper*/,
                                              bool /*b*/,
                                              const MScriptSrcLocation & location )
{
    incompatibleTypes(location, MScriptValue::TYPE_COMPLEX, MScriptValue::TYPE_BOOL);
}

inline void MScriptExprAlgebra::complexComplex ( MScriptValue & result,
                                                 const MScriptValue::Data::Complex & a,
                                                 MScriptExpr::Operator oper,
                                                 const MScriptValue::Data::Complex & b,
                                                 const MScriptSrcLocation & location )
{
    
}

inline void MScriptExprAlgebra::floatInt ( MScriptValue & result,
                                           double a,
                                           MScriptExpr::Operator oper,
                                           long long b,
                                           const MScriptSrcLocation & location )
{
    bool boolResult = false;
    bool br;
    double r = 1.0;

    switch ( oper )
    {
        case MScriptExpr::OPER_ADD:  r = a + b; break;
        case MScriptExpr::OPER_SUB:  r = a - b; break;
        case MScriptExpr::OPER_MULT: r = a * b; break;

        case MScriptExpr::OPER_EQ:   br = ( a == b ); boolResult = true; break;
        case MScriptExpr::OPER_NE:   br = ( a != b ); boolResult = true; break;
        case MScriptExpr::OPER_LT:   br = ( a  < b ); boolResult = true; break;
        case MScriptExpr::OPER_LE:   br = ( a <= b ); boolResult = true; break;
        case MScriptExpr::OPER_GE:   br = ( a >= b ); boolResult = true; break;
        case MScriptExpr::OPER_GT:   br = ( a  > b ); boolResult = true; break;
        case MScriptExpr::OPER_LOR:  br = ( a || b ); boolResult = true; break;
        case MScriptExpr::OPER_LAND: br = ( a && b ); boolResult = true; break;

        case MScriptExpr::OPER_DIV:
            if ( true == checkDivisor(location, b) )
            {
                r = a / b;
            }
            break;

        case MScriptExpr::OPER_MOD:  case MScriptExpr::OPER_SHR:  case MScriptExpr::OPER_SHL:
        case MScriptExpr::OPER_BOR:  case MScriptExpr::OPER_BXOR: case MScriptExpr::OPER_BAND:
            incompatibleTypes(location, MScriptValue::TYPE_INT, MScriptValue::TYPE_FLOAT);
            return;

        default:
            undefinedOperation(location);
            return;
    }

    if ( true == boolResult )
    {
        result.m_type = MScriptValue::TYPE_BOOL;
        result.m_data.m_bool = br;
    }
    else
    {
        result.m_type = MScriptValue::TYPE_FLOAT;
        result.m_data.m_float = r;
    }
}

inline void MScriptExprAlgebra::floatString ( MScriptValue & result,
                                              double /*a*/,
                                              MScriptExpr::Operator /*oper*/,
                                              const MScriptValue::Data::String & /*b*/,
                                              const MScriptSrcLocation & location )
{
    incompatibleTypes(location, MScriptValue::TYPE_FLOAT, MScriptValue::TYPE_STRING);
}

inline void MScriptExprAlgebra::stringInt ( MScriptValue & result,
                                            const MScriptValue::Data::String & a,
                                            MScriptExpr::Operator oper,
                                            long long b,
                                            const MScriptSrcLocation & location )
{
    intString ( result, b, oper, a, location );
}

inline void MScriptExprAlgebra::stringFloat ( MScriptValue & result,
                                              const MScriptValue::Data::String & a,
                                              MScriptExpr::Operator oper,
                                              double b,
                                              const MScriptSrcLocation & location )
{
    floatString ( result, b, oper, a, location );
}

inline bool MScriptExprAlgebra::checkShift ( const MScriptSrcLocation & location,
                                             long long by )
{
    if ( (size_t) llabs(by) >= sizeof(by) )
    {
        m_interpret->interpreterMessage ( location,
                                          MScriptBase::MT_ERROR,
                                          QObject::tr ( "attempting to shift value by number greater than its size" )
                                                      . toStdString() );

        return false;
    }

    return true;
}

inline bool MScriptExprAlgebra::checkDivisor ( const MScriptSrcLocation & location,
                                               long long div )
{
    if ( 0 == div )
    {
        m_interpret->interpreterMessage ( location,
                                          MScriptBase::MT_ERROR,
                                          QObject::tr ( "division by zero" ) . toStdString() );

        return false;
    }

    return true;
}

bool MScriptExprAlgebra::checkAssignment ( const MScriptSrcLocation & location,
                                           const MScriptValue & value )
{
    if (
           ( MScriptValue::TYPE_SYMBOL == value.m_type )
               ||
           (
               ( MScriptValue::TYPE_EXPR == value.m_type )
                   &&
               ( MScriptExpr::OPER_INDEX == value.m_data.m_expr->oper() )
                   &&
               ( MScriptValue::TYPE_SYMBOL == value.m_data.m_expr->lVal().m_type )
           )
       )
    {
        return true;
    }

    m_interpret->interpreterMessage ( location,
                                      MScriptBase::MT_ERROR,
                                      QObject::tr ( "only variable can be assigned a value" ) . toStdString() );
    return false;
}

inline void MScriptExprAlgebra::undefinedOperation ( const MScriptSrcLocation & location )
{
    m_interpret->interpreterMessage ( location,
                                      MScriptBase::MT_ERROR,
                                      QObject::tr ( "undefined operation" ) . toStdString() );
}

void MScriptExprAlgebra::incompatibleType ( const MScriptSrcLocation & location,
                                            const MScriptValue::Type type )
{
    switch ( type )
    {
        case MScriptValue::TYPE_EMPTY:
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "empty value used in computation" ) . toStdString() );
            break;
        case MScriptValue::TYPE_EXPR:
        case MScriptValue::TYPE_SYMBOL:
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "illegal value in computation" ) . toStdString() );
            break;
        case MScriptValue::TYPE_INT:
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "this operation cannot be performed on a integer" )
                                                          . toStdString() );
            break;
        case MScriptValue::TYPE_FLOAT:
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "this operation cannot be performed on a floating point "
                                                            "number" ) . toStdString() );
            break;
        case MScriptValue::TYPE_STRING:
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "this operation cannot be performed on a string" )
                                                          . toStdString() );
            break;
        case MScriptValue::TYPE_BOOL:
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "this operation cannot be performed on a boolean value" )
                                                          . toStdString() );
            break;
        case MScriptValue::TYPE_COMPLEX:
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "this operation cannot be performed on a complex number" )
                                                          . toStdString() );
            break;
    }
}

void MScriptExprAlgebra::incompatibleTypes ( const MScriptSrcLocation & location,
                                             const MScriptValue::Type typeA,
                                             const MScriptValue::Type typeB )
{
    switch ( typeA )
    {
        case MScriptValue::TYPE_EMPTY:
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "empty value used in computation" ) . toStdString() );
            break;
        case MScriptValue::TYPE_EXPR:
        case MScriptValue::TYPE_SYMBOL:
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "this operation cannot be performed between these "
                                                            "datatypes" ) . toStdString() );
            break;
        case MScriptValue::TYPE_INT:
            switch ( typeA )
            {
                case MScriptValue::TYPE_EMPTY:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "empty value used in computation" ).toStdString() );
                    break;
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between these "
                                                                    "datatypes" ) . toStdString() );
                    break;
                case MScriptValue::TYPE_INT:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between two "
                                                                    "integers" ) . toStdString() );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between an "
                                                                    "integer and a floating point number" )
                                                                  . toStdString() );
                    break;
                case MScriptValue::TYPE_STRING:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between an "
                                                                    "integer and a string" ) . toStdString() );
                    break;
                case MScriptValue::TYPE_BOOL:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between an "
                                                                    "integer and a boolean value" ) . toStdString() );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between an "
                                                                    "integer and a complex number" ) . toStdString() );
                    break;
            }
            break;
        case MScriptValue::TYPE_FLOAT:
            switch ( typeA )
            {
                case MScriptValue::TYPE_EMPTY:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "empty value used in computation" ).toStdString() );
                    break;
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between these "
                                                                    "datatypes" ) . toStdString() );
                    break;
                case MScriptValue::TYPE_INT:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between a "
                                                                    "floating point number and an integer" ) . toStdString() );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between two "
                                                                    "floating point numbers" )
                                                                  . toStdString() );
                    break;
                case MScriptValue::TYPE_STRING:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between a "
                                                                    "floating point number and a string" )
                                                                  . toStdString() );
                    break;
                case MScriptValue::TYPE_BOOL:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between a "
                                                                    "floating point number and a boolean value" )
                                                                  . toStdString() );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between a "
                                                                    "floating point number and a complex number" )
                                                                  . toStdString() );
                    break;
            }
            break;
        case MScriptValue::TYPE_STRING:
            switch ( typeA )
            {
                case MScriptValue::TYPE_EMPTY:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "empty value used in computation" ).toStdString() );
                    break;
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between these "
                                                                    "datatypes" ) . toStdString() );
                    break;
                case MScriptValue::TYPE_INT:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between a "
                                                                    "string and an integer" )
                                                                  . toStdString() );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between a "
                                                                    "string and a floating point number" )
                                                                  . toStdString() );
                    break;
                case MScriptValue::TYPE_STRING:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between two "
                                                                    "strings" )
                                                                  . toStdString() );
                    break;
                case MScriptValue::TYPE_BOOL:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between a "
                                                                    "string and a boolean value" )
                                                                  . toStdString() );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between a "
                                                                    "string and a complex number" )
                                                                  . toStdString() );
                    break;
            }
            break;
        case MScriptValue::TYPE_BOOL:
            switch ( typeA )
            {
                case MScriptValue::TYPE_EMPTY:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "empty value used in computation" ).toStdString() );
                    break;
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between these "
                                                                    "datatypes" ) . toStdString() );
                    break;
                case MScriptValue::TYPE_INT:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between a "
                                                                    "boolean value and an integer" )
                                                                  . toStdString() );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between a "
                                                                    "boolean value and a floating point number" )
                                                                  . toStdString() );
                    break;
                case MScriptValue::TYPE_STRING:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between a "
                                                                    "boolean value and a string" )
                                                                  . toStdString() );
                    break;
                case MScriptValue::TYPE_BOOL:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between two "
                                                                    "boolean values" )
                                                                  . toStdString() );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between a "
                                                                    "boolean value and a complex number" )
                                                                  . toStdString() );
                    break;
            }
            break;
        case MScriptValue::TYPE_COMPLEX:
            switch ( typeA )
            {
                case MScriptValue::TYPE_EMPTY:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "empty value used in computation" ).toStdString() );
                    break;
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between these "
                                                                    "datatypes" ) . toStdString() );
                    break;
                case MScriptValue::TYPE_INT:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between a "
                                                                    "complex number and an integer" )
                                                                  . toStdString() );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between a "
                                                                    "complex number and a floating point number" )
                                                                  . toStdString() );
                    break;
                case MScriptValue::TYPE_STRING:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between a "
                                                                    "complex number and a string" )
                                                                  . toStdString() );
                    break;
                case MScriptValue::TYPE_BOOL:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between a "
                                                                    "complex number and a boolean value" )
                                                                  . toStdString() );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    m_interpret->interpreterMessage ( location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "this operation cannot be performed between two "
                                                                    "complex numbers" )
                                                                  . toStdString() );
                    break;
            }
            break;
    }
}
