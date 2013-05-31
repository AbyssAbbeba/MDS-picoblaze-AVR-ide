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
 * @ingroup Compiler
 * @file CompilerExpr.h
 */
// =============================================================================

#ifndef COMPILEREXPR_H
#define COMPILEREXPR_H

#include "CompilerBase.h"

// Standard header files.
#include <string>
#include <ostream>

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerExpr
 */
class CompilerExpr
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
                    CompilerExpr * m_expr;

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
                Value ( CompilerExpr * expr );

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
            OPER_NONE           = 0,    ///< This makes only the m_lValue valid.
            OPER_ADD            = L'+', ///<
            OPER_SUB            = L'-', ///<
            OPER_MULT           = L'*', ///<
            OPER_DIV            = L'/', ///<
            OPER_MOD            = L'%', ///<
            OPER_DOT            = L'.', ///<
            OPER_CALL           = L'C', ///<
            OPER_BOR            = L'|', ///< Bitwise OR
            OPER_BXOR           = L'^', ///< Bitwise XOR
            OPER_BAND           = L'&', ///< Bitwise AND
            OPER_LOR            = L'⋁', ///< Logical OR
            OPER_LXOR           = L'⊕', ///< Logical XOR
            OPER_LAND           = L'⋀', ///< Logical AND
            OPER_LOW            = L'l', ///<
            OPER_HIGH           = L'h', ///<
            OPER_EQ             = L'≡', ///<
            OPER_NE             = L'≠', ///<
            OPER_LT             = L'<', ///<
            OPER_LE             = L'≤', ///<
            OPER_GE             = L'≥', ///<
            OPER_GT             = L'>', ///<
            OPER_SHR            = L'≫', ///<
            OPER_SHL            = L'≪', ///<
            OPER_CMPL           = L'~', ///<
            OPER_NOT            = L'!', ///<
            OPER_AT             = L'@', ///<
            OPER_ASSIGN         = L'=', ///<
            OPER_ADD_ASSIGN     = 1,    ///<
            OPER_SUB_ASSIGN     = 2,    ///<
            OPER_MUL_ASSIGN     = 3,    ///<
            OPER_DIV_ASSIGN     = 4,    ///<
            OPER_MOD_ASSIGN     = 5,    ///<
            OPER_SHL_ASSIGN     = 6,    ///<
            OPER_SHR_ASSIGN     = 7,    ///<
            OPER_AND_ASSIGN     = 8,    ///<
            OPER_ORB_ASSIGN     = 9,    ///<
            OPER_XOR_ASSIGN     = 10,   ///<
            OPER_INC            = 11,   ///<
            OPER_DEC            = 12,   ///<
            OPER_INTERVALS      = 13,   ///<
            OPER_ADD_INV        = 14,   ///<
            OPER_INT_PROM       = 15    ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] location
         */
        CompilerExpr ( CompilerBase::SourceLocation location = CompilerBase::SourceLocation() );

        /**
         * @brief
         * @param[in] value
         * @param[in] location
         */
        CompilerExpr ( Value value,
                       CompilerBase::SourceLocation location = CompilerBase::SourceLocation() );

        /**
         * @brief
         * @param[in] oper
         * @param[in] value
         * @param[in] location
         */
        CompilerExpr ( Operator oper,
                       Value value,
                       CompilerBase::SourceLocation location = CompilerBase::SourceLocation() );

        /**
         * @brief
         * @param[in] oper
         * @param[in] value
         * @param[in] location
         */
        CompilerExpr ( char oper,
                       Value value,
                       CompilerBase::SourceLocation location = CompilerBase::SourceLocation() );

        /**
         * @brief
         * @param[in] value
         * @param[in] oper
         * @param[in] location
         */
        CompilerExpr ( Value value,
                       Operator oper,
                       CompilerBase::SourceLocation location = CompilerBase::SourceLocation() );

        /**
         * @brief
         * @param[in] value
         * @param[in] oper
         * @param[in] location
         */
        CompilerExpr ( Value value,
                       char oper,
                       CompilerBase::SourceLocation location = CompilerBase::SourceLocation() );

        /**
         * @brief
         * @param[in] lValue
         * @param[in] oper
         * @param[in] rValue
         * @param[in] location
         */
        CompilerExpr ( Value lValue,
                       Operator oper,
                       Value rValue,
                       CompilerBase::SourceLocation location = CompilerBase::SourceLocation() );

        /**
         * @brief
         * @param[in] lValue
         * @param[in] oper
         * @param[in] rValue
         * @param[in] location
         */
        CompilerExpr ( Value lValue,
                       char oper,
                       Value rValue,
                       CompilerBase::SourceLocation location = CompilerBase::SourceLocation() );

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        CompilerExpr * first();

        /**
         * @brief
         * @return
         */
        CompilerExpr * last();

        /**
         * @brief Insert link to the chain right after the link represented by this object.
         * @param[in,out] next
         * @return
         */
        CompilerExpr * insertLink ( CompilerExpr * chainLink );

        /**
         * @brief Append link at the end of the chain.
         * @param[in,out] chainLink
         * @return
         */
        CompilerExpr * appendLink ( CompilerExpr * chainLink );

        /**
         * @brief Insert link before the first link in the chain.
         * @param[in,out] chainLink
         * @return
         */
        CompilerExpr * prependLink ( CompilerExpr * chainLink );

        /**
         * @brief
         */
        void completeDelete();

        /**
         * @brief
         * @param[in,out] expr
         */
        static void completeDelete ( CompilerExpr * expr );

        /**
         * @brief
         * @return this
         */
        CompilerExpr * unlink();

        /**
         * @brief
         */
        CompilerExpr * copyEntireChain() const;

        /**
         * @brief
         */
        CompilerExpr * copyChainLink() const;

        /**
         * @brief
         * @param[in] index
         */
        CompilerExpr * operator [] ( int index );

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] index
         */
        inline CompilerExpr * at ( int index )
        {
            return (*this)[index];
        }

        /**
         * @brief
         * @return
         */
        CompilerExpr * prev() const
        {
            return m_prev;
        }

        /**
         * @brief
         * @return
         */
        CompilerExpr * next() const
        {
            return m_next;
        }

        /**
         * @brief
         * @return
         */
        const CompilerBase::SourceLocation & location() const
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
        CompilerBase::SourceLocation m_location;

        /**
         * @brief
         */
        CompilerExpr * m_next;

        /**
         * @brief
         */
        CompilerExpr * m_prev;
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
                                 const CompilerExpr::Value & val );

    /**
     * @brief
     * @param[in,out] out
     * @param[in] val
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const CompilerExpr::Operator & opr );

    /**
     * @brief
     * @param[in,out] out
     * @param[in] val
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const CompilerExpr * const expr );

    /**
     * @brief
     * @param[in,out] out
     * @param[in] val
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const CompilerExpr & expr );
//@}

#endif // COMPILEREXPR_H
