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
 * @ingroup Compiler
 * @file CompilerExpr.h
 */
// =============================================================================

#ifndef COMPILEREXPR_H
#define COMPILEREXPR_H

// Compiler compiler header files.
#include "CompilerBase.h"
#include "CompilerValue.h"
#include "CompilerSerializable.h"
#include "CompilerSourceLocation.h"

// Standard header files.
#include <string>
#include <ostream>

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerExpr
 */
class CompilerExpr : public CompilerSerializable
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Operator
        {
            OPER_NONE            = 0,    ///< This makes only the m_lValue valid.
            OPER_ADD             = L'+', ///<
            OPER_SUB             = L'-', ///<
            OPER_MULT            = L'*', ///<
            OPER_DIV             = L'/', ///<
            OPER_MOD             = L'%', ///<
            OPER_DOT             = L'.', ///<
            OPER_CALL            = L'C', ///<
            OPER_BOR             = L'|', ///< Bitwise OR
            OPER_BXOR            = L'^', ///< Bitwise XOR
            OPER_BAND            = L'&', ///< Bitwise AND
            OPER_LOR             = L'⋁', ///< Logical OR
            OPER_LXOR            = L'⊕', ///< Logical XOR
            OPER_LAND            = L'⋀', ///< Logical AND
            OPER_LOW             = L'l', ///<
            OPER_HIGH            = L'h', ///<
            OPER_EQ              = L'≡', ///<
            OPER_NE              = L'≠', ///<
            OPER_LT              = L'<', ///<
            OPER_LE              = L'≤', ///<
            OPER_GE              = L'≥', ///<
            OPER_GT              = L'>', ///<
            OPER_SHR             = L'≫', ///<
            OPER_SHL             = L'≪', ///<
            OPER_CMPL            = L'~', ///<
            OPER_NOT             = L'!', ///<
            OPER_AT              = L'@', ///<
            OPER_ASSIGN          = L'=', ///<
            OPER_HASH            = L'#', ///<
            OPER_ADD_ASSIGN      = 1,    ///<
            OPER_SUB_ASSIGN      = 2,    ///<
            OPER_MUL_ASSIGN      = 3,    ///<
            OPER_DIV_ASSIGN      = 4,    ///<
            OPER_MOD_ASSIGN      = 5,    ///<
            OPER_SHL_ASSIGN      = 6,    ///<
            OPER_SHR_ASSIGN      = 7,    ///<
            OPER_AND_ASSIGN      = 8,    ///<
            OPER_ORB_ASSIGN      = 9,    ///<
            OPER_XOR_ASSIGN      = 10,   ///<
            OPER_INC             = 11,   ///<
            OPER_DEC             = 12,   ///<
            OPER_INTERVALS       = 13,   ///<
            OPER_ADD_INV         = 14,   ///<
            OPER_INT_PROM        = 15,   ///<
            OPER_NAND            = 16,   ///<
            OPER_REF             = 17,   ///< Reference
            OPER_DEREF           = 18,   ///< Dereference
            OPER_TERNARY         = 19,   ///<
            OPER_COLON           = 20,   ///<
            OPER_INDEX           = 21,   ///<
            OPER_POST_INC        = 22,   ///<
            OPER_POST_DEC        = 23,   ///<
            OPER_PRE_INC         = 24,   ///<
            OPER_PRE_DEC         = 25,   ///<
            OPER_DATATYPE        = 26,   ///<
            OPER_ARROW           = 27,   ///<
            OPER_COMMA           = 28,   ///<
            OPER_SIZEOF          = 29,   ///<
            OPER_DATAATTR        = 30,   ///<
            OPER_PTRATTR         = 31,   ///<
            OPER_CUSTOM_DATATYPE = 32    ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] input
         */
        explicit CompilerExpr ( CompilerSerializer & input );

        /**
         * @brief
         * @param[in] location
         */
        explicit CompilerExpr ( CompilerSourceLocation location = CompilerSourceLocation() );

        /**
         * @brief
         * @param[in] value
         * @param[in] location
         */
        explicit CompilerExpr ( CompilerValue value,
                                CompilerSourceLocation location = CompilerSourceLocation() );

        /**
         * @brief
         * @param[in] oper
         * @param[in] value
         * @param[in] location
         */
        explicit CompilerExpr ( Operator oper,
                                CompilerValue value,
                                CompilerSourceLocation location = CompilerSourceLocation() );

        /**
         * @brief
         * @param[in] oper
         * @param[in] value
         * @param[in] location
         */
        explicit CompilerExpr ( char oper,
                                CompilerValue value,
                                CompilerSourceLocation location = CompilerSourceLocation() );

        /**
         * @brief
         * @param[in] value
         * @param[in] oper
         * @param[in] location
         */
        explicit CompilerExpr ( CompilerValue value,
                                Operator oper,
                                CompilerSourceLocation location = CompilerSourceLocation() );

        /**
         * @brief
         * @param[in] value
         * @param[in] oper
         * @param[in] location
         */
        explicit CompilerExpr ( CompilerValue value,
                                char oper,
                                CompilerSourceLocation location = CompilerSourceLocation() );

        /**
         * @brief
         * @param[in] lValue
         * @param[in] oper
         * @param[in] rValue
         * @param[in] location
         */
        explicit CompilerExpr ( CompilerValue lValue,
                                Operator oper,
                                CompilerValue rValue,
                                CompilerSourceLocation location = CompilerSourceLocation() );

        /**
         * @brief
         * @param[in] lValue
         * @param[in] oper
         * @param[in] rValue
         * @param[in] location
         */
        explicit CompilerExpr ( CompilerValue lValue,
                                char oper,
                                CompilerValue rValue,
                                CompilerSourceLocation location = CompilerSourceLocation() );

        /**
         * @brief
         */
        virtual ~CompilerExpr() {}

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

        /**
         * @brief
         * @param[in,out]
         */
        virtual void serialize ( CompilerSerializer & output ) const override;

        /**
         * @brief
         * @param[in,out]
         */
        virtual void deserialize ( CompilerSerializer & input ) override;

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
            if ( nullptr == this )
            {
                return nullptr;
            }

            return m_prev;
        }

        /**
         * @brief
         * @return
         */
        CompilerExpr * next() const
        {
            if ( nullptr == this )
            {
                return nullptr;
            }

            return m_next;
        }

        /**
         * @brief
         * @return
         */
        const CompilerSourceLocation & location() const
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
        const CompilerValue & lVal() const
        {
            return m_lValue;
        }

        /**
         * @brief
         * @return
         */
        const CompilerValue & rVal() const
        {
            return m_rValue;
        }

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        CompilerValue m_lValue;

        /**
         * @brief
         */
        CompilerValue m_rValue;

        /**
         * @brief
         */
        Operator m_operator;

        /**
         * @brief
         */
        CompilerSourceLocation m_location;

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
