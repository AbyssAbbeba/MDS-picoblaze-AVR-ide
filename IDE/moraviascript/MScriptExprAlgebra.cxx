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
 * @ingroup MoraviaScript
 * @file MScriptExprAlgebra.cxx
 */
// =============================================================================

#include "MScriptExprAlgebra.h"

// MScript language interpreter header files.
#include "MScriptBase.h"
#include "MScriptInterpretInterface.h"

// Standard header files.
#include <cmath>
#include <regex>
#include <complex>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <sstream>

// Used for i18n only.
#include <QObject>

#include <iostream> // DEBUG

bool MScriptExprAlgebra::isUnary ( const MScriptExpr::Operator oper )
{
    switch ( oper )
    {
        case MScriptExpr::OPER_IS_BOOL:         case MScriptExpr::OPER_IS_INT:
        case MScriptExpr::OPER_IS_STRING:       case MScriptExpr::OPER_IS_FLOAT:
        case MScriptExpr::OPER_IS_COMPLEX:      case MScriptExpr::OPER_IS_EMPTY:
        case MScriptExpr::OPER_TO_BOOL:         case MScriptExpr::OPER_TO_INT:
        case MScriptExpr::OPER_TO_STRING:       case MScriptExpr::OPER_TO_FLOAT:
        case MScriptExpr::OPER_IS_INFINITY:     case MScriptExpr::OPER_IS_POSITIVE:
        case MScriptExpr::OPER_IS_NEGATIVE:     case MScriptExpr::OPER_IS_FINITE:
        case MScriptExpr::OPER_REAL:            case MScriptExpr::OPER_IMG_UNIT:
        case MScriptExpr::OPER_SIN:             case MScriptExpr::OPER_COS:
        case MScriptExpr::OPER_TAN:             case MScriptExpr::OPER_ARCSIN:
        case MScriptExpr::OPER_ARCCOS:          case MScriptExpr::OPER_ARCTAN:
        case MScriptExpr::OPER_SINH:            case MScriptExpr::OPER_COSH:
        case MScriptExpr::OPER_TANH:            case MScriptExpr::OPER_ARCSINH:
        case MScriptExpr::OPER_ARCCOSH:         case MScriptExpr::OPER_ARCTANH:
        case MScriptExpr::OPER_CEIL:            case MScriptExpr::OPER_ROUND:
        case MScriptExpr::OPER_FLOOR:           case MScriptExpr::OPER_ABS:
        case MScriptExpr::OPER_NOT:             case MScriptExpr::OPER_CMPL:
        case MScriptExpr::OPER_ADD_INV:         case MScriptExpr::OPER_INT_PROM:
        case MScriptExpr::OPER_IS_NAN:          case MScriptExpr::OPER_LENGTH:
            return true;
        default:
            return false;
    }
}

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
                    result.m_data.m_bool = ( ( 0 == input.m_data.m_float ) ? 1 : 0 );
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
                    checkFloat(location, result.m_data.m_float);
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

        case MScriptExpr::OPER_LENGTH:
            if ( MScriptValue::TYPE_STRING == input.m_type )
            {
                result.m_type = MScriptValue::TYPE_INT;
                result.m_data.m_integer = ( long long ) ( input.m_data.m_string.m_size );
            }
            else
            {
                incompatibleType(location, input.m_type);
            }
            break;
        case MScriptExpr::OPER_IS_BOOL:
            result.m_type = MScriptValue::TYPE_BOOL;
            result.m_data.m_bool = ( MScriptValue::TYPE_BOOL == input.m_type );
            break;
        case MScriptExpr::OPER_IS_INT:
            result.m_type = MScriptValue::TYPE_BOOL;
            result.m_data.m_bool = ( MScriptValue::TYPE_INT == input.m_type );
            break;
        case MScriptExpr::OPER_IS_STRING:
            result.m_type = MScriptValue::TYPE_BOOL;
            result.m_data.m_bool = ( MScriptValue::TYPE_STRING == input.m_type );
            break;
        case MScriptExpr::OPER_IS_FLOAT:
            result.m_type = MScriptValue::TYPE_BOOL;
            result.m_data.m_bool = ( MScriptValue::TYPE_FLOAT == input.m_type );
            break;
        case MScriptExpr::OPER_IS_COMPLEX:
            result.m_type = MScriptValue::TYPE_BOOL;
            result.m_data.m_bool = ( MScriptValue::TYPE_COMPLEX == input.m_type );
            break;
        case MScriptExpr::OPER_IS_EMPTY:
            result.m_type = MScriptValue::TYPE_BOOL;
            result.m_data.m_bool = ( MScriptValue::TYPE_EMPTY == input.m_type );
            break;
        case MScriptExpr::OPER_IS_NAN:
            result.m_type = MScriptValue::TYPE_BOOL;
            result.m_data.m_bool = false;
            if ( MScriptValue::TYPE_FLOAT == input.m_type )
            {
                result.m_data.m_bool = std::isnan(input.m_data.m_float);
            }
            break;
        case MScriptExpr::OPER_IS_INFINITY:
            result.m_type = MScriptValue::TYPE_BOOL;
            result.m_data.m_bool = false;
            if ( MScriptValue::TYPE_FLOAT == input.m_type )
            {
                result.m_data.m_bool = std::isinf(input.m_data.m_float);
            }
            break;
        case MScriptExpr::OPER_IS_POSITIVE:
            result.m_data.m_bool = false;
            if ( MScriptValue::TYPE_FLOAT == input.m_type )
            {
                result.m_data.m_bool = !std::signbit(input.m_data.m_float);
            }
            else if ( MScriptValue::TYPE_INT == input.m_type )
            {
                result.m_data.m_bool = ( input.m_data.m_integer >= 0 );
            }
            break;
        case MScriptExpr::OPER_IS_NEGATIVE:
            result.m_data.m_bool = false;
            if ( MScriptValue::TYPE_FLOAT == input.m_type )
            {
                result.m_data.m_bool = std::signbit(input.m_data.m_float);
            }
            else if ( MScriptValue::TYPE_INT == input.m_type )
            {
                result.m_data.m_bool = ( input.m_data.m_integer < 0 );
            }
            break;
        case MScriptExpr::OPER_IS_FINITE:
            result.m_type = MScriptValue::TYPE_BOOL;
            result.m_data.m_bool = true;
            if ( MScriptValue::TYPE_FLOAT == input.m_type )
            {
                result.m_data.m_bool = std::isfinite(input.m_data.m_float);
            }
            break;
        case MScriptExpr::OPER_REAL:
            if ( MScriptValue::TYPE_COMPLEX == input.m_type )
            {
                result.m_type = MScriptValue::TYPE_FLOAT;
                result.m_data.m_float = input.m_data.m_complex.m_r;
            }
            else
            {
                incompatibleType(location, input.m_type);
            }
            break;
        case MScriptExpr::OPER_IMG_UNIT:
            if ( MScriptValue::TYPE_COMPLEX == input.m_type )
            {
                result.m_type = MScriptValue::TYPE_FLOAT;
                result.m_data.m_float = input.m_data.m_complex.m_i;
            }
            else
            {
                incompatibleType(location, input.m_type);
            }
            break;
        case MScriptExpr::OPER_TO_BOOL:
            result.m_type = MScriptValue::TYPE_BOOL;
            switch ( input.m_type )
            {
                case MScriptValue::TYPE_INT:
                    result.m_data.m_bool = ( 0 != input.m_data.m_integer );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    result.m_data.m_bool = ( 0 != input.m_data.m_float );
                    break;
                case MScriptValue::TYPE_BOOL:
                    result.m_data.m_bool = input.m_data.m_bool;
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    result.m_data.m_bool = ( MScriptComplex() != input.m_data.m_complex );
                    break;
                case MScriptValue::TYPE_STRING:
                    result.m_data.m_bool = ( 0 != input.m_data.m_string.m_size );
                    break;
                case MScriptValue::TYPE_EMPTY:
                    result.m_data.m_bool = false;
                    break;
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    incompatibleType(location, input.m_type);
                    break;
            }
            break;
        case MScriptExpr::OPER_TO_INT:
            result.m_type = MScriptValue::TYPE_INT;
            switch ( input.m_type )
            {
                case MScriptValue::TYPE_INT:
                    result.m_data.m_integer = input.m_data.m_integer;
                    break;
                case MScriptValue::TYPE_FLOAT:
                    result.m_data.m_integer = ( long long ) input.m_data.m_float;
                    break;
                case MScriptValue::TYPE_BOOL:
                    result.m_data.m_integer = ( input.m_data.m_bool ? 1 : 0 );
                    break;
                case MScriptValue::TYPE_STRING:
                {
                    int base;
                    std::string str ( input.m_data.m_string.m_data, input.m_data.m_string.m_size );
                    if ( true == std::regex_match ( str, std::regex("[0-9]+") ) )
                    {
                        // Decimal number.
                        base = 10;
                    }
                    else if ( true == std::regex_match ( str, std::regex("0[xX][0-9a-fA-F]+") ) )
                    {
                        // Hexadecimal number.
                        base = 2;
                    }
                    else if ( true == std::regex_match ( str, std::regex("0[bB][01]+") ) )
                    {
                        // Binary number.
                        base = 2;
                    }
                    else if ( true == std::regex_match ( str, std::regex("0[0-7]+") ) )
                    {
                        // Octal number
                        base = 2;
                    }
                    else
                    {
                        m_interpret->interpreterMessage ( location,
                                                          MScriptBase::MT_ERROR,
                                                          QObject::tr ( "not a number: " )
                                                                      . toStdString() + "`" + str + "'" );
                        break;
                    }

                    result.m_data.m_integer = strtoll ( input.m_data.m_string.m_data, nullptr, base );
                    if ( ( LLONG_MIN == result.m_data.m_integer ) || ( LLONG_MAX == result.m_data.m_integer ) )
                    {
                        m_interpret->interpreterMessage ( location,
                                                          MScriptBase::MT_ERROR,
                                                          QObject::tr ( "number is too big: " )
                                                                      . toStdString()
                                                                      + "`"
                                                                      + std::string ( input.m_data.m_string.m_data )
                                                                      + "'" );
                    }
                    break;
                }
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_COMPLEX:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    incompatibleType(location, input.m_type);
                    break;
            }
            break;
        case MScriptExpr::OPER_TO_STRING:
        {
            std::ostringstream strstream;
            result.m_type = MScriptValue::TYPE_STRING;
            switch ( input.m_type )
            {
                case MScriptValue::TYPE_INT:
                    strstream << input.m_data.m_integer;
                    break;
                case MScriptValue::TYPE_FLOAT:
                    strstream << input.m_data.m_float;
                    break;
                case MScriptValue::TYPE_BOOL:
                    strstream << std::boolalpha << input.m_data.m_bool << std::noboolalpha;
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    strstream << std::scientific << input.m_data.m_complex.m_r << " + "
                              << input.m_data.m_complex.m_i << "i";
                    break;
                case MScriptValue::TYPE_STRING:
                    input.makeCopy(result);
                    return;
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    incompatibleType(location, input.m_type);
                    return;
            }
            result.m_data.m_string.m_size = strstream.str().size();
            result.m_data.m_string.m_data = (char*) malloc ( 1 + result.m_data.m_string.m_size );
            strcpy ( result.m_data.m_string.m_data, strstream.str().c_str() );
            break;
        }
        case MScriptExpr::OPER_TO_FLOAT:
            result.m_type = MScriptValue::TYPE_FLOAT;
            switch ( input.m_type )
            {
                case MScriptValue::TYPE_INT:
                    result.m_data.m_float = double(input.m_data.m_integer);
                    break;
                case MScriptValue::TYPE_FLOAT:
                    result.m_data.m_float = input.m_data.m_float;
                    break;
                case MScriptValue::TYPE_BOOL:
                    result.m_data.m_float = ( input.m_data.m_bool ? 1.0d : 0.0d );
                    break;
                case MScriptValue::TYPE_STRING:
                    result.m_data.m_float = strtod ( input.m_data.m_string.m_data, nullptr );
                    if ( ( -HUGE_VAL == result.m_data.m_float ) || ( HUGE_VAL == result.m_data.m_float ) )
                    {
                        m_interpret->interpreterMessage ( location,
                                                          MScriptBase::MT_ERROR,
                                                          QObject::tr ( "number is too big: " )
                                                                      . toStdString()
                                                                      + "`"
                                                                      + std::string ( input.m_data.m_string.m_data,
                                                                                      input.m_data.m_string.m_size )
                                                                      + "'" );
                    }
                    break;
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_COMPLEX:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    incompatibleType(location, input.m_type);
                    break;
            }
            break;
        case MScriptExpr::OPER_SIN:
            switch ( input.m_type )
            {
                case MScriptValue::TYPE_INT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = sin ( input.m_data.m_integer );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = sin ( input.m_data.m_float );
                    break;
                case MScriptValue::TYPE_BOOL:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = sin ( input.m_data.m_bool ? 1 : 0 );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    result.m_type = MScriptValue::TYPE_COMPLEX;
                    result.m_data.m_complex = sin ( std::complex<double> ( input.m_data.m_complex.m_r,
                                                                           input.m_data.m_complex.m_i ) );
                    break;
                case MScriptValue::TYPE_STRING:
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    incompatibleType(location, input.m_type);
                    break;
            }
            break;
        case MScriptExpr::OPER_COS:
            switch ( input.m_type )
            {
                case MScriptValue::TYPE_INT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = cos ( input.m_data.m_integer );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = cos ( input.m_data.m_float );
                    break;
                case MScriptValue::TYPE_BOOL:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = cos ( input.m_data.m_bool ? 1 : 0 );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    result.m_type = MScriptValue::TYPE_COMPLEX;
                    result.m_data.m_complex = cos ( std::complex<double> ( input.m_data.m_complex.m_r,
                                                                           input.m_data.m_complex.m_i ) );
                    break;
                case MScriptValue::TYPE_STRING:
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    incompatibleType(location, input.m_type);
                    break;
            }
            break;
        case MScriptExpr::OPER_TAN:
            switch ( input.m_type )
            {
                case MScriptValue::TYPE_INT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = tan ( input.m_data.m_integer );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = tan ( input.m_data.m_float );
                    break;
                case MScriptValue::TYPE_BOOL:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = tan ( input.m_data.m_bool ? 1 : 0 );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    result.m_type = MScriptValue::TYPE_COMPLEX;
                    result.m_data.m_complex = tan ( std::complex<double> ( input.m_data.m_complex.m_r,
                                                                           input.m_data.m_complex.m_i ) );
                    break;
                case MScriptValue::TYPE_STRING:
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    incompatibleType(location, input.m_type);
                    break;
            }
            break;
        case MScriptExpr::OPER_ARCSIN:
            switch ( input.m_type )
            {
                case MScriptValue::TYPE_INT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = asin ( input.m_data.m_integer );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = asin ( input.m_data.m_float );
                    break;
                case MScriptValue::TYPE_BOOL:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = asin ( input.m_data.m_bool ? 1 : 0 );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    result.m_type = MScriptValue::TYPE_COMPLEX;
                    result.m_data.m_complex = asin ( std::complex<double> ( input.m_data.m_complex.m_r,
                                                                            input.m_data.m_complex.m_i ) );
                    break;
                case MScriptValue::TYPE_STRING:
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    incompatibleType(location, input.m_type);
                    break;
            }
            break;
        case MScriptExpr::OPER_ARCCOS:
            switch ( input.m_type )
            {
                case MScriptValue::TYPE_INT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = acos ( input.m_data.m_integer );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = acos ( input.m_data.m_float );
                    break;
                case MScriptValue::TYPE_BOOL:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = acos ( input.m_data.m_bool ? 1 : 0 );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    result.m_type = MScriptValue::TYPE_COMPLEX;
                    result.m_data.m_complex = acos ( std::complex<double> ( input.m_data.m_complex.m_r,
                                                                            input.m_data.m_complex.m_i ) );
                    break;
                case MScriptValue::TYPE_STRING:
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    incompatibleType(location, input.m_type);
                    break;
            }
            break;
        case MScriptExpr::OPER_ARCTAN:
            switch ( input.m_type )
            {
                case MScriptValue::TYPE_INT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = atan ( input.m_data.m_integer );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = atan ( input.m_data.m_float );
                    break;
                case MScriptValue::TYPE_BOOL:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = atan ( input.m_data.m_bool ? 1 : 0 );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    result.m_type = MScriptValue::TYPE_COMPLEX;
                    result.m_data.m_complex = atan ( std::complex<double> ( input.m_data.m_complex.m_r,
                                                                            input.m_data.m_complex.m_i ) );
                    break;
                case MScriptValue::TYPE_STRING:
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    incompatibleType(location, input.m_type);
                    break;
            }
            break;
        case MScriptExpr::OPER_SINH:
            switch ( input.m_type )
            {
                case MScriptValue::TYPE_INT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = sinh ( input.m_data.m_integer );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = sinh ( input.m_data.m_float );
                    break;
                case MScriptValue::TYPE_BOOL:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = sinh ( input.m_data.m_bool ? 1 : 0 );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    result.m_type = MScriptValue::TYPE_COMPLEX;
                    result.m_data.m_complex = sinh ( std::complex<double> ( input.m_data.m_complex.m_r,
                                                                            input.m_data.m_complex.m_i ) );
                    break;
                case MScriptValue::TYPE_STRING:
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    incompatibleType(location, input.m_type);
                    break;
            }
            break;
        case MScriptExpr::OPER_COSH:
            switch ( input.m_type )
            {
                case MScriptValue::TYPE_INT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = cosh ( input.m_data.m_integer );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = cosh ( input.m_data.m_float );
                    break;
                case MScriptValue::TYPE_BOOL:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = cosh ( input.m_data.m_bool ? 1 : 0 );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    result.m_type = MScriptValue::TYPE_COMPLEX;
                    result.m_data.m_complex = cosh ( std::complex<double> ( input.m_data.m_complex.m_r,
                                                                            input.m_data.m_complex.m_i ) );
                    break;
                case MScriptValue::TYPE_STRING:
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    incompatibleType(location, input.m_type);
                    break;
            }
            break;
        case MScriptExpr::OPER_TANH:
            switch ( input.m_type )
            {
                case MScriptValue::TYPE_INT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = tanh ( input.m_data.m_integer );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = tanh ( input.m_data.m_float );
                    break;
                case MScriptValue::TYPE_BOOL:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = tanh ( input.m_data.m_bool ? 1 : 0 );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    result.m_type = MScriptValue::TYPE_COMPLEX;
                    result.m_data.m_complex= tanh ( std::complex<double> ( input.m_data.m_complex.m_r,
                                                                           input.m_data.m_complex.m_i ) );
                    break;
                case MScriptValue::TYPE_STRING:
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    incompatibleType(location, input.m_type);
                    break;
            }
            break;
        case MScriptExpr::OPER_ARCSINH:
            switch ( input.m_type )
            {
                case MScriptValue::TYPE_INT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = asinh ( input.m_data.m_integer );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = asinh ( input.m_data.m_float );
                    break;
                case MScriptValue::TYPE_BOOL:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = asinh ( input.m_data.m_bool ? 1 : 0 );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    result.m_type = MScriptValue::TYPE_COMPLEX;
                    result.m_data.m_complex = asinh ( std::complex<double> ( input.m_data.m_complex.m_r,
                                                                             input.m_data.m_complex.m_i ) );
                    break;
                case MScriptValue::TYPE_STRING:
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    incompatibleType(location, input.m_type);
                    break;
            }
            break;
        case MScriptExpr::OPER_ARCCOSH:
            switch ( input.m_type )
            {
                case MScriptValue::TYPE_INT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = acosh ( input.m_data.m_integer );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = acosh ( input.m_data.m_float );
                    break;
                case MScriptValue::TYPE_BOOL:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = acosh ( input.m_data.m_bool ? 1 : 0 );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    result.m_type = MScriptValue::TYPE_COMPLEX;
                    result.m_data.m_complex = acosh ( std::complex<double> ( input.m_data.m_complex.m_r,
                                                                             input.m_data.m_complex.m_i ) );
                    break;
                case MScriptValue::TYPE_STRING:
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    incompatibleType(location, input.m_type);
                    break;
            }
            break;
        case MScriptExpr::OPER_ARCTANH:
            switch ( input.m_type )
            {
                case MScriptValue::TYPE_INT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = atanh ( input.m_data.m_integer );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = atanh ( input.m_data.m_float );
                    break;
                case MScriptValue::TYPE_BOOL:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = atanh ( input.m_data.m_bool ? 1 : 0 );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    result.m_type = MScriptValue::TYPE_COMPLEX;
                    result.m_data.m_complex = atanh ( std::complex<double> ( input.m_data.m_complex.m_r,
                                                                             input.m_data.m_complex.m_i ) );
                    break;
                case MScriptValue::TYPE_STRING:
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
                    incompatibleType(location, input.m_type);
                    break;
            }
            break;
        case MScriptExpr::OPER_CEIL:
            if ( MScriptValue::TYPE_FLOAT == input.m_type )
            {
                result.m_type = MScriptValue::TYPE_FLOAT;
                result.m_data.m_float = ceil ( input.m_data.m_float );
            }
            else
            {
                incompatibleType(location, input.m_type);
            }
            break;
        case MScriptExpr::OPER_ROUND:
            if ( MScriptValue::TYPE_FLOAT == input.m_type )
            {
                result.m_type = MScriptValue::TYPE_FLOAT;
                result.m_data.m_float = round ( input.m_data.m_float );
            }
            else
            {
                incompatibleType(location, input.m_type);
            }
            break;
        case MScriptExpr::OPER_FLOOR:
            if ( MScriptValue::TYPE_FLOAT == input.m_type )
            {
                result.m_type = MScriptValue::TYPE_FLOAT;
                result.m_data.m_float = floor ( input.m_data.m_float );
            }
            else
            {
                incompatibleType(location, input.m_type);
            }
            break;
        case MScriptExpr::OPER_ABS:
            switch ( input.m_type )
            {
                case MScriptValue::TYPE_INT:
                    result.m_type = MScriptValue::TYPE_INT;
                    result.m_data.m_integer = abs ( input.m_data.m_integer );
                    break;
                case MScriptValue::TYPE_FLOAT:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = atanh ( input.m_data.m_float );
                    break;
                case MScriptValue::TYPE_COMPLEX:
                    result.m_type = MScriptValue::TYPE_FLOAT;
                    result.m_data.m_float = std::abs ( std::complex<double> ( input.m_data.m_complex.m_r,
                                                                              input.m_data.m_complex.m_i ) );
                    break;
                case MScriptValue::TYPE_BOOL:
                case MScriptValue::TYPE_STRING:
                case MScriptValue::TYPE_EMPTY:
                case MScriptValue::TYPE_EXPR:
                case MScriptValue::TYPE_SYMBOL:
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
    if ( MScriptExpr::OPER_TO_COMPLEX == oper )
    {
        result.m_type = MScriptValue::TYPE_COMPLEX;
        result.m_data.m_complex.m_r = left.m_data.m_float;
        result.m_data.m_complex.m_i = right.m_data.m_float;
        return;
    }

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
            const MScriptComplex & a = left.m_data.m_complex;
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
    bool floatResult = false;
    bool boolResult = false;
    bool br;
    long long r = 1ll;
    double fr = 1.0d;

    switch ( oper )
    {
        case MScriptExpr::OPER_ADD:  r = a + b; checkSignOverflow ( location, a,  b, r, true  ); break;
        case MScriptExpr::OPER_SUB:  r = a - b; checkSignOverflow ( location, a, -b, r, true  ); break;
        case MScriptExpr::OPER_MULT: r = a * b; checkSignOverflow ( location, a,  b, r, false ); break;
        case MScriptExpr::OPER_MIN:  r = ( a < b ) ? a : b; break;
        case MScriptExpr::OPER_MAX:  r = ( a > b ) ? a : b; break;
        case MScriptExpr::OPER_BOR:  r = a | b; break;
        case MScriptExpr::OPER_BXOR: r = a ^ b; break;
        case MScriptExpr::OPER_BAND: r = a & b; break;
        case MScriptExpr::OPER_POW:  fr = pow(double(a), double(b)); floatResult = true; break;
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
    else if ( true == floatResult )
    {
        result.m_type = MScriptValue::TYPE_FLOAT;
        result.m_data.m_float = fr;
        checkFloat(location, fr);
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
    double r = 1.0d;

    switch ( oper )
    {
        case MScriptExpr::OPER_ADD:  r = a + b; break;
        case MScriptExpr::OPER_SUB:  r = a - b; break;
        case MScriptExpr::OPER_MULT: r = a * b; break;

        case MScriptExpr::OPER_MIN:  r = ( double(a) < b ) ? double(a) : b; break;
        case MScriptExpr::OPER_MAX:  r = ( double(a) > b ) ? double(a) : b; break;
        case MScriptExpr::OPER_POW:  r = pow(double(a), b); break;

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
        checkFloat(location, r);
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

        case MScriptExpr::OPER_MIN:  case MScriptExpr::OPER_MAX:  case MScriptExpr::OPER_POW:
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

        case MScriptExpr::OPER_MIN:  r = ( a < b ) ? a : b; break;
        case MScriptExpr::OPER_MAX:  r = ( a > b ) ? a : b; break;
        case MScriptExpr::OPER_POW:  r = pow ( a, b ); break;

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
        checkFloat(location, r);
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
            result.m_data.m_string.m_size = a.m_size + b.m_size + 1;
            result.m_data.m_string.m_data = (char*) malloc ( result.m_data.m_string.m_size );

            for ( unsigned int i = 0; i < a.m_size; i++ )
            {
                result.m_data.m_string.m_data[i] = a.m_data[i];
            }
            for ( unsigned int i = 0; i < b.m_size; i++ )
            {
                result.m_data.m_string.m_data [ i + a.m_size ] = b.m_data[i];
            }
            result.m_data.m_string.m_data [ result.m_data.m_string.m_size - 1 ] = '\0';

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

        case MScriptExpr::OPER_MIN:
        {
            const MScriptValue::Data::String & src = ( ( a.m_size < b.m_size ) ? a : b );
            result.m_type = MScriptValue::TYPE_STRING;
            result.m_data.m_string.m_size = src.m_size;
            result.m_data.m_string.m_data = (char*) malloc ( result.m_data.m_string.m_size + 1 );
            strcpy(result.m_data.m_string.m_data, src.m_data);
            return;
        }
        case MScriptExpr::OPER_MAX:
        {
            const MScriptValue::Data::String & src = ( ( a.m_size > b.m_size ) ? a : b );
            result.m_type = MScriptValue::TYPE_STRING;
            result.m_data.m_string.m_size = src.m_size;
            result.m_data.m_string.m_data = (char*) malloc ( result.m_data.m_string.m_size + 1 );
            strcpy(result.m_data.m_string.m_data, src.m_data);
            return;
        }

        case MScriptExpr::OPER_POW:
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
        case MScriptExpr::OPER_ADD:
            r = a + ( b ? 1 : 0 );
            checkSignOverflow ( location, a, ( b ?  1 : 0 ), r, true );
            break;
        case MScriptExpr::OPER_SUB:
            r = a - ( b ? 1 : 0 );
            checkSignOverflow ( location, a, ( b ? -1 : 0 ), r, true );
            break;
        case MScriptExpr::OPER_MULT: r = a * ( b ? 1 : 0 ); break;
        case MScriptExpr::OPER_LOR:  br = ( a || b ); boolResult = true; break;
        case MScriptExpr::OPER_LAND: br = ( a && b ); boolResult = true; break;
        case MScriptExpr::OPER_EQ:   br = ( a == b ); boolResult = true; break;
        case MScriptExpr::OPER_NE:   br = ( a != b ); boolResult = true; break;
        case MScriptExpr::OPER_LT:   br = ( a  < b ); boolResult = true; break;
        case MScriptExpr::OPER_LE:   br = ( a <= b ); boolResult = true; break;
        case MScriptExpr::OPER_GE:   br = ( a >= b ); boolResult = true; break;
        case MScriptExpr::OPER_GT:   br = ( a  > b ); boolResult = true; break;

        case MScriptExpr::OPER_MIN:  r = ( a < (b?1:0) ) ? a : (b?1:0); break;
        case MScriptExpr::OPER_MAX:  r = ( a > (b?1:0) ) ? a : (b?1:0); break;
        case MScriptExpr::OPER_POW:  r = ( true == b ) ? a : 1; break;

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
                                             long long _a,
                                             MScriptExpr::Operator oper,
                                             const MScriptComplex & b,
                                             const MScriptSrcLocation & location )
{
    bool boolResult = false;
    bool br;
    MScriptComplex r;
    MScriptComplex a;

    r.m_r = 1.0;
    r.m_i = 1.0;

    a.m_r = double(_a);
    a.m_i = 0.0;

    switch ( oper )
    {
        case MScriptExpr::OPER_LOR:  br = ( a || b ); boolResult = true; break;
        case MScriptExpr::OPER_LAND: br = ( a && b ); boolResult = true; break;
        case MScriptExpr::OPER_EQ:   br = ( a == b ); boolResult = true; break;
        case MScriptExpr::OPER_NE:   br = ( a != b ); boolResult = true; break;

        case MScriptExpr::OPER_ADD:  r = a + b;  break;
        case MScriptExpr::OPER_SUB:  r = a - b;  break;
        case MScriptExpr::OPER_MULT: r = a * b;  break;
        case MScriptExpr::OPER_DIV:
            if ( true == checkDivisor ( location, a, b) )
            {
                r = a / b;
            }
            break;

        case MScriptExpr::OPER_POW:
        case MScriptExpr::OPER_MIN:  case MScriptExpr::OPER_MAX:
        case MScriptExpr::OPER_LT:   case MScriptExpr::OPER_LE:
        case MScriptExpr::OPER_GE:   case MScriptExpr::OPER_GT:
        case MScriptExpr::OPER_BOR:  case MScriptExpr::OPER_BXOR:
        case MScriptExpr::OPER_BAND: case MScriptExpr::OPER_SHR:
        case MScriptExpr::OPER_SHL:  case MScriptExpr::OPER_MOD:
            incompatibleTypes(location, MScriptValue::TYPE_COMPLEX, MScriptValue::TYPE_COMPLEX);
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
        result.m_type = MScriptValue::TYPE_COMPLEX;
        result.m_data.m_complex = r;
        checkComplex(location, r);
    }
}

inline void MScriptExprAlgebra::floatBool ( MScriptValue & /*result*/,
                                            double /*a*/,
                                            MScriptExpr::Operator /*oper*/,
                                            bool /*b*/,
                                            const MScriptSrcLocation & location )
{
    incompatibleTypes(location, MScriptValue::TYPE_FLOAT, MScriptValue::TYPE_BOOL);
}

inline void MScriptExprAlgebra::floatComplex ( MScriptValue & result,
                                               double _a,
                                               MScriptExpr::Operator oper,
                                               const MScriptComplex & b,
                                               const MScriptSrcLocation & location )
{
    bool boolResult = false;
    bool br;
    MScriptComplex r;
    MScriptComplex a;

    r.m_r = 1.0;
    r.m_i = 1.0;

    a.m_r = _a;
    a.m_i = 0.0;

    switch ( oper )
    {
        case MScriptExpr::OPER_LOR:  br = ( a || b ); boolResult = true; break;
        case MScriptExpr::OPER_LAND: br = ( a && b ); boolResult = true; break;
        case MScriptExpr::OPER_EQ:   br = ( a == b ); boolResult = true; break;
        case MScriptExpr::OPER_NE:   br = ( a != b ); boolResult = true; break;

        case MScriptExpr::OPER_ADD:  r = a + b;  break;
        case MScriptExpr::OPER_SUB:  r = a - b;  break;
        case MScriptExpr::OPER_MULT: r = a * b;  break;
        case MScriptExpr::OPER_DIV:
            if ( true == checkDivisor ( location, a, b) )
            {
                r = a / b;
            }
            break;

        case MScriptExpr::OPER_POW:
        case MScriptExpr::OPER_MIN:  case MScriptExpr::OPER_MAX:
        case MScriptExpr::OPER_LT:   case MScriptExpr::OPER_LE:
        case MScriptExpr::OPER_GE:   case MScriptExpr::OPER_GT:
        case MScriptExpr::OPER_BOR:  case MScriptExpr::OPER_BXOR:
        case MScriptExpr::OPER_BAND: case MScriptExpr::OPER_SHR:
        case MScriptExpr::OPER_SHL:  case MScriptExpr::OPER_MOD:
            incompatibleTypes(location, MScriptValue::TYPE_COMPLEX, MScriptValue::TYPE_COMPLEX);
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
        result.m_type = MScriptValue::TYPE_COMPLEX;
        result.m_data.m_complex = r;
        checkComplex(location, r);
    }
}

inline void MScriptExprAlgebra::stringBool ( MScriptValue & /*result*/,
                                             const MScriptValue::Data::String & /*a*/,
                                             MScriptExpr::Operator /*oper*/,
                                             bool /*b*/,
                                             const MScriptSrcLocation & location )
{
    incompatibleTypes(location, MScriptValue::TYPE_STRING, MScriptValue::TYPE_BOOL);
}

inline void MScriptExprAlgebra::stringComplex ( MScriptValue & /*result*/,
                                                const MScriptValue::Data::String & /*a*/,
                                                MScriptExpr::Operator /*oper*/,
                                                const MScriptComplex & /*b*/,
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
    bool boolResult = false;
    bool br;
    long long r = 1;

    switch ( oper )
    {
        case MScriptExpr::OPER_ADD:
            r = ( a ? 1 : 0 ) + b;
            checkSignOverflow ( location, ( a ? 1 : 0 ), b, r, true );
            break;
        case MScriptExpr::OPER_SUB:
            r = ( a ? 1 : 0 ) - b;
            checkSignOverflow ( location, ( a ? -1 : 0 ), b, r, true );
            break;
        case MScriptExpr::OPER_MULT: r = ( a ? 1 : 0 ) * b; break;
        case MScriptExpr::OPER_LOR:  br = ( a || b ); boolResult = true; break;
        case MScriptExpr::OPER_LAND: br = ( a && b ); boolResult = true; break;
        case MScriptExpr::OPER_EQ:   br = ( a == b ); boolResult = true; break;
        case MScriptExpr::OPER_NE:   br = ( a != b ); boolResult = true; break;
        case MScriptExpr::OPER_LT:   br = ( a  < b ); boolResult = true; break;
        case MScriptExpr::OPER_LE:   br = ( a <= b ); boolResult = true; break;
        case MScriptExpr::OPER_GE:   br = ( a >= b ); boolResult = true; break;
        case MScriptExpr::OPER_GT:   br = ( a  > b ); boolResult = true; break;

        case MScriptExpr::OPER_MIN:  r = ( (a?1:0) < b ) ? (a?1:0) : b; break;
        case MScriptExpr::OPER_MAX:  r = ( (a?1:0) > b ) ? (a?1:0) : b; break;
        case MScriptExpr::OPER_POW:  r = ( a ? 1 : 0 ); break;

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

inline void MScriptExprAlgebra::boolFloat ( MScriptValue & /*result*/,
                                            bool /*a*/,
                                            MScriptExpr::Operator /*oper*/,
                                            double /*b*/,
                                            const MScriptSrcLocation & location )
{
    incompatibleTypes(location, MScriptValue::TYPE_BOOL, MScriptValue::TYPE_FLOAT);
}

inline void MScriptExprAlgebra::boolString ( MScriptValue & /*result*/,
                                             bool /*a*/,
                                             MScriptExpr::Operator /*oper*/,
                                             const MScriptValue::Data::String & /*b*/,
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
    bool r = false;

    switch ( oper )
    {
        case MScriptExpr::OPER_LOR:  r = ( a || b ); break;
        case MScriptExpr::OPER_LAND: r = ( a && b ); break;
        case MScriptExpr::OPER_EQ:   r = ( a == b ); break;
        case MScriptExpr::OPER_NE:   r = ( a != b ); break;
        case MScriptExpr::OPER_LT:   r = ( a  < b ); break;
        case MScriptExpr::OPER_LE:   r = ( a <= b ); break;
        case MScriptExpr::OPER_GE:   r = ( a >= b ); break;
        case MScriptExpr::OPER_GT:   r = ( a  > b ); break;

        case MScriptExpr::OPER_MIN:  r = ( a && b ) ? true : false; break;
        case MScriptExpr::OPER_MAX:  r = ( a || b ) ? true : false; break;
        case MScriptExpr::OPER_POW:  r = a; break;

        case MScriptExpr::OPER_ADD:  case MScriptExpr::OPER_SUB:
        case MScriptExpr::OPER_MULT: case MScriptExpr::OPER_BOR:
        case MScriptExpr::OPER_BXOR: case MScriptExpr::OPER_BAND:
        case MScriptExpr::OPER_SHR:  case MScriptExpr::OPER_SHL:
        case MScriptExpr::OPER_DIV:  case MScriptExpr::OPER_MOD:
            incompatibleTypes(location, MScriptValue::TYPE_BOOL, MScriptValue::TYPE_BOOL);
            return;

        default:
            undefinedOperation(location);
            return;
    }

    result.m_type = MScriptValue::TYPE_BOOL;
    result.m_data.m_bool = r;
}

inline void MScriptExprAlgebra::boolComplex ( MScriptValue & /*result*/,
                                              bool /*a*/,
                                              MScriptExpr::Operator /*oper*/,
                                              const MScriptComplex & /*b*/,
                                              const MScriptSrcLocation & location )
{
    incompatibleTypes(location, MScriptValue::TYPE_BOOL, MScriptValue::TYPE_COMPLEX);
}

inline void MScriptExprAlgebra::complexInt ( MScriptValue & result,
                                             const MScriptComplex & a,
                                             MScriptExpr::Operator oper,
                                             long long _b,
                                             const MScriptSrcLocation & location )
{
    bool boolResult = false;
    bool br;
    MScriptComplex r;
    MScriptComplex b;

    r.m_r = 1.0;
    r.m_i = 1.0;

    b.m_r = double(_b);
    b.m_i = 0.0;

    switch ( oper )
    {
        case MScriptExpr::OPER_LOR:  br = ( a || b ); boolResult = true; break;
        case MScriptExpr::OPER_LAND: br = ( a && b ); boolResult = true; break;
        case MScriptExpr::OPER_EQ:   br = ( a == b ); boolResult = true; break;
        case MScriptExpr::OPER_NE:   br = ( a != b ); boolResult = true; break;

        case MScriptExpr::OPER_ADD:  r = a + b;  break;
        case MScriptExpr::OPER_SUB:  r = a - b;  break;
        case MScriptExpr::OPER_MULT: r = a * b;  break;
        case MScriptExpr::OPER_DIV:
            if ( true == checkDivisor ( location, a, b) )
            {
                r = a / b;
            }
            break;

        case MScriptExpr::OPER_POW:
        case MScriptExpr::OPER_MIN:  case MScriptExpr::OPER_MAX:
        case MScriptExpr::OPER_LT:   case MScriptExpr::OPER_LE:
        case MScriptExpr::OPER_GE:   case MScriptExpr::OPER_GT:
        case MScriptExpr::OPER_BOR:  case MScriptExpr::OPER_BXOR:
        case MScriptExpr::OPER_BAND: case MScriptExpr::OPER_SHR:
        case MScriptExpr::OPER_SHL:  case MScriptExpr::OPER_MOD:
            incompatibleTypes(location, MScriptValue::TYPE_COMPLEX, MScriptValue::TYPE_COMPLEX);
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
        result.m_type = MScriptValue::TYPE_COMPLEX;
        result.m_data.m_complex = r;
        checkComplex(location, r);
    }
}

inline void MScriptExprAlgebra::complexFloat ( MScriptValue & result,
                                               const MScriptComplex & a,
                                               MScriptExpr::Operator oper,
                                               double _b,
                                               const MScriptSrcLocation & location )
{
    bool boolResult = false;
    bool br;
    MScriptComplex r;
    MScriptComplex b;

    r.m_r = 1.0;
    r.m_i = 1.0;

    b.m_r = _b;
    b.m_i = 0.0;

    switch ( oper )
    {
        case MScriptExpr::OPER_LOR:  br = ( a || b ); boolResult = true; break;
        case MScriptExpr::OPER_LAND: br = ( a && b ); boolResult = true; break;
        case MScriptExpr::OPER_EQ:   br = ( a == b ); boolResult = true; break;
        case MScriptExpr::OPER_NE:   br = ( a != b ); boolResult = true; break;

        case MScriptExpr::OPER_ADD:  r = a + b;  break;
        case MScriptExpr::OPER_SUB:  r = a - b;  break;
        case MScriptExpr::OPER_MULT: r = a * b;  break;
        case MScriptExpr::OPER_DIV:
            if ( true == checkDivisor ( location, a, b) )
            {
                r = a / b;
            }
            break;

        case MScriptExpr::OPER_POW:
        case MScriptExpr::OPER_MIN:  case MScriptExpr::OPER_MAX:
        case MScriptExpr::OPER_LT:   case MScriptExpr::OPER_LE:
        case MScriptExpr::OPER_GE:   case MScriptExpr::OPER_GT:
        case MScriptExpr::OPER_BOR:  case MScriptExpr::OPER_BXOR:
        case MScriptExpr::OPER_BAND: case MScriptExpr::OPER_SHR:
        case MScriptExpr::OPER_SHL:  case MScriptExpr::OPER_MOD:
            incompatibleTypes(location, MScriptValue::TYPE_COMPLEX, MScriptValue::TYPE_COMPLEX);
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
        result.m_type = MScriptValue::TYPE_COMPLEX;
        result.m_data.m_complex = r;
        checkComplex(location, r);
    }
}

inline void MScriptExprAlgebra::complexString ( MScriptValue & /*result*/,
                                                const MScriptComplex & /*a*/,
                                                MScriptExpr::Operator /*oper*/,
                                                const MScriptValue::Data::String & /*b*/,
                                                const MScriptSrcLocation & location )
{
    incompatibleTypes(location, MScriptValue::TYPE_COMPLEX, MScriptValue::TYPE_STRING);
}

inline void MScriptExprAlgebra::complexBool ( MScriptValue & /*result*/,
                                              const MScriptComplex & /*a*/,
                                              MScriptExpr::Operator /*oper*/,
                                              bool /*b*/,
                                              const MScriptSrcLocation & location )
{
    incompatibleTypes(location, MScriptValue::TYPE_COMPLEX, MScriptValue::TYPE_BOOL);
}

inline void MScriptExprAlgebra::complexComplex ( MScriptValue & result,
                                                 const MScriptComplex & a,
                                                 MScriptExpr::Operator oper,
                                                 const MScriptComplex & b,
                                                 const MScriptSrcLocation & location )
{
    bool boolResult = false;
    bool br;
    MScriptComplex r;

    r.m_r = 1.0;
    r.m_i = 1.0;

    switch ( oper )
    {
        case MScriptExpr::OPER_LOR:  br = ( a || b ); boolResult = true; break;
        case MScriptExpr::OPER_LAND: br = ( a && b ); boolResult = true; break;
        case MScriptExpr::OPER_EQ:   br = ( a == b ); boolResult = true; break;
        case MScriptExpr::OPER_NE:   br = ( a != b ); boolResult = true; break;

        case MScriptExpr::OPER_ADD:  r = a + b;  break;
        case MScriptExpr::OPER_SUB:  r = a - b;  break;
        case MScriptExpr::OPER_MULT: r = a * b;  break;
        case MScriptExpr::OPER_DIV:
            if ( true == checkDivisor ( location, a, b) )
            {
                r = a / b;
            }
            break;

        case MScriptExpr::OPER_POW:
        case MScriptExpr::OPER_MIN:  case MScriptExpr::OPER_MAX:
        case MScriptExpr::OPER_LT:   case MScriptExpr::OPER_LE:
        case MScriptExpr::OPER_GE:   case MScriptExpr::OPER_GT:
        case MScriptExpr::OPER_BOR:  case MScriptExpr::OPER_BXOR:
        case MScriptExpr::OPER_BAND: case MScriptExpr::OPER_SHR:
        case MScriptExpr::OPER_SHL:  case MScriptExpr::OPER_MOD:
            incompatibleTypes(location, MScriptValue::TYPE_COMPLEX, MScriptValue::TYPE_COMPLEX);
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
        result.m_type = MScriptValue::TYPE_COMPLEX;
        result.m_data.m_complex = r;
        checkComplex(location, r);
    }
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

        case MScriptExpr::OPER_POW:     r = pow ( a, double(b) ); break;
        case MScriptExpr::OPER_MIN:     r = ( a < b ) ? a : b;    break;
        case MScriptExpr::OPER_MAX:     r = ( a > b ) ? a : b;    break;

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
        checkFloat(location, r);
    }
}

inline void MScriptExprAlgebra::floatString ( MScriptValue & /*result*/,
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

inline void MScriptExprAlgebra::stringFloat ( MScriptValue & /*result*/,
                                              const MScriptValue::Data::String & /*a*/,
                                              MScriptExpr::Operator /*oper*/,
                                              double /*b*/,
                                              const MScriptSrcLocation & location )
{
    incompatibleTypes(location, MScriptValue::TYPE_STRING, MScriptValue::TYPE_FLOAT);
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
                                               double div )
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

inline bool MScriptExprAlgebra::checkDivisor ( const MScriptSrcLocation & location,
                                               const MScriptComplex & a,
                                               const MScriptComplex & b )
{
    if ( false == a.divisableBy(b) )
    {
        m_interpret->interpreterMessage ( location,
                                          MScriptBase::MT_ERROR,
                                          QObject::tr ( "undefined result of complex division" ) . toStdString() );

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

inline void MScriptExprAlgebra::checkSignOverflow ( const MScriptSrcLocation & location,
                                                    long long a,
                                                    long long b,
                                                    long long c,
                                                    bool addition )
{
    if ( ( 0 == a ) || ( 0 == b ) )
    {
        return;
    }

    if ( true == addition )
    {
        if ( a > 0 )
        {
            if ( b > 0 )
            {
                if ( c > 0 )
                {
                    return;
                }
            }
            else if ( -b > a )
            {
                if ( c < 0 )
                {
                    return;
                }
            }
        }
        else if ( b < 0 )
        {
            if ( c < 0 )
            {
                return;
            }
        }
        else if ( ( b > -a ) && ( c > 0 ) )
        {
            return;
        }
    }
    else
    {
        if ( ( a > 0 ) == ( b > 0 ) )
        {
            if ( c > 0 )
            {
                return;
            }
        }
        else if ( c < 0 )
        {
            return;
        }
    }

    m_interpret->interpreterMessage ( location,
                                      MScriptBase::MT_ERROR,
                                      QObject::tr ( "integer overflow" ) . toStdString() );
}

inline void MScriptExprAlgebra::checkComplex ( const MScriptSrcLocation & location,
                                               const MScriptComplex & value )
{
    checkFloat(location, value.m_i);
    checkFloat(location, value.m_r);
}

inline void MScriptExprAlgebra::checkFloat ( const MScriptSrcLocation & location,
                                             double value )
{
    if ( true == std::isnan(value) )
    {
        m_interpret->interpreterMessage ( location,
                                          MScriptBase::MT_ERROR,
                                          QObject::tr ( "result is not a number" ) . toStdString() );
    }
    else if ( true == std::isinf(value) )
    {
        m_interpret->interpreterMessage ( location,
                                          MScriptBase::MT_WARNING,
                                          QObject::tr ( "result is infinity" ) . toStdString() );
    }
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
            switch ( typeB )
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
            switch ( typeB )
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
            switch ( typeB )
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
            switch ( typeB )
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
            switch ( typeB )
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
