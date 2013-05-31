// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup MoraviaScript
 * @file MScriptExpr.h
 */
// =============================================================================

#ifndef MSCRIPTEXPR_H
#define MSCRIPTEXPR_H

// MScript language interpreter header files.
#include "MScriptSrcLocation.h"

// Standard header files.
#include <string>
#include <ostream>

/**
 * @brief
 * @ingroup MoraviaScript
 * @class MScriptExpr
 */
class MScriptExpr
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        class Value
        {
            ////    Public Datatypes    ////
            public:
                /**
                 * @brief
                 */
                enum Type
                {
                    TYPE_EMPTY = 0, ///<
                    TYPE_INT,       ///<
                    TYPE_REAL,      ///<
                    TYPE_EXPR,      ///<
                    TYPE_SYMBOL,    ///<
                    TYPE_ARRAY      ///<
                };

                /**
                 * @brief
                 */
                union Data
                {
                    ///
                    long long m_integer;

                    ///
                    double m_real;

                    ///
                    MScriptExpr * m_expr;

                    ///
                    char * m_symbol;

                    ///
                    struct CharArray
                    {
                        ///
                        unsigned char * m_data;

                        ///
                        int m_size;
                    } m_array;
                };

            ////    Constructors and Destructors    ////
            public:
                /**
                 * @brief
                 */
                Value();

                /**
                 * @brief
                 * @param[in] value
                 */
                Value ( int value );

                /**
                 * @brief
                 * @param[in] value
                 */
                Value ( long long value );

                /**
                 * @brief
                 * @param[in] value
                 */
                Value ( float value );

                /**
                 * @brief
                 * @param[in] value
                 */
                Value ( double value );

                /**
                 * @brief
                 * @param[in,out] expr
                 */
                Value ( MScriptExpr * expr );

                /**
                 * @brief
                 * @param[in] string
                 */
                Value ( const char * string );

                /**
                 * @brief
                 * @param[in] string
                 */
                Value ( const std::string & string );

                /**
                 * @brief
                 * @param[in] array
                 * @param[in] size
                 */
                Value ( const unsigned char * array,
                        int size );

                /**
                 * @brief
                 * @param[in,out] array
                 * @param[in] size
                 * @param[in] copy
                 */
                Value ( unsigned char * array,
                        int size,
                        bool copy = false );

            ////    Public Operations    ////
            public:
                /**
                 * @brief
                 * @return
                 */
                Value & makeCopy() const;

                /**
                 * @brief
                 */
                void completeDelete();

            ////    Public Attributes    ////
            public:
                /**
                 * @brief
                 */
                Data m_data;

                /**
                 * @brief
                 */
                Type m_type;
        };

        /**
         * @brief
         */
        enum Operator
        {
            OPER_NONE = 0,   ///< a
            OPER_ADD,        ///< a + b
            OPER_SUB,        ///< a - b
            OPER_MULT,       ///< a * b
            OPER_DIV,        ///< a / b
            OPER_MOD,        ///< a % b
            OPER_CALL,       ///< a ( b[] )
            OPER_BOR,        ///< a | b
            OPER_BXOR,       ///< a ^ b
            OPER_BAND,       ///< a & b
            OPER_LOR,        ///< a || b
            OPER_LAND,       ///< a && b
            OPER_EQ,         ///< a == b
            OPER_NE,         ///< a != b
            OPER_LT,         ///< a < b
            OPER_LE,         ///< a <= b
            OPER_GE,         ///< a >= b
            OPER_GT,         ///< a > b
            OPER_SHR,        ///< a >> b
            OPER_SHL,        ///< a << b
            OPER_CMPL,       ///< ~ a
            OPER_NOT,        ///< ! a
            OPER_ADD_ASSIGN, ///< a += b
            OPER_SUB_ASSIGN, ///< a -= b
            OPER_MUL_ASSIGN, ///< a *= b
            OPER_DIV_ASSIGN, ///< a /= b
            OPER_MOD_ASSIGN, ///< a %= b
            OPER_SHL_ASSIGN, ///< a <<= b
            OPER_SHR_ASSIGN, ///< a >>= b
            OPER_AND_ASSIGN, ///< a &= b
            OPER_ORB_ASSIGN, ///< a |= b
            OPER_XOR_ASSIGN, ///< a ^= b
            OPER_PRE_INC,    ///< ++ a
            OPER_PRE_DEC,    ///< -- a
            OPER_POST_INC,   ///< a ++
            OPER_POST_DEC,   ///< a --
            OPER_ASSIGN,     ///< a = b
            OPER_INDEX,      ///< a [ b ]
            OPER_TERNARY,    ///< a ? `e', where `e' is supposed to stand for ( a : b ).
            OPER_COLON,      ///< a : b, this is supposed to be used with the ternary operator.
            OPER_COMMA,      ///< a , b
            OPER_REF,        ///< & a
            OPER_INT_PROM,   ///< Integer promotion: ( + a )
            OPER_ADD_INV     ///< Additive inverse: ( - a )
        };

    ////    Constructors and Destructors    ////
    public:
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
        MScriptExpr ( Value value,
                       MScriptSrcLocation location = MScriptSrcLocation() );

        /**
         * @brief
         * @param[in] oper
         * @param[in] value
         * @param[in] location
         */
        MScriptExpr ( Operator oper,
                       Value value,
                       MScriptSrcLocation location = MScriptSrcLocation() );

        /**
         * @brief
         * @param[in] oper
         * @param[in] value
         * @param[in] location
         */
        MScriptExpr ( char oper,
                       Value value,
                       MScriptSrcLocation location = MScriptSrcLocation() );

        /**
         * @brief
         * @param[in] value
         * @param[in] oper
         * @param[in] location
         */
        MScriptExpr ( Value value,
                       Operator oper,
                       MScriptSrcLocation location = MScriptSrcLocation() );

        /**
         * @brief
         * @param[in] value
         * @param[in] oper
         * @param[in] location
         */
        MScriptExpr ( Value value,
                       char oper,
                       MScriptSrcLocation location = MScriptSrcLocation() );

        /**
         * @brief
         * @param[in] lValue
         * @param[in] oper
         * @param[in] rValue
         * @param[in] location
         */
        MScriptExpr ( Value lValue,
                       Operator oper,
                       Value rValue,
                       MScriptSrcLocation location = MScriptSrcLocation() );

        /**
         * @brief
         * @param[in] lValue
         * @param[in] oper
         * @param[in] rValue
         * @param[in] location
         */
        MScriptExpr ( Value lValue,
                       char oper,
                       Value rValue,
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
        const Value & lVal() const
        {
            return m_lValue;
        }

        /**
         * @brief
         * @return
         */
        const Value & rVal() const
        {
            return m_rValue;
        }

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        Value m_lValue;

        /**
         * @brief
         */
        Value m_rValue;

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
                                 const MScriptExpr::Value & val );

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
