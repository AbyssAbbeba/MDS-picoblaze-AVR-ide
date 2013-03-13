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
 * @file AsmAvr8SemanticAnalyzer.h
 */
// =============================================================================

#ifndef ASMAVR8SEMANTICANALYSER_H
#define ASMAVR8SEMANTICANALYSER_H

#include "../../CompilerSemanticInterface.h"

/**
 * @brief
 * @ingroup Compiler
 * @class AsmAvr8SemanticAnalyzer
 */
class AsmAvr8SemanticAnalyzer : public CompilerSemanticInterface
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] opts
         * @param[in] filename
         * @return
         */
        AsmAvr8SemanticAnalyzer ( CompilerCore * compilerCore,
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

#endif // ASMAVR8SEMANTICANALYSER_H
