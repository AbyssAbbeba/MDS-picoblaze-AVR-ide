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
 * @ingroup Avr8Asm
 * @file AsmAvr8SemanticAnalyzer.h
 */
// =============================================================================

#ifndef ASMAVR8SEMANTICANALYSER_H
#define ASMAVR8SEMANTICANALYSER_H

#include "../../CompilerSemanticAnalyzer.h"

/**
 * @brief
 * @ingroup Avr8Asm
 * @class AsmAvr8SemanticAnalyzer
 */
class AsmAvr8SemanticAnalyzer : public CompilerSemanticAnalyzer
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] opts
         * @param[in] filename
         * @return
         */
        AsmAvr8SemanticAnalyzer ( CompilerSemanticInterface * compilerCore,
                                  CompilerOptions * const opts )
                                : CompilerSemanticAnalyzer ( compilerCore, opts ) {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] codeTree
         */
        void process ( CompilerStatement * codeTree );

        /**
         * @brief
         * @param[in] deviceName
         */
        void setDevice ( const std::string & deviceName );
};

#endif // ASMAVR8SEMANTICANALYSER_H
