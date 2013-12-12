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
 * @file CompilerCPreprocessor.h
 */
// =============================================================================

#ifndef COMPILERCPREPROCESSOR_H
#define COMPILERCPREPROCESSOR_H

// Forward declarations.
class CompilerOptions;
class CompilerSemanticInterface;

// Standard headers.
#include <cstdio>

// Boost regular expressions library.
#include <boost/regex.hpp>

/**
 * @brief
 * @ingroup CompilerC
 * @class CompilerCPreprocessor
 */
class CompilerCPreprocessor
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         */
        CompilerCPreprocessor ( CompilerSemanticInterface * compilerCore,
                                CompilerOptions * opts );

        /**
         * @brief
         */
        virtual ~CompilerCPreprocessor();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] sourceFile
         * @return
         */
        char * processFile ( FILE * sourceFile );
};

#endif // COMPILERCPREPROCESSOR_H
