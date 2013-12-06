// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup MoraviaScript
 * @file MScriptExpr.h
 */
// =============================================================================

#ifndef MSCRIPTEXPR_H
#define MSCRIPTEXPR_H

// MScript language interpreter header files.
#include "MScriptValue.h"
#include "MScriptSrcLocation.h"
#include "MScriptSerializable.h"

// Standard header files.
#include <string>
#include <ostream>

/**
 * @brief
 * @ingroup MoraviaScript
 * @class MScriptExpr
 */
class MScriptExpr : public MScriptSerializable
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Operator
        {
            OPER_NONE = 0,    ///< a
            OPER_ADD,         ///< a + b
            OPER_SUB,         ///< a - b
            OPER_MULT,        ///< a * b
            OPER_DIV,         ///< a / b
            OPER_MOD,         ///< a % b
            OPER_CALL,        ///< a ( b[] )
            OPER_BOR,         ///< a | b
            OPER_BXOR,        ///< a ^ b
            OPER_BAND,        ///< a & b
            OPER_LOR,         ///< a || b
            OPER_LAND,        ///< a && b
            OPER_EQ,          ///< a == b
            OPER_NE,          ///< a != b
            OPER_LT,          ///< a < b
            OPER_LE,          ///< a <= b
            OPER_GE,          ///< a >= b
            OPER_GT,          ///< a > b
            OPER_SHR,         ///< a >> b
            OPER_SHL,         ///< a << b
            OPER_CMPL,        ///< ~ a
            OPER_NOT,         ///< ! a
            OPER_ASSIGN,      ///< a = b
            OPER_ASSIGN_REF,  ///< a = &b;
            OPER_ADD_ASSIGN,  ///< a += b
            OPER_SUB_ASSIGN,  ///< a -= b
            OPER_MUL_ASSIGN,  ///< a *= b
            OPER_DIV_ASSIGN,  ///< a /= b
            OPER_MOD_ASSIGN,  ///< a %= b
            OPER_SHL_ASSIGN,  ///< a <<= b
            OPER_SHR_ASSIGN,  ///< a >>= b
            OPER_BAND_ASSIGN, ///< a &= b
            OPER_BOR_ASSIGN,  ///< a |= b
            OPER_LAND_ASSIGN, ///< a &&= b
            OPER_LOR_ASSIGN,  ///< a ||= b
            OPER_XOR_ASSIGN,  ///< a ^= b
            OPER_POW_ASSIGN,  ///< a **= b
            OPER_PRE_INC,     ///< ++ a
            OPER_PRE_DEC,     ///< -- a
            OPER_POST_INC,    ///< a ++
            OPER_POST_DEC,    ///< a --
            OPER_INDEX,       ///< a [ b ]
            OPER_TERNARY,     ///< a ? `e', where `e' is supposed to stand for ( a : b ).
            OPER_COLON,       ///< a : b, this is supposed to be used with the ternary operator.
            OPER_COMMA,       ///< a , b
            OPER_REF,         ///< & a
            OPER_INT_PROM,    ///< Integer promotion: ( + a )
            OPER_ADD_INV,     ///< Additive inverse: ( - a )
            OPER_POW,         ///< a ** b (a raised to the power of b)
            OPER_LENGTH,      ///<
            OPER_IS_BOOL,     ///<
            OPER_IS_INT,      ///<
            OPER_IS_STRING,   ///<
            OPER_IS_FLOAT,    ///<
            OPER_IS_COMPLEX,  ///<
            OPER_IS_EMPTY,    ///<
            OPER_TO_BOOL,     ///<
            OPER_TO_INT,      ///<
            OPER_TO_STRING,   ///<
            OPER_TO_FLOAT,    ///<
            OPER_TO_COMPLEX,  ///<
            OPER_IS_NAN,      ///<
            OPER_IS_INFINITY, ///<
            OPER_IS_POSITIVE, ///<
            OPER_IS_NEGATIVE, ///<
            OPER_IS_FINITE,   ///<
            OPER_REAL,        ///<
            OPER_IMG_UNIT,    ///<
            OPER_SIN,         ///<
            OPER_COS,         ///<
            OPER_TAN,         ///<
            OPER_ARCSIN,      ///<
            OPER_ARCCOS,      ///<
            OPER_ARCTAN,      ///<
            OPER_SINH,        ///<
            OPER_COSH,        ///<
            OPER_TANH,        ///<
            OPER_ARCSINH,     ///<
            OPER_ARCCOSH,     ///<
            OPER_ARCTANH,     ///<
            OPER_CEIL,        ///<
            OPER_ROUND,       ///<
            OPER_FLOOR,       ///<
            OPER_ABS,         ///<
            OPER_MIN,         ///<
            OPER_MAX          ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] input
         */
        MScriptExpr ( MScriptSerializer & input );

        /**
         * @brief
         * @param[in] location
         */
        MScriptExpr ( MScriptSrcLocation location = MScriptSrcLocation() );

        /**
         * @brief
         * @param[in] value
         * @param[in] location
         */
        MScriptExpr ( MScriptValue value,
                      MScriptSrcLocation location = MScriptSrcLocation() );

        /**
         * @brief
         * @param[in] oper
         * @param[in] value
         * @param[in] location
         */
        MScriptExpr ( Operator oper,
                      MScriptValue value,
                      MScriptSrcLocation location = MScriptSrcLocation() );

        /**
         * @brief
         * @param[in] value
         * @param[in] oper
         * @param[in] location
         */
        MScriptExpr ( MScriptValue value,
                      Operator oper,
                      MScriptSrcLocation location = MScriptSrcLocation() );

        /**
         * @brief
         * @param[in] lValue
         * @param[in] oper
         * @param[in] rValue
         * @param[in] location
         */
        MScriptExpr ( MScriptValue lValue,
                      Operator oper,
                      MScriptValue rValue,
                      MScriptSrcLocation location = MScriptSrcLocation() );

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        MScriptExpr * first();

        /**
         * @brief
         * @return
         */
        MScriptExpr * last();

        /**
         * @brief Insert link to the chain right after the link represented by this object.
         * @param[in,out] next
         * @return
         */
        MScriptExpr * insertLink ( MScriptExpr * chainLink );

        /**
         * @brief Append link at the end of the chain.
         * @param[in,out] chainLink
         * @return
         */
        MScriptExpr * appendLink ( MScriptExpr * chainLink );

        /**
         * @brief Insert link before the first link in the chain.
         * @param[in,out] chainLink
         * @return
         */
        MScriptExpr * prependLink ( MScriptExpr * chainLink );

        /**
         * @brief
         */
        void completeDelete();

        /**
         * @brief
         * @param[in,out] expr
         */
        static void completeDelete ( MScriptExpr * expr );

        /**
         * @brief
         * @return this
         */
        MScriptExpr * unlink();

        /**
         * @brief
         */
        MScriptExpr * copyEntireChain() const;

        /**
         * @brief
         */
        MScriptExpr * copyChainLink() const;

        /**
         * @brief
         * @param[in] index
         */
        MScriptExpr * operator [] ( int index );

        /// @name Serialization methods.
        //@{
            /**
             * @brief
             * @param[in,out]
             */
            virtual void serialize ( MScriptSerializer & output ) const;

            /**
             * @brief
             * @param[in,out]
             */
            virtual void deserialize ( MScriptSerializer & input );
        //@}

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] index
         */
        inline MScriptExpr * at ( int index )
        {
            return (*this)[index];
        }

        /**
         * @brief
         * @return
         */
        MScriptExpr * prev() const
        {
            return m_prev;
        }

        /**
         * @brief
         * @return
         */
        MScriptExpr * next() const
        {
            return m_next;
        }

        /**
         * @brief
         * @return
         */
        const MScriptSrcLocation & location() const
        {
            return m_location;
        }

        /**
         * @brief
         * @return
         */
        const Operator & oper() const
        {
            return m_operator;
        }

        /**
         * @brief
         * @return
         */
        const MScriptValue & lVal() const
        {
            return m_lValue;
        }

        /**
         * @brief
         * @return
         */
        const MScriptValue & rVal() const
        {
            return m_rValue;
        }

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        MScriptValue m_lValue;

        /**
         * @brief
         */
        MScriptValue m_rValue;

        /**
         * @brief
         */
        Operator m_operator;

        /**
         * @brief
         */
        MScriptSrcLocation m_location;

        /**
         * @brief
         */
        MScriptExpr * m_next;

        /**
         * @brief
         */
        MScriptExpr * m_prev;
};

/// @name Tracing operators
//@{
    /**
     * @brief
     * @param[in,out] out
     * @param[in] val
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const MScriptExpr::Operator & opr );

    /**
     * @brief
     * @param[in,out] out
     * @param[in] val
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const MScriptExpr * const expr );

    /**
     * @brief
     * @param[in,out] out
     * @param[in] val
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const MScriptExpr & expr );
//@}

#endif // MSCRIPTEXPR_H
