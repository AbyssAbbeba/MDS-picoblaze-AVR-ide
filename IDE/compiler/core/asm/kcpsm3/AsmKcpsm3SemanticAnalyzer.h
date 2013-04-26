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
 * @file AsmKcpsm3SemanticAnalyzer.h
 */
// =============================================================================

#ifndef ASMKCPSM3SEMANTICANALYSER_H
#define ASMKCPSM3SEMANTICANALYSER_H

#include "../../CompilerSemanticInterface.h"

/**
 * @brief
 * @ingroup Compiler
 * @class AsmKcpsm3SemanticAnalyzer
 */
class AsmKcpsm3SemanticAnalyzer : public CompilerSemanticInterface
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] opts
         * @param[in] filename
         * @return
         */
        AsmKcpsm3SemanticAnalyzer ( CompilerCore * compilerCore,
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

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] codeTree
         */
        inline void processDeclarations ( CompilerStatement * & node );

        /**
         * @brief
         * @param[in] name
         * @param[in] value
         */
        inline void addToSymbolTable ( CompilerExpr * const name,
                                       CompilerExpr * const value );
};

#endif // ASMKCPSM3SEMANTICANALYSER_H
