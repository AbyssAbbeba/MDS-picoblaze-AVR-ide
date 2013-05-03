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
 * @ingroup Pic8Asm
 * @file AsmPic8SemanticAnalyzer.h
 */
// =============================================================================

#ifndef ASMPIC8SEMANTICANALYSER_H
#define ASMPIC8SEMANTICANALYSER_H

#include "../../SemanticAnalyzer.h"

/**
 * @brief
 * @ingroup Pic8Asm
 * @class AsmPic8SemanticAnalyzer
 */
class AsmPic8SemanticAnalyzer : public SemanticAnalyzer
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] opts
         * @param[in] filename
         * @return
         */
        AsmPic8SemanticAnalyzer ( CompilerSemanticInterface * compilerCore,
                                  CompilerOptions * const opts )
                                : SemanticAnalyzer ( compilerCore, opts ) {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] codeTree
         */
        void process ( CompilerStatement * codeTree );
};

#endif // ASMPIC8SEMANTICANALYSER_H
