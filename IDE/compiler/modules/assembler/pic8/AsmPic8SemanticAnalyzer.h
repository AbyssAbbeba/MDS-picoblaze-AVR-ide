// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Pic8Asm
 * @file AsmPic8SemanticAnalyzer.h
 */
// =============================================================================

#ifndef ASMPIC8SEMANTICANALYSER_H
#define ASMPIC8SEMANTICANALYSER_H

#include "CompilerSemanticAnalyzer.h"

/**
 * @brief
 * @ingroup Pic8Asm
 * @class AsmPic8SemanticAnalyzer
 */
class AsmPic8SemanticAnalyzer : public CompilerSemanticAnalyzer
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

#endif // ASMPIC8SEMANTICANALYSER_H
