// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
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
    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        enum InMode
        {
            MODE_NORMAL, ///<
            MODE_STRING, ///<
            MODE_CHAR,   ///<
            MODE_ANG_BR, ///<
            MODE_COMMENT ///<
        };

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
        virtual ~CompilerCPreprocessor() {}

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] inputFiles
         * @return
         */
        char * processFiles ( const std::vector<FILE *> & inputFiles );

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] length
         * @param[in,out] line
         * @param[in] pos
         */
        void cutLine ( ssize_t & length,
                       char * line,
                       unsigned int pos );

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

        /**
         * @brief
         * @param[in,out] inBuffer
         * @param[in,out] mergeBuffer
         * @param[in,out] lineLen
         * @param[in,out] inBufferSize
         * @param[in,out] mergeBufferSize
         * @param[in,out] mergeBufferCurP
         * @return
         */
        inline bool lineMerge ( char *  & inBuffer,
                                char *  & mergeBuffer,
                                ssize_t & lineLen,
                                size_t  & inBufferSize,
                                size_t  & mergeBufferSize,
                                size_t  & mergeBufferCurP );

    ////    Protected Attributes    ////
    protected:
        ///
        CompilerParserInterface * const m_compilerCore;

        ///
        CompilerOptions * const m_opts;

    ////    Private Attributes    ////
    private:
        ///
        InMode m_inmode;
};

#endif // COMPILERCPREPROCESSOR_H
