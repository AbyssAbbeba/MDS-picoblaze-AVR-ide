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
                    struct
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
            OPER_AT             = L'@', ///<
            OPER_ASSIGN         = L'='  ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        CompilerExpr();

        /**
         * @brief
         * @param[in] value
         */
        CompilerExpr ( Value value );

        /**
         * @brief
         * @param[in] oper
         * @param[in] value
         */
        CompilerExpr ( Operator oper,
                       Value value );

        /**
         * @brief
         * @param[in] oper
         * @param[in] value
         */
        CompilerExpr ( char oper,
                       Value value );

        /**
         * @brief
         * @param[in] value
         * @param[in] oper
         */
        CompilerExpr ( Value value,
                       Operator oper );

        /**
         * @brief
         * @param[in] value
         * @param[in] oper
         */
        CompilerExpr ( Value value,
                       char oper );

        /**
         * @brief
         * @param[in] lValue
         * @param[in] oper
         * @param[in] rValue
         */
        CompilerExpr ( Value lValue,
                       Operator oper,
                       Value rValue );

        /**
         * @brief
         * @param[in] lValue
         * @param[in] oper
         * @param[in] rValue
         */
        CompilerExpr ( Value lValue,
                       char oper,
                       Value rValue );

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        CompilerExpr * makeCopy() const;

        /**
         * @brief
         * @return
         */
        CompilerExpr * first();

        /**
         * @brief
         * @param[in,out] next
         * @return
         */
        CompilerExpr * addLink ( CompilerExpr * next );

        /**
         * @brief
         */
        void completeDelete();

        /**
         * @brief
         * @param[in,out] expr
         */
        static void completeDelete ( CompilerExpr * expr );

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
                                 const CompilerExpr * expr );
//@}

#endif // COMPILEREXPR_H
