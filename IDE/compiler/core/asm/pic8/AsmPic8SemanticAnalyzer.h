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
 * @file AsmPic8SemanticAnalyzer.h
 */
// =============================================================================

#ifndef ASMPIC8SEMANTICANALYSER_H
#define ASMPIC8SEMANTICANALYSER_H

#include "../../CompilerSemanticInterface.h"

/**
 * @brief
 * @ingroup Compiler
 * @class AsmPic8SemanticAnalyzer
 */
class AsmPic8SemanticAnalyzer : public CompilerSemanticInterface
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] opts
         * @param[in] filename
         * @return
         */
        AsmPic8SemanticAnalyzer ( CompilerCore * compilerCore,
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

#endif // ASMPIC8SEMANTICANALYSER_H
