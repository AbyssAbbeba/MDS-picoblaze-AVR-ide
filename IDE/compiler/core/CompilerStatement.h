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
 * @file CompilerStatement.h
 */
// =============================================================================


#ifndef COMPILERSTATEMENT_H
#define COMPILERSTATEMENT_H

#include "CompilerBase.h"
#include "CompilerExpr.h"
#include "StatementTypes.h"

#include <ostream>

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerStatement
 */
class CompilerStatement
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
         * @param[in] location
         * @param[in] type
         * @param[in] args
         */
        CompilerStatement ( CompilerBase::SourceLocation location,
                            StatementTypes::StatementType type,
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
        const CompilerBase::SourceLocation & location() const
        {
            return m_location;
        }

        /**
         * @brief
         * @return
         */
        StatementTypes::StatementType type() const
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
        StatementTypes::StatementType m_type;

        /**
         * @brief
         */
        CompilerBase::SourceLocation m_location;

        /**
         * @brief
         */
        int m_userData;

        /**
         * @brief
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
