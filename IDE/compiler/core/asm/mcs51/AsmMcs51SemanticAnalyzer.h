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
 * @file AsmMcs51SemanticAnalyzer.h
 */
// =============================================================================

#ifndef ASMMCS51SEMANTICANALYSER_H
#define ASMMCS51SEMANTICANALYSER_H

#include "../../CompilerSemanticInterface.h"

/**
 * @brief
 * @ingroup Compiler
 * @class AsmMcs51SemanticAnalyzer
 */
class AsmMcs51SemanticAnalyzer : public CompilerSemanticInterface
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] opts
         * @param[in] filename
         * @return
         */
        AsmMcs51SemanticAnalyzer ( CompilerCore * compilerCore,
                                   CompilerOptions * const opts,
                                   const std::string & filename )
                                 : CompilerSemanticInterface ( compilerCore, opts, filename ) {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] codeTree
         */
        void process ( CompilerStatement * & codeTree );
};

#endif // ASMMCS51SEMANTICANALYSER_H
