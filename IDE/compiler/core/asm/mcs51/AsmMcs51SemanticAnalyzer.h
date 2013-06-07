// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Mcs51Asm
 * @file AsmMcs51SemanticAnalyzer.h
 */
// =============================================================================

#ifndef ASMMCS51SEMANTICANALYSER_H
#define ASMMCS51SEMANTICANALYSER_H

#include "../../CompilerSemanticAnalyzer.h"

/**
 * @brief
 * @ingroup Mcs51Asm
 * @class AsmMcs51SemanticAnalyzer
 */
class AsmMcs51SemanticAnalyzer : public CompilerSemanticAnalyzer
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] opts
         * @param[in] filename
         * @return
         */
        AsmMcs51SemanticAnalyzer ( CompilerSemanticInterface * compilerCore,
                                   CompilerOptions * const opts )
                                 : CompilerSemanticAnalyzer ( compilerCore, opts ) {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] codeTree
         */
        void process ( CompilerStatement * codeTree );
};

#endif // ASMMCS51SEMANTICANALYSER_H
