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
 * @file CompilerStatement.h
 */
// =============================================================================

#ifndef COMPILERSTATEMENT_H
#define COMPILERSTATEMENT_H

// Common compiler header files.
#include "CompilerBase.h"
#include "CompilerExpr.h"
#include "CompilerStatementTypes.h"
#include "CompilerSerializable.h"
#include "CompilerSourceLocation.h"

// Standard header files.
#include <ostream>

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerStatement
 */
class CompilerStatement : public CompilerSerializable
{
    ////    Friends    ////
    friend std::ostream & operator << ( std::ostream & out,
                                        const CompilerStatement * stmt );

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        ~CompilerStatement();

        /**
         * @brief
         */
        CompilerStatement();

        /**
         * @brief
         * @param[in,out] input
         */
        CompilerStatement ( CompilerSerializer & input );

        /**
         * @brief
         * @param[in] location
         * @param[in] type
         * @param[in] args
         */
        CompilerStatement ( CompilerSourceLocation location,
                            CompilerStatementTypes::StatementType type,
                            CompilerExpr * args = NULL );

    ////    Public Operations    ////
    public:
        /**
         * @brief
         */
        void completeDelete();

        /**
         * @brief
         * @param[in,out] stmt
         */
        static void completeDelete ( CompilerStatement * stmt );

        /**
         * @brief
         * @param[in,out] branch
         * @return
         */
        CompilerStatement * createBranch ( CompilerStatement * branch );

        /**
         * @brief
         * @return
         */
        CompilerStatement * first();

        /**
         * @brief
         * @return
         */
        CompilerStatement * last();

        /**
         * @brief
         * @return
         */
        CompilerStatement * lastLeaf();

        /**
         * @brief
         * @return this
         */
        CompilerStatement * unlink();

        /**
         * @brief
         */
        CompilerStatement * copyEntireChain() const;

        /**
         * @brief
         */
        CompilerStatement * copyChainLink() const;

        /**
         * @brief Insert link to the chain right after the link represented by this object.
         * @param[in,out] next
         * @return
         */
        CompilerStatement * insertLink ( CompilerStatement * chainLink );

        /**
         * @brief
         * @param[in,out] chainLink
         * @return
         */
        CompilerStatement * appendLink ( CompilerStatement * chainLink );

        /**
         * @brief Insert link before the first link in the chain.
         * @param[in,out] chainLink
         * @return
         */
        CompilerStatement * prependLink ( CompilerStatement * chainLink );

        /**
         * @brief
         * @param[in,out] chainLink
         * @return
         */
        CompilerStatement * appendArgsLink ( CompilerExpr * chainLink );

        /**
         * @brief
         * @param[in,out] out
         * @param[in] level
         * @param[in] lineString
         * @return
         */
        std::ostream & print ( std::ostream & out,
                               int level = 0,
                               std::string lineString = "1" ) const;

        /**
         * @brief
         * @param[in,out]
         */
        void serializeTree ( CompilerSerializer & output ) const;

        /**
         * @brief
         * @param[in,out]
         */
        virtual void serialize ( CompilerSerializer & output ) const;

        /**
         * @brief
         * @param[in,out]
         */
        virtual void deserialize ( CompilerSerializer & input );

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        CompilerStatement * prev() const
        {
            return m_prev;
        }

        /**
         * @brief
         * @return
         */
        CompilerStatement * next() const
        {
            return m_next;
        }

        /**
         * @brief
         * @return
         */
        CompilerStatement * branch() const
        {
            return m_branch;
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
        CompilerStatementTypes::StatementType type() const
        {
            return m_type;
        }

        /**
         * @brief
         * @return
         */
        CompilerExpr * args() const
        {
            return m_args;
        }

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        CompilerStatementTypes::StatementType m_type;

        /**
         * @brief
         */
        CompilerSourceLocation m_location;

        /**
         * @brief
         * @warning This attribute is not a subject for serialization, set to 0 when deserialized.
         */
        int m_userData;

        /**
         * @brief
         * @warning This attribute is not a subject for serialization, set to -1 when deserialized.
         */
        int m_serialNumber;

        /**
         * @brief
         */
        CompilerExpr * m_args;

        /**
         * @brief
         */
        CompilerStatement * m_prev;

        /**
         * @brief
         */
        CompilerStatement * m_next;

        /**
         * @brief
         */
        CompilerStatement * m_branch;
};

/**
 * @brief Tracing operator
 * @param[in,out] out
 * @param[in] stmt
 * @return
 */
std::ostream & operator << ( std::ostream & out,
                             const CompilerStatement * stmt );

#endif // COMPILERSTATEMENT_H
