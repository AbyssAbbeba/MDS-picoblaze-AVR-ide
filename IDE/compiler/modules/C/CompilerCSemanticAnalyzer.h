// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @ingroup CompilerC
 * @file CompilerCSemanticAnalyzer.h
 */
// =============================================================================

#ifndef COMPILERCSEMANTICANALYZER_H
#define COMPILERCSEMANTICANALYZER_H

// Common compiler header files.
#include "CompilerSemanticAnalyzer.h"

/**
 * @brief
 * @ingroup CompilerC
 * @class CompilerCSemanticAnalyzer
 */
class CompilerCSemanticAnalyzer : public CompilerSemanticAnalyzer
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         */
        CompilerCSemanticAnalyzer ( CompilerSemanticInterface * compilerCore,
                                    CompilerOptions * opts );

        /**
         * @brief
         */
        virtual ~CompilerCSemanticAnalyzer();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] codeTree
         */
        virtual void process ( CompilerStatement * codeTree );

        /**
         * @brief
         * @param[in] deviceName
         */
        virtual void setDevice ( const std::string & deviceName );
};

#endif // COMPILERCSEMANTICANALYZER_H