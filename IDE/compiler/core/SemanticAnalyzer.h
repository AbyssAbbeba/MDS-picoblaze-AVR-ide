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
 * @file SemanticAnalyzer.h
 */
// =============================================================================

#ifndef SEMANTICANALYSER_H
#define SEMANTICANALYSER_H

// Common compiler header files.
#include "CompilerSemanticInterface.h"
#include "CompilerCore.h"
#include "CompilerOptions.h"

/**
 * @brief
 * @ingroup Compiler
 * @class SemanticAnalyzer
 */
class SemanticAnalyzer
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] opts
         * @param[in] filename
         * @return
         */
        SemanticAnalyzer ( CompilerSemanticInterface * compilerCore,
                           CompilerOptions * opts )
                         : m_compilerCore ( compilerCore ),
                           m_opts(opts) {};
        /**
         * @brief
         */
        virtual ~SemanticAnalyzer() {};

    protected:
        /**
         * @brief
         */
        SemanticAnalyzer() : m_compilerCore ( NULL ), m_opts ( NULL ) {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] codeTree
         */
        virtual void process ( CompilerStatement * codeTree ) = 0;

    ////    Protected Attributes    ////
    protected:
        /**
         * @brief
         */
        CompilerSemanticInterface * const m_compilerCore;

        /**
         * @brief
         */
        CompilerOptions * const m_opts;
};

#endif // SEMANTICANALYSER_H
