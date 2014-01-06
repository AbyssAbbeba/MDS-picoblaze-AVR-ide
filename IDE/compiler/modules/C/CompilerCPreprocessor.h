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
class CompilerParserInterface;

// Standard headers.
#include <cstdio>
#include <vector>

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
        CompilerCPreprocessor ( CompilerParserInterface * compilerCore,
                                CompilerOptions * opts );

        /**
         * @brief
         */
        virtual ~CompilerCPreprocessor();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] inputFiles
         * @return
         */
        char * processFiles ( const std::vector<FILE *> & inputFiles );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] length
         * @param[in,out] line
         * @param[in,out] bufferSize
         * @return
         */
        inline bool processLine ( ssize_t & length,
                                  char * & line,
                                  size_t & bufferSize );

    ////    Protected Attributes    ////
    protected:
        /**
         * @brief
         */
        CompilerParserInterface * const m_compilerCore;

        /**
         * @brief
         */
        CompilerOptions * const m_opts;
};

#endif // COMPILERCPREPROCESSOR_H
