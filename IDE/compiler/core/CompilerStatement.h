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
         * @param[in,out] next
         * @return
         */
        CompilerStatement * addLink ( CompilerStatement * next );

        /**
         * @brief
         * @param[in,out] chainLink
         * @return
         */
        CompilerStatement * addArgsLink ( CompilerExpr * chainLink );

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

// Tracing operator
std::ostream & operator << ( std::ostream & out,
                             const CompilerStatement * stmt );

#endif // COMPILERSTATEMENT_H
