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

// Compiler header files.
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
            OPER_NONE = 0,              ///<
            OPER_ADD,                   ///<
            OPER_SUB,                   ///<
            OPER_MULT,                  ///<
            OPER_DIV,                   ///<
            OPER_MOD,                   ///<
            OPER_DOT,                   ///<
            OPER_CALL,                  ///<
            OPER_BOR,                   ///< Bitwise OR
            OPER_BXOR,                  ///< Bitwise XOR
            OPER_BAND,                  ///< Bitwise AND
            OPER_LOR,                   ///< Logical OR
            OPER_LXOR,                  ///< Logical XOR
            OPER_LAND,                  ///< Logical AND
            OPER_LOW,                   ///<
            OPER_HIGH,                  ///<
            OPER_EQ,                    ///<
            OPER_NE,                    ///<
            OPER_LT,                    ///<
            OPER_LE,                    ///<
            OPER_GE,                    ///<
            OPER_GT,                    ///<
            OPER_SHR,                   ///<
            OPER_SHL,                   ///<
            OPER_CMPL,                  ///<
            OPER_NOT,                   ///<
            OPER_AT,                    ///<
            OPER_ASSIGN,                ///<
            OPER_HASH,                  ///<
            OPER_ADD_ASSIGN,            ///<
            OPER_SUB_ASSIGN,            ///<
            OPER_MUL_ASSIGN,            ///<
            OPER_DIV_ASSIGN,            ///<
            OPER_MOD_ASSIGN,            ///<
            OPER_SHL_ASSIGN,            ///<
            OPER_SHR_ASSIGN,            ///<
            OPER_AND_ASSIGN,            ///<
            OPER_ORB_ASSIGN,            ///<
            OPER_XOR_ASSIGN,            ///<
            OPER_INC,                   ///<
            OPER_DEC,                   ///<
            OPER_INTERVALS,             ///<
            OPER_ADD_INV,               ///<
            OPER_INT_PROM,              ///<
            OPER_NAND,                  ///<
            OPER_REF,                   ///<
            OPER_DEREF,                 ///< Dereference
            OPER_CONDITION,             ///<
            OPER_COLON,                 ///<
            OPER_INDEX,                 ///<
            OPER_PRE_INC,               ///<
            OPER_PRE_DEC,               ///<
            OPER_DATATYPE,              ///<
            OPER_ARROW,                 ///<
            OPER_COMMA,                 ///<
            OPER_SIZEOF,                ///<
            OPER_FIXED_DATATYPE,        ///<
            OPER_CAST,                  ///<
            OPER_ADDR,                  ///<
            OPER_DECLARATION,           ///<
            OPER_INIT,                  ///<
            OPER_PAIR,                  ///<
            OPER_POINTER,               ///<
            OPER_ARRAY,                 ///<
            OPER_FUNCTION,              ///<
            OPER_GENERIC,               ///<
            OPER_ALIGNOF,               ///<
            OPER_ALIGNAS,               ///<
            OPER_ASSERT,                ///<
            OPER_COMPOUND,              ///<
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
         * @return
         */
        CompilerExpr * copyEntireChain() const;

        /**
         * @brief
         * @return
         */
        CompilerExpr * copyChainLink() const;

        /**
         * @brief
         * @param[in] index
         * @return
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
